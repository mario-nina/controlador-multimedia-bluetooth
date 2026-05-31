#pragma once

#include "pines.h"
#include "driver/gpio.h"

#define LED_PERIODO_RAPIDO_US   200000UL
#define LED_PERIODO_LENTO_US   1000000UL

typedef enum {
    LED_AZUL = PIN_LED_AZUL,
    LED_ROJO = PIN_LED_ROJO
} led_id_t;

typedef enum {
    LED_APAGADO,
    LED_ENCENDIDO,
    LED_PARPADEO_RAPIDO,
    LED_PARPADEO_LENTO
} led_patron_t;

/**
 * @brief Inicializa el módulo de control de LEDs.
 *
 * Configura GPIO26 y GPIO27 como salidas digitales.
 * Ambos LEDs inician apagados.
 */
void control_leds_init(void);

/**
 * @brief Aplica un patrón de iluminación a un LED.
 *
 * @param led    Identificador del LED a controlar.
 * @param patron Patrón de iluminación a aplicar.
 */
void control_leds_set(led_id_t led, led_patron_t patron);