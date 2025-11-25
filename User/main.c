#include "main.h"
static void systick_task_config(void)
{
  systick_config();                                                                        // 滴答定时器初始化
  // INA226_Device_Info.CHG_Ina226_Init(GPIOE, GPIO_PIN_2, GPIO_PIN_3, Ina226_7bit_address0); // 初始化INA226设备
  led_gpio_config();                                                                       // led初始化
  debug_init_config(115200);                                                               // 串口初始化
  Rs485_Init_config(115200);                                                               // RS485串口初始化
  eeprom_config(GPIOB, GPIO_PIN_6, GPIO_PIN_7);                                            // EEPROM配置
  // INA226_Device_Info.CHG_Ina226_Unlock_Alert();                                            // 解锁INA226的警报功能
  Key_Configuration();                                                                     // 按键初始化
  w25qxx_spi_config();
  INA228_Device_Func.CHG_INA228_Config(GPIOE,GPIO_PIN_2,GPIO_PIN_3,Ina228_7bit_address0);      // INA228配置
  delay_ms(10);
  // can_config();                                                                            // CAN配置
  
}

int main(void)
{
  systick_task_config(); // 初始化所有外设

  debug_send_it_data((uint8_t *)"System Init OK\r\n", 16);
  rs485_send_it_data((uint8_t *)"RS485 Init OK\r\n", 16);
  
  // WHT_GPIO_BSP.WHT_Set_Pin(PortC, Pin4, Low); 
  // WHT_GPIO_BSP.WHT_Set_Pin(PortC, Pin5, Low); 
  // WHT_GPIO_BSP.WHT_Set_Pin(PortA, Pin7, Low); 
  while (1)
  {
    APP_PC_Task(); // PC任务

  

    // rs485_send_it_data((uint8_t *)"RS485 Init OK\r\n", 16);
  }
}


