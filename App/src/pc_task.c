#include "../App/inc/pc_task.h"

static PC_Transmit_Buffer_t *Get_485_Data(void (**usart_send_datar)(uint8_t *buffer, uint8_t length))
{
	for (size_t i = 0; i < CHCHE_COUNT_485; i++)
	{
		if (Rs485_Receive_Buffer[i].Buffer_Status == 1)
		{
			if (Rs485_Receive_Buffer[i].Buffer_Length > 3)
			{
				*usart_send_datar = rs485_send_it_data;
				return &Rs485_Receive_Buffer[i];
			}
			else
			{
				Rs485_Receive_Buffer[i].Buffer_Status = 0; // 重置状态
			}
		}
	}
	return NULL;
}

static PC_Transmit_Buffer_t *Get_Debug_Data(void (**usart_send_datar)(uint8_t *buffer, uint8_t length))
{
	for (size_t i = 0; i < CHCHE_COUNT; i++)
	{
		if (Debug_Receive_Buffer[i].Buffer_Status == 1)
		{
			if (Debug_Receive_Buffer[i].Buffer_Length > 3)
			{
				*usart_send_datar = debug_send_it_data;
				return &Debug_Receive_Buffer[i];
			}
			else
			{
				Debug_Receive_Buffer[i].Buffer_Status = 0; // 重置状态
			}
		}
	}
	return NULL;
}

void APP_PC_Task(void)
{
	PC_Transmit_Buffer_t *PC_Transmit_Buffer;
	void (*Usart_Send_Data)(uint8_t *buf, uint8_t count);
	for (;;)
	{
		PC_Transmit_Buffer = Get_485_Data(&Usart_Send_Data);
		if (PC_Transmit_Buffer != NULL)
		{
			// led_on(1);
			break;
		}
		PC_Transmit_Buffer = Get_Debug_Data(&Usart_Send_Data);
		if (PC_Transmit_Buffer != NULL)
		{
			// led_on(2);
			break;
		}
		delay_ms(3); // 无数据时延时3ms
	}
	command_parsing(PC_Transmit_Buffer, Usart_Send_Data);
	memset(PC_Transmit_Buffer->Buffer, 0, PC_Transmit_Buffer->Buffer_Length);
	PC_Transmit_Buffer->Buffer_Status = 0; // 重置状态
	PC_Transmit_Buffer->Buffer_Length = 0;
	// led_off(1);
	// led_off(2);
}
