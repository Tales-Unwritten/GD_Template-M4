#include "../Hardware/modbus/inc/modbus_port.h"

void Modbus_PortSend(uint8_t *buf,uint16_t len)
{

    // for(uint16_t i=0;i<len;i++)
    // {

    //     while(!(USART1->SR & USART_SR_TXE));

    //     USART1->DR = buf[i];

    // }
    rs485_send_it_data(buf,len)


}


// void USART1_IRQHandler(void)
// {

//     if(USART_GetITStatus(USART1,USART_IT_RXNE))
//     {

//         uint8_t data = USART_ReceiveData(USART1);

//         Modbus_ReceiveByte(data);

//     }

//     if(USART_GetITStatus(USART1,USART_IT_IDLE))
//     {

//         volatile uint32_t temp;

//         temp = USART1->SR;
//         temp = USART1->DR;

//         Modbus_FrameProcess();

//     }

// }

// 十七、测试示例
// 
// 主站发送：
// 
// 01 03 00 00 00 02 CRC
// 
// 读取：
// 
// DeviceBlock[0]
// DeviceBlock[1]
// 
// 返回：
// 
// 01 03 04 00 01 00 64 CRC
