/**
 * @file control_leds.h
 * @brief Control de LEDs de estado del sistema mediante patrones de iluminación.
 */

#pragma once

#include "pines.h"

#define LED_PERIODO_RAPIDO_US   200000UL   /**< Período de parpadeo rápido: 200ms */
#define LED_PERIODO_LENTO_US   1000000UL   /**< Período de parpadeo lento: 1000ms */

/**
 * @brief Identificadores de los LEDs del sistema.
 */
typedef enum {
    LED_AZUL = PIN_LED_AZUL,  /**< LED de estado Bluetooth */
    LED_ROJO = PIN_LED_ROJO   /**< LED de estado batería   */
} led_id_t;

/**
 * @brief Patrones de iluminación disponibles.
 */
typedef enum {
    LED_APAGADO,         /**< Apagado permanentemente  */
    LED_ENCENDIDO,       /**< Encendido permanentemente */
    LED_PARPADEO_RAPIDO, /**< Parpadeo cada 200ms       */
    LED_PARPADEO_LENTO   /**< Parpadeo cada 1000ms      */
} led_patron_t;

/**
 * @brief Inicializa el módulo de control de LEDs.
 *
 * Configura PIN_LED_AZUL y PIN_LED_ROJO como salidas digitales.
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