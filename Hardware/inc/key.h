#pragma once
#include "gd32f4xx.h"
#include "systick.h"
#include "../Hardware/inc/bsp_led.h"
#include "../Hardware/inc/debug_uart.h"


//单击 双击 长按 时间定义
typedef enum
{
    key_none = 0,
    key_single_click,
    key_double_click,
    key_long_press
} key_enum_t;

typedef struct
{
    uint32_t   Key1_Status;
    uint32_t   Key2_Status;
    uint8_t    Key1_Count;
    uint8_t    Key2_Count;
    key_enum_t Key1_State;
    key_enum_t Key2_State;
} key_struct_t;

#define KEY1_RCU               RCU_GPIOA
#define KEY1_PORT              GPIOA
#define KEY1_PIN               GPIO_PIN_0
#define KEY1_EXTI_PORT_SOURCE  EXTI_SOURCE_GPIOA
#define KEY1_EXTI_PIN_SOURCE   EXTI_SOURCE_PIN0
#define KEY1_EXTI_LINE         EXTI_0
#define KEY1_EXTI_IRQn         EXTI0_IRQn
#define KEY1_EXTI_IRQHandler   EXTI0_IRQHandler


#define KEY2_RCU               RCU_GPIOC
#define KEY2_PORT              GPIOC
#define KEY2_PIN               GPIO_PIN_13

#define KEY2_EXTI_PORT_SOURCE  EXTI_SOURCE_GPIOC
#define KEY2_EXTI_PIN_SOURCE   EXTI_SOURCE_PIN13
#define KEY2_EXTI_LINE         EXTI_13
#define KEY2_EXTI_IRQn         EXTI10_15_IRQn
#define KEY2_EXTI_IRQHandler   EXTI10_15_IRQHandler


void configure_key_init(void);

void key_logic_handle(void);
