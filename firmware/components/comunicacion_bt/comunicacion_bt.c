/**
 * @file comunicacion_bt.c
 * @brief Implementación del módulo BLE HID mediante stack NimBLE.
 */

#include "comunicacion_bt.h"
#include "hid.h"
#include "esp_log.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "services/gap/ble_svc_gap.h"

static const char *TAG = "comunicacion_bt";

#define NOMBRE_DISPOSITIVO    "Controlador Multimedia"
#define BLE_APPEARANCE_HID    0x03C4  /**< HID Keyboard/Remote appearance */

static uint16_t conn_handle                    = BLE_HS_CONN_HANDLE_NONE;
static bool     bt_conectado                   = false;
static void   (*callback_estado)(bool conectado) = NULL;

static void iniciar_advertising(void);

/**
 * @brief Callback de eventos GAP — gestiona conexión y desconexión.
 */
static int callback_gap(struct ble_gap_event *event, void *arg)
{
    switch (event->type) {

        case BLE_GAP_EVENT_CONNECT:
            if (event->connect.status == 0) {
                conn_handle  = event->connect.conn_handle;
                bt_conectado = true;
                hid_set_notificaciones(true);
                ESP_LOGI(TAG, "Dispositivo conectado — handle: %d", conn_handle);
                if (callback_estado) callback_estado(true);
            } else {
                ESP_LOGW(TAG, "Conexión fallida — reiniciando advertising");
                bt_conectado = false;
                iniciar_advertising();
            }
            break;

        case BLE_GAP_EVENT_DISCONNECT:
            conn_handle  = BLE_HS_CONN_HANDLE_NONE;
            bt_conectado = false;
            hid_set_notificaciones(false);
            ESP_LOGI(TAG, "Dispositivo desconectado — reiniciando advertising");
            if (callback_estado) callback_estado(false);
            iniciar_advertising();
            break;

        case BLE_GAP_EVENT_SUBSCRIBE:
            ESP_LOGI(TAG, "Suscripción — handle:%d notify:%d",
                     event->subscribe.attr_handle,
                     event->subscribe.cur_notify);
            hid_set_notificaciones(event->subscribe.cur_notify);
            break;

        case BLE_GAP_EVENT_MTU:
            ESP_LOGI(TAG, "MTU negociado: %d", event->mtu.value);
            break;

        default:
            break;
    }

    return 0;
}

/**
 * @brief Inicia el advertising BLE con nombre y appearance configurados.
 */
static void iniciar_advertising(void)
{
    struct ble_gap_adv_params params = {0};
    struct ble_hs_adv_fields  fields = {0};

    fields.flags               = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.name                = (uint8_t *)NOMBRE_DISPOSITIVO;
    fields.name_len            = strlen(NOMBRE_DISPOSITIVO);
    fields.name_is_complete    = 1;
    fields.appearance          = BLE_APPEARANCE_HID;
    fields.appearance_is_present = 1;

    int rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        ESP_LOGE(TAG, "Error al configurar advertising fields: %d", rc);
        return;
    }

    params.conn_mode = BLE_GAP_CONN_MODE_UND;
    params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    rc = ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER,
                           &params, callback_gap, NULL);
    if (rc != 0) {
        ESP_LOGE(TAG, "Error al iniciar advertising: %d", rc);
    } else {
        ESP_LOGI(TAG, "Advertising iniciado — \"%s\"", NOMBRE_DISPOSITIVO);
    }
}

/**
 * @brief Callback ejecutado cuando el stack NimBLE está sincronizado y listo.
 */
static void on_stack_listo(void)
{
    ESP_LOGI(TAG, "Stack NimBLE listo");
    iniciar_advertising();
}

/**
 * @brief Tarea del host NimBLE — requerida por la arquitectura del stack.
 */
static void tarea_host_nimble(void *param)
{
    ESP_LOGI(TAG, "Tarea host NimBLE iniciada");
    nimble_port_run();
    nimble_port_freertos_deinit();
}

void comunicacion_bt_init(void)
{
    nimble_port_init();

    ble_hs_cfg.sync_cb = on_stack_listo;

    ble_svc_gap_device_name_set(NOMBRE_DISPOSITIVO);
    hid_init();

    nimble_port_freertos_init(tarea_host_nimble);

    ESP_LOGI(TAG, "Módulo Bluetooth inicializado — NimBLE");
}

bool comunicacion_bt_conectado(void)
{
    return bt_conectado;
}

void comunicacion_bt_enviar_uso(uint16_t hid_uso)
{
    if (!bt_conectado) {
        ESP_LOGW(TAG, "Envío descartado — sin conexión BLE");
        return;
    }
    hid_send_report(hid_uso, conn_handle);
}

void comunicacion_bt_set_callback_estado(void (*callback)(bool conectado))
{
    callback_estado = callback;
}