#include "../App/inc/command_parsing.h"

static uint16_t send_buffer[16] = {0};
static bool send_idx = 0;

// 命令处理函数类型
typedef void (*cmd_handler_t)(void);

// 命令处理函数
static void handle_bus_voltage(void) {
    float val = INA226_Device_Info.CHG_Ina226_Get_Bus_Voltage();
    sprintf((char *)send_buffer, "Vbus_Val=%0.4f\r\n", val);
}

static void handle_current(void) {
    float val = INA226_Device_Info.CHG_Ina226_Get_Current();
    sprintf((char *)send_buffer, "Curr_Val=%0.4f\r\n", val);
}

static void handle_shunt_voltage(void) {
    float val = INA226_Device_Info.CHG_Ina226_Get_Shunt_Voltage();
    sprintf((char *)send_buffer, "shunt_voltage=%0.4f\r\n", val);
}

static void handle_power(void) {
    uint16_t val = INA226_Device_Info.CHG_Ina226_Get_Power();
    sprintf((char *)send_buffer, "Power_Value=0x%04X\r\n", val);
}

static void handle_unlock_alert(void) {
    uint16_t val = INA226_Device_Info.CHG_Ina226_Get_Die_ID();
    sprintf((char *)send_buffer, "Alert is Ulock=0x%04X\r\n", val);
}

static void handle_manufacturer_id(void) {
    uint16_t val = INA226_Device_Info.CHG_Ina226_Get_Manufacturer_ID();
    sprintf((char *)send_buffer, "Manufacturer_ID=0x%04X\r\n", val);
}

static void handle_die_id(void) {
    uint16_t val = INA226_Device_Info.CHG_Ina226_Get_Die_ID();
    sprintf((char *)send_buffer, "Die_ID=0x%04X\r\n", val);
}

// 命令表
static const struct {
    const char *cmd;
    cmd_handler_t handler;
} cmd_table[] = {
    {"get_bus_voltage\r\n", handle_bus_voltage},
    {"get_current\r\n", handle_current},
    {"get_shunt_voltage\r\n", handle_shunt_voltage},
    {"get_power\r\n", handle_power},
    {"ulock_alert\r\n", handle_unlock_alert},
    {"get_manufacturer_id\r\n", handle_manufacturer_id},
    {"get_die_id\r\n", handle_die_id}
};

void command_parsing(void)
{
    for (size_t i = 0; i < CHCHE_COUNT; i++)
    {
        if (Debug_Receive_Buffer[i].Buffer_Status == 1)
        {
            // 查找匹配的命令
            for (size_t j = 0; j < sizeof(cmd_table) / sizeof(cmd_table[0]); j++)
            {
                if (memcmp(Debug_Receive_Buffer[i].Buffer, cmd_table[j].cmd,
                           Debug_Receive_Buffer[i].Buffer_Length) == 0)
                {
                    cmd_table[j].handler();
                    send_idx = 1;
                    break;
                }
            }
        }

        if (send_idx == 1)
        {
            debug_send_it_data((uint8_t *)send_buffer, strlen((const char *)send_buffer));
            send_idx = 0;
            memset(send_buffer, 0, sizeof(send_buffer));
        }
        Debug_Receive_Buffer[i].Buffer_Status = 0;
        Debug_Receive_Buffer[i].Buffer_Length = 0;
    }
}