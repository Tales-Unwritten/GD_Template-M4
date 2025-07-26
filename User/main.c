#include "main.h"
// int main(void)
// {

//   systick_config(); // 滴答定时器初始化
//   // INA226_Device_Info.CHG_Ina226_Init(GPIOE, GPIO_PIN_2, GPIO_PIN_3, Ina226_7bit_address0); // 初始化INA226设备
//   led_gpio_config();                            // led初始化
//   debug_init_config(115200);                    // 串口初始化
//   can_config();                                 // CAN配置
//   eeprom_config(GPIOB, GPIO_PIN_6, GPIO_PIN_7); // EEPROM配置
//   Rs485_Init_config(115200);                    // RS485串口初始化
//   // BMP280_Init();
//   // dip_switch_init(); // 初始化拨码开关
//   // Key_Configuration(); // 按键初始化
//   // INA226_Device_Info.CHG_Ina226_Unlock_Alert();            // 解锁INA226的警报功能

//   Key_Configuration(); // 按键初始化
//   w25qxx_spi_config(); // W25Q128 SPI配置
//   // uint8_t read_data0 = 0;                                                           // 定义读取数据变量
//   // uint8_t read_data1 = 0;
//   // uint8_t read_data2 = 0;
//   uint8_t tx_page[8] = {0x60, 0x31, 0x32, 0x03, 0x94, 0x55, 0x66, 0x65}; // 定义发送数据
//   uint8_t rx_page[32];                                                    // 定义接收数据
//   // ee_Write_Byte(0x00, 0x85); // 写入测试数据到地址0x00
//   // ee_Write_Byte(0x01, 0xF0); // 写入测试数据到地址0x01
//   // ee_Write_Byte(0x02, 0x99); // 写入测试数据到地址0x02
//   // ee_erase_All();
//   My_W25Q28_TEXT(); // 调用W25Q128测试函数
//   while (1)
//   {
//     // Key_Scan(); // 扫描按键状态
//     // float bus_voltage = INA226_Device_Info.CHG_Ina226_Get_Bus_Voltage(); // 获取总线电压
//     // float current = INA226_Device_Info.CHG_Ina226_Get_Current();         // 获取电流
//     // INA226_Alert_detection();                                            // 检测INA226的警报引脚状态
//     // // BMP280_Test();                                                   // 测试BMP280传感器

//     // printf("Bus Voltage: %.3f V\r\n", bus_voltage); // 打印总线电压
//     // printf("Current: %.3f A\r\n", current);         // 打印电流
//     // delay_ms(300);                                   // 延时30ms
//     // text_INA226_I2C();                              // 调试INA226 I2C通信

//     // bitband_gpio_write_bit(GPIOC, GPIO_PIN_5, 1); // 设置GPIOC的PIN5引脚为高电平
//     // delay_ms(1000); // 延时1秒
//     // bitband_gpio_write_bit(GPIOC, GPIO_PIN_5, 0); // 设置GPIOC的PIN5引脚为低电平
//     // delay_ms(1000); // 延时1秒

//     // if (bitband_gpio_read(GPIOC, 13)) // 读取GPIOC的PIN13引脚状态
//     // {
//     //   led_on(1); // 如果引脚为高电平，点亮LED1
//     //   value = 1; // 设置value为1
//     // }
//     // else
//     // {
//     //   led_off(1); // 如果引脚为低电平，熄灭LED1
//     //   value = 0; // 设置value为0
//     // }
//     // verify_dip_switch();
//     // uint8_t ack = ee_Check_Device();

//     // if (ack == 1) // 如果返回值为0xFF，表示EEPROM未连接
//     // {
//     //   printf("EEPROM not connected!\r\n");
//     //   led_off(1); // 熄灭LED1
//     // }
//     // else if (ack == 0) // 如果返回值为0x00，表示EEPROM连接正常
//     // {
//     //   printf("EEPROM connected successfully!\r\n");
//     //   led_on(1); // 点亮LED1
//     // }
//     // ee_Check_Device_Read_Write(); // 读取和写入EEPROM数据
//     // 擦除EEPROM全部内容

//     // ee_erase_All();
//     // ee_Write_Byte(0x00, 0x59); // 写入测试数据到地址0x00
//     // delay_ms(10); // 延时10ms

//     // ee_Write_Byte(0x02, 0x55); // 写入测试数据到地址0x02
//     // delay_ms(10); // 延时10ms
//     // read_data0 = ee_Read_Byte(0x00); // 从地址0x00读取数据
//     // read_data1 = ee_Read_Byte(0x01); // 从地址0x01读取数据
//     // read_data2 = ee_Read_Byte(0x02); // 从地址0x02读取数据

//     // printf("read data from address 0x00: 0x%02X\r\n", read_data0);
//     // printf("read data from address 0x02: 0x%02X\r\n", read_data1);
//     // printf("read data from address 0x02: 0x%02X\r\n", read_data2);
//     // ee_Test_Read_Write_Multi_Byte(); // 测试读写多字节数据
//     // ee_Test_Read_Write_Page(); // 测试读写页面数据
//     // ee_Write_Page_Byte(1, tx_page, sizeof(tx_page)); // 写入页面数据
//     // delay_ms(1000);                                   // 延时1秒
//     // ee_Read_Page_Byte(1, rx_page, sizeof(rx_page)); // 读取页面数据
//     // printf("Read Page Data: \r\n");
//     // for (uint8_t i = 0; i < sizeof(rx_page); i++)
//     // {
//     //   printf("0x%02X \r\n", rx_page[i]); // 打印读取的数据
//     //   delay_ms(1000);                      // 延时1秒
//     // }
//     // ee_Test_Read_Multi_Byte(); // 测试读取多字节数据
//     // w25qxx_read(rx_page, 0x000004, 32);
//     // for (size_t i = 0; i < sizeof(rx_page); i++)
//     // {
//     //   printf("0x%02X ", rx_page[i]);
//     // }
//     // printf("\r\n");

//     // delay_ms(1000);            // 延时1秒
//     can_transmit_text(); // 发送文本数据
//   }
// }


//=======================================================================================

/*!
    \file    main.c
    \brief   communication_among_Devices in normal mode

    \version 2024-12-20, V3.3.1, firmware for GD32F4xx
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f4xx.h"
#include <stdio.h>

/* select can */
// #define DEV_CAN0_USED
// #define DEV_CAN1_USED

#ifdef DEV_CAN0_USED
#define CANX CAN0
#else
#define CANX CAN1
#endif



void nvic_config(void);
void led_config(void);
void gpio_config(void);
ErrStatus can_networking(void);
void can_networking_init(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{

FlagStatus receive_flag;
uint8_t transmit_number = 0x0;
can_receive_message_struct receive_message;
can_trasnmit_message_struct transmit_message;



  uint8_t i = 0;
  uint32_t timeout = 0xFFFF;
  uint8_t transmit_mailbox = 0;

  receive_flag = RESET;
  /* configure Tamper key */
  /* configure GPIO */
  gpio_config();
  /* configure USART */
  /* configure NVIC */
  nvic_config();
  /* configure leds */
  /* set all leds off */
  /* initialize CAN */
  can_networking_init();

  /* enable CAN receive FIFO1 not empty interrupt */
  can_interrupt_enable(CANX, CAN_INT_RFNE1);

  /* initialize transmit message */
  can_struct_para_init(CAN_TX_MESSAGE_STRUCT, &transmit_message);
  transmit_message.tx_sfid = 0x00;
  transmit_message.tx_efid = 0xaabb;
  transmit_message.tx_ft = CAN_FT_DATA;
  transmit_message.tx_ff = CAN_FF_EXTENDED;
  transmit_message.tx_dlen = 8;

  transmit_message.tx_data[0] = 0xA0;
  transmit_message.tx_data[1] = 0xA1;
  transmit_message.tx_data[2] = 0xA2;
  transmit_message.tx_data[3] = 0xA3;
  transmit_message.tx_data[4] = 0xA4;
  transmit_message.tx_data[5] = 0xA5;
  transmit_message.tx_data[6] = 0xA6;
  transmit_message.tx_data[7] = 0xA7;

  printf("Please press the Tamper key to transmit data!\r\n");

  /* initialize receive message */
  can_struct_para_init(CAN_RX_MESSAGE_STRUCT, &receive_message);

  while (1)
  {
    if (transmit_number == 0x10)
    {
      transmit_number = 0x00;
    }
    else
    {
      transmit_message.tx_data[0] = transmit_number++;
      printf("\r\n can transmit data:");
      for (i = 0; i < transmit_message.tx_dlen; i++)
      {
        printf(" %02x", transmit_message.tx_data[i]);
      }
      /* transmit message */
      transmit_mailbox = can_message_transmit(CANX, &transmit_message);
      /* waiting for transmit completed */
      timeout = 0xFFFF;
      while ((CAN_TRANSMIT_OK != can_transmit_states(CANX, transmit_mailbox)) && (0 != timeout))
      {
        timeout--;
      }
    }
  }
  if (SET == receive_flag)
  {
    receive_flag = RESET;
    printf("\r\n can receive data:");
    for (i = 0; i < receive_message.rx_dlen; i++)
    {
      printf(" %02x", receive_message.rx_data[i]);
    }
  }
}


void can_networking_init(void)
{
    can_parameter_struct can_parameter;
    can_filter_parameter_struct can_filter;

    can_struct_para_init(CAN_INIT_STRUCT, &can_parameter);
    can_struct_para_init(CAN_FILTER_STRUCT, &can_filter);

    /* initialize CAN register */
    can_deinit(CANX);

    /* initialize CAN */
    can_parameter.time_triggered = DISABLE;
    can_parameter.auto_bus_off_recovery = ENABLE;
    can_parameter.auto_wake_up = DISABLE;
    can_parameter.auto_retrans = ENABLE;
    can_parameter.rec_fifo_overwrite = DISABLE;
    can_parameter.trans_fifo_order = DISABLE;
    can_parameter.working_mode = CAN_NORMAL_MODE;
    can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    can_parameter.time_segment_1 = CAN_BT_BS1_7TQ;
    can_parameter.time_segment_2 = CAN_BT_BS2_2TQ;
    /* baudrate 1Mbps */
    can_parameter.prescaler = 5;
    can_init(CANX, &can_parameter);

    /* initialize filter */
#ifdef DEV_CAN0_USED
    /* CAN0 filter number */
    can_filter.filter_number = 0;
#else
    /* CAN1 filter number */
    can_filter.filter_number = 15;
#endif
    /* initialize filter */
    can_filter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter.filter_list_high = 0x0000;
    can_filter.filter_list_low = 0x0000;
    can_filter.filter_mask_high = 0x0000;
    can_filter.filter_mask_low = 0x0000;
    can_filter.filter_fifo_number = CAN_FIFO1;
    can_filter.filter_enable = ENABLE;
    can_filter_init(&can_filter);
}

/*!
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
#ifdef DEV_CAN0_USED
    /* configure CAN0 NVIC */
    nvic_irq_enable(CAN0_RX1_IRQn, 0, 0);
#else
    /* configure CAN1 NVIC */
    nvic_irq_enable(CAN1_RX1_IRQn, 0, 0);
#endif
}

void gpio_config(void)
{
    /* enable can clock */
    rcu_periph_clock_enable(RCU_CAN0);
    rcu_periph_clock_enable(RCU_CAN1);
    rcu_periph_clock_enable(RCU_GPIOB);

    /* configure CAN0 GPIO */
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_8);

    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_9);

    /* configure CAN1 GPIO */
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_12);

    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_13);
}

/* retarget the C library printf function to the usart */
// int fputc(int ch, FILE *f)
// {
//     usart_data_transmit(usart1, (uint8_t)ch);
//     while (RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE))
//         ;
//     return ch;
// }
