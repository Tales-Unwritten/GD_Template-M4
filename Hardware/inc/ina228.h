#pragma once

#include "gd32f4xx.h"
#include "systick.h"

#include "../Hardware/inc/soft_i2c.h"
void INA228_Config(uint32_t GPIOx, uint16_t SCL_Pin, uint16_t SDA_Pin, uint16_t Addr);
void text_online_INA228(void);

