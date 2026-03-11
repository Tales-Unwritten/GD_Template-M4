#pragma once

#include "../Hardware/modbus/inc/modbus.h"
#include "../Hardware/modbus/inc/modbus_crc.h"
#include "../Hardware/modbus/inc/modbus_map.h"
#include "../Hardware/inc/rs485.h"

#include <stdint.h>

void Modbus_PortSend(uint8_t *buf,uint16_t len);
