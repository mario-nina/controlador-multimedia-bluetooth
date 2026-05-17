#include "encoder.h"
#include "driver_entrada.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "driver/pulse_cnt.h"

static const char *TAG = "encoder";

static QueueHandle_t cola_eventos = NULL;
static pcnt_unit_handle_t pcnt_unit = NULL;

/**
 * @brief Callback del PCNT — dispara cuando el contador alcanza un watch point.
 */
static bool callback_pcnt(pcnt_unit_handle_t unit,
                           const pcnt_watch_event_data_t *edata,
                           void *user_ctx)
{
    static volatile int      ultimo_valor  = 0;
    static volatile int64_t  ultimo_tiempo = 0;

    int64_t ahora = esp_timer_get_time();
    int valor = edata->watch_point_value;

    /* Descartar si cambió de dirección en menos de 50ms */
    if ((valor != ultimo_valor) && ((ahora - ultimo_tiempo) < 50000)) {
        return pdFALSE;
    }

    ultimo_valor  = valor;
    ultimo_tiempo = ahora;

    BaseType_t debe_cambiar_contexto = pdFALSE;
    QueueHandle_t cola = (QueueHandle_t)user_ctx;

    evento_entrada_t evento = (valor > 0) ? EVT_VOL_SUBIR : EVT_VOL_BAJAR;

    xQueueSendFromISR(cola, &evento, &debe_cambiar_contexto);
    return (debe_cambiar_contexto == pdTRUE);
}

void encoder_init(QueueHandle_t cola)
{
    cola_eventos = cola;

    gpio_set_pull_mode(GPIO_ENCODER_CLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_ENCODER_DT,  GPIO_PULLUP_ONLY);

    pcnt_unit_config_t unit_config = {
        .high_limit =  2,
        .low_limit  = -2,
    };
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 10000,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num  = GPIO_ENCODER_CLK,
        .level_gpio_num = GPIO_ENCODER_DT,
    };
    pcnt_channel_handle_t chan_a = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_a_config, &chan_a));

    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num  = GPIO_ENCODER_DT,
        .level_gpio_num = GPIO_ENCODER_CLK,
    };
    pcnt_channel_handle_t chan_b = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_b_config, &chan_b));

    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(chan_a,
        PCNT_CHANNEL_EDGE_ACTION_DECREASE,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(chan_a,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(chan_b,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE,
        PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(chan_b,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit,  2));
    ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, -2));

    pcnt_event_callbacks_t cbs = {
        .on_reach = callback_pcnt,
    };
    ESP_ERROR_CHECK(pcnt_unit_register_event_callbacks(pcnt_unit, &cbs, cola_eventos));

    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));

    ESP_LOGI(TAG, "Encoder PCNT inicializado — GPIO18 GPIO19");
}