#include "control_leds.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"

static const char *TAG = "control_leds";

static esp_timer_handle_t timer_led_azul = NULL;
static esp_timer_handle_t timer_led_rojo = NULL;

static uint8_t estado_led_azul = 0;
static uint8_t estado_led_rojo = 0;

static void callback_timer_azul(void *arg)
{
    estado_led_azul = !estado_led_azul;
    gpio_set_level(PIN_LED_AZUL, estado_led_azul);
}

static void callback_timer_rojo(void *arg)
{
    estado_led_rojo = !estado_led_rojo;
    gpio_set_level(PIN_LED_ROJO, estado_led_rojo);
}

static void detener_timer(led_id_t led)
{
    if (led == LED_AZUL && timer_led_azul != NULL) {
        esp_timer_stop(timer_led_azul);
        esp_timer_delete(timer_led_azul);
        timer_led_azul = NULL;
    } else if (led == LED_ROJO && timer_led_rojo != NULL) {
        esp_timer_stop(timer_led_rojo);
        esp_timer_delete(timer_led_rojo);
        timer_led_rojo = NULL;
    }
    gpio_set_level(led, 0);
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
    gpio_config(&io_conf);

    gpio_set_level(PIN_LED_AZUL, 0);
    gpio_set_level(PIN_LED_ROJO, 0);

    ESP_LOGI(TAG, "Módulo de control de LEDs inicializado");
}

void control_leds_set(led_id_t led, led_patron_t patron)
{
    detener_timer(led);

    switch (patron) {
        case LED_APAGADO:
            gpio_set_level(led, 0);
            break;

        case LED_ENCENDIDO:
            gpio_set_level(led, 1);
            break;

        case LED_PARPADEO_RAPIDO:
        case LED_PARPADEO_LENTO: {
            uint64_t periodo_us = (patron == LED_PARPADEO_RAPIDO)
                                  ? LED_PERIODO_RAPIDO_US
                                  : LED_PERIODO_LENTO_US;

            esp_timer_create_args_t args = {
                .callback = (led == LED_AZUL) ? callback_timer_azul : callback_timer_rojo,
                .arg      = NULL,
                .name     = (led == LED_AZUL) ? "timer_azul" : "timer_rojo",
            };

            esp_timer_handle_t *handle = (led == LED_AZUL) ? &timer_led_azul : &timer_led_rojo;
            esp_timer_create(&args, handle);
            esp_timer_start_periodic(*handle, periodo_us);
            break;
        }

        default:
            ESP_LOGW(TAG, "Patrón desconocido: %d", patron);
            break;
    }
}