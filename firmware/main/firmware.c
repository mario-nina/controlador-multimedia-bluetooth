/**
 * @file firmware.c
 * @brief Aplicación principal — coordina subsistemas y máquina de estados.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_pm.h"
#include "nvs_flash.h"

#include "control_leds.h"
#include "driver_entrada.h"
#include "comunicacion_bt.h"
#include "gestion_energia.h"

static const char *TAG = "firmware";

/* Prioridades de tareas */
#define TAREA_PRIO_INPUT  5
#define TAREA_PRIO_CMD    4

/* Códigos de uso HID Consumer Control */
#define HID_PLAY_PAUSE  0x00CD  /**< Reproducir / Pausar      */
#define HID_SIGUIENTE   0x00B5  /**< Siguiente pista           */
#define HID_ANTERIOR    0x00B6  /**< Pista anterior            */
#define HID_SILENCIAR   0x00E2  /**< Silenciar / Activar audio */
#define HID_VOL_SUBIR   0x00E9  /**< Subir volumen             */
#define HID_VOL_BAJAR   0x00EA  /**< Bajar volumen             */

/**
 * @brief Estados operativos del sistema.
 */
typedef enum {
    SYS_ADVERTISING, /**< Sin conexión BLE — anunciando  */
    SYS_CONNECTED,   /**< Conexión BLE activa             */
} sys_state_t;

static sys_state_t       estado_sistema = SYS_ADVERTISING;
static SemaphoreHandle_t mutex_estado   = NULL;
static QueueHandle_t     cola_comandos  = NULL;

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

/**
 * @brief Actualiza los LEDs según el estado BT y el nivel de batería.
 */
static void actualizar_leds(void)
{
    bool            conectado = (sys_get_state() == SYS_CONNECTED);
    nivel_bateria_t nivel     = gestion_energia_get_nivel();

    control_leds_set(LED_AZUL, conectado ? LED_APAGADO : LED_PARPADEO_RAPIDO);

    switch (nivel) {
        case BATERIA_OK:
            control_leds_set(LED_ROJO, LED_APAGADO);
            break;
        case BATERIA_BAJA:
            control_leds_set(LED_ROJO, LED_PARPADEO_LENTO);
            break;
        case BATERIA_CRITICA:
            control_leds_set(LED_ROJO, LED_PARPADEO_RAPIDO);
            break;
    }
}

static void on_estado_bt(bool conectado)
{
    sys_set_state(conectado ? SYS_CONNECTED : SYS_ADVERTISING);
    ESP_LOGI(TAG, "Estado BLE: %s", conectado ? "CONECTADO" : "ADVERTISING");
    actualizar_leds();
}

static void on_nivel_bateria(nivel_bateria_t nivel)
{
    (void)nivel;
    actualizar_leds();
}

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
    (void)arg;
    QueueHandle_t    cola_entrada = driver_entrada_get_queue();
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
    (void)arg;
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
                case EVT_PLAY_PAUSE: uso = HID_PLAY_PAUSE; break;
                case EVT_SIGUIENTE:  uso = HID_SIGUIENTE;  break;
                case EVT_ANTERIOR:   uso = HID_ANTERIOR;   break;
                case EVT_SILENCIAR:  uso = HID_SILENCIAR;  break;
                case EVT_VOL_SUBIR:  uso = HID_VOL_SUBIR;  break;
                case EVT_VOL_BAJAR:  uso = HID_VOL_BAJAR;  break;
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
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    /* Habilitar modem sleep para reducir consumo del radio BLE */
    esp_pm_config_t pm_config = {
        .max_freq_mhz       = 160,
        .min_freq_mhz       = 80,
        .light_sleep_enable = false,
    };
    esp_pm_configure(&pm_config);

    mutex_estado = xSemaphoreCreateMutex();
    if (mutex_estado == NULL) {
        ESP_LOGE(TAG, "Error al crear mutex de estado");
        return;
    }

    cola_comandos = xQueueCreate(10, sizeof(evento_entrada_t));
    if (cola_comandos == NULL) {
        ESP_LOGE(TAG, "Error al crear cola de comandos");
        return;
    }

    control_leds_init();
    actualizar_leds();

    driver_entrada_init();
    gestion_energia_init();
    gestion_energia_set_callback(on_nivel_bateria);
    comunicacion_bt_init();
    comunicacion_bt_set_callback_estado(on_estado_bt);

    ESP_LOGI(TAG, "Sistema inicializado — esperando conexión BLE");

    BaseType_t ok;
    ok = xTaskCreate(input_task,   "input_task",   2048, NULL, TAREA_PRIO_INPUT, NULL);
    if (ok != pdPASS) ESP_LOGE(TAG, "Error al crear input_task");

    ok = xTaskCreate(command_task, "command_task", 2048, NULL, TAREA_PRIO_CMD,   NULL);
    if (ok != pdPASS) ESP_LOGE(TAG, "Error al crear command_task");
}