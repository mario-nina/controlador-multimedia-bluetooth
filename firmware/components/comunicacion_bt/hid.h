#pragma once

#include <stdint.h>

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

/**
 * @brief Actualiza el estado de suscripción de notificaciones.
 *
 * @param activas 1 si Ubuntu activó notificaciones, 0 si las desactivó.
 */
void hid_set_notificaciones(int activas);