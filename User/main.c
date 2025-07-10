#include "main.h"
int main(void)
{

  systick_config(); // 滴答定时器初始化
  // INA226_Device_Info.CHG_Ina226_Init(GPIOE, GPIO_PIN_2, GPIO_PIN_3, Ina226_7bit_address0); // 初始化INA226设备
  led_gpio_config();                            // led初始化
  debug_init_config(115200);                    // 串口初始化
  can_config();                                 // CAN配置
  eeprom_config(GPIOB, GPIO_PIN_6, GPIO_PIN_7); // EEPROM配置
  Rs485_Init_config(115200);                    // RS485串口初始化
  // BMP280_Init();
  // dip_switch_init(); // 初始化拨码开关
  // Key_Configuration(); // 按键初始化
  // INA226_Device_Info.CHG_Ina226_Unlock_Alert();            // 解锁INA226的警报功能

  Key_Configuration();                                                             // 按键初始化
  // uint8_t read_data0 = 0;                                                           // 定义读取数据变量
  // uint8_t read_data1 = 0;
  // uint8_t read_data2 = 0;
  uint8_t tx_page[8] = {0x60, 0x31, 0x32, 0x03, 0x94, 0x55, 0x66, 0x65}; // 定义发送数据
  uint8_t rx_page[8]; // 定义接收数据
  // ee_Write_Byte(0x00, 0x85); // 写入测试数据到地址0x00
  // ee_Write_Byte(0x01, 0xF0); // 写入测试数据到地址0x01
  // ee_Write_Byte(0x02, 0x99); // 写入测试数据到地址0x02
  // ee_erase_All();
  while (1)
  {
    // Key_Scan(); // 扫描按键状态
    // float bus_voltage = INA226_Device_Info.CHG_Ina226_Get_Bus_Voltage(); // 获取总线电压
    // float current = INA226_Device_Info.CHG_Ina226_Get_Current();         // 获取电流
    // INA226_Alert_detection();                                            // 检测INA226的警报引脚状态
    // // BMP280_Test();                                                   // 测试BMP280传感器

    // printf("Bus Voltage: %.3f V\r\n", bus_voltage); // 打印总线电压
    // printf("Current: %.3f A\r\n", current);         // 打印电流
    // delay_ms(300);                                   // 延时30ms
    // text_INA226_I2C();                              // 调试INA226 I2C通信

    // bitband_gpio_write_bit(GPIOC, GPIO_PIN_5, 1); // 设置GPIOC的PIN5引脚为高电平
    // delay_ms(1000); // 延时1秒
    // bitband_gpio_write_bit(GPIOC, GPIO_PIN_5, 0); // 设置GPIOC的PIN5引脚为低电平
    // delay_ms(1000); // 延时1秒

    // if (bitband_gpio_read(GPIOC, 13)) // 读取GPIOC的PIN13引脚状态
    // {
    //   led_on(1); // 如果引脚为高电平，点亮LED1
    //   value = 1; // 设置value为1
    // }
    // else
    // {
    //   led_off(1); // 如果引脚为低电平，熄灭LED1
    //   value = 0; // 设置value为0
    // }
    // verify_dip_switch();
    // uint8_t ack = ee_Check_Device();

    // if (ack == 1) // 如果返回值为0xFF，表示EEPROM未连接
    // {
    //   printf("EEPROM not connected!\r\n");
    //   led_off(1); // 熄灭LED1
    // }
    // else if (ack == 0) // 如果返回值为0x00，表示EEPROM连接正常
    // {
    //   printf("EEPROM connected successfully!\r\n");
    //   led_on(1); // 点亮LED1
    // }
    // ee_Check_Device_Read_Write(); // 读取和写入EEPROM数据
    // 擦除EEPROM全部内容

    // ee_erase_All();
    // ee_Write_Byte(0x00, 0x59); // 写入测试数据到地址0x00
    // delay_ms(10); // 延时10ms

    // ee_Write_Byte(0x02, 0x55); // 写入测试数据到地址0x02
    // delay_ms(10); // 延时10ms
    // read_data0 = ee_Read_Byte(0x00); // 从地址0x00读取数据
    // read_data1 = ee_Read_Byte(0x01); // 从地址0x01读取数据
    // read_data2 = ee_Read_Byte(0x02); // 从地址0x02读取数据


      // printf("read data from address 0x00: 0x%02X\r\n", read_data0);
      // printf("read data from address 0x02: 0x%02X\r\n", read_data1);
      // printf("read data from address 0x02: 0x%02X\r\n", read_data2);
    // ee_Test_Read_Write_Multi_Byte(); // 测试读写多字节数据
    // ee_Test_Read_Write_Page(); // 测试读写页面数据
    // ee_Write_Page_Byte(1, tx_page, sizeof(tx_page)); // 写入页面数据
    delay_ms(10); // 延时10ms
    ee_Read_Page_Byte(1, rx_page, sizeof(rx_page)); // 读取页面数据
    printf("Read Page Data: \r\n");
    for (uint8_t i = 0; i < sizeof(rx_page); i++)
    {
      printf("0x%02X \r\n", rx_page[i]); // 打印读取的数据
      delay_ms(10); // 延时10ms
    }
    ee_Test_Read_Multi_Byte(); // 测试读取多字节数据
  delay_ms(1000); // 延时1秒
}
}