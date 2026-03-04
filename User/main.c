#include "main.h"
#define but 115200


static void systick_task_config(void)
{
  systick_config();                                                                          // 滴答定时器初始化
  eeprom_config(GPIOB, GPIO_PIN_6, GPIO_PIN_7);                                              // EEPROM配置
  Save_BaudValue(baud_table[10]);
  uint32_t bt= Load_BaudValue();

  // INA226_Device_Info.CHG_Ina226_Init(GPIOB, GPIO_PIN_14, GPIO_PIN_15, Ina226_7bit_address0); // 初始化INA226设备
  led_gpio_config();                                                                         // led初始化
  debug_init_config(bt);                                                                // 串口初始化
  printf("Bt=%d\r\n",bt);
  Rs485_Init_config(RS485_USART_BAUDRATE);                                                                // RS485串口初始化
  usartx_config(bt);                                                                    // USARTx串口初始化
  // INA226_Device_Info.CHG_Ina226_Unlock_Alert();                                           // 解锁INA226的警报功能
  w25qxx_spi_config();
  INA228_Device_Func.CHG_INA228_Config(GPIOE, GPIO_PIN_2, GPIO_PIN_3, Ina228_7bit_address0); // INA228配置
  Key_BSP_Init();                                                                 // 按键定时器初始化
  // delay_ms(10);
  // can_config();                                                                           // CAN配置
  // ee_Test_Read_Multi_Byte();
}


int main(void)
{
  systick_task_config(); // 初始化所有外设
  ee_Test_Read_Multi_Byte();

  while (1)
  {
    APP_PC_Task(); // PC任务
  }
}
