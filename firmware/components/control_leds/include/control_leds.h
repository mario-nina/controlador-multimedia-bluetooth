#pragma once

#include "driver/gpio.h"

/* Períodos de parpadeo en microsegundos */
#define LED_PERIODO_RAPIDO_US  200000UL  /**< Parpadeo rápido: 200ms  */
#define LED_PERIODO_LENTO_US  1000000UL  /**< Parpadeo lento: 1000ms  */

/**
 * @brief Identificadores de los LEDs del sistema.
 */
typedef enum {
    LED_AZUL = 26, /**< LED de estado Bluetooth (GPIO26) */
    LED_ROJO = 27  /**< LED de estado batería (GPIO27)   */
} led_id_t;

/**
 * @brief Patrones de iluminación disponibles.
 */
typedef enum {
    LED_APAGADO,         /**< LED apagado permanentemente  */
    LED_ENCENDIDO,       /**< LED encendido permanentemente */
    LED_PARPADEO_RAPIDO, /**< Parpadeo cada 200ms           */
    LED_PARPADEO_LENTO   /**< Parpadeo cada 1000ms          */
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