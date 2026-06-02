/**
 * @file pines.h
 * @brief Mapa centralizado de asignación de pines GPIO del sistema.
 */

#pragma once

/* Botones */
#define PIN_BOTON_NEXT        22  /**< Siguiente pista    */
#define PIN_BOTON_PREV        23  /**< Pista anterior     */
#define PIN_BOTON_MUTE_UNMUTE 25  /**< Mute/Unmute        */

/* Encoder rotativo EC11 */
#define PIN_ENCODER_CLK  18  /**< Señal de reloj          */
#define PIN_ENCODER_DT   19  /**< Señal de datos          */
#define PIN_ENCODER_SW   21  /**< Pulsador del encoder    */

/* LEDs de estado */
#define PIN_LED_AZUL     32  /**< Estado Bluetooth        */
#define PIN_LED_ROJO     33  /**< Estado batería          */

/* ADC */
#define PIN_ADC_BATERIA  34  /**< GPIO del divisor resistivo de batería — ADC1_CH6 */