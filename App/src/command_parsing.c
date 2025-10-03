#include "../App/inc/command_parsing.h"

static char send_buffer[128] = {0}; // Increased buffer size to 128 bytes

// 命令处理函数类型
typedef void (*cmd_handler_t)(void);

// 命令处理函数
static void handle_bus_voltage(void)
{
    float val = INA226_Device_Info.CHG_Ina226_Get_Bus_Voltage();
    sprintf((char *)send_buffer, "Vbus_Val=%0.4f\r\n", val);
}

static void handle_current(void)
{
    float val = INA226_Device_Info.CHG_Ina226_Get_Current();
    sprintf((char *)send_buffer, "Curr_Val=%0.4f\r\n", val);
}

static void handle_shunt_voltage(void)
{
    float val = INA226_Device_Info.CHG_Ina226_Get_Shunt_Voltage();
    sprintf((char *)send_buffer, "shunt_voltage=%0.4f\r\n", val);
}

static void handle_power(void)
{
    uint16_t val = INA226_Device_Info.CHG_Ina226_Get_Power();
    sprintf((char *)send_buffer, "Power_Value=0x%04X\r\n", val);
}

static void handle_unlock_alert(void)
{
    uint16_t val = INA226_Device_Info.CHG_Ina226_Get_Die_ID();
    sprintf((char *)send_buffer, "Alert is Ulock=0x%04X\r\n", val);
}

static void handle_manufacturer_id(void)
{
    uint16_t val = INA226_Device_Info.CHG_Ina226_Get_Manufacturer_ID();
    sprintf((char *)send_buffer, "Manufacturer_ID=0x%04X\r\n", val);
}

static void handle_die_id(void)
{
    uint16_t val = INA226_Device_Info.CHG_Ina226_Get_Die_ID();
    sprintf((char *)send_buffer, "Die_ID=0x%04X\r\n", val);
}

// 命令表
static const struct
{
    const char *cmd;
    cmd_handler_t handler;
} cmd_table[] = {
    {"get_bus_voltage\r\n", handle_bus_voltage},
    {"get_current\r\n", handle_current},
    {"get_shunt_voltage\r\n", handle_shunt_voltage},
    {"get_power\r\n", handle_power},
    {"ulock_alert\r\n", handle_unlock_alert},
    {"get_manufacturer_id\r\n", handle_manufacturer_id},
    {"get_die_id\r\n", handle_die_id}};

static void command_parsing(void)
{
    for (size_t i = 0; i < CHCHE_COUNT; i++)
    {
        if (Debug_Receive_Buffer[i].Buffer_Status == 1)
        {
            bool command_found = false;

            // 查找 INA226 命令
            for (size_t j = 0; j < 7; j++)
            {
                if (memcmp(Debug_Receive_Buffer[i].Buffer, cmd_table[j].cmd, Debug_Receive_Buffer[i].Buffer_Length) == 0)
                {
                    cmd_table[j].handler();
                    command_found = true;
                    break;
                }
            }

            // 查找 Relay 命令
            if (!command_found)
            {
                for (size_t j = 0; j < 34; j++)
                {
                    if (memcmp(Debug_Receive_Buffer[i].Buffer, channel_cmd[j].rese_mess, Debug_Receive_Buffer[i].Buffer_Length) == 0)
                    {
                        sprintf((char *)send_buffer, channel_cmd[j].send_mess, strlen(channel_cmd[j].send_mess));
                        command_found = true;
                        break;
                    }
                }
            }

            // 未找到匹配命令
            if (!command_found)
            {
                sprintf((char*)send_buffer, "Error_Command\r\n");
            }

            // 发送数据并清理缓冲区
            debug_send_it_data((uint8_t *)send_buffer, strlen((const char *)send_buffer));
            memset(Debug_Receive_Buffer[i].Buffer, 0, sizeof(Debug_Receive_Buffer[i].Buffer));
            memset(send_buffer, 0, sizeof(send_buffer));
            Debug_Receive_Buffer[i].Buffer_Status = 0;
            Debug_Receive_Buffer[i].Buffer_Length = 0;
        }
    }
}

void App_Task(void)
{
    command_parsing();
}