#pragma once
#include "../Hardware/inc/eeprom.h"

#define EEPROM_ADDR_BAUD_VAL0    0x00   // 4 字节存储
#define EEPROM_ADDR_BAUD_VAL1    0x10   // 4 字节存储
#define EEPROM_ADDR_BAUD_VAL2    0x20   // 4 字节存储

extern void Save_BaudValue(uint32_t baud, uint8_t interface);
extern void interface_init(uint8_t interface);
extern uint32_t Load_BaudValue(uint8_t interface); // Ensure this matches the definition
extern void config_baudrate(uint32_t baud, uint8_t interface); // Ensure this matches the definition in the source file

extern const uint32_t BaudValue[];