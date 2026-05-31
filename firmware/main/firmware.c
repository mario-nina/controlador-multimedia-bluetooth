#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "control_leds.h"
#include "driver_entrada.h"
#include "comunicacion_bt.h"
#include "gestion_energia.h"

static const char *TAG = "firmware";

/* --- Prioridades de tareas --- */
#define TAREA_PRIO_INPUT    5
#define TAREA_PRIO_CMD      4

/* --- Máquina de estados del sistema --- */
typedef enum {
    SYS_ADVERTISING, /**< Sin conexión BLE — anunciando */
    SYS_CONNECTED,   /**< Conexión BLE activa */
    SYS_SLEEPING,    /**< Modo bajo consumo — pendiente Fase 10 */
} sys_state_t;

static sys_state_t       estado_sistema = SYS_ADVERTISING;
static SemaphoreHandle_t mutex_estado   = NULL;

static sys_state_t sys_get_state(void)
{
    sys_state_t estado;
    xSemaphoreTake(mutex_estado, portMAX_DELAY);
    estado = estado_sistema;
    xSemaphoreGive(mutex_estado);
    return estado;
}

static void sys_set_state(sys_state_t nuevo_estado)
{
    xSemaphoreTake(mutex_estado, portMAX_DELAY);
    estado_sistema = nuevo_estado;
    xSemaphoreGive(mutex_estado);
}

/* --- Cola de comandos --- */
static QueueHandle_t cola_comandos = NULL;

/* --- Callback de estado BLE --- */
static void on_estado_bt(int conectado)
{
    if (conectado) {
        sys_set_state(SYS_CONNECTED);
        control_leds_set(LED_AZUL, LED_ENCENDIDO);
        ESP_LOGI(TAG, "Estado: SYS_CONNECTED");
    } else {
        sys_set_state(SYS_ADVERTISING);
        control_leds_set(LED_AZUL, LED_PARPADEO_RAPIDO);
        ESP_LOGI(TAG, "Estado: SYS_ADVERTISING");
    }
}

/* --- Nombre del evento para log --- */
static const char *nombre_evento(evento_entrada_t evento)
{
    switch (evento) {
        case EVT_PLAY_PAUSE: return "EVT_PLAY_PAUSE";
        case EVT_SIGUIENTE:  return "EVT_SIGUIENTE";
        case EVT_ANTERIOR:   return "EVT_ANTERIOR";
        case EVT_SILENCIAR:  return "EVT_SILENCIAR";
        case EVT_VOL_SUBIR:  return "EVT_VOL_SUBIR";
        case EVT_VOL_BAJAR:  return "EVT_VOL_BAJAR";
        default:             return "EVT_DESCONOCIDO";
    }
}

/**
 * @brief Tarea de entrada — reenvía eventos del hardware a command_task.
 */
static void input_task(void *arg)
{
    QueueHandle_t cola_entrada = driver_entrada_get_queue();
    evento_entrada_t evento;

    while (1) {
        if (xQueueReceive(cola_entrada, &evento, portMAX_DELAY)) {
            xQueueSend(cola_comandos, &evento, 0);
        }
    }
}

/**
 * @brief Tarea de comandos — procesa eventos y envía reportes HID.
 */
static void command_task(void *arg)
{
    evento_entrada_t evento;

    while (1) {
        if (xQueueReceive(cola_comandos, &evento, portMAX_DELAY)) {

            if (sys_get_state() != SYS_CONNECTED) {
                ESP_LOGW(TAG, "Comando descartado — sin conexión BLE");
                continue;
            }

            ESP_LOGI(TAG, "Enviando: %s", nombre_evento(evento));

            uint16_t uso = 0;
            switch (evento) {
                case EVT_PLAY_PAUSE: uso = 0x00CD; break;
                case EVT_SIGUIENTE:  uso = 0x00B5; break;
                case EVT_ANTERIOR:   uso = 0x00B6; break;
                case EVT_SILENCIAR:  uso = 0x00E2; break;
                case EVT_VOL_SUBIR:  uso = 0x00E9; break;
                case EVT_VOL_BAJAR:  uso = 0x00EA; break;
                default: continue;
            }

            comunicacion_bt_enviar_uso(uso);
        }
    }
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    mutex_estado  = xSemaphoreCreateMutex();
    cola_comandos = xQueueCreate(10, sizeof(evento_entrada_t));

    control_leds_init();
    control_leds_set(LED_AZUL, LED_PARPADEO_RAPIDO);

    driver_entrada_init();
	gestion_energia_init();
    comunicacion_bt_init();
    comunicacion_bt_set_callback_estado(on_estado_bt);

    sys_set_state(SYS_ADVERTISING);
    ESP_LOGI(TAG, "Sistema inicializado — esperando conexión BLE");

    xTaskCreate(input_task,   "input_task",   2048, NULL, TAREA_PRIO_INPUT, NULL);
    xTaskCreate(command_task, "command_task", 2048, NULL, TAREA_PRIO_CMD,   NULL);
}
