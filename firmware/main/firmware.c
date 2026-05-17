#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "control_leds.h"
#include "driver_entrada.h"
#include "comunicacion_bt.h"
#include "gestion_energia.h"

void app_main(void)
{
    /* Inicializar módulos del sistema */
    control_leds_init();
    driver_entrada_init();
    comunicacion_bt_init();
    gestion_energia_init();

    /* Secuencia de prueba — validación física de GPIO26 y GPIO27 */
    control_leds_set(LED_AZUL, LED_PARPADEO_RAPIDO);
    vTaskDelay(pdMS_TO_TICKS(3000));

    control_leds_set(LED_AZUL, LED_ENCENDIDO);
    vTaskDelay(pdMS_TO_TICKS(3000));

    control_leds_set(LED_ROJO, LED_PARPADEO_LENTO);
    vTaskDelay(pdMS_TO_TICKS(3000));

    control_leds_set(LED_ROJO, LED_PARPADEO_RAPIDO);
    vTaskDelay(pdMS_TO_TICKS(3000));

    control_leds_set(LED_AZUL, LED_APAGADO);
    control_leds_set(LED_ROJO, LED_APAGADO);
}