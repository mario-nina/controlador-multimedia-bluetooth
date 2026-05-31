/**
 * @file encoder.h
 * @brief Driver interno del encoder rotativo EC11 mediante periférico PCNT.
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

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