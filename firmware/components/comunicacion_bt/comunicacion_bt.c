#include "comunicacion_bt.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "services/gap/ble_svc_gap.h"

static const char *TAG = "comunicacion_bt";

#define NOMBRE_DISPOSITIVO "Controlador Multimedia"

static uint16_t conn_handle = BLE_HS_CONN_HANDLE_NONE;
static int bt_conectado = 0;

/* --- Prototipos internos --- */
static void iniciar_advertising(void);

/**
 * @brief Callback de eventos GAP — conexión y desconexión.
 */
static int callback_gap(struct ble_gap_event *event, void *arg)
{
    switch (event->type) {

        case BLE_GAP_EVENT_CONNECT:
            if (event->connect.status == 0) {
                conn_handle = event->connect.conn_handle;
                bt_conectado = 1;
                ESP_LOGI(TAG, "Dispositivo conectado — handle: %d", conn_handle);
            } else {
                ESP_LOGW(TAG, "Conexión fallida — reiniciando advertising");
                bt_conectado = 0;
                iniciar_advertising();
            }
            break;

        case BLE_GAP_EVENT_DISCONNECT:
            conn_handle = BLE_HS_CONN_HANDLE_NONE;
            bt_conectado = 0;
            ESP_LOGI(TAG, "Dispositivo desconectado — reiniciando advertising");
            iniciar_advertising();
            break;

        default:
            break;
    }

    return 0;
}

/**
 * @brief Inicia el advertising BLE.
 */
static void iniciar_advertising(void)
{
    struct ble_gap_adv_params params = {0};
    struct ble_hs_adv_fields fields = {0};

    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.name = (uint8_t *)NOMBRE_DISPOSITIVO;
    fields.name_len = strlen(NOMBRE_DISPOSITIVO);
    fields.name_is_complete = 1;

    ble_gap_adv_set_fields(&fields);

    params.conn_mode = BLE_GAP_CONN_MODE_UND;
    params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    int rc = ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER,
                               &params, callback_gap, NULL);
    if (rc != 0) {
        ESP_LOGE(TAG, "Error al iniciar advertising: %d", rc);
    } else {
        ESP_LOGI(TAG, "Advertising iniciado — \"%s\"", NOMBRE_DISPOSITIVO);
    }
}

/**
 * @brief Callback ejecutado cuando el stack NimBLE está listo.
 */
static void on_stack_listo(void)
{
    ESP_LOGI(TAG, "Stack NimBLE listo");
    iniciar_advertising();
}

/**
 * @brief Tarea del host NimBLE — requerida por la arquitectura de NimBLE.
 */
static void tarea_host_nimble(void *param)
{
    ESP_LOGI(TAG, "Tarea host NimBLE iniciada");
    nimble_port_run();
    nimble_port_freertos_deinit();
}

void comunicacion_bt_init(void)
{
    /* NVS requerido para guardar claves de bonding */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    nimble_port_init();

    ble_hs_cfg.sync_cb = on_stack_listo;

    ble_svc_gap_device_name_set(NOMBRE_DISPOSITIVO);

    nimble_port_freertos_init(tarea_host_nimble);

    ESP_LOGI(TAG, "Módulo Bluetooth inicializado — NimBLE");
}

int comunicacion_bt_conectado(void)
{
    return bt_conectado;
}
