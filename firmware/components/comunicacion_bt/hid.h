/**
 * @file hid.h
 * @brief Interfaz interna del servicio GATT HID Consumer Control.
 *
 * @note Este header es privado al componente comunicacion_bt.
 *       No debe incluirse desde otros componentes.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Inicializa y registra el servicio GATT HID Consumer Control.
 */
void hid_init(void);

/**
 * @brief Envía un reporte HID Consumer Control al host conectado.
 *
 * Envía el usage_id activo seguido de un key-up (0x0000) obligatorio.
 *
 * @param usage_id    Código de uso HID Consumer Control.
 * @param conn_handle Handle de la conexión BLE activa.
 */
void hid_send_report(uint16_t usage_id, uint16_t conn_handle);

/**
 * @brief Actualiza el estado de suscripción a notificaciones GATT.
 *
 * @param activas true si el host activó notificaciones, false si las desactivó.
 */
void hid_set_notificaciones(bool activas);