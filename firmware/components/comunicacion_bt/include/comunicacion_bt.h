#pragma once

/**
 * @brief Módulo de comunicación Bluetooth HID.
 *
 * Gestiona el stack NimBLE, el perfil HID Consumer Control y la
 * transmisión de reportes multimedia hacia el dispositivo emparejado.
 */

/**
 * @brief Inicializa el módulo de comunicación Bluetooth.
 */
void comunicacion_bt_init(void);
