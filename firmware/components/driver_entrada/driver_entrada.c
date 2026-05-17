#include "driver_entrada.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"

static const char *TAG = "driver_entrada";

static QueueHandle_t cola_eventos = NULL;

/* Timestamps del último evento válido por pin */
static volatile int64_t ultimo_evento_next = 0;
static volatile int64_t ultimo_evento_prev = 0;
static volatile int64_t ultimo_evento_mute = 0;

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

void driver_entrada_init(void)
{
    cola_eventos = xQueueCreate(10, sizeof(evento_entrada_t));

    /* Configurar GPIOs de botones */
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << GPIO_BOTON_NEXT) |
                        (1ULL << GPIO_BOTON_PREV) |
                        (1ULL << GPIO_BOTON_MUTE),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_NEGEDGE,
    };
    gpio_config(&io_conf);

    /* Instalar servicio ISR y registrar handlers */
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_BOTON_NEXT, isr_boton, (void *)GPIO_BOTON_NEXT);
    gpio_isr_handler_add(GPIO_BOTON_PREV, isr_boton, (void *)GPIO_BOTON_PREV);
    gpio_isr_handler_add(GPIO_BOTON_MUTE, isr_boton, (void *)GPIO_BOTON_MUTE);

    ESP_LOGI(TAG, "Driver de entrada inicializado — botones GPIO%d GPIO%d GPIO%d",
             GPIO_BOTON_NEXT, GPIO_BOTON_PREV, GPIO_BOTON_MUTE);
}

QueueHandle_t driver_entrada_get_queue(void)
{
    return cola_eventos;
}