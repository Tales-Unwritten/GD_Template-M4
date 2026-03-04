#include "../App/inc/pc_task.h"


uint32_t baud;


const uint32_t baud_table[] = {
    110,     // 0
    300,     // 1
    600,     // 2
    1200,    // 3
    2400,    // 4
    4800,    // 5
    9600,    // 6
    19200,   // 7
    38400,   // 8
    57600,   // 9
    115200,  // 10
    128000,  // 11
    230400,  // 12
    256000,  // 13
    460800,  // 14
    500000,  // 15
    512000,  // 16
    600000,  // 17
    750000,  // 18
    921600,  // 19
    1000000, // 20
    1500000, // 21
    2000000  // 22
};

// 写入
void Save_BaudValue(uint32_t baud)
{
    uint8_t *p = (uint8_t *)&baud;
    for(int i = 0; i < 4; i++) {
        ee_Write_Byte(EEPROM_ADDR_BAUD_VAL + i, p[i]);
    }
}

// 读取 + 验证
uint32_t Load_BaudValue(void)
{
    uint32_t baud = 0;
    uint8_t *p = (uint8_t *)&baud;
    for(int i = 0; i < 4; i++) {
        p[i] = ee_Read_Byte(EEPROM_ADDR_BAUD_VAL + i);
    }
    // 验证是否在合法范围内
    for(int i = 0; i < sizeof(baud_table)/sizeof(baud_table[0]); i++) 
    {
        if (baud == baud_table[i]) 
        {
            return baud;  // 合法
        }
    }
    return 115200;  // 非法则返回默认值
}



static PC_Transmit_Buffer_t *Get_PC_Data(void (**usart_send_data)(uint8_t *buffer, uint8_t length))
{
    for (size_t i = 0; i < CHCHE_COUNT; i++)
    {
        if (Rs485_Receive_Buffer[i].Buffer_Status)
        {
            led_on (2);
            led_off(1);
            led_off(3);
            *usart_send_data = rs485_send_it_data;
            return &Rs485_Receive_Buffer[i];
        }
        if (Debug_Receive_Buffer[i].Buffer_Status)
        {
            led_on (1);
            led_off(2);
            led_off(3);
            *usart_send_data = debug_send_it_data;
            return &Debug_Receive_Buffer[i];
        }
		if (USARTx_Receive_Buffer[i].Buffer_Status)
		{
            led_on (3);
            led_off(1);
            led_off(2);
			*usart_send_data = usartx_send_it_data;
			return &USARTx_Receive_Buffer[i];
		}
		
    }
    return NULL;
}

void APP_PC_Task(void)
{
    void (*Usart_Send_Data)(uint8_t *buf, uint8_t count);
    PC_Transmit_Buffer_t *PC_Transmit_Buffer = Get_PC_Data(&Usart_Send_Data);
    
    if (PC_Transmit_Buffer != NULL)
    {
        command_parsing(PC_Transmit_Buffer, Usart_Send_Data);
        memset(PC_Transmit_Buffer->Buffer, 0, sizeof(PC_Transmit_Buffer->Buffer));
        PC_Transmit_Buffer->Buffer_Status = 0;
        led_off(1);
        led_off(2);
        led_off(3);
    }
    // example_usage(); // 按键逻辑处理
}

