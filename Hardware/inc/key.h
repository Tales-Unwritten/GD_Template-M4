#pragma once

#include "gd32f4xx.h"
#include "systick.h"
#include "../Hardware/inc/bsp_led.h"
#include "../Hardware/inc/debug_uart.h"

#define KEY1_RCU RCU_GPIOA
#define KEY1_PORT GPIOA
#define KEY1_PIN GPIO_PIN_0
#define KEY1_EXTI_PORT_SOURCE EXTI_SOURCE_GPIOA
#define KEY1_EXTI_PIN_SOURCE EXTI_SOURCE_PIN0
#define KEY1_EXTI_LINE EXTI_0
#define KEY1_EXTI_IRQn EXTI0_IRQn
#define KEY1_EXTI_IRQHandler EXTI0_IRQHandler




#define KEY2_RCU RCU_GPIOC
#define KEY2_PORT GPIOC
#define KEY2_PIN GPIO_PIN_13

#define KEY2_EXTI_PORT_SOURCE EXTI_SOURCE_GPIOC
#define KEY2_EXTI_PIN_SOURCE EXTI_SOURCE_PIN13
#define KEY2_EXTI_LINE EXTI_13
#define KEY2_EXTI_IRQn EXTI10_15_IRQn
#define KEY2_EXTI_IRQHandler EXTI10_15_IRQHandler


void Key_Configuration(void);

void Key_Scan(void);
