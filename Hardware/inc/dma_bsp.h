//备注：拷贝代码请加上作者信息
//作者：王海涛
//邮箱：1126471088@qq.com
//版本：V1.0.0
/********************************************************
    说明：
    1、提供DMA实时状态。
    2、由外部配置DMA相关参数。
    3、支持查询DMA传输个数。
    4、支持手动关闭DMA。
    5、由外部提供DMA回调函数。
    6、支持DMA内存拷贝（自动锁定空闲通道，拷贝完后自动释放通道）。
    7、基于stm32f4xx标准库V1.9.0
**********************************************************/

#ifndef __DMA_BSP_H__
#define __DMA_BSP_H__

#include "gd32f4xx.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_dma.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

/*DMA外设枚举*/
typedef enum
{
    // DMA0
    Periph_SPI3_RX_1          = DMA0,//SPI3接收
    Periph_I2C1_RX_1          = DMA0,//I2C1接收
    Periph_TIM4_CH1           = DMA0,//定时器4通道1
    Periph_I2S3_EXT_RX_1      = DMA0,//I2S3接收
    Periph_UART5_RX           = DMA0,//串口5接收
    Periph_UART8_TX           = DMA0,//串口8发送
    Periph_TIM5_CH3_TIM5_UP   = DMA0,//定时器5通道3和更新
    Periph_TIM2_UP_TIM2_CH3   = DMA0,//定时器2通道3和更新
    Periph_USART3_RX          = DMA0,//串口3接收
    Periph_UART7_TX           = DMA0,//串口7发送
    Periph_TIM5_CH4_TIM5_TRIG_1 = DMA0,//定时器5通道4和触发
    Periph_TIM6_UP            = DMA0,//定时器6更新
    Periph_SPI3_RX_2          = DMA0,//SPI3接收
    Periph_TIM7_UP_1          = DMA0,//定时器7更新
    Periph_I2S3_EXT_RX_2      = DMA0,//I2S3 EXT接收
    Periph_I2C3_RX            = DMA0,//I2S3接收
    Periph_UART4_RX           = DMA0,//串口4接收
    Periph_TIM3_CH4_TIM3_UP   = DMA0,//定时器3通道4和更新
    Periph_TIM5_CH1           = DMA0,//定时器5通道1
    Periph_I2C2_RX_1          = DMA0,//I2C2接收
    Periph_SPI2_RX            = DMA0,//SPI2接收
    Periph_TIM4_CH2           = DMA0,//定时器4通道2
    Periph_I2S2_EXT_RX        = DMA0,//I2S2 EXT接收
    Periph_USART3_TX_1        = DMA0,//串口3发送
    Periph_UART7_RX           = DMA0,//串口7接收
    Periph_TIM5_TCH4_TIM5_TRIG_2 = DMA0,//定时器5通道4和触发
    Periph_I2C2_RX_2          = DMA0,//I2C2接收
    Periph_SPI2_TX            = DMA0,//SPI2接收
    Periph_TIM7_UP_2          = DMA0,//定时器7更新
    Periph_I2S2_EXT_TX        = DMA0,//I2S2 EXT发送
    Periph_I2C3_TX            = DMA0,//I2C3发送
    Periph_UART4_TX           = DMA0,//串口4发送
    Periph_TIM3_CH1_TIM3_TRIG = DMA0,//定时器3通道1和触发
    Periph_TIM5_CH2           = DMA0,//定时器5通道2
    Periph_USART3_TX_2        = DMA0,//串口3发送
    Periph_SPI3_TX_1          = DMA0,//SPI3发送
    Periph_I2C1_RX_2          = DMA0,//I2C1接收
    Periph_I2S3_EXT_TX        = DMA0,//I2S3 EXT发送
    Periph_TIM2_CH1           = DMA0,//定时器2通道1
    Periph_USART2_RX          = DMA0,//串口2接收
    Periph_TIM3_CH2           = DMA0,//定时器3通道2
    Periph_DAC_CH1            = DMA0,//DAC通道1
    Periph_I2C1_TX_1          = DMA0,//I2C1发送
    Periph_TIM4_UP            = DMA0,//定时器4更新
    Periph_TIM2_CH2_TIM2_CH4  = DMA0,//定时器2通道2和通道4
    Periph_USART2_TX          = DMA0,//串口2发送
    Periph_UART8_RX           = DMA0,//串口8接收
    Periph_TIM5_UP            = DMA0,//定时器5更新
    Periph_DAC_CH2            = DMA0,//DAC通道2
    Periph_SPI3_TX_2          = DMA0,//SPI3发送
    Periph_I2C1_TX_2          = DMA0,//I2C1发送
    Periph_TIM4_CH3           = DMA0,//定时器4通道3
    Periph_TIM2_UP_TIM2_CH4   = DMA0,//定时器2更新和通道4
    Periph_UART5_TX           = DMA0,//串口5发送
    Periph_TIM3_CH3           = DMA0,//定时器3通道3
    Periph_I2C2_TX            = DMA0,//I2C2发送
    // DMA1
    Periph_ADC1_1             = DMA1,//ADC1
    Periph_ADC3_1             = DMA1,//ADC3
    Periph_SPI1_RX_1          = DMA1,//SPI1接收
    Periph_SPI4_RX_1          = DMA1,//SPI4接收
    Periph_TIM1_TRIG          = DMA1,//定时器1触发
    Periph_DCMI_1             = DMA1,//DCMI
    Periph_ADC3_2             = DMA1,//ADC3
    Periph_SPI4_TX_1          = DMA1,//SPI4发送
    Periph_USART6_RX_1        = DMA1,//串口6接收
    Periph_TIM1_CH1_1         = DMA1,//定时器1通道1
    Periph_TIM8_UP            = DMA1,//定时器8更新
    Periph_TIM8_CH1_TIM8_CH2_TIM8_CH3 = DMA1,//定时器8通道1和2和3
    Periph_ADC2_1             = DMA1,//ADC2
    Periph_SPI1_RX_2          = DMA1,//SPI1接收
    Periph_USART1_RX_1        = DMA1,//串口1接收
    Periph_USART6_RX_2        = DMA1,//串口6接收
    Periph_TIM1_CH2           = DMA1,//定时器1通道2
    Periph_TIM8_CH1           = DMA1,//定时器8通道1
    Periph_ADC2_2             = DMA1,//ADC2
    Periph_SPI5_RX_1          = DMA1,//SPI5接收
    Periph_SPI1_TX_1          = DMA1,//SPI1发送
    Periph_SDIO_1             = DMA1,//SDIO
    Periph_SPI4_RX_2          = DMA1,//SPI4接收
    Periph_TIM1_CH1_2         = DMA1,//定时器1通道1
    Periph_TIM8_CH2           = DMA1,//定时器8通道2
    Periph_ADC1_2             = DMA1,//ADC1
    Periph_SPI5_TX_1          = DMA1,//SPI5发送
    Periph_SPI4_TX_2          = DMA1,//SPI4发送
    Periph_TIM1_CH4_TIM1_TRIG_TIM1_COM = DMA1,//定时器1通道4和触发和比较
    Periph_TIM8_CH3           = DMA1,//定时器8通道3
    Periph_SPI6_TX            = DMA1,//SPI6发送
    Periph_CRYP_OUT           = DMA1,//CRYP输出
    Periph_SPI1_TX_2          = DMA1,//SPI1发送
    Periph_USART1_RX_2        = DMA1,//串口1接收
    Periph_TIM1_UP            = DMA1,//定时器1更新
    Periph_SPI5_RX_2          = DMA1,//SPI5接收
    Periph_TIM1_CH1_TIM1_CH2_TIM1_CH3 = DMA1,//定时器1通道1和2和3
    Periph_SPI6_RX            = DMA1,//SPI6接收
    Periph_CRYP_IN            = DMA1,//CRYP输入
    Periph_SDIO_2             = DMA1,//SDIO
    Periph_USART6_TX_1        = DMA1,//串口6发送
    Periph_TIM1_CH3           = DMA1,//定时器1通道3
    Periph_SPI5_TX_2          = DMA1,//SPI5发送
    Periph_DCMI_2             = DMA1,//DCMI
    Periph_HASH_IN            = DMA1,//HASH输入
    Periph_USART1_TX          = DMA1,//串口1发送
    Periph_USART6_TX_2        = DMA1,//串口6发送
    Periph_TIM8_CH4_TIM8_TRIG_TIM8_COM = DMA1,//定时器8通道4和触发和比较  
}WHT_DMA_Periph_enum;

/*DMA信号流枚举*/
typedef enum
{
    Stream_SPI3_RX_1          = DMA_CH0,//SPI3接收
    Stream_I2C1_RX_1          = DMA_CH0,//I2C1接收
    Stream_TIM4_CH1           = DMA_CH0,//定时器4通道1
    Stream_I2S3_EXT_RX_1      = DMA_CH0,//I2S3接收
    Stream_UART5_RX           = DMA_CH0,//串口5接收
    Stream_UART8_TX           = DMA_CH0,//串口8发送
    Stream_TIM5_CH3_TIM5_UP   = DMA_CH0,//定时器5通道3和更新
    Stream_TIM2_UP_TIM2_CH3   = DMA_CH1,//定时器2通道3和更新
    Stream_USART3_RX          = DMA_CH1,//串口3接收
    Stream_UART7_TX           = DMA_CH1,//串口7发送
    Stream_TIM5_CH4_TIM5_TRIG_1 = DMA_CH1,//定时器5通道4和触发
    Stream_TIM6_UP            = DMA_CH1,//定时器6更新
    Stream_SPI3_RX_2          = DMA_CH2,//SPI3接收
    Stream_TIM7_UP_1          = DMA_CH2,//定时器7更新
    Stream_I2S3_EXT_RX_2      = DMA_CH2,//I2S3 EXT接收
    Stream_I2C3_RX            = DMA_CH2,//I2S3接收
    Stream_UART4_RX           = DMA_CH2,//串口4接收
    Stream_TIM3_CH4_TIM3_UP   = DMA_CH2,//定时器3通道4和更新
    Stream_TIM5_CH1           = DMA_CH2,//定时器5通道1
    Stream_I2C2_RX_1          = DMA_CH2,//I2C2接收
    Stream_SPI2_RX            = DMA_CH3,//SPI2接收
    Stream_TIM4_CH2           = DMA_CH3,//定时器4通道2
    Stream_I2S2_EXT_RX        = DMA_CH3,//I2S2 EXT接收
    Stream_USART3_TX_1        = DMA_CH3,//串口3发送
    Stream_UART7_RX           = DMA_CH3,//串口7接收
    Stream_TIM5_TCH4_TIM5_TRIG_2 = DMA_CH3,//定时器5通道4和触发
    Stream_I2C2_RX_2          = DMA_CH3,//I2C2接收
    Stream_SPI2_TX            = DMA_CH4,//SPI2接收
    Stream_TIM7_UP_2          = DMA_CH4,//定时器7更新
    Stream_I2S2_EXT_TX        = DMA_CH4,//I2S2 EXT发送
    Stream_I2C3_TX            = DMA_CH4,//I2C3发送
    Stream_UART4_TX           = DMA_CH4,//串口4发送
    Stream_TIM3_CH1_TIM3_TRIG = DMA_CH4,//定时器3通道1和触发
    Stream_TIM5_CH2           = DMA_CH4,//定时器5通道2
    Stream_USART3_TX_2        = DMA_CH4,//串口3发送
    Stream_SPI3_TX_1          = DMA_CH5,//SPI3发送
    Stream_I2C1_RX_2          = DMA_CH5,//I2C1接收
    Stream_I2S3_EXT_TX        = DMA_CH5,//I2S3 EXT发送
    Stream_TIM2_CH1           = DMA_CH5,//定时器2通道1
    Stream_USART2_RX          = DMA_CH5,//串口2接收
    Stream_TIM3_CH2           = DMA_CH5,//定时器3通道2
    Stream_DAC_CH1            = DMA_CH5,//DAC通道1
    Stream_I2C1_TX_1          = DMA_CH6,//I2C1发送
    Stream_TIM4_UP            = DMA_CH6,//定时器4更新
    Stream_TIM2_CH2_TIM2_CH4  = DMA_CH6,//定时器2通道2和通道4
    Stream_USART2_TX          = DMA_CH6,//串口2发送
    Stream_UART8_RX           = DMA_CH6,//串口8接收
    Stream_TIM5_UP            = DMA_CH6,//定时器5更新
    Stream_DAC_CH2            = DMA_CH6,//DAC通道2
    Stream_SPI3_TX_2          = DMA_CH7,//SPI3发送
    Stream_I2C1_TX_2          = DMA_CH7,//I2C1发送
    Stream_TIM4_CH3           = DMA_CH7,//定时器4通道3
    Stream_TIM2_UP_TIM2_CH4   = DMA_CH7,//定时器2更新和通道4
    Stream_UART5_TX           = DMA_CH7,//串口5发送
    Stream_TIM3_CH3           = DMA_CH7,//定时器3通道3
    Stream_I2C2_TX            = DMA_CH7,//I2C2发送

    Stream_ADC1_1             = DMA_CH0,//ADC1
    Stream_ADC3_1             = DMA_CH0,//ADC3
    Stream_SPI1_RX_1          = DMA_CH0,//SPI1接收
    Stream_SPI4_RX_1          = DMA_CH0,//SPI4接收
    Stream_TIM1_TRIG          = DMA_CH0,//定时器1触发
    Stream_DCMI_1             = DMA_CH1,//DCMI
    Stream_ADC3_2             = DMA_CH1,//ADC3
    Stream_SPI4_TX_1          = DMA_CH1,//SPI4发送
    Stream_USART6_RX_1        = DMA_CH1,//串口6接收
    Stream_TIM1_CH1_1         = DMA_CH1,//定时器1通道1
    Stream_TIM8_UP            = DMA_CH1,//定时器8更新
    Stream_TIM8_CH1_TIM8_CH2_TIM8_CH3 = DMA_CH2,//定时器8通道1和2和3
    Stream_ADC2_1             = DMA_CH2,//ADC2
    Stream_SPI1_RX_2          = DMA_CH2,//SPI1接收
    Stream_USART1_RX_1        = DMA_CH2,//串口1接收
    Stream_USART6_RX_2        = DMA_CH2,//串口6接收
    Stream_TIM1_CH2           = DMA_CH2,//定时器1通道2
    Stream_TIM8_CH1           = DMA_CH2,//定时器8通道1
    Stream_ADC2_2             = DMA_CH3,//ADC2
    Stream_SPI5_RX_1          = DMA_CH3,//SPI5接收
    Stream_SPI1_TX_1          = DMA_CH3,//SPI1发送
    Stream_SDIO_1             = DMA_CH3,//SDIO
    Stream_SPI4_RX_2          = DMA_CH3,//SPI4接收
    Stream_TIM1_CH1_2         = DMA_CH3,//定时器1通道1
    Stream_TIM8_CH2           = DMA_CH3,//定时器8通道2
    Stream_ADC1_2             = DMA_CH4,//ADC1
    Stream_SPI5_TX_1          = DMA_CH4,//SPI5发送
    Stream_SPI4_TX_2          = DMA_CH4,//SPI4发送
    Stream_TIM1_CH4_TIM1_TRIG_TIM1_COM = DMA_CH4,//定时器1通道4和触发和比较
    Stream_TIM8_CH3           = DMA_CH4,//定时器8通道3
    Stream_SPI6_TX            = DMA_CH5,//SPI6发送
    Stream_CRYP_OUT           = DMA_CH5,//CRYP输出
    Stream_SPI1_TX_2          = DMA_CH5,//SPI1发送
    Stream_USART1_RX_2        = DMA_CH5,//串口1接收
    Stream_TIM1_UP            = DMA_CH5,//定时器1更新
    Stream_SPI5_RX_2            = DMA_CH5,//SPI5接收
    Stream_TIM1_CH1_TIM1_CH2_TIM1_CH3 = DMA_CH6,//定时器1通道1和2和3
    Stream_SPI6_RX            = DMA_CH6,//SPI6接收
    Stream_CRYP_IN            = DMA_CH6,//CRYP输入
    Stream_SDIO_2             = DMA_CH6,//SDIO
    Stream_USART6_TX_1        = DMA_CH6,//串口6发送
    Stream_TIM1_CH3           = DMA_CH6,//定时器1通道3
    Stream_SPI5_TX_2          = DMA_CH6,//SPI5发送
    Stream_DCMI_2             = DMA_CH7,//DCMI
    Stream_HASH_IN            = DMA_CH7,//HASH输入
    Stream_USART1_TX          = DMA_CH7,//串口1发送
    Stream_USART6_TX_2        = DMA_CH7,//串口6发送
    Stream_TIM8_CH4_TIM8_TRIG_TIM8_COM = DMA_CH7,//定时器8通道4和触发和比较  
}WHT_DMA_Stream_enum;

/*DMA信号通道枚举*/
typedef enum
{
    Channel_SPI3_RX_1          = DMA_SUBPERI0,//SPI3接收
    Channel_I2C1_RX_1          = DMA_SUBPERI1,//I2C1接收
    Channel_TIM4_CH1           = DMA_SUBPERI2,//定时器4通道1
    Channel_I2S3_EXT_RX_1      = DMA_SUBPERI3,//I2S3接收
    Channel_UART5_RX           = DMA_SUBPERI4,//串口5接收
    Channel_UART8_TX           = DMA_SUBPERI5,//串口8发送
    Channel_TIM5_CH3_TIM5_UP   = DMA_SUBPERI6,//定时器5通道3和更新
    Channel_TIM2_UP_TIM2_CH3   = DMA_SUBPERI3,//定时器2通道3和更新
    Channel_USART3_RX          = DMA_SUBPERI4,//串口3接收
    Channel_UART7_TX           = DMA_SUBPERI5,//串口7发送
    Channel_TIM5_CH4_TIM5_TRIG_1 = DMA_SUBPERI6,//定时器5通道4和触发
    Channel_TIM6_UP            = DMA_SUBPERI7,//定时器6更新
    Channel_SPI3_RX_2          = DMA_SUBPERI0,//SPI3接收
    Channel_TIM7_UP_1          = DMA_SUBPERI1,//定时器7更新
    Channel_I2S3_EXT_RX_2      = DMA_SUBPERI2,//I2S3 EXT接收
    Channel_I2C3_RX            = DMA_SUBPERI3,//I2S3接收
    Channel_UART4_RX           = DMA_SUBPERI4,//串口4接收
    Channel_TIM3_CH4_TIM3_UP   = DMA_SUBPERI5,//定时器3通道4和更新
    Channel_TIM5_CH1           = DMA_SUBPERI6,//定时器5通道1
    Channel_I2C2_RX_1          = DMA_SUBPERI7,//I2C2接收
    Channel_SPI2_RX            = DMA_SUBPERI0,//SPI2接收
    Channel_TIM4_CH2           = DMA_SUBPERI2,//定时器4通道2
    Channel_I2S2_EXT_RX        = DMA_SUBPERI3,//I2S2 EXT接收
    Channel_USART3_TX_1        = DMA_SUBPERI4,//串口3发送
    Channel_UART7_RX           = DMA_SUBPERI5,//串口7接收
    Channel_TIM5_TCH4_TIM5_TRIG_2 = DMA_SUBPERI6,//定时器5通道4和触发
    Channel_I2C2_RX_2          = DMA_SUBPERI7,//I2C2接收
    Channel_SPI2_TX            = DMA_SUBPERI0,//SPI2接收
    Channel_TIM7_UP_2          = DMA_SUBPERI1,//定时器7更新
    Channel_I2S2_EXT_TX        = DMA_SUBPERI2,//I2S2 EXT发送
    Channel_I2C3_TX            = DMA_SUBPERI3,//I2C3发送
    Channel_UART4_TX           = DMA_SUBPERI4,//串口4发送
    Channel_TIM3_CH1_TIM3_TRIG = DMA_SUBPERI5,//定时器3通道1和触发
    Channel_TIM5_CH2           = DMA_SUBPERI6,//定时器5通道2
    Channel_USART3_TX_2        = DMA_SUBPERI7,//串口3发送
    Channel_SPI3_TX_1          = DMA_SUBPERI0,//SPI3发送
    Channel_I2C1_RX_2          = DMA_SUBPERI1,//I2C1接收
    Channel_I2S3_EXT_TX        = DMA_SUBPERI2,//I2S3 EXT发送
    Channel_TIM2_CH1           = DMA_SUBPERI3,//定时器2通道1
    Channel_USART2_RX          = DMA_SUBPERI4,//串口2接收
    Channel_TIM3_CH2           = DMA_SUBPERI5,//定时器3通道2
    Channel_DAC_CH1            = DMA_SUBPERI7,//DAC通道1
    Channel_I2C1_TX_1          = DMA_SUBPERI1,//I2C1发送
    Channel_TIM4_UP            = DMA_SUBPERI2,//定时器4更新
    Channel_TIM2_CH2_TIM2_CH4  = DMA_SUBPERI3,//定时器2通道2和通道4
    Channel_USART2_TX          = DMA_SUBPERI4,//串口2发送
    Channel_UART8_RX           = DMA_SUBPERI5,//串口8接收
    Channel_TIM5_UP            = DMA_SUBPERI6,//定时器5更新
    Channel_DAC_CH2            = DMA_SUBPERI7,//DAC通道2
    Channel_SPI3_TX_2          = DMA_SUBPERI0,//SPI3发送
    Channel_I2C1_TX_2          = DMA_SUBPERI1,//I2C1发送
    Channel_TIM4_CH3           = DMA_SUBPERI2,//定时器4通道3
    Channel_TIM2_UP_TIM2_CH4   = DMA_SUBPERI3,//定时器2更新和通道4
    Channel_UART5_TX           = DMA_SUBPERI4,//串口5发送
    Channel_TIM3_CH3           = DMA_SUBPERI5,//定时器3通道3
    Channel_I2C2_TX            = DMA_SUBPERI7,//I2C2发送

    Channel_ADC1_1             = DMA_SUBPERI0,//ADC1
    Channel_ADC3_1             = DMA_SUBPERI2,//ADC3
    Channel_SPI1_RX_1          = DMA_SUBPERI3,//SPI1接收
    Channel_SPI4_RX_1          = DMA_SUBPERI4,//SPI4接收
    Channel_TIM1_TRIG          = DMA_SUBPERI6,//定时器1触发
    Channel_DCMI_1             = DMA_SUBPERI1,//DCMI
    Channel_ADC3_2             = DMA_SUBPERI2,//ADC3
    Channel_SPI4_TX_1          = DMA_SUBPERI4,//SPI4发送
    Channel_USART6_RX_1        = DMA_SUBPERI5,//串口6接收
    Channel_TIM1_CH1_1         = DMA_SUBPERI6,//定时器1通道1
    Channel_TIM8_UP            = DMA_SUBPERI7,//定时器8更新
    Channel_TIM8_CH1_TIM8_CH2_TIM8_CH3 = DMA_SUBPERI0,//定时器8通道1和2和3
    Channel_ADC2_1             = DMA_SUBPERI1,//ADC2
    Channel_SPI1_RX_2          = DMA_SUBPERI3,//SPI1接收
    Channel_USART1_RX_1        = DMA_SUBPERI4,//串口1接收
    Channel_USART6_RX_2        = DMA_SUBPERI5,//串口6接收
    Channel_TIM1_CH2           = DMA_SUBPERI6,//定时器1通道2
    Channel_TIM8_CH1           = DMA_SUBPERI7,//定时器8通道1
    Channel_ADC2_2             = DMA_SUBPERI1,//ADC2
    Channel_SPI5_RX_1          = DMA_SUBPERI2,//SPI5接收
    Channel_SPI1_TX_1          = DMA_SUBPERI3,//SPI1发送
    Channel_SDIO_1             = DMA_SUBPERI4,//SDIO
    Channel_SPI4_RX_2          = DMA_SUBPERI5,//SPI4接收
    Channel_TIM1_CH1_2         = DMA_SUBPERI6,//定时器1通道1
    Channel_TIM8_CH2           = DMA_SUBPERI7,//定时器8通道2
    Channel_ADC1_2             = DMA_SUBPERI0,//ADC1
    Channel_SPI5_TX_1          = DMA_SUBPERI2,//SPI5发送
    Channel_SPI4_TX_2          = DMA_SUBPERI5,//SPI4发送
    Channel_TIM1_CH4_TIM1_TRIG_TIM1_COM = DMA_SUBPERI6,//定时器1通道4和触发和比较
    Channel_TIM8_CH3           = DMA_SUBPERI7,//定时器8通道3
    Channel_SPI6_TX            = DMA_SUBPERI1,//SPI6发送
    Channel_CRYP_OUT           = DMA_SUBPERI2,//CRYP输出
    Channel_SPI1_TX_2          = DMA_SUBPERI3,//SPI1发送
    Channel_USART1_RX_2        = DMA_SUBPERI4,//串口1接收
    Channel_TIM1_UP            = DMA_SUBPERI6,//定时器1更新
    Channel_SPI5_RX_2            = DMA_SUBPERI7,//SPI5接收
    Channel_TIM1_CH1_TIM1_CH2_TIM1_CH3 = DMA_SUBPERI0,//定时器1通道1和2和3
    Channel_SPI6_RX            = DMA_SUBPERI1,//SPI6接收
    Channel_CRYP_IN            = DMA_SUBPERI2,//CRYP输入
    Channel_SDIO_2             = DMA_SUBPERI4,//SDIO
    Channel_USART6_TX_1        = DMA_SUBPERI5,//串口6发送
    Channel_TIM1_CH3           = DMA_SUBPERI6,//定时器1通道3
    Channel_SPI5_TX_2          = DMA_SUBPERI7,//SPI5发送
    Channel_DCMI_2             = DMA_SUBPERI1,//DCMI
    Channel_HASH_IN            = DMA_SUBPERI2,//HASH输入
    Channel_USART1_TX          = DMA_SUBPERI4,//串口1发送
    Channel_USART6_TX_2        = DMA_SUBPERI5,//串口6发送
    Channel_TIM8_CH4_TIM8_TRIG_TIM8_COM = DMA_SUBPERI7,//定时器8通道4和触发和比较  
}WHT_DMA_Channel_enum;

/*DMA状态枚举*/
typedef enum
{
    TRx_TC = 0,//传输完成
    TRx_HT = 1,//传输过半
    TRx_TE = 2,//传输错误
    TRx_TS = 3,//传输开始
}WHT_DMA_State_enum;

/*DMA信息结构体*/
typedef struct
{
    unsigned char Mount_Count;//数据流挂载个数
    FlagStatus BSP_Mutex;     //驱动锁
    WHT_DMA_State_enum State; //传输状态
}WHT_DMA_Info_t;

/*DMA配置结构体*/
typedef struct
{
    WHT_DMA_Periph_enum Periph;     //外设号
    WHT_DMA_Stream_enum Stream;     //流
    WHT_DMA_Channel_enum Channel;   //通道
    dma_single_data_parameter_struct * DMA_InitStruct;//初始化结构体
    void (*IT_Callback)(void);      //回调函数
    volatile WHT_DMA_Info_t* Info;  //信息
    void* Private_Data;             //私有数据
}WHT_DMA_Config_t;

/*DMA回调函数结构体*/
typedef struct
{
    ErrStatus (*WHT_Register)(const WHT_DMA_Config_t* config);//注册
    void (*WHT_Config)(const WHT_DMA_Config_t* config);         //每个设备可自行配置
    unsigned short (*WHT_Get_Curr_Data_Count)(const WHT_DMA_Config_t* config);//获取当前DMA传输个数
    void (*WHT_Close)(const WHT_DMA_Config_t* config);          //DMA关闭
}WHT_DMA_BSP_t;

/*全局常量*/
extern const WHT_DMA_BSP_t WHT_DMA_BSP;

/*DMA内存拷贝API*/
extern void WHT_DMA_BSP_Memcpy(void* _Dst, void const* _Src, unsigned int _Size);//无法知道传输的状态

#endif // !__DMA_BSP_H__
