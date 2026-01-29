#include "../App/inc/command_parsing.h"

void command_parsing(PC_Transmit_Buffer_t *PC_Transmit_Buffer, void (*Usart_Send_Data)(uint8_t *buf, uint8_t count))
{
    for (size_t i = 0; i < CHCHE_COUNT; i++)
    {

        if (PC_Transmit_Buffer[i].Buffer_Status == 1 && PC_Transmit_Buffer[i].Buffer_Length > 3)
        {
            // if (strchr((char *)PC_Transmit_Buffer[i].Buffer, '\r') == NULL && strchr((char *)PC_Transmit_Buffer[i].Buffer, '\n') == NULL)
            // {
            //     // Usart_Send_Data((uint8_t *)"format_error\r\n", strlen("format_error\r\n"));
            //     Usart_Send_Data((uint8_t *)"format_error\r\n",15);
            //     break;
            // }
            bool command_found = false;
            for (size_t j = 0; j < sizeof(cmd_table) / sizeof(cmd_table[0]); j++)
            {
                if (memcmp((char *)PC_Transmit_Buffer[i].Buffer, cmd_table[j].cmd, strlen(cmd_table[j].cmd)) == 0)
                {
                    cmd_table[j].handler(); // 调用对应的处理函数
                    Usart_Send_Data((uint8_t *)send_buffer, strlen(send_buffer));
                    command_found = true;
                    break;
                }
            }
            if (!command_found)
            {
                for (size_t j = 0; j < sizeof(channel_cmd) / sizeof(channel_cmd[0]); j++)
                {
                    if (memcmp((char *)PC_Transmit_Buffer[i].Buffer, channel_cmd[j].rese_mess, strlen(channel_cmd[j].rese_mess)) == 0)
                    {
                        Usart_Send_Data((uint8_t *)channel_cmd[j].send_mess, strlen(channel_cmd[j].send_mess));
                        command_found = true;
                        break;
                    }
                }
            }
            memset(send_buffer, 0, sizeof(send_buffer)); // 清空发送缓冲区
            // if (!command_found)
            // {
            //     // Usart_Send_Data((uint8_t *)"unknown_command\r\n", strlen("unknown_command\r\n"));
            //     Usart_Send_Data((uint8_t *)"unknown_command\r\n",18);
            //     break;
            // }
        }
    }
}
