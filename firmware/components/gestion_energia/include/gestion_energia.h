#pragma once

/**
 * @brief Niveles de batería del sistema.
 */
typedef enum {
    BATERIA_OK,      /**< Voltaje > 3600mV */
    BATERIA_BAJA,    /**< Voltaje < 3400mV */
    BATERIA_CRITICA, /**< Voltaje < 3200mV */
} nivel_bateria_t;

/**
 * @brief Inicializa el módulo de gestión de energía.
 *
 * Configura el ADC1 canal 6 (GPIO34) para lectura del divisor de batería.
 * Crea la tarea de monitoreo periódico.
 */
void gestion_energia_init(void);

/**
 * @brief Retorna el nivel de batería actual.
 *
 * @return nivel_bateria_t Nivel evaluado en el último ciclo de monitoreo.
 */
nivel_bateria_t gestion_energia_get_nivel(void);