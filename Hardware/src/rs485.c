#include "../Hardware/inc/rs485.h"

PC_Transmit_Buffer_t Rs485_Receive_Buffer[CHCHE_COUNT];

static struct
{
	volatile FlagStatus Finish_Flag;
	uint8_t Buffer_Length;
	uint8_t Buffer[128];
} Rs485_IT_Secd_Buffer;


static void Rs485_gpio_init(uint32_t band_rate)
{
	/* 开启时钟 */
	rcu_periph_clock_enable(RS485_USART_TX_RCU); 
	rcu_periph_clock_enable(RS485_USART_RX_RCU); 
	rcu_periph_clock_enable(RS485_USART_RCU);	 

	/* 配置GPIO复用功能 */
	gpio_af_set(RS485_USART_TX_PORT, RS485_USART_AF, RS485_USART_TX_PIN);
	gpio_af_set(RS485_USART_RX_PORT, RS485_USART_AF, RS485_USART_RX_PIN);

	/* 配置GPIO的模式 */
	/* 配置TX为复用模式 上拉模式 */
	gpio_mode_set(RS485_USART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, RS485_USART_TX_PIN);
	/* 配置RX为复用模式 上拉模式 */
	gpio_mode_set(RS485_USART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, RS485_USART_RX_PIN);

	/* 配置TX为推挽输出 50MHZ */
	gpio_output_options_set(RS485_USART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RS485_USART_TX_PIN);
	/* 配置RX为推挽输出 50MHZ */
	gpio_output_options_set(RS485_USART_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RS485_USART_RX_PIN);

	/* 配置串口的参数 */
	usart_deinit(RS485_USART);						           // 复位串口
	usart_baudrate_set(RS485_USART, band_rate);		           // 设置波特率
	usart_parity_config(RS485_USART, USART_PM_NONE);           // 没有校验位
	usart_word_length_set(RS485_USART, USART_WL_8BIT);         // 8位数据位
	usart_stop_bit_set(RS485_USART, USART_STB_1BIT);           // 1位停止位
	nvic_irq_enable(RS485_USART_IRQ, 0, 1);
	
	usart_transmit_config(RS485_USART, USART_TRANSMIT_ENABLE); // 使能串口发送
	usart_receive_config(RS485_USART, USART_RECEIVE_ENABLE);
	usart_interrupt_enable(RS485_USART, USART_INT_RBNE);
	usart_interrupt_enable(RS485_USART, USART_INT_IDLE);
	usart_enable(RS485_USART);								   // 使能串口
}

static void Rs485_en_gpio_init(void)
{
	/* 开启GPIOG时钟 */
	rcu_periph_clock_enable(RS485_USART_EN_RCU);

	/* 配置GPIOG13为推挽输出 */
	gpio_mode_set(RS485_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RS485_EN_PIN);
	gpio_output_options_set(RS485_EN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RS485_EN_PIN);

	/* 使能RS485 */
	gpio_bit_set(RS485_EN_PORT, RS485_EN_PIN); // 设置为高电平使能
}


static void set_rs485_en(uint8_t en)
{
	if (en)
	{
		gpio_bit_reset(RS485_EN_PORT, RS485_EN_PIN); // 设置为高电平使能
	}
	else
	{
		gpio_bit_set(RS485_EN_PORT, RS485_EN_PIN); // 设置为低电平禁用
	}
}


void rs485_send_it_data(uint8_t *buffer, uint8_t length)
{	led_on(2); // 打开LED2指示发送状态
	set_rs485_en(1); // 使能RS485发送
	if (length > sizeof(Rs485_IT_Secd_Buffer.Buffer))
	{
		length = sizeof(Rs485_IT_Secd_Buffer.Buffer); // 限制长度
	}

	Rs485_IT_Secd_Buffer.Finish_Flag = RESET;			 // 重置完成标志
	Rs485_IT_Secd_Buffer.Buffer_Length = length;		 // 设置缓冲区长度
	memcpy(Rs485_IT_Secd_Buffer.Buffer, buffer, length); // 复制数据到缓冲区

	usart_interrupt_enable(RS485_USART, USART_INT_TBE); // 使能发送中断
	usart_interrupt_enable(RS485_USART, USART_INT_TC);	// 使能发送完成中断
}

void rs485_send_data(uint8_t *buffer, uint8_t length)
{
	set_rs485_en(1); // 使能RS485发送
	uint32_t tx_count;
	usart_interrupt_disable(RS485_USART, USART_FLAG_TBE); // 禁止发送中断
	usart_interrupt_disable(RS485_USART, USART_FLAG_TC);  // 禁止发送完成中断
	usart_flag_clear(RS485_USART, USART_FLAG_TBE);		  // 清除发送数据缓冲区标志
	usart_flag_clear(RS485_USART, USART_FLAG_TC);		  // 清除发送完成标志

	for (tx_count = 0; tx_count < length; tx_count++)
	{
		usart_data_transmit(RS485_USART, buffer[tx_count]);
		while (RESET == usart_flag_get(RS485_USART, USART_FLAG_TBE))
			; // 等待发送数据缓冲区标志置位
	}
	while (RESET == usart_flag_get(RS485_USART, USART_FLAG_TC))
		; // 等待发送完成
	{
		/* code */
	}
	set_rs485_en(0); // 禁用RS485发送和切换到接收模式
}

void Rs485_Init_config(uint32_t band_rate)
{
	Rs485_gpio_init(band_rate); // 初始化串口GPIO
	Rs485_en_gpio_init();		// 初始化RS485使能GPIO
}

void USART1_IRQHandler(void)
{
	static uint8_t Temp_Recevice_Buffer[sizeof(Rs485_Receive_Buffer[0].Buffer)];
	static uint8_t Temp_Recevice_Count = 0;
	static uint8_t Send_Count = 0;

	/* 处理错误中断 - 优先处理错误 */
	if (usart_interrupt_flag_get(RS485_USART, USART_INT_FLAG_ERR_ORERR) ||
		usart_interrupt_flag_get(RS485_USART, USART_INT_FLAG_ERR_FERR) ||
		usart_interrupt_flag_get(RS485_USART, USART_INT_FLAG_ERR_NERR) ||
		usart_interrupt_flag_get(RS485_USART, USART_INT_FLAG_PERR))
	{
		/* 清除所有错误标志 */
		usart_flag_clear(RS485_USART, USART_FLAG_ORERR | USART_FLAG_FERR |
										  USART_FLAG_NERR | USART_FLAG_PERR);
		usart_data_receive(RS485_USART); // 读取数据寄存器清除错误

		/* 错误发生时重置接收状态 */
		memset(Temp_Recevice_Buffer, 0, sizeof(Temp_Recevice_Buffer));
		Temp_Recevice_Count = 0;
		return; // 错误处理后直接返回
	}

	/* 接收缓冲区非空中断 */
	if (usart_interrupt_flag_get(RS485_USART, USART_INT_FLAG_RBNE) == SET)
	{
		if (Temp_Recevice_Count < (sizeof(Temp_Recevice_Buffer) - 1))
		{
			Temp_Recevice_Buffer[Temp_Recevice_Count++] = usart_data_receive(RS485_USART);
		}
		else
		{
			usart_data_receive(RS485_USART); // 丢弃数据
		}
	}

	/* 空闲中断 - 接收完成 */
	if (usart_interrupt_flag_get(RS485_USART, USART_INT_FLAG_IDLE) == SET)
	{
		usart_data_receive(RS485_USART); // 清除空闲标志

		if (Temp_Recevice_Count > 0 && memchr(Temp_Recevice_Buffer, '\r\n', Temp_Recevice_Count) != NULL)
		{
			/* 查找空闲缓冲区并存储数据 */
			for (size_t i = 0; i < CHCHE_COUNT; i++)
			{
				if (Rs485_Receive_Buffer[i].Buffer_Status == 0)
				{
					led_toggle(2); // 切换LED2状态

					Rs485_Receive_Buffer[i].Buffer_Length = Temp_Recevice_Count;
					memcpy(Rs485_Receive_Buffer[i].Buffer, Temp_Recevice_Buffer, Temp_Recevice_Count);
					Rs485_Receive_Buffer[i].Buffer_Status = 1; // 标记为已接收
					break;
				}
			}
		}

		/* 清空临时接收缓冲区 */
		memset(Temp_Recevice_Buffer, 0, sizeof(Temp_Recevice_Buffer));
		Temp_Recevice_Count = 0;
	}

	/* 发送缓冲区空中断 */
	if (usart_interrupt_flag_get(RS485_USART, USART_INT_FLAG_TBE) == SET)
	{
		if (Rs485_IT_Secd_Buffer.Finish_Flag == RESET && Send_Count < Rs485_IT_Secd_Buffer.Buffer_Length)
		{
			usart_data_transmit(RS485_USART, Rs485_IT_Secd_Buffer.Buffer[Send_Count++]);
		}
		else
		{
			usart_interrupt_disable(RS485_USART, USART_INT_TBE); // 禁止发送中断
		}
	}

	/* 发送完成中断 */
	if (usart_interrupt_flag_get(RS485_USART, USART_INT_FLAG_TC) == SET)
	{
		usart_interrupt_disable(RS485_USART, USART_INT_TC); // 禁止发送完成中断
		Rs485_IT_Secd_Buffer.Finish_Flag = SET;				// 设置完成标志
		Send_Count = 0;										// 重置发送计数器
		set_rs485_en(0);									// 禁用RS485发送和切换到接收模式
		led_off(2); // 关闭LED2状态指示
	}
}
