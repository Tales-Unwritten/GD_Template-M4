#pragma once
#include "gd32f4xx.h"
#include "systick.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../Hardware/inc/debug_uart.h"
#include "../Hardware/inc/bsp_led.h"
#include "../App/inc/command_parsing.h"
#include "../Hardware/inc/key.h"
#include "../Hardware/inc/eeprom.h"


#define EEPROM_ADDR_BAUD_VAL    0x00   // 4 字节存储

void Save_BaudValue(uint32_t baud);
uint32_t Load_BaudValue(void);
extern const uint32_t baud_table[];
void APP_PC_Task(void);