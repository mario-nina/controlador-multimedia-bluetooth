/**
 * @file encoder.h
 * @brief Driver interno del encoder rotativo EC11 mediante periférico PCNT.
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define ENCODER_WATCHPOINT       4        /**< Pulsos de cuadratura por evento — 1 detente físico EC11 */
#define ENCODER_DEBOUNCE_US      50000UL  /**< Intervalo mínimo entre pulsos válidos en µs             */
#define ENCODER_FILTRO_GLITCH_NS 10000UL  /**< Duración máxima de glitch ignorado por hardware en ns   */

/**
 * @brief Inicializa el periférico PCNT para el encoder rotativo.
 *
 * Configura PIN_ENCODER_CLK y PIN_ENCODER_DT con decodificación
 * de cuadratura en hardware. Publica EVT_VOL_SUBIR y EVT_VOL_BAJAR
 * en la cola según la dirección de giro.
 *
 * @param cola Cola donde se publican los eventos de volumen.
 */
void encoder_init(QueueHandle_t cola);