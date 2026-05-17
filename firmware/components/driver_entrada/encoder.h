#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/** @brief GPIOs del encoder rotativo EC11 */
#define GPIO_ENCODER_CLK  18
#define GPIO_ENCODER_DT   19

/**
 * @brief Inicializa el periférico PCNT para el encoder rotativo.
 *
 * @param cola Cola donde se publican los eventos de volumen.
 */
void encoder_init(QueueHandle_t cola);