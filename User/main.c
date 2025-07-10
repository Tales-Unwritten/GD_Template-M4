#include "main.h"
int main(void)
{

  systick_config(); // �δ�ʱ����ʼ��
  // INA226_Device_Info.CHG_Ina226_Init(GPIOE, GPIO_PIN_2, GPIO_PIN_3, Ina226_7bit_address0); // ��ʼ��INA226�豸
  led_gpio_config();                            // led��ʼ��
  debug_init_config(115200);                    // ���ڳ�ʼ��
  can_config();                                 // CAN����
  eeprom_config(GPIOB, GPIO_PIN_6, GPIO_PIN_7); // EEPROM����
  Rs485_Init_config(115200);                    // RS485���ڳ�ʼ��
  // BMP280_Init();
  // dip_switch_init(); // ��ʼ�����뿪��
  // Key_Configuration(); // ������ʼ��
  // INA226_Device_Info.CHG_Ina226_Unlock_Alert();            // ����INA226�ľ�������

  Key_Configuration();                                                             // ������ʼ��
  // uint8_t read_data0 = 0;                                                           // �����ȡ���ݱ���
  // uint8_t read_data1 = 0;
  // uint8_t read_data2 = 0;
  uint8_t tx_page[8] = {0x60, 0x31, 0x32, 0x03, 0x94, 0x55, 0x66, 0x65}; // ���巢������
  uint8_t rx_page[8]; // �����������
  // ee_Write_Byte(0x00, 0x85); // д��������ݵ���ַ0x00
  // ee_Write_Byte(0x01, 0xF0); // д��������ݵ���ַ0x01
  // ee_Write_Byte(0x02, 0x99); // д��������ݵ���ַ0x02
  // ee_erase_All();
  while (1)
  {
    // Key_Scan(); // ɨ�谴��״̬
    // float bus_voltage = INA226_Device_Info.CHG_Ina226_Get_Bus_Voltage(); // ��ȡ���ߵ�ѹ
    // float current = INA226_Device_Info.CHG_Ina226_Get_Current();         // ��ȡ����
    // INA226_Alert_detection();                                            // ���INA226�ľ�������״̬
    // // BMP280_Test();                                                   // ����BMP280������

    // printf("Bus Voltage: %.3f V\r\n", bus_voltage); // ��ӡ���ߵ�ѹ
    // printf("Current: %.3f A\r\n", current);         // ��ӡ����
    // delay_ms(300);                                   // ��ʱ30ms
    // text_INA226_I2C();                              // ����INA226 I2Cͨ��

    // bitband_gpio_write_bit(GPIOC, GPIO_PIN_5, 1); // ����GPIOC��PIN5����Ϊ�ߵ�ƽ
    // delay_ms(1000); // ��ʱ1��
    // bitband_gpio_write_bit(GPIOC, GPIO_PIN_5, 0); // ����GPIOC��PIN5����Ϊ�͵�ƽ
    // delay_ms(1000); // ��ʱ1��

    // if (bitband_gpio_read(GPIOC, 13)) // ��ȡGPIOC��PIN13����״̬
    // {
    //   led_on(1); // �������Ϊ�ߵ�ƽ������LED1
    //   value = 1; // ����valueΪ1
    // }
    // else
    // {
    //   led_off(1); // �������Ϊ�͵�ƽ��Ϩ��LED1
    //   value = 0; // ����valueΪ0
    // }
    // verify_dip_switch();
    // uint8_t ack = ee_Check_Device();

    // if (ack == 1) // �������ֵΪ0xFF����ʾEEPROMδ����
    // {
    //   printf("EEPROM not connected!\r\n");
    //   led_off(1); // Ϩ��LED1
    // }
    // else if (ack == 0) // �������ֵΪ0x00����ʾEEPROM��������
    // {
    //   printf("EEPROM connected successfully!\r\n");
    //   led_on(1); // ����LED1
    // }
    // ee_Check_Device_Read_Write(); // ��ȡ��д��EEPROM����
    // ����EEPROMȫ������

    // ee_erase_All();
    // ee_Write_Byte(0x00, 0x59); // д��������ݵ���ַ0x00
    // delay_ms(10); // ��ʱ10ms

    // ee_Write_Byte(0x02, 0x55); // д��������ݵ���ַ0x02
    // delay_ms(10); // ��ʱ10ms
    // read_data0 = ee_Read_Byte(0x00); // �ӵ�ַ0x00��ȡ����
    // read_data1 = ee_Read_Byte(0x01); // �ӵ�ַ0x01��ȡ����
    // read_data2 = ee_Read_Byte(0x02); // �ӵ�ַ0x02��ȡ����


      // printf("read data from address 0x00: 0x%02X\r\n", read_data0);
      // printf("read data from address 0x02: 0x%02X\r\n", read_data1);
      // printf("read data from address 0x02: 0x%02X\r\n", read_data2);
    // ee_Test_Read_Write_Multi_Byte(); // ���Զ�д���ֽ�����
    // ee_Test_Read_Write_Page(); // ���Զ�дҳ������
    // ee_Write_Page_Byte(1, tx_page, sizeof(tx_page)); // д��ҳ������
    delay_ms(10); // ��ʱ10ms
    ee_Read_Page_Byte(1, rx_page, sizeof(rx_page)); // ��ȡҳ������
    printf("Read Page Data: \r\n");
    for (uint8_t i = 0; i < sizeof(rx_page); i++)
    {
      printf("0x%02X \r\n", rx_page[i]); // ��ӡ��ȡ������
      delay_ms(10); // ��ʱ10ms
    }
    ee_Test_Read_Multi_Byte(); // ���Զ�ȡ���ֽ�����
  delay_ms(1000); // ��ʱ1��
}
}