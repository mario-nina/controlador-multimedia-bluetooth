#pragma once

#include <stdint.h>

/**
 * @brief Inicializa el stack NimBLE y comienza advertising BLE.
 *
 * @note NVS Flash debe inicializarse antes de llamar esta función.
 */
void comunicacion_bt_init(void);

/**
 * @brief Retorna 1 si hay conexión BLE activa, 0 si no.
 */
int comunicacion_bt_conectado(void);

/**
 * @brief Envía un uso HID Consumer Control al dispositivo conectado.
 *
 * @param hid_uso Código de uso HID (ej. 0x00CD para Play/Pause).
 */
void comunicacion_bt_enviar_uso(uint16_t hid_uso);

/**
 * @brief Registra un callback que se llama cuando cambia el estado de conexión.
 *
 * @param callback Función que recibe 1 al conectar, 0 al desconectar.
 */
void comunicacion_bt_set_callback_estado(void (*callback)(int conectado));