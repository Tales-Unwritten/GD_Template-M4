#include "../App/inc/command_parsing.h"

void command_parsing(PC_Transmit_Buffer_t *pc_buffer, void (*Usart_send_buffer)(uint8_t *buffer, uint8_t length))
{
    char response[300];
    if (pc_buffer->Buffer_Status == 1) // 检查缓冲区状态
    {
        
    }
}