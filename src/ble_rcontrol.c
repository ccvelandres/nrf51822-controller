#include "ble_rcontrol.h"
#include "ble_srv_common.h"
#include "app_error.h"

#define NRF_LOG_MODULE_NAME "rctrl"
#include "nrf_log.h"

static uint32_t _command_char_add(ble_rcontrol_t *p_rctrl, const ble_rcontrol_init_t *p_rctrl_init)
{
    ble_uuid_t ble_uuid;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t attr_md;
    ble_gatts_attr_md_t desc_md;
    ble_gatts_attr_t attr_char_value;

    uint8_t desc_name[] = "Command";
    const uint16_t desc_name_len = sizeof(desc_name);

    memset(&desc_md, 0, sizeof(desc_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&desc_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&desc_md.write_perm);
    desc_md.vloc = BLE_GATTS_VLOC_STACK;
    desc_md.rd_auth = 0;
    desc_md.wr_auth = 0;
    desc_md.vlen = 0;

    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.write = 1;
    char_md.char_props.write_wo_resp = 1;
    char_md.char_ext_props.wr_aux = 0;
    char_md.p_char_user_desc = desc_name;
    char_md.p_user_desc_md = &desc_md;
    char_md.char_user_desc_size = desc_name_len;
    char_md.char_user_desc_max_size = desc_name_len;

    memset(&ble_uuid, 0, sizeof(ble_uuid));
    ble_uuid.type = p_rctrl->uuid_type;
    ble_uuid.uuid = BLE_UUID_RCONTROL_CHAR_COMMAND;

    memset(&attr_md, 0, sizeof(attr_md));
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = BLE_RCONTROL_PAYLOAD_MAX_LEN;

    return sd_ble_gatts_characteristic_add(p_rctrl->service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_rctrl->cmd_handle);
}

static uint32_t _response_char_add(ble_rcontrol_t *p_rctrl, const ble_rcontrol_init_t *p_rctrl_init)
{
    ble_uuid_t ble_uuid;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t attr_md;
    ble_gatts_attr_md_t desc_md;
    ble_gatts_attr_t attr_char_value;

    uint8_t desc_name[] = "Response";
    const uint16_t desc_name_len = sizeof(desc_name);

    memset(&desc_md, 0, sizeof(desc_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&desc_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&desc_md.write_perm);
    desc_md.vloc = BLE_GATTS_VLOC_STACK;
    desc_md.rd_auth = 0;
    desc_md.wr_auth = 0;
    desc_md.vlen = 0;

    memset(&ble_uuid, 0, sizeof(ble_uuid));
    ble_uuid.type = p_rctrl->uuid_type;
    ble_uuid.uuid = BLE_UUID_RCONTROL_CHAR_RESPONSE;

    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.indicate = 1;
    char_md.char_props.read = 1;
    char_md.p_cccd_md = &cccd_md;
    char_md.char_ext_props.wr_aux = 0;
    char_md.p_char_user_desc = desc_name;
    char_md.p_user_desc_md = &desc_md;
    char_md.char_user_desc_size = desc_name_len;
    char_md.char_user_desc_max_size = desc_name_len;

    memset(&attr_md, 0, sizeof(attr_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = sizeof(uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = BLE_RCONTROL_PAYLOAD_MAX_LEN;

    return sd_ble_gatts_characteristic_add(p_rctrl->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_rctrl->rsp_handle);
}

static void _on_write_evt(ble_rcontrol_t *p_rctrl, ble_evt_t *p_ble_evt)
{
    ble_gatts_evt_write_t *p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if (p_rctrl->cmd_handle.value_handle == p_evt_write->handle)
    {
        NRF_LOG_DEBUG("write_evt: cmd %u\r\n", p_evt_write->len);
        // NRF_LOG_HEXDUMP_DEBUG(p_evt_write->data, p_evt_write->len);
        if (NULL != p_rctrl->handler)
        {
            p_rctrl->handler(p_rctrl, (ble_rcontrol_payload_t *)p_evt_write->data, p_evt_write->len);
        }
    }
    else if (p_evt_write->handle == p_rctrl->rsp_handle.cccd_handle)
    {
        NRF_LOG_DEBUG("write_evt: rsp: %d\r\n", p_evt_write->len);
        if (2 == p_evt_write->len)
        {
            p_rctrl->indication_enabled = ble_srv_is_indication_enabled(p_evt_write->data);
        }
    }
}

uint32_t ble_rcontrol_init(ble_rcontrol_t *p_rctrl, const ble_rcontrol_init_t *p_rctrl_init)
{
    uint32_t err_code;
    ble_uuid_t ble_uuid;
    ble_uuid128_t base_uuid = BLE_UUID_RCONTROL_BASE;

    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_rctrl->uuid_type);
    APP_ERROR_CHECK(err_code);

    ble_uuid.type = p_rctrl->uuid_type;
    ble_uuid.uuid = BLE_UUID_RCONTROL_SERVICE;

    NRF_LOG_INFO("BLE Motor Service UUID: %04x\r\n", ble_uuid.uuid);
    NRF_LOG_INFO("BLE Motor Service UUID Type: %04x\r\n", ble_uuid.type);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_rctrl->service_handle);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("BLE Motor Service Handle: %04x\r\n", p_rctrl->service_handle);

    err_code = _command_char_add(p_rctrl, p_rctrl_init);
    APP_ERROR_CHECK(err_code);

    err_code = _response_char_add(p_rctrl, p_rctrl_init);
    APP_ERROR_CHECK(err_code);

    p_rctrl->conn_handle = BLE_CONN_HANDLE_INVALID;

    return NRF_SUCCESS;
}

void ble_rcontrol_evt_handler(ble_rcontrol_t *p_rctrl, ble_evt_t *p_ble_evt)
{
    if (NULL == p_rctrl || NULL == p_ble_evt)
        return;

    switch (p_ble_evt->header.evt_id)
    {
    case BLE_GAP_EVT_CONNECTED:
    {
        NRF_LOG_DEBUG("connected\r\n");
        p_rctrl->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
        break;
    }
    case BLE_GAP_EVT_DISCONNECTED:
    {
        NRF_LOG_DEBUG("disconnected\r\n");
        p_rctrl->conn_handle = BLE_CONN_HANDLE_INVALID;
        break;
    }
    case BLE_GATTS_EVT_WRITE:
    {
        _on_write_evt(p_rctrl, p_ble_evt);
        break;
    }
    default:
        break;
    }
}

void ble_rcontrol_sys_handler(ble_rcontrol_t *p_rctrl, uint32_t sys_evt)
{
}

uint32_t ble_rcontrol_set_handler(ble_rcontrol_t *p_rctrl, ble_rcontrol_handler_t handler)
{
    if (NULL == p_rctrl || NULL == handler)
        return NRF_ERROR_INVALID_PARAM;

    p_rctrl->handler = handler;

    return NRF_SUCCESS;
}

uint32_t ble_rcontrol_send(ble_rcontrol_t *p_rctrl, ble_rcontrol_payload_t *payload, uint16_t len)
{
    if (NULL == p_rctrl || BLE_CONN_HANDLE_INVALID == p_rctrl->conn_handle)
        return NRF_ERROR_INVALID_STATE;

    if (BLE_RCONTROL_PAYLOAD_MAX_LEN < len)
        return NRF_ERROR_INVALID_PARAM;

    ble_gatts_hvx_params_t hvx_params;

    memset(&hvx_params, 0, sizeof(hvx_params));
    hvx_params.handle = p_rctrl->rsp_handle.value_handle;
    hvx_params.p_data = (uint8_t *)payload;
    hvx_params.p_len = &len;
    hvx_params.type = BLE_GATT_HVX_INDICATION;

    NRF_LOG_DEBUG("send: %u\r\n", len);
    return sd_ble_gatts_hvx(p_rctrl->conn_handle, &hvx_params);
}
