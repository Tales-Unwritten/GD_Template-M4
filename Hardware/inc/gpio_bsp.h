//备注：拷贝代码请加上作者信息
//作者：王海涛
//邮箱：1126471088@qq.com
//版本：V1.0.0
/********************************************************
    说明：
    1、GPIO端口时钟开启或关闭。
    2、GPIO引脚模式配置。
    3、GPIO引脚状态获取或设置。
    4、GPIO端口16个引脚状态获取或设置。
    5、位操作GPIO端口引脚(操作几个引脚的话很推荐此方法)。
    6、基于gd32f4xx标准库V3.3.1
**********************************************************/

#pragma once

/*****************************************************/
#include "gd32f4xx.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_gpio.h"
/*****************************************************/

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

/*GPIO端口枚举*/
typedef enum
{
    PortA = (uint32_t)GPIOA,//端口A
    PortB = (uint32_t)GPIOB,//端口B
    PortC = (uint32_t)GPIOC,//端口C
    PortD = (uint32_t)GPIOD,//端口D
    PortE = (uint32_t)GPIOE,//端口E
    PortF = (uint32_t)GPIOF,//端口F
    PortG = (uint32_t)GPIOG,//端口G
    PortH = (uint32_t)GPIOH,//端口H
    PortI = (uint32_t)GPIOI,//端口I
}WHT_GPIO_Port_enum;

/*GPIO引脚号枚举*/
typedef enum
{
    Pin0  = GPIO_PIN_0, //引脚0号
    Pin1  = GPIO_PIN_1, //引脚1号
    Pin2  = GPIO_PIN_2, //引脚2号
    Pin3  = GPIO_PIN_3, //引脚3号
    Pin4  = GPIO_PIN_4, //引脚4号
    Pin5  = GPIO_PIN_5, //引脚5号
    Pin6  = GPIO_PIN_6, //引脚6号
    Pin7  = GPIO_PIN_7, //引脚7号
    Pin8  = GPIO_PIN_8, //引脚8号
    Pin9  = GPIO_PIN_9, //引脚9号
    Pin10 = GPIO_PIN_10,//引脚10号
    Pin11 = GPIO_PIN_11,//引脚11号
    Pin12 = GPIO_PIN_12,//引脚12号
    Pin13 = GPIO_PIN_13,//引脚13号
    Pin14 = GPIO_PIN_14,//引脚14号
    Pin15 = GPIO_PIN_15,//引脚15号
    PinAll= GPIO_PIN_ALL,//所有引脚
}WHT_GPIO_Pin_enum;

/*GPIO寄存器偏移地址枚举*/
typedef enum
{
    GPIO_IDR_Offset_Address = 0x10,//输入寄存器偏移量
    GPIO_ODR_Offset_Address = 0x14,//输出寄存器偏移量
    GPIO_BRR_Offset_Address = 0x18,//置位寄存器偏移量
    GPIO_BSRR_Offset_Address= 0x28,//复位寄存器偏移量
}WHT_GPIO_Bit_Offset_Address_enum;

/*GPIO状态枚举*/
typedef enum
{
    Low = 0,      //低电平
    Hig = 1,      //高电平
    Bit_Reset = 0,//低电平
    Bit_Set   = 1,//高电平
}WHT_GPIO_State_enum;

/*GPIO工作模式枚举*/
typedef enum
{
    Mode_Ain,        //模拟输入
    Mode_In_FLOATING,//浮空输入
    Mode_IPD,        //下拉输入
    Mode_IPU,        //上拉输入
    Mode_Out_OD,     //开漏输出
    Mode_Out_PP,     //推挽输出
    Mode_OPD_PP,     //下拉推挽输出
    Mode_OPU_PP,     //上拉推挽输出
    Mode_AF_OD,      //开漏复用输出
    Mode_AF_PP,      //推挽复用输出
}WHT_GPIO_Mode_enum;

/*GPIO复用功能枚举*/
typedef enum
{
    AFIO_RTC_50Hz = GPIO_AF_0,//RTC_50Hz (也是复位默认功能)
    AFIO_MCO      = GPIO_AF_0,//主时钟输出1和主时钟输出2功能 (也是复位默认功能)
    AFIO_TAMPER   = GPIO_AF_0,//篡改1和篡改2功能 (也是复位默认功能)
    AFIO_SWJ      = GPIO_AF_0,//SWD和JTAG功能 (也是复位默认功能)
    AFIO_Default  = GPIO_AF_0,//复位默认功能
    AFIO_TIM0     = GPIO_AF_1,
	  AFIO_TIM1     = GPIO_AF_1,
    AFIO_TIM2     = GPIO_AF_2,
    AFIO_TIM3     = GPIO_AF_2,
    AFIO_TIM4     = GPIO_AF_2,
    AFIO_TIM7     = GPIO_AF_3,
    AFIO_TIM8     = GPIO_AF_3,
    AFIO_TIM9     = GPIO_AF_3,
    AFIO_TIM10    = GPIO_AF_3,
    AFIO_TIM11    = GPIO_AF_9,
    AFIO_TIM12    = GPIO_AF_9,
    AFIO_TIM13    = GPIO_AF_9,
    AFIO_I2C0     = GPIO_AF_4,
    AFIO_I2C1     = GPIO_AF_4,
    AFIO_I2C2     = GPIO_AF_4,
    AFIO_I2C1_    = GPIO_AF_9,
    AFIO_I2C2_    = GPIO_AF_9,
    AFIO_SPI0     = GPIO_AF_5,
    AFIO_SPI1     = GPIO_AF_5,
    AFIO_SPI2     = GPIO_AF_5,
    AFIO_SPI1_    = GPIO_AF_7,
    AFIO_SPI2_    = GPIO_AF_7,
    AFIO_SPI3     = GPIO_AF_5,
    AFIO_SPI4     = GPIO_AF_5,
    AFIO_SPI5     = GPIO_AF_5,
		AFIO_USART0   = GPIO_AF_7,
    AFIO_USART1   = GPIO_AF_7,
    AFIO_USART2   = GPIO_AF_7,
    AFIO_UART3    = GPIO_AF_8,
    AFIO_UART4    = GPIO_AF_8,
    AFIO_USART5   = GPIO_AF_8,
    AFIO_UART6    = GPIO_AF_8,
    AFIO_UART7    = GPIO_AF_8,
    AFIO_CAN0     = GPIO_AF_9,
    AFIO_CAN1     = GPIO_AF_9,
		AFIO_CTC      = GPIO_AF_9,
    AFIO_USB_FS   = GPIO_AF_10,
    AFIO_USB_HS   = GPIO_AF_10,
		AFIO_USB_HS_  = GPIO_AF_12,
    AFIO_Ethernet = GPIO_AF_11,
		AFIO_EXMC     = GPIO_AF_12,
    AFIO_SDIO     = GPIO_AF_12,
    AFIO_DCI      = GPIO_AF_13,
    AFIO_LTI      = GPIO_AF_14,
    AFIO_EVENTOUT = GPIO_AF_15,
}WHT_GPIO_AFIO_enum;

/*GPIO回调函数结构体*/
typedef struct
{
    void (*WHT_Set_Clock)(WHT_GPIO_Port_enum portx, ControlStatus state);                           //使能端口时钟
    void (*WHT_Set_Mode)(WHT_GPIO_Port_enum portx, uint16_t pinx, WHT_GPIO_Mode_enum mode);         //设置工作模式 pinx for WHT_GPIO_Pin_enum
    void (*WHT_Set_Pin)(WHT_GPIO_Port_enum portx, uint16_t pinx, WHT_GPIO_State_enum state);        //设置引脚状态 pinx for WHT_GPIO_Pin_enum
    WHT_GPIO_State_enum(*WHT_Get_Pin)(WHT_GPIO_Port_enum portx, WHT_GPIO_Pin_enum pinx);            //获取引脚状态
    uint16_t (*WHT_Get_Prot)(WHT_GPIO_Port_enum portx);                                             //获取一组引脚状态
    void (*WHT_Set_Prot)(WHT_GPIO_Port_enum portx, uint16_t port_value);                            //设置一组引脚状态
    void (*WHT_Set_AFIO)(WHT_GPIO_Port_enum portx, WHT_GPIO_Pin_enum pinx, WHT_GPIO_AFIO_enum AFIO);//设置引脚复用功能
    void (*WHT_Register_Bit_Output)(WHT_GPIO_Port_enum portx, WHT_GPIO_Pin_enum pinx, volatile uint32_t** output);//注册位操作
    void (*WHT_Register_Bit_Input)(WHT_GPIO_Port_enum portx, WHT_GPIO_Pin_enum pinx, volatile uint32_t** input);  //注册位操作
}WHT_GPIO_BSP_t;

/*全局常量*/
extern const WHT_GPIO_BSP_t WHT_GPIO_BSP;

