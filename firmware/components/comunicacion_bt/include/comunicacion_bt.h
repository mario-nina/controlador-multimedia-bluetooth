#pragma once

/**
 * @brief Inicializa el stack NimBLE y comienza advertising BLE.
 */
void comunicacion_bt_init(void);

/**
 * @brief Retorna 1 si hay conexión BLE activa, 0 si no.
 */
int comunicacion_bt_conectado(void);
