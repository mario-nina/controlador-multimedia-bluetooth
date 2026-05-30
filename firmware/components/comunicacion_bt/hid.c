#include "hid.h"
#include "esp_log.h"
#include "host/ble_hs.h"
#include "host/ble_gatt.h"

static const char *TAG = "hid";

static uint16_t hid_report_handle     = 0;
static int      notificaciones_activas = 0;

static const uint8_t hid_descriptor[] __attribute__((used)) = {
    0x05, 0x0C,
    0x09, 0x01,
    0xA1, 0x01,
    0x85, 0x01,
    0x15, 0x00,
    0x26, 0xFF, 0x03,
    0x19, 0x00,
    0x2A, 0xFF, 0x03,
    0x75, 0x10,
    0x95, 0x01,
    0x81, 0x00,
    0xC0
};

/**
 * @brief Callback de lectura para Report Map.
 */
static int callback_report_map(uint16_t conn_handle, uint16_t attr_handle,
                                struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    return os_mbuf_append(ctxt->om, hid_descriptor, sizeof(hid_descriptor));
}

/**
 * @brief Callback de lectura para HID Information.
 */
static int callback_hid_info(uint16_t conn_handle, uint16_t attr_handle,
                              struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    static const uint8_t info[] = {
        0x11, 0x01,  /* HID version 1.11 */
        0x00,        /* Country code */
        0x01         /* Flags: remoteable */
    };
    return os_mbuf_append(ctxt->om, info, sizeof(info));
}

/**
 * @brief Callback de lectura para HID Report.
 */
static int callback_hid_report(uint16_t conn_handle, uint16_t attr_handle,
                                struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    return 0;
}

/**
 * @brief Callback de escritura para HID Control Point.
 */
static int callback_hid_control(uint16_t conn_handle, uint16_t attr_handle,
                                  struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    return 0;
}

/**
 * @brief Callback de lectura para Report Reference descriptor.
 */
static int callback_report_ref(uint16_t conn_handle, uint16_t attr_handle,
                                struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    static const uint8_t ref[] = { 0x01, 0x01 }; /* Report ID=1, Input */
    return os_mbuf_append(ctxt->om, ref, sizeof(ref));
}

/**
 * @brief Callback de lectura/escritura para Protocol Mode.
 */
static int callback_protocol_mode(uint16_t conn_handle, uint16_t attr_handle,
                                   struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    static uint8_t mode = 0x01; /* Report Protocol Mode */
    if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR) {
        return os_mbuf_append(ctxt->om, &mode, sizeof(mode));
    }
    return 0;
}

/**
 * @brief Callback de lectura para PnP ID — Device Information Service.
 */
static int callback_pnp_id(uint16_t conn_handle, uint16_t attr_handle,
                             struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    static const uint8_t pnp[] = {
        0x02,        /* Vendor ID source: USB */
        0xE5, 0x02,  /* Vendor ID: Espressif (0x02E5) */
        0x00, 0x00,  /* Product ID */
        0x00, 0x01,  /* Product version */
    };
    return os_mbuf_append(ctxt->om, pnp, sizeof(pnp));
}

static struct ble_gatt_dsc_def hid_report_dscs[] = {
    {
        .uuid      = BLE_UUID16_DECLARE(0x2908),
        .att_flags = BLE_ATT_F_READ,
        .access_cb = callback_report_ref,
    },
    { 0 }
};

static const struct ble_gatt_svc_def hid_gatt_svcs[] = {
    {
        /* HID Service */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(0x1812),
        .characteristics = (struct ble_gatt_chr_def[]) {
            {
                /* Protocol Mode */
                .uuid      = BLE_UUID16_DECLARE(0x2A4E),
                .access_cb = callback_protocol_mode,
                .flags     = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE_NO_RSP,
            },
            {
                /* HID Report Map */
                .uuid      = BLE_UUID16_DECLARE(0x2A4B),
                .access_cb = callback_report_map,
                .flags     = BLE_GATT_CHR_F_READ,
            },
            {
                /* HID Report — Consumer Control */
                .uuid        = BLE_UUID16_DECLARE(0x2A4D),
                .access_cb   = callback_hid_report,
                .flags       = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
                .val_handle  = &hid_report_handle,
                .descriptors = hid_report_dscs,
            },
            {
                /* HID Information */
                .uuid      = BLE_UUID16_DECLARE(0x2A4A),
                .access_cb = callback_hid_info,
                .flags     = BLE_GATT_CHR_F_READ,
            },
            {
                /* HID Control Point */
                .uuid      = BLE_UUID16_DECLARE(0x2A4C),
                .access_cb = callback_hid_control,
                .flags     = BLE_GATT_CHR_F_WRITE_NO_RSP,
            },
            { 0 }
        },
    },
    {
        /* Device Information Service */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(0x180A),
        .characteristics = (struct ble_gatt_chr_def[]) {
            {
                /* PnP ID */
                .uuid      = BLE_UUID16_DECLARE(0x2A50),
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
    ble_gatts_count_cfg(hid_gatt_svcs);
    ble_gatts_add_svcs(hid_gatt_svcs);
    ESP_LOGI(TAG, "Servicio HID Consumer Control registrado");
}

void hid_set_notificaciones(int activas)
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

    /* Enviar tecla presionada */
    om = ble_hs_mbuf_from_flat(&usage_id, sizeof(usage_id));
    ble_gatts_notify_custom(conn_handle, hid_report_handle, om);

    /* Enviar key-up — obligatorio */
    uint16_t key_up = 0x0000;
    om = ble_hs_mbuf_from_flat(&key_up, sizeof(key_up));
    ble_gatts_notify_custom(conn_handle, hid_report_handle, om);

    ESP_LOGI(TAG, "Reporte HID enviado: 0x%04X", usage_id);
}