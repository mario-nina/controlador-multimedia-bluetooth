/**
 * @file botones.c
 * @brief Implementación del driver de botones con debouncing por timestamp.
 */

#include "botones.h"
#include "driver_entrada.h"
#include "pines.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"

static const char *TAG = "botones";

static QueueHandle_t cola_eventos = NULL;

/* Tabla de debouncing — indexada por número de GPIO */
static volatile int64_t ultimo_evento[40] = {0};

static void IRAM_ATTR isr_boton(void *arg)
{
    int gpio      = (int)arg;
    int64_t ahora = esp_timer_get_time();

    if ((ahora - ultimo_evento[gpio]) < BOTON_DEBOUNCE_US) return;
    ultimo_evento[gpio] = ahora;

    evento_entrada_t evento;
    switch (gpio) {
        case PIN_BOTON_NEXT:        evento = EVT_SIGUIENTE;   break;
        case PIN_BOTON_PREV:        evento = EVT_ANTERIOR;    break;
        case PIN_BOTON_MUTE_UNMUTE: evento = EVT_MUTE_UNMUTE; break;
        case PIN_ENCODER_SW:        evento = EVT_PLAY_PAUSE;  break;
        default: return;
    }

    BaseType_t debe_cambiar_contexto = pdFALSE;
    xQueueSendFromISR(cola_eventos, &evento, &debe_cambiar_contexto);
    if (debe_cambiar_contexto) portYIELD_FROM_ISR();
}

void botones_init(QueueHandle_t cola)
{
    cola_eventos = cola;

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_BOTON_NEXT)        |
                        (1ULL << PIN_BOTON_PREV)        |
                        (1ULL << PIN_BOTON_MUTE_UNMUTE) |
                        (1ULL << PIN_ENCODER_SW),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_NEGEDGE,
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_BOTON_NEXT,        isr_boton, (void *)PIN_BOTON_NEXT));
    ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_BOTON_PREV,        isr_boton, (void *)PIN_BOTON_PREV));
    ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_BOTON_MUTE_UNMUTE, isr_boton, (void *)PIN_BOTON_MUTE_UNMUTE));
    ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_ENCODER_SW,        isr_boton, (void *)PIN_ENCODER_SW));

    ESP_LOGI(TAG, "Botones inicializados — GPIO%d GPIO%d GPIO%d GPIO%d",
             PIN_BOTON_NEXT, PIN_BOTON_PREV, PIN_BOTON_MUTE_UNMUTE, PIN_ENCODER_SW);
}