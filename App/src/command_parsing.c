#include "../App/inc/command_parsing.h"

void command_parsing(PC_Transmit_Buffer_t *PC_Transmit_Buffer, void (*Usart_Send_Data)(uint8_t *buf, uint8_t count))
{
    for (size_t i = 0; i < CHCHE_COUNT; i++)
    {
        if (PC_Transmit_Buffer->Buffer_Status == 1)
        {
            bool command_found = false;
            for (size_t j = 0; j < 7; j++)
            {
                if (memcmp((char *)PC_Transmit_Buffer->Buffer, cmd_table[j].cmd, strlen(cmd_table[j].cmd)) == 0)
                {
                    cmd_table[j].handler(); // 调用对应的处理函数
                    Usart_Send_Data((uint8_t *)send_buffer, strlen(send_buffer));
                    command_found = true;
                    memset(PC_Transmit_Buffer->Buffer, 0, sizeof(PC_Transmit_Buffer->Buffer));
                    PC_Transmit_Buffer->Buffer_Status = 0; // 重置状态
                    PC_Transmit_Buffer->Buffer_Length = 0;
                    break;
                }
            }
            if (!command_found)
            {
                for (size_t i = 0; i < 34; i++)
                {
                    if (memcmp((char *)PC_Transmit_Buffer->Buffer, channel_cmd[i].rese_mess, strlen(channel_cmd[i].rese_mess)) == 0)
                    {
                        Usart_Send_Data((uint8_t *)channel_cmd[i].send_mess, strlen(channel_cmd[i].send_mess));
                        command_found = true;
                        memset(PC_Transmit_Buffer->Buffer, 0, sizeof(PC_Transmit_Buffer->Buffer));
                        PC_Transmit_Buffer->Buffer_Status = 0; // 重置状态
                        PC_Transmit_Buffer->Buffer_Length = 0;
                        break;
                    }
                }
            }
            if (!command_found)
            {
                Usart_Send_Data((uint8_t *)"Unknown command\r\n", strlen("Unknown command\r\n"));
                memset(PC_Transmit_Buffer->Buffer, 0, sizeof(PC_Transmit_Buffer->Buffer));
                PC_Transmit_Buffer->Buffer_Status = 0; // 重置状态
                PC_Transmit_Buffer->Buffer_Length = 0;
            }
        }
    }
}
