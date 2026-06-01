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
#include "comunicacion_bt.h"

/* Valores de protocolo HID */
#define HID_KEY_UP                0x0000  /**< Reporte de tecla liberada — obligatorio tras cada uso */
#define HID_PROTOCOL_MODE_REPORT  0x01    /**< Modo de protocolo: Report Protocol Mode              */

/* UUIDs de servicios GATT */
#define UUID_HID_SERVICE       0x1812  /**< HID Service                 */
#define UUID_DIS_SERVICE       0x180A  /**< Device Information Service  */

/* UUIDs de características GATT */
#define UUID_CHR_PROTOCOL_MODE 0x2A4E  /**< Protocol Mode               */
#define UUID_CHR_REPORT_MAP    0x2A4B  /**< Report Map                  */
#define UUID_CHR_REPORT        0x2A4D  /**< HID Report                  */
#define UUID_CHR_HID_INFO      0x2A4A  /**< HID Information             */
#define UUID_CHR_HID_CONTROL   0x2A4C  /**< HID Control Point           */
#define UUID_CHR_PNP_ID        0x2A50  /**< PnP ID                      */
#define UUID_DSC_REPORT_REF    0x2908  /**< Report Reference descriptor  */

/**
 * @brief Inicializa y registra el servicio GATT HID Consumer Control.
 */
void hid_init(void);

/**
 * @brief Envía un reporte HID Consumer Control al host conectado.
 *
 * Envía el usage_id activo seguido de un key-up (HID_KEY_UP) obligatorio.
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