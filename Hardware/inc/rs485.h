#pragma once

#define CHCHE_COUNT 8

#include "gd32f4xx.h"
#include "systick.h"
#include "stdio.h"
#include "string.h"
#include "../Hardware/inc/bsp_led.h"

#define RS485_USART_TX_RCU   RCU_GPIOA   // 串口TX的端口时钟
#define RS485_USART_RX_RCU   RCU_GPIOA   // 串口RX的端口时钟
#define RS485_USART_RCU      RCU_USART1  // 串口1的时钟

#define RS485_USART_IRQ      USART1_IRQn // 串口1的中断号

#define RS485_USART_TX_PORT  GPIOA		 // 串口TX的端口
#define RS485_USART_RX_PORT  GPIOA		 // 串口RX的端口
#define RS485_USART_AF       GPIO_AF_7   // 串口1的复用功能
#define RS485_USART_TX_PIN   GPIO_PIN_2  // 串口TX的引脚
#define RS485_USART_RX_PIN   GPIO_PIN_3  // 串口RX的引脚


#define RS485_EN_PIN         GPIO_PIN_13  // RS485使能引脚
#define RS485_EN_PORT        GPIOG        // RS485使能端口
#define RS485_USART_EN_RCU   RCU_GPIOG    // RS485使能端口时钟


#define RS485_USART          USART1       // 串口1

#ifndef __PC_Transmit_Buffer_t__
#define __PC_Transmit_Buffer_t__
typedef struct
{
    uint8_t Buffer_Status; // 状态
    uint8_t Buffer_Length;  // 长度
    uint8_t Buffer[250];   // 缓存
} PC_Transmit_Buffer_t;

#endif // !__PC_Transmit_Buffer_t__


void Rs485_Init_config(uint32_t band_rate); // 初始化RS485串口
void rs485_send_it_data(uint8_t *buffer, uint8_t length); // 发送数据（中断方式）
void rs485_send_data(uint8_t *buffer, uint8_t length); // 发送数据（轮询方式）


extern PC_Transmit_Buffer_t RS485_Receive_Buffer[CHCHE_COUNT];
