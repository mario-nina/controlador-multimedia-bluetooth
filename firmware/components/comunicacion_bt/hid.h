#pragma once

#include "host/ble_hs.h"

/**
 * @brief Inicializa el servicio GATT HID Consumer Control.
 */
void hid_init(void);

/**
 * @brief Envía un reporte HID Consumer Control.
 *
 * @param usage_id Código de uso HID (ej. 0x00CD para Play/Pause).
 * @param conn_handle Handle de la conexión BLE activa.
 */
void hid_send_report(uint16_t usage_id, uint16_t conn_handle);