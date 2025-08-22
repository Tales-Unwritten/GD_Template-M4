#include "main.h"
static void systick_task_config(void)
{
  systick_config(); // 滴答定时器初始化
  INA226_Device_Info.CHG_Ina226_Init(GPIOE, GPIO_PIN_2, GPIO_PIN_3, Ina226_7bit_address0); // 初始化INA226设备
  led_gpio_config();                            // led初始化
  debug_init_config(115200);                    // 串口初始化
  can_config();                                 // CAN配置
  eeprom_config(GPIOB, GPIO_PIN_6, GPIO_PIN_7); // EEPROM配置
  Rs485_Init_config(115200);                    // RS485串口初始化
  INA226_Device_Info.CHG_Ina226_Unlock_Alert();            // 解锁INA226的警报功能
  Key_Configuration(); // 按键初始化
  w25qxx_spi_config(); // W25Q128 SPI配置
  
}

int main(void)
{
  systick_task_config(); // 初始化所有外设
  while (1)
  {
    test_bitband_gpio_functions(); // 测试bitband GPIO函数
    delay_ms(1000); // 延时1秒
  }
}
