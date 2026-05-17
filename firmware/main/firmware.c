#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "control_leds.h"
#include "driver_entrada.h"
#include "comunicacion_bt.h"
#include "gestion_energia.h"

/* Tarea de prueba — imprime cada evento recibido */
static void tarea_prueba_botones(void *arg)
{
    QueueHandle_t cola = driver_entrada_get_queue();
    evento_entrada_t evento;

    while (1) {
        if (xQueueReceive(cola, &evento, portMAX_DELAY)) {
            switch (evento) {
                case EVT_SIGUIENTE:
                    ESP_LOGI("prueba", "Evento: EVT_SIGUIENTE (GPIO%d)", GPIO_BOTON_NEXT);
                    control_leds_set(LED_AZUL, LED_ENCENDIDO);
                    vTaskDelay(pdMS_TO_TICKS(300));
                    control_leds_set(LED_AZUL, LED_APAGADO);
                    break;
                case EVT_ANTERIOR:
                    ESP_LOGI("prueba", "Evento: EVT_ANTERIOR (GPIO%d)", GPIO_BOTON_PREV);
                    control_leds_set(LED_AZUL, LED_ENCENDIDO);
                    vTaskDelay(pdMS_TO_TICKS(300));
                    control_leds_set(LED_AZUL, LED_APAGADO);
                    break;
                case EVT_SILENCIAR:
                    ESP_LOGI("prueba", "Evento: EVT_SILENCIAR (GPIO%d)", GPIO_BOTON_MUTE);
                    control_leds_set(LED_ROJO, LED_ENCENDIDO);
                    vTaskDelay(pdMS_TO_TICKS(300));
                    control_leds_set(LED_ROJO, LED_APAGADO);
                    break;
                default:
                    break;
            }
        }
    }
}

void app_main(void)
{
    /* Inicializar módulos del sistema */
    control_leds_init();
    driver_entrada_init();
    comunicacion_bt_init();
    gestion_energia_init();

    /* Crear tarea de prueba */
    xTaskCreate(tarea_prueba_botones, "prueba_botones", 4096, NULL, 5, NULL);
}