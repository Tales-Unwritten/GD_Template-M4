#pragma once

#include "gd32f4xx.h"
#include "systick.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../Hardware/inc/bsp_led.h"



#define CHCHE_COUNT 4

#define DEBUG_USART_TX_RCU RCU_GPIOA // 串口TX的端口时钟
#define DEBUG_USART_RX_RCU RCU_GPIOA // 串口RX的端口时钟
#define DEBUG_USART_RCU RCU_USART0	 // 串口0的时钟

#define DEBUG_USART_TX_PORT GPIOA	   // 串口TX的端口
#define DEBUG_USART_RX_PORT GPIOA	   // 串口RX的端口
#define DEBUG_USART_AF GPIO_AF_7	   // 串口0的复用功能
#define DEBUG_USART_TX_PIN GPIO_PIN_9  // 串口TX的引脚
#define DEBUG_USART_RX_PIN GPIO_PIN_10 // 串口RX的引脚

#define DEBUG_USART USART0			// 串口0
#define DEBUG_USART_IRQ USART0_IRQn // 串口0的中断号

#ifndef __PC_Transmit_Buffer_t__
#define __PC_Transmit_Buffer_t__
typedef struct
{
	uint8_t Buffer_Status; // 状态
	uint8_t Buffer_Length;  // 长度
	uint8_t Buffer[250];   // 缓存
} PC_Transmit_Buffer_t;

#endif // !__PC_Transmit_Buffer_t__

void debug_init_config(uint32_t band_rate);				  // 配置串口
void debug_send_it_data(uint8_t *buffer, uint8_t length); // 发送数据
void debug_send_data(uint8_t *buffer, uint8_t length);	  // 发送数据

extern PC_Transmit_Buffer_t Debug_Receive_Buffer[CHCHE_COUNT];
