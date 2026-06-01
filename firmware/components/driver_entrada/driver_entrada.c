/**
 * @file driver_entrada.c
 * @brief Implementación del driver de entrada — coordina botones y encoder.
 */

#include "driver_entrada.h"
#include "botones.h"
#include "encoder.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "driver_entrada";

static QueueHandle_t cola_eventos = NULL;

void driver_entrada_init(void)
{
    cola_eventos = xQueueCreate(COLA_ENTRADA_TAM, sizeof(evento_entrada_t));
    if (cola_eventos == NULL) {
        ESP_LOGE(TAG, "Error al crear cola de eventos");
        return;
    }

    /* ISR_FLAGS_DEFAULT: sin flags especiales, prioridad estándar */
    ESP_ERROR_CHECK(gpio_install_isr_service(ISR_FLAGS_DEFAULT));

    botones_init(cola_eventos);
    encoder_init(cola_eventos);

    ESP_LOGI(TAG, "Driver de entrada inicializado");
}

QueueHandle_t driver_entrada_get_queue(void)
{
    return cola_eventos;
}