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

#include "config.h"
#include "control_leds.h"
#include "driver_entrada.h"
#include "comunicacion_bt.h"
#include "gestion_energia.h"

static const char *TAG = "firmware";

/* Tabla de nombres de eventos para logging */
static const char *const NOMBRES_EVENTOS[] = {
    [EVT_PLAY_PAUSE]  = "EVT_PLAY_PAUSE",
    [EVT_SIGUIENTE]   = "EVT_SIGUIENTE",
    [EVT_ANTERIOR]    = "EVT_ANTERIOR",
    [EVT_MUTE_UNMUTE] = "EVT_MUTE_UNMUTE",
    [EVT_VOL_SUBIR]   = "EVT_VOL_SUBIR",
    [EVT_VOL_BAJAR]   = "EVT_VOL_BAJAR",
};

/**
 * @brief Estados operativos del sistema.
 */
typedef enum {
    SYS_ADVERTISING, /**< Sin conexión BLE — anunciando */
    SYS_CONNECTED,   /**< Conexión BLE activa            */
} sys_state_t;

static volatile sys_state_t estado_sistema = SYS_ADVERTISING;
static SemaphoreHandle_t    mutex_estado   = NULL;

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
 *
 * @param conectado true si hay conexión BLE activa.
 * @param nivel     Nivel de batería actual.
 */
static void actualizar_leds(bool conectado, nivel_bateria_t nivel)
{
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
    actualizar_leds(conectado, gestion_energia_get_nivel());
}

static void on_nivel_bateria(nivel_bateria_t nivel)
{
    actualizar_leds(sys_get_state() == SYS_CONNECTED, nivel);
}

/**
 * @brief Tarea de comandos — procesa eventos y envía reportes HID.
 */
static void command_task(void *arg)
{
    (void)arg;
    QueueHandle_t    cola_entrada = driver_entrada_get_queue();
    evento_entrada_t evento;

    while (1) {
        if (xQueueReceive(cola_entrada, &evento, portMAX_DELAY)) {

            if (sys_get_state() != SYS_CONNECTED) {
                ESP_LOGW(TAG, "Comando descartado — sin conexión BLE");
                continue;
            }

            if (evento < (sizeof(NOMBRES_EVENTOS) / sizeof(NOMBRES_EVENTOS[0]))) {
                ESP_LOGI(TAG, "Enviando: %s", NOMBRES_EVENTOS[evento]);
            }

            uint16_t uso = 0;
            switch (evento) {
                case EVT_PLAY_PAUSE:  uso = HID_PLAY_PAUSE;  break;
                case EVT_SIGUIENTE:   uso = HID_SIGUIENTE;    break;
                case EVT_ANTERIOR:    uso = HID_ANTERIOR;     break;
                case EVT_MUTE_UNMUTE: uso = HID_MUTE_UNMUTE;  break;
                case EVT_VOL_SUBIR:   uso = HID_VOL_SUBIR;    break;
                case EVT_VOL_BAJAR:   uso = HID_VOL_BAJAR;    break;
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

    esp_pm_config_t pm_config = {
        .max_freq_mhz       = PM_CPU_FREQ_MAX_MHZ,
        .min_freq_mhz       = PM_CPU_FREQ_MIN_MHZ,
        .light_sleep_enable = false,
    };
    ESP_ERROR_CHECK(esp_pm_configure(&pm_config));

    mutex_estado = xSemaphoreCreateMutex();
    if (mutex_estado == NULL) {
        ESP_LOGE(TAG, "Error al crear mutex de estado");
        return;
    }

    control_leds_init();
    actualizar_leds(false, BATERIA_OK);

    driver_entrada_init();
    gestion_energia_init();
    gestion_energia_set_callback(on_nivel_bateria);
    comunicacion_bt_init();
    comunicacion_bt_set_callback_estado(on_estado_bt);

    ESP_LOGI(TAG, "Sistema inicializado — esperando conexión BLE");

    BaseType_t ok;
    ok = xTaskCreate(command_task, "command_task", TAREA_STACK_SIZE, NULL, TAREA_PRIO_CMD, NULL);
    if (ok != pdPASS) ESP_LOGE(TAG, "Error al crear command_task");
}