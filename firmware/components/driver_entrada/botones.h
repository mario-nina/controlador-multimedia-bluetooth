/**
 * @file botones.h
 * @brief Driver interno de botones táctiles con debouncing por timestamp.
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define BOTON_DEBOUNCE_US  150000UL  /**< Tiempo mínimo entre eventos válidos en µs */

/**
 * @brief Inicializa los GPIOs de botones y el pulsador del encoder.
 *
 * Configura GPIO con pull-up interno e interrupción por flanco descendente.
 * Debe llamarse después de instalar el servicio ISR global.
 *
 * @param cola Cola donde se publican los eventos de entrada.
 */
void botones_init(QueueHandle_t cola);