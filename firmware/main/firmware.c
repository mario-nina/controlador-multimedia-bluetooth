#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_AZUL_GPIO 26
#define BLINK_PERIOD_MS 500

void app_main(void)
{
    /* Configurar GPIO26 como salida */
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_AZUL_GPIO),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    /* Blink indefinido */
    while (1) {
        gpio_set_level(LED_AZUL_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
        gpio_set_level(LED_AZUL_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
    }
}