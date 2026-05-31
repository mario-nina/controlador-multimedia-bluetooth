#include "gestion_energia.h"
#include "esp_log.h"

static const char *TAG = "gestion_energia";

/* TODO Fase 10:
 *   - Inicializar ADC1 canal 6 (GPIO34) para lectura de voltaje de batería
 *   - Crear tarea FreeRTOS de monitoreo periódico
 *   - Implementar lógica de light sleep cuando BLE esté inactivo
 *   - Activar estado SYS_SLEEPING en la FSM principal
 */

void gestion_energia_init(void)
{
    ESP_LOGI(TAG, "Módulo de gestión de energía — pendiente Fase 10");
}
