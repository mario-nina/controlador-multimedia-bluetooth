#include "gestion_energia.h"
#include "pines.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "gestion_energia";

/* --- Umbrales de batería en mV --- */
#define UMBRAL_BAJA     3400
#define UMBRAL_CRITICA  3200

/* --- Número de muestras para promediado --- */
#define NUM_MUESTRAS  16

static adc_oneshot_unit_handle_t adc_handle   = NULL;
static adc_cali_handle_t         adc_cali     = NULL;
static nivel_bateria_t           nivel_actual = BATERIA_OK;

/**
 * @brief Lee el voltaje de la batería en mV usando el divisor resistivo.
 *
 * El divisor 100kΩ/100kΩ divide el voltaje a la mitad.
 * Se multiplica por 2 para obtener el voltaje real de la batería.
 *
 * @return Voltaje de batería en mV.
 */
static int leer_voltaje_bateria(void)
{
    int suma = 0;

    for (int i = 0; i < NUM_MUESTRAS; i++) {
        int raw = 0;
        adc_oneshot_read(adc_handle, ADC_CHANNEL_6, &raw);
        int mv = 0;
        adc_cali_raw_to_voltage(adc_cali, raw, &mv);
        suma += mv;
    }

    int promedio_mv = suma / NUM_MUESTRAS;
    return promedio_mv * 2;
}

/**
 * @brief Evalúa el voltaje y retorna el nivel de batería correspondiente.
 */
static nivel_bateria_t evaluar_nivel(int voltaje_mv)
{
    if (voltaje_mv < UMBRAL_CRITICA) {
        return BATERIA_CRITICA;
    } else if (voltaje_mv < UMBRAL_BAJA) {
        return BATERIA_BAJA;
    }
    return BATERIA_OK;
}

/**
 * @brief Tarea de monitoreo periódico de batería.
 */
static void power_task(void *arg)
{
    while (1) {
        int voltaje_mv = leer_voltaje_bateria();
        nivel_actual   = evaluar_nivel(voltaje_mv);

        const char *nivel_str = (nivel_actual == BATERIA_OK)      ? "OK" :
                                (nivel_actual == BATERIA_BAJA)     ? "BAJA" : "CRITICA";

        ESP_LOGI(TAG, "Batería: %dmV — Nivel: %s", voltaje_mv, nivel_str);

        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}

void gestion_energia_init(void)
{
    /* Configurar ADC1 */
    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_cfg, &adc_handle));

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten    = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_6, &chan_cfg));

    /* Calibración */
    adc_cali_line_fitting_config_t cali_cfg = {
        .unit_id  = ADC_UNIT_1,
        .atten    = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    esp_err_t ret = adc_cali_create_scheme_line_fitting(&cali_cfg, &adc_cali);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Calibración no disponible — usando valores sin calibrar");
        adc_cali = NULL;
    }

    /* Crear tarea de monitoreo */
    xTaskCreate(power_task, "power_task", 2048, NULL, 2, NULL);

    ESP_LOGI(TAG, "Gestión de energía inicializada — GPIO%d", PIN_ADC_BATERIA);
}

nivel_bateria_t gestion_energia_get_nivel(void)
{
    return nivel_actual;
}