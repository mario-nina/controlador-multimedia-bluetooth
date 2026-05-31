#pragma once

/**
 * @brief Módulo de gestión de energía — pendiente Fase 10.
 *
 * Planificado:
 *   - Lectura periódica del voltaje de batería via ADC1 canal 6 (GPIO34)
 *   - Gestión de modos de bajo consumo (light sleep cuando BLE inactivo)
 *   - Indicación de batería baja mediante LED rojo
 *   - Activación del estado SYS_SLEEPING en la FSM principal
 */

/** @brief Inicializa el módulo de gestión de energía. */
void gestion_energia_init(void);
