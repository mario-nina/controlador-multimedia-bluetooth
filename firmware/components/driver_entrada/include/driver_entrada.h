#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/**
 * @brief Eventos de entrada del sistema.
 */
typedef enum {
    EVT_PLAY_PAUSE, /**< Pulsador del encoder — reproducir/pausar */
    EVT_SIGUIENTE,  /**< Botón Next */
    EVT_ANTERIOR,   /**< Botón Previous */
    EVT_SILENCIAR,  /**< Botón Mute */
    EVT_VOL_SUBIR,  /**< Encoder CW — subir volumen */
    EVT_VOL_BAJAR,  /**< Encoder CCW — bajar volumen */
} evento_entrada_t;

/**
 * @brief Inicializa el driver de entrada: botones y encoder.
 */
void driver_entrada_init(void);

/**
 * @brief Retorna la cola de eventos de entrada.
 *
 * @return QueueHandle_t Cola de eventos.
 */
QueueHandle_t driver_entrada_get_queue(void);