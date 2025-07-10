#pragma once
#include "GD32F4xx.h"
#include "systick.h"
#include "stdint.h"
#include "string.h"
/* select can */
// #define DEV_CAN0_USED
// #define DEV_CAN1_USED


void can_config(void); // CAN配置函数声明
void can_transmit_data(uint8_t *data, uint8_t len);
uint8_t can_receive_data(uint8_t *out_data);


#ifdef DEV_CAN0_USED
#define CANX CAN0
#else
#define CANX CAN1
#endif


