/**
 * @file gestion_energia.c
 * @brief Implementación del monitoreo de batería mediante ADC con promediado y calibración.
 */

#include "gestion_energia.h"
#include "pines.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "gestion_energia";

#define UMBRAL_BAJA        3400  /**< Voltaje mínimo nivel OK en mV    */
#define UMBRAL_CRITICA     3200  /**< Voltaje mínimo nivel BAJA en mV  */
#define NUM_MUESTRAS         16  /**< Muestras ADC para promediado     */
#define CANAL_ADC    ADC_CHANNEL_6  /**< Canal ADC1 para PIN_ADC_BATERIA */
#define PERIODO_MONITOREO_MS  30000  /**< Ciclo de monitoreo en ms      */

static adc_oneshot_unit_handle_t adc_handle   = NULL;
static adc_cali_handle_t         adc_cali     = NULL;
static nivel_bateria_t           nivel_actual = BATERIA_OK;
static void (*callback_nivel)(nivel_bateria_t nivel) = NULL;

/**
 * @brief Lee el voltaje de la batería en mV usando el divisor resistivo.
 *
 * Promedia NUM_MUESTRAS lecturas ADC y multiplica por 2 para compensar
 * el divisor resistivo 100kΩ/100kΩ.
 *
 * @return Voltaje estimado de la batería en mV, o 0 si el ADC no está listo.
 */
static int leer_voltaje_bateria(void)
{
    if (adc_handle == NULL || adc_cali == NULL) {
        ESP_LOGW(TAG, "ADC no inicializado");
        return 0;
    }

    int suma = 0;

    for (int i = 0; i < NUM_MUESTRAS; i++) {
        int raw = 0;
        if (adc_oneshot_read(adc_handle, CANAL_ADC, &raw) != ESP_OK) {
            ESP_LOGW(TAG, "Error en lectura ADC — muestra %d descartada", i);
            continue;
        }
        int mv = 0;
        adc_cali_raw_to_voltage(adc_cali, raw, &mv);
        suma += mv;
    }

    return (suma / NUM_MUESTRAS) * 2;
}

/**
 * @brief Evalúa el voltaje y retorna el nivel de batería correspondiente.
 *
 * @param voltaje_mv Voltaje de batería en mV.
 * @return Nivel de batería evaluado.
 */
static nivel_bateria_t evaluar_nivel(int voltaje_mv)
{
    if (voltaje_mv < UMBRAL_CRITICA) return BATERIA_CRITICA;
    if (voltaje_mv < UMBRAL_BAJA)    return BATERIA_BAJA;
    return BATERIA_OK;
}

/**
 * @brief Tarea de monitoreo periódico de batería.
 *
 * Evalúa el nivel cada PERIODO_MONITOREO_MS ms e invoca el callback
 * solo cuando el nivel cambia.
 */
static void power_task(void *arg)
{
    nivel_bateria_t nivel_anterior = BATERIA_OK;

    while (1) {
        int voltaje_mv = leer_voltaje_bateria();
        nivel_actual   = evaluar_nivel(voltaje_mv);

        const char *nivel_str = (nivel_actual == BATERIA_OK)     ? "OK"     :
                                (nivel_actual == BATERIA_BAJA)    ? "BAJA"   : "CRITICA";

        ESP_LOGI(TAG, "Batería: %dmV — Nivel: %s", voltaje_mv, nivel_str);

        if (nivel_actual != nivel_anterior) {
            nivel_anterior = nivel_actual;
            if (callback_nivel) callback_nivel(nivel_actual);
        }

        vTaskDelay(pdMS_TO_TICKS(PERIODO_MONITOREO_MS));
    }
}

void gestion_energia_init(void)
{
    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_cfg, &adc_handle));

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten    = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, CANAL_ADC, &chan_cfg));

    adc_cali_line_fitting_config_t cali_cfg = {
        .unit_id  = ADC_UNIT_1,
        .atten    = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    if (adc_cali_create_scheme_line_fitting(&cali_cfg, &adc_cali) != ESP_OK) {
        ESP_LOGW(TAG, "Calibración no disponible — lecturas sin calibrar");
        adc_cali = NULL;
    }

    BaseType_t ret = xTaskCreate(power_task, "power_task", 2048, NULL, 2, NULL);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Error al crear power_task");
    }

    ESP_LOGI(TAG, "Gestión de energía inicializada — GPIO%d", PIN_ADC_BATERIA);
}

nivel_bateria_t gestion_energia_get_nivel(void)
{
    return nivel_actual;
}

void gestion_energia_set_callback(void (*callback)(nivel_bateria_t nivel))
{
    callback_nivel = callback;
}