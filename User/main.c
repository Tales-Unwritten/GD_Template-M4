#include "main.h"
static void systick_task_config(void)
{
  systick_config();                                                                          // 滴答定时器初始化
  INA226_Device_Info.CHG_Ina226_Init(GPIOB, GPIO_PIN_14, GPIO_PIN_15, Ina226_7bit_address0); // 初始化INA226设备
  led_gpio_config();                                                                         // led初始化
  debug_init_config(2000000);                                                                // 串口初始化
  Rs485_Init_config(2000000);                                                                // RS485串口初始化
  USARTx_Init_config(2000000);                                                               // USARTx串口初始化
  eeprom_config(GPIOB, GPIO_PIN_6, GPIO_PIN_7);                                              // EEPROM配置
  // INA226_Device_Info.CHG_Ina226_Unlock_Alert();                                           // 解锁INA226的警报功能
  Key_Configuration(); // 按键初始化
  w25qxx_spi_config();
  INA228_Device_Func.CHG_INA228_Config(GPIOE, GPIO_PIN_2, GPIO_PIN_3, Ina228_7bit_address0); // INA228配置
  // delay_ms(10);
  // can_config();                                                                           // CAN配置
}

int main(void)
{
  systick_task_config(); // 初始化所有外设

  while (1)
  {
    APP_PC_Task(); // PC任务
  }
}
