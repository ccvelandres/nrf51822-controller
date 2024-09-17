#ifndef BLE_RCONTROL_H
#define BLE_RCONTROL_H

#include "ble.h"
#include <stdbool.h>

#define BLE_UUID_RCONTROL_BASE {{0x2b,0x23,0xdc,0x98,0xdc,0x7a,0x4a,0xb1,0xa8,0xc8,0x7a,0x83,0xef,0xfb,0xaf,0xa2}}
#define BLE_UUID_RCONTROL_SERVICE 0xE780
#define BLE_UUID_RCONTROL_CHAR_COMMAND 0xE781
#define BLE_UUID_RCONTROL_CHAR_RESPONSE 0xE782

#define BLE_RCONTROL_PAYLOAD_MAX_LEN (32)

typedef uint16_t ble_rcontrol_cmd_t;

typedef struct _ble_rcontrol_payload_s {
    uint16_t cmd;
    uint8_t payload[];
} __attribute__((packed)) ble_rcontrol_payload_t;

typedef struct _ble_rcontrol_s ble_rcontrol_t;

typedef void (*ble_rcontrol_handler_t)(ble_rcontrol_t *p_rctrl, const ble_rcontrol_payload_t *payload, uint16_t len);

typedef struct _ble_rcontrol_init {
} ble_rcontrol_init_t;

struct _ble_rcontrol_s
{
    uint8_t uuid_type;
    uint16_t service_handle;
    uint16_t conn_handle;
    uint16_t desc_handle;
    bool indication_enabled;
    ble_gatts_char_handles_t cmd_handle;
    uint16_t cmd_desc_handle;
    ble_gatts_char_handles_t rsp_handle;
    uint16_t rsp_desc_handle;
    ble_rcontrol_handler_t handler;
};

uint32_t ble_rcontrol_init(ble_rcontrol_t *p_rctrl, const ble_rcontrol_init_t *p_rctrl_init);

void ble_rcontrol_evt_handler(ble_rcontrol_t *p_rctrl, ble_evt_t *p_ble_evt);
void ble_rcontrol_sys_handler(ble_rcontrol_t *p_rctrl, uint32_t sys_evt);

uint32_t ble_rcontrol_set_handler(ble_rcontrol_t *p_rctrl, ble_rcontrol_handler_t handler);

uint32_t ble_rcontrol_send(ble_rcontrol_t *p_rctrl, ble_rcontrol_payload_t *payload, uint16_t len);

#endif