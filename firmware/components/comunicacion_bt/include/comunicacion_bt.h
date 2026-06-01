/**
 * @file comunicacion_bt.h
 * @brief Interfaz pública del módulo de comunicación Bluetooth BLE HID.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/* Configuración del dispositivo BLE */
#define BT_NOMBRE_DISPOSITIVO  "Controlador Multimedia"  /**< Nombre visible en el emparejamiento BLE */
#define BT_APPEARANCE_HID       0x03C4                   /**< Appearance HID Keyboard/Remote          */

/* Códigos de uso HID Consumer Control */
#define HID_PLAY_PAUSE   0x00CD  /**< Reproducir/pausar  */
#define HID_SIGUIENTE    0x00B5  /**< Siguiente pista     */
#define HID_ANTERIOR     0x00B6  /**< Pista anterior      */
#define HID_MUTE_UNMUTE  0x00E2  /**< Mute/unmute         */
#define HID_VOL_SUBIR    0x00E9  /**< Subir volumen       */
#define HID_VOL_BAJAR    0x00EA  /**< Bajar volumen       */

/**
 * @brief Inicializa el stack NimBLE y comienza advertising BLE.
 *
 * @pre nvs_flash_init() debe haberse llamado exitosamente antes.
 */
void comunicacion_bt_init(void);

/**
 * @brief Indica si hay una conexión BLE activa.
 *
 * @return true si hay conexión activa, false si no.
 */
bool comunicacion_bt_conectado(void);

/**
 * @brief Envía un código de uso HID Consumer Control al dispositivo conectado.
 *
 * Si no hay conexión activa el comando se descarta silenciosamente.
 *
 * @param hid_uso Código de uso HID.
 */
void comunicacion_bt_enviar_uso(uint16_t hid_uso);

/**
 * @brief Registra un callback invocado cuando cambia el estado de conexión.
 *
 * El callback se llama desde el contexto de la tarea NimBLE.
 *
 * @param callback Función que recibe true al conectar, false al desconectar.
 */
void comunicacion_bt_set_callback_estado(void (*callback)(bool conectado));