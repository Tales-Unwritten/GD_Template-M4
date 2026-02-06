#include "../App/inc/pc_task.h"


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
    example_usage(); // 按键逻辑处理
}

