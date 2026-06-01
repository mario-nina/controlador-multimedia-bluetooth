/**
 * @file driver_entrada.h
 * @brief Driver de entrada del sistema — botones y encoder rotativo.
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "config.h"

/**
 * @brief Eventos de entrada del sistema.
 */
typedef enum {
    EVT_PLAY_PAUSE,  /**< Pulsador del encoder — reproducir/pausar */
    EVT_SIGUIENTE,   /**< Botón siguiente pista                    */
    EVT_ANTERIOR,    /**< Botón pista anterior                     */
    EVT_MUTE_UNMUTE, /**< Botón mute/unmute                        */
    EVT_VOL_SUBIR,   /**< Encoder CW — subir volumen               */
    EVT_VOL_BAJAR,   /**< Encoder CCW — bajar volumen              */
} evento_entrada_t;

/**
 * @brief Inicializa el driver de entrada: botones y encoder.
 *
 * Debe llamarse antes de driver_entrada_get_queue().
 */
void driver_entrada_init(void);

/**
 * @brief Retorna la cola de eventos de entrada.
 *
 * @return Cola de eventos, o NULL si no fue inicializado.
 */
QueueHandle_t driver_entrada_get_queue(void);