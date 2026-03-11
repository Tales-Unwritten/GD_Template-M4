#pragma once

#include "../Hardware/modbus/inc/modbus_crc.h"
#include "../Hardware/modbus/inc/modbus_map.h"
#include "../Hardware/modbus/inc/modbus_port.h"

#include <stdint.h>

#define MODBUS_SLAVE_ADDR  0x01

#define MODBUS_RX_BUF_SIZE 256
#define MODBUS_TX_BUF_SIZE 256

typedef struct
{
    uint8_t rx_buf[MODBUS_RX_BUF_SIZE];
    uint16_t rx_cnt;

    uint8_t tx_buf[MODBUS_TX_BUF_SIZE];
    uint16_t tx_len;

}modbus_t;

extern modbus_t Modbus;

void Modbus_Init(void);

void Modbus_ReceiveByte(uint8_t data);

void Modbus_FrameProcess(void);