#pragma once

/**
 * @brief Módulo driver de entrada.
 *
 * Gestiona la lectura de botones y encoder rotativo mediante interrupciones
 * GPIO con debouncing, y publica eventos en una cola FreeRTOS.
 */

/**
 * @brief Inicializa el módulo driver de entrada.
 */
void driver_entrada_init(void);
