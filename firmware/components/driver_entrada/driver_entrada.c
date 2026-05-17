#include "driver_entrada.h"
#include "botones.h"
#include "encoder.h"
#include "esp_log.h"

static const char *TAG = "driver_entrada";

static QueueHandle_t cola_eventos = NULL;

void driver_entrada_init(void)
{
    cola_eventos = xQueueCreate(10, sizeof(evento_entrada_t));

    botones_init(cola_eventos);
    encoder_init(cola_eventos);

    ESP_LOGI(TAG, "Driver de entrada inicializado");
}

QueueHandle_t driver_entrada_get_queue(void)
{
    return cola_eventos;
}