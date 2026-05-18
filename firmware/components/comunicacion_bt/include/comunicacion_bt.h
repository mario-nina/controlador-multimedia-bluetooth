#pragma once

#include <stdint.h>

/**
 * @brief Inicializa el stack NimBLE y comienza advertising BLE.
 */
void comunicacion_bt_init(void);

/**
 * @brief Retorna 1 si hay conexión BLE activa, 0 si no.
 */
int comunicacion_bt_conectado(void);

/**
 * @brief Retorna el handle de la conexión BLE activa.
 *
 * @return uint16_t Connection handle.
 */
uint16_t comunicacion_bt_get_conn_handle(void);
/**
 * @brief Registra un callback que se llama cuando cambia el estado de conexión.
 *
 * @param callback Función que recibe 1 al conectar, 0 al desconectar.
 */
void comunicacion_bt_set_callback_estado(void (*callback)(int conectado));