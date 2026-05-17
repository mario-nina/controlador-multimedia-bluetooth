#include "botones.h"
#include "driver_entrada.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"

static const char *TAG = "botones";

static QueueHandle_t cola_eventos = NULL;

static volatile int64_t ultimo_evento_next = 0;
static volatile int64_t ultimo_evento_prev = 0;
static volatile int64_t ultimo_evento_mute = 0;
static volatile int64_t ultimo_evento_sw   = 0;

/**
 * @brief ISR genérica para botones con debouncing por timestamp.
 */
static void IRAM_ATTR isr_boton(void *arg)
{
    int gpio = (int)arg;
    int64_t ahora = esp_timer_get_time();

    volatile int64_t *ultimo = NULL;
    evento_entrada_t evento;

    switch (gpio) {
        case GPIO_BOTON_NEXT:
            ultimo = &ultimo_evento_next;
            evento = EVT_SIGUIENTE;
            break;
        case GPIO_BOTON_PREV:
            ultimo = &ultimo_evento_prev;
            evento = EVT_ANTERIOR;
            break;
        case GPIO_BOTON_MUTE:
            ultimo = &ultimo_evento_mute;
            evento = EVT_SILENCIAR;
            break;
        case GPIO_ENCODER_SW:
            ultimo = &ultimo_evento_sw;
            evento = EVT_PLAY_PAUSE;
            break;
        default:
            return;
    }

    if ((ahora - *ultimo) < DEBOUNCE_TIEMPO_US) {
        return;
    }
    *ultimo = ahora;

    BaseType_t debe_cambiar_contexto = pdFALSE;
    xQueueSendFromISR(cola_eventos, &evento, &debe_cambiar_contexto);
    if (debe_cambiar_contexto) {
        portYIELD_FROM_ISR();
    }
}

void botones_init(QueueHandle_t cola)
{
    cola_eventos = cola;

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << GPIO_BOTON_NEXT) |
                        (1ULL << GPIO_BOTON_PREV) |
                        (1ULL << GPIO_BOTON_MUTE) |
                        (1ULL << GPIO_ENCODER_SW),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_NEGEDGE,
    };
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_BOTON_NEXT, isr_boton, (void *)GPIO_BOTON_NEXT);
    gpio_isr_handler_add(GPIO_BOTON_PREV, isr_boton, (void *)GPIO_BOTON_PREV);
    gpio_isr_handler_add(GPIO_BOTON_MUTE, isr_boton, (void *)GPIO_BOTON_MUTE);
    gpio_isr_handler_add(GPIO_ENCODER_SW, isr_boton, (void *)GPIO_ENCODER_SW);

    ESP_LOGI(TAG, "Botones inicializados — GPIO22 GPIO23 GPIO25 GPIO21");
}