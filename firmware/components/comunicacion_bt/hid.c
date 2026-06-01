/**
 * @file hid.c
 * @brief Implementación del servicio GATT HID Consumer Control sobre NimBLE.
 */

#include "hid.h"
#include "esp_log.h"
#include "host/ble_hs.h"
#include "host/ble_gatt.h"

static const char *TAG = "hid";

static uint16_t hid_report_handle     = 0;
static bool     notificaciones_activas = false;

/*
 * Descriptor HID — Consumer Control Page (Usage Page 0x0C)
 * Formato de reporte: 1 byte Report ID + 2 bytes Usage ID (16 bits)
 * Rango de uso: 0x0000 a 0x03FF (Consumer Control usages)
 */
static const uint8_t hid_descriptor[] __attribute__((used)) = {
    0x05, 0x0C,        /* Usage Page: Consumer                */
    0x09, 0x01,        /* Usage: Consumer Control             */
    0xA1, 0x01,        /* Collection: Application             */
    0x85, 0x01,        /*   Report ID: 1                      */
    0x15, 0x00,        /*   Logical Minimum: 0                */
    0x26, 0xFF, 0x03,  /*   Logical Maximum: 1023             */
    0x19, 0x00,        /*   Usage Minimum: 0                  */
    0x2A, 0xFF, 0x03,  /*   Usage Maximum: 1023               */
    0x75, 0x10,        /*   Report Size: 16 bits              */
    0x95, 0x01,        /*   Report Count: 1                   */
    0x81, 0x00,        /*   Input: Data, Array, Absolute      */
    0xC0               /* End Collection                      */
};

static int callback_report_map(uint16_t conn_handle, uint16_t attr_handle,
                                struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)conn_handle; (void)attr_handle; (void)arg;
    return os_mbuf_append(ctxt->om, hid_descriptor, sizeof(hid_descriptor));
}

static int callback_hid_info(uint16_t conn_handle, uint16_t attr_handle,
                              struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)conn_handle; (void)attr_handle; (void)arg;
    static const uint8_t info[] = {
        0x11, 0x01,  /* HID version 1.11   */
        0x00,        /* Country code: none  */
        0x01         /* Flags: remoteable   */
    };
    return os_mbuf_append(ctxt->om, info, sizeof(info));
}

static int callback_hid_report(uint16_t conn_handle, uint16_t attr_handle,
                                struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)conn_handle; (void)attr_handle; (void)ctxt; (void)arg;
    return 0;
}

static int callback_hid_control(uint16_t conn_handle, uint16_t attr_handle,
                                 struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)conn_handle; (void)attr_handle; (void)ctxt; (void)arg;
    return 0;
}

static int callback_report_ref(uint16_t conn_handle, uint16_t attr_handle,
                                struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)conn_handle; (void)attr_handle; (void)arg;
    static const uint8_t ref[] = { 0x01, 0x01 }; /* Report ID=1, Type=Input */
    return os_mbuf_append(ctxt->om, ref, sizeof(ref));
}

static int callback_protocol_mode(uint16_t conn_handle, uint16_t attr_handle,
                                   struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)conn_handle; (void)attr_handle; (void)arg;
    static uint8_t mode = HID_PROTOCOL_MODE_REPORT;
    if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR) {
        return os_mbuf_append(ctxt->om, &mode, sizeof(mode));
    }
    return 0;
}

static int callback_pnp_id(uint16_t conn_handle, uint16_t attr_handle,
                             struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)conn_handle; (void)attr_handle; (void)arg;
    static const uint8_t pnp[] = {
        0x02,        /* Vendor ID source: USB          */
        0xE5, 0x02,  /* Vendor ID: Espressif (0x02E5)  */
        0x00, 0x00,  /* Product ID                     */
        0x00, 0x01,  /* Product version: 0.1           */
    };
    return os_mbuf_append(ctxt->om, pnp, sizeof(pnp));
}

static struct ble_gatt_dsc_def hid_report_dscs[] = {
    {
        .uuid      = BLE_UUID16_DECLARE(UUID_DSC_REPORT_REF),
        .att_flags = BLE_ATT_F_READ,
        .access_cb = callback_report_ref,
    },
    { 0 }
};

static const struct ble_gatt_svc_def hid_gatt_svcs[] = {
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(UUID_HID_SERVICE),
        .characteristics = (struct ble_gatt_chr_def[]) {
            {
                .uuid      = BLE_UUID16_DECLARE(UUID_CHR_PROTOCOL_MODE),
                .access_cb = callback_protocol_mode,
                .flags     = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE_NO_RSP,
            },
            {
                .uuid      = BLE_UUID16_DECLARE(UUID_CHR_REPORT_MAP),
                .access_cb = callback_report_map,
                .flags     = BLE_GATT_CHR_F_READ,
            },
            {
                .uuid        = BLE_UUID16_DECLARE(UUID_CHR_REPORT),
                .access_cb   = callback_hid_report,
                .flags       = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
                .val_handle  = &hid_report_handle,
                .descriptors = hid_report_dscs,
            },
            {
                .uuid      = BLE_UUID16_DECLARE(UUID_CHR_HID_INFO),
                .access_cb = callback_hid_info,
                .flags     = BLE_GATT_CHR_F_READ,
            },
            {
                .uuid      = BLE_UUID16_DECLARE(UUID_CHR_HID_CONTROL),
                .access_cb = callback_hid_control,
                .flags     = BLE_GATT_CHR_F_WRITE_NO_RSP,
            },
            { 0 }
        },
    },
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(UUID_DIS_SERVICE),
        .characteristics = (struct ble_gatt_chr_def[]) {
            {
                .uuid      = BLE_UUID16_DECLARE(UUID_CHR_PNP_ID),
                .access_cb = callback_pnp_id,
                .flags     = BLE_GATT_CHR_F_READ,
            },
            { 0 }
        },
    },
    { 0 }
};

void hid_init(void)
{
    ESP_ERROR_CHECK(ble_gatts_count_cfg(hid_gatt_svcs));
    ESP_ERROR_CHECK(ble_gatts_add_svcs(hid_gatt_svcs));
    ESP_LOGI(TAG, "Servicio HID Consumer Control registrado");
}

void hid_set_notificaciones(bool activas)
{
    notificaciones_activas = activas;
    ESP_LOGI(TAG, "Notificaciones: %s", activas ? "activas" : "inactivas");
}

void hid_send_report(uint16_t usage_id, uint16_t conn_handle)
{
    if (hid_report_handle == 0) {
        ESP_LOGW(TAG, "Handle HID no inicializado");
        return;
    }

    if (!notificaciones_activas) {
        ESP_LOGW(TAG, "Reporte descartado — notificaciones no activas");
        return;
    }

    struct os_mbuf *om;

    om = ble_hs_mbuf_from_flat(&usage_id, sizeof(usage_id));
    if (om == NULL) {
        ESP_LOGE(TAG, "Error al asignar mbuf para usage_id");
        return;
    }
    ble_gatts_notify_custom(conn_handle, hid_report_handle, om);

    uint16_t key_up = HID_KEY_UP;
    om = ble_hs_mbuf_from_flat(&key_up, sizeof(key_up));
    if (om == NULL) {
        ESP_LOGE(TAG, "Error al asignar mbuf para key-up");
        return;
    }
    ble_gatts_notify_custom(conn_handle, hid_report_handle, om);

    ESP_LOGI(TAG, "Reporte HID enviado: 0x%04X", usage_id);
}