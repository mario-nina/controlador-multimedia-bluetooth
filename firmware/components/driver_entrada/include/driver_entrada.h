#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/* GPIOs de los botones */
#define GPIO_BOTON_NEXT  22  /**< Botón siguiente pista */
#define GPIO_BOTON_PREV  23  /**< Botón pista anterior  */
#define GPIO_BOTON_MUTE  25  /**< Botón silenciar       */

/* Tiempo mínimo entre eventos válidos en microsegundos */
#define DEBOUNCE_TIEMPO_US  50000UL  /**< 50ms de debouncing */

/**
 * @brief Tipos de eventos de entrada del sistema.
 */
typedef enum {
    EVT_PLAY_PAUSE, /**< Encoder presionado — reproducir/pausar */
    EVT_SIGUIENTE,  /**< Botón Next — siguiente pista           */
    EVT_ANTERIOR,   /**< Botón Previous — pista anterior        */
    EVT_SILENCIAR,  /**< Botón Mute — silenciar/activar         */
    EVT_VOL_SUBIR,  /**< Encoder CW — subir volumen             */
    EVT_VOL_BAJAR,  /**< Encoder CCW — bajar volumen            */
} evento_entrada_t;

/**
 * @brief Inicializa el driver de entrada.
 *
 * Configura los GPIOs de botones con pull-up interno e interrupciones
 * por flanco de bajada. Crea la cola de eventos de entrada.
 */
void driver_entrada_init(void);

/**
 * @brief Retorna el handle de la cola de eventos de entrada.
 *
 * @return QueueHandle_t Handle de la cola FreeRTOS.
 */
QueueHandle_t driver_entrada_get_queue(void);