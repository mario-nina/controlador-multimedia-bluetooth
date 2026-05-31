/**
 * @file control_leds.c
 * @brief Implementación del control de LEDs mediante patrones de iluminación.
 */

#include "control_leds.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"

static const char *TAG = "control_leds";

/* Estado interno de cada LED */
typedef struct {
    esp_timer_handle_t timer;
    uint8_t            nivel;
    int                pin;
} led_state_t;

static led_state_t leds[2] = {
    [0] = { .timer = NULL, .nivel = 0, .pin = PIN_LED_AZUL },
    [1] = { .timer = NULL, .nivel = 0, .pin = PIN_LED_ROJO },
};

/**
 * @brief Retorna el índice interno del LED.
 */
static int led_index(led_id_t led)
{
    return (led == LED_AZUL) ? 0 : 1;
}

/**
 * @brief Callback genérico de timer — alterna el nivel del LED.
 */
static void callback_timer(void *arg)
{
    led_state_t *state = (led_state_t *)arg;
    state->nivel = !state->nivel;
    gpio_set_level(state->pin, state->nivel);
}

/**
 * @brief Detiene el timer activo de un LED sin modificar el nivel GPIO.
 */
static void detener_timer(led_id_t led)
{
    led_state_t *state = &leds[led_index(led)];

    if (state->timer != NULL) {
        esp_timer_stop(state->timer);
        esp_timer_delete(state->timer);
        state->timer = NULL;
    }
}

void control_leds_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_LED_AZUL) | (1ULL << PIN_LED_ROJO),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    gpio_set_level(PIN_LED_AZUL, 0);
    gpio_set_level(PIN_LED_ROJO, 0);

    ESP_LOGI(TAG, "LEDs inicializados — GPIO%d GPIO%d", PIN_LED_AZUL, PIN_LED_ROJO);
}

void control_leds_set(led_id_t led, led_patron_t patron)
{
    led_state_t *state = &leds[led_index(led)];

    detener_timer(led);

    switch (patron) {
        case LED_APAGADO:
            state->nivel = 0;
            gpio_set_level(state->pin, 0);
            break;

        case LED_ENCENDIDO:
            state->nivel = 1;
            gpio_set_level(state->pin, 1);
            break;

        case LED_PARPADEO_RAPIDO:
        case LED_PARPADEO_LENTO: {
            uint64_t periodo_us = (patron == LED_PARPADEO_RAPIDO)
                                  ? LED_PERIODO_RAPIDO_US
                                  : LED_PERIODO_LENTO_US;

            esp_timer_create_args_t args = {
                .callback = callback_timer,
                .arg      = state,
                .name     = (led == LED_AZUL) ? "timer_azul" : "timer_rojo",
            };

            ESP_ERROR_CHECK(esp_timer_create(&args, &state->timer));
            ESP_ERROR_CHECK(esp_timer_start_periodic(state->timer, periodo_us));
            break;
        }

        default:
            ESP_LOGW(TAG, "Patrón desconocido: %d", patron);
            break;
    }
}