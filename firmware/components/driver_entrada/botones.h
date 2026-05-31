#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "pines.h"

/** @brief Tiempo mínimo entre eventos válidos — anti-rebote */
#define DEBOUNCE_TIEMPO_US  150000UL

/**
 * @brief Inicializa los GPIOs de botones y el pulsador del encoder.
 *
 * @param cola Cola donde se publican los eventos de entrada.
 */
void botones_init(QueueHandle_t cola);
