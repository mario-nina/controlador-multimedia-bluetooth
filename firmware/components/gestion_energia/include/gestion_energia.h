/**
 * @file gestion_energia.h
 * @brief Monitoreo de batería y gestión de niveles de energía del sistema.
 */

#pragma once

#include "esp_adc/adc_oneshot.h"
#include "config.h"

/* Configuración del ADC */
#define CANAL_ADC_BATERIA             ADC_CHANNEL_6  /**< Canal ADC1 correspondiente a GPIO34 — PIN_ADC_BATERIA */
#define BATERIA_NUM_MUESTRAS          16              /**< Muestras ADC para promediado                         */

/* Umbrales de nivel de batería */
#define BATERIA_UMBRAL_BAJA_MV        3400  /**< Voltaje mínimo nivel OK en mV   */
#define BATERIA_UMBRAL_CRITICA_MV     3200  /**< Voltaje mínimo nivel BAJA en mV */

/* Configuración de monitoreo */
#define BATERIA_PERIODO_MONITOREO_MS  10000  /**< Ciclo de monitoreo en ms        */

/**
 * @brief Niveles de batería del sistema.
 */
typedef enum {
    BATERIA_OK,      /**< Voltaje >= 3400mV — operación normal  */
    BATERIA_BAJA,    /**< Voltaje < 3400mV — advertencia        */
    BATERIA_CRITICA, /**< Voltaje < 3200mV — apagado inminente  */
} nivel_bateria_t;

/**
 * @brief Inicializa el módulo de gestión de energía.
 *
 * Configura el ADC1 en PIN_ADC_BATERIA para lectura del divisor resistivo.
 * Crea la tarea de monitoreo periódico cada BATERIA_PERIODO_MONITOREO_MS ms.
 */
void gestion_energia_init(void);

/**
 * @brief Retorna el nivel de batería actual.
 *
 * @return Nivel evaluado en el último ciclo de monitoreo.
 */
nivel_bateria_t gestion_energia_get_nivel(void);

/**
 * @brief Registra un callback invocado cuando cambia el nivel de batería.
 *
 * El callback se llama desde la tarea de monitoreo, no desde una ISR.
 *
 * @param callback Función que recibe el nuevo nivel de batería.
 */
void gestion_energia_set_callback(void (*callback)(nivel_bateria_t nivel));