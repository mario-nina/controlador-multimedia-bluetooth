#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#include "control_leds.h"
#include "driver_entrada.h"
#include "comunicacion_bt.h"
#include "gestion_energia.h"

static const char *TAG = "firmware";

/* --- Prioridades de tareas --- */
#define TAREA_PRIO_INPUT    5
#define TAREA_PRIO_CMD      4
#define TAREA_PRIO_POWER    2

/* --- Máquina de estados del sistema --- */
typedef enum {
    SYS_ADVERTISING, /**< Sin conexión BLE — buscando dispositivo */
    SYS_CONNECTED,   /**< Conexión BLE establecida */
    SYS_SLEEPING,    /**< Modo de bajo consumo activo */
} sys_state_t;

static sys_state_t     estado_sistema = SYS_ADVERTISING;
static SemaphoreHandle_t mutex_estado = NULL;

/**
 * @brief Obtiene el estado actual del sistema de forma segura.
 *
 * @return sys_state_t Estado actual.
 */
static sys_state_t sys_get_state(void)
{
    sys_state_t estado;
    xSemaphoreTake(mutex_estado, portMAX_DELAY);
    estado = estado_sistema;
    xSemaphoreGive(mutex_estado);
    return estado;
}

/**
 * @brief Actualiza el estado del sistema de forma segura.
 *
 * @param nuevo_estado Nuevo estado a establecer.
 */
static void sys_set_state(sys_state_t nuevo_estado)
{
    xSemaphoreTake(mutex_estado, portMAX_DELAY);
    estado_sistema = nuevo_estado;
    xSemaphoreGive(mutex_estado);
}

/* --- Colas de tareas --- */
static QueueHandle_t cola_comandos = NULL;

/* --- Nombres de estados para log --- */
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
 * @brief Tarea de entrada — recibe eventos del hardware y los reenvía a command_task.
 */
static void input_task(void *arg)
{
    QueueHandle_t cola_entrada = driver_entrada_get_queue();
    evento_entrada_t evento;

    while (1) {
        if (xQueueReceive(cola_entrada, &evento, portMAX_DELAY)) {
            ESP_LOGD(TAG, "input_task: evento recibido — %s", nombre_evento(evento));
            xQueueSend(cola_comandos, &evento, 0);
        }
    }
}

/**
 * @brief Tarea de comandos — procesa eventos según el estado del sistema.
 */
static void command_task(void *arg)
{
    evento_entrada_t evento;

    while (1) {
        if (xQueueReceive(cola_comandos, &evento, portMAX_DELAY)) {
            sys_state_t estado = sys_get_state();

            if (estado != SYS_CONNECTED) {
                ESP_LOGW(TAG, "command_task: comando descartado — sin conexión BLE");
                continue;
            }

            ESP_LOGI(TAG, "command_task: procesando — %s", nombre_evento(evento));

            /* Fase 8: reemplazar ESP_LOGI por ble_hid_send_report() */
        }
    }
}

void app_main(void)
{
    mutex_estado  = xSemaphoreCreateMutex();
    cola_comandos = xQueueCreate(10, sizeof(evento_entrada_t));

    control_leds_init();
    driver_entrada_init();
    comunicacion_bt_init();
    gestion_energia_init();

    /* Estado inicial: sin conexión BLE */
    sys_set_state(SYS_ADVERTISING);
    ESP_LOGI(TAG, "Estado inicial: SYS_ADVERTISING");

    xTaskCreate(input_task,   "input_task",   2048, NULL, TAREA_PRIO_INPUT, NULL);
    xTaskCreate(command_task, "command_task", 2048, NULL, TAREA_PRIO_CMD,   NULL);

    ESP_LOGI(TAG, "Sistema inicializado — tareas FreeRTOS activas");
}