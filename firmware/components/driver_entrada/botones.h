#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/** @brief GPIOs de los botones */
#define GPIO_BOTON_NEXT  22
#define GPIO_BOTON_PREV  23
#define GPIO_BOTON_MUTE  25
#define GPIO_ENCODER_SW  21

/** @brief Tiempo mínimo entre eventos válidos — anti-rebote */
#define DEBOUNCE_TIEMPO_US  50000UL

/**
 * @brief Inicializa los GPIOs de botones y el pulsador del encoder.
 *
 * @param cola Cola donde se publican los eventos de entrada.
 */
void botones_init(QueueHandle_t cola);