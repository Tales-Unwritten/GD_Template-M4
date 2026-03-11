#pragma once

#include "../Hardware/modbus/inc/modbus_crc.h"
#include "../Hardware/modbus/inc/modbus.h"
#include "../Hardware/modbus/inc/modbus_port.h"

#include <stdint.h>

typedef enum
{
    MODBUS_BLOCK_HOLDING, // Holding register block
    MODBUS_BLOCK_INPUT    // Input register block
} modbus_block_type_t;


uint16_t Modbus_ReadRegister(uint16_t addr,modbus_block_type_t type);

void Modbus_WriteRegister(uint16_t addr,uint16_t value);