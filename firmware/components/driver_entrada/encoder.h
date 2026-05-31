#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "pines.h"

/**
 * @brief Inicializa el periférico PCNT para el encoder rotativo.
 *
 * @param cola Cola donde se publican los eventos de volumen.
 */
void encoder_init(QueueHandle_t cola);
