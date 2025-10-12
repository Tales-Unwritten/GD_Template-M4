#pragma ocne
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../Hardware/inc/debug_uart.h"
#include "../Hardware/inc/rs485.h"
#include "../Hardware/inc/ina226.h"

static char send_buffer[128]; // Define send_buffer with sufficient size


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
    // uint16_t val = INA226_Device_Info.CHG_Ina226_Get_Die_ID();
    // sprintf((char *)send_buffer, "Alert is Ulock=0x%04X\r\n", val);
    INA226_Device_Info.CHG_Ina226_Get_Die_ID();
    sprintf((char *)send_buffer, "Alert is Ulock\r\n");
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
typedef struct
{
    const char *cmd;
    cmd_handler_t handler;
} cmd_struct;

static const cmd_struct cmd_table[] = {
    {"get_bus_voltage\r\n", handle_bus_voltage},
    {"get_current\r\n", handle_current},
    {"get_shunt_voltage\r\n", handle_shunt_voltage},
    {"get_power\r\n", handle_power},
    {"ulock_alert\r\n", handle_unlock_alert},
    {"get_manufacturer_id\r\n", handle_manufacturer_id},
    {"get_die_id\r\n", handle_die_id}};

static char send_buffer[128]; // Increased buffer size to 128 bytes

typedef struct
{
    char *rese_mess;
    char *send_mess;
} ChannelCmdDef;
static const ChannelCmdDef channel_cmd[34] = {
    {"Channel_all_open\r\n", "Channel_is_all_open\r\n"},
    {"Channel_all_close\r\n", "Channel_is_all_close\r\n"},
    {"Channel01_open\r\n", "Channel01_is_open\r\n"},
    {"Channel02_open\r\n", "Channel02_is_open\r\n"},
    {"Channel03_open\r\n", "Channel03_is_open\r\n"},
    {"Channel04_open\r\n", "Channel04_is_open\r\n"},
    {"Channel05_open\r\n", "Channel05_is_open\r\n"},
    {"Channel06_open\r\n", "Channel06_is_open\r\n"},
    {"Channel07_open\r\n", "Channel07_is_open\r\n"},
    {"Channel08_open\r\n", "Channel08_is_open\r\n"},
    {"Channel09_open\r\n", "Channel09_is_open\r\n"},
    {"Channel10_open\r\n", "Channel10_is_open\r\n"},
    {"Channel11_open\r\n", "Channel11_is_open\r\n"},
    {"Channel12_open\r\n", "Channel12_is_open\r\n"},
    {"Channel13_open\r\n", "Channel13_is_open\r\n"},
    {"Channel14_open\r\n", "Channel14_is_open\r\n"},
    {"Channel15_open\r\n", "Channel15_is_open\r\n"},
    {"Channel16_open\r\n", "Channel16_is_open\r\n"},
    {"Channel01_close\r\n", "Channel01_is_close\r\n"},
    {"Channel02_close\r\n", "Channel02_is_close\r\n"},
    {"Channel03_close\r\n", "Channel03_is_close\r\n"},
    {"Channel04_close\r\n", "Channel04_is_close\r\n"},
    {"Channel05_close\r\n", "Channel05_is_close\r\n"},
    {"Channel06_close\r\n", "Channel06_is_close\r\n"},
    {"Channel07_close\r\n", "Channel07_is_close\r\n"},
    {"Channel08_close\r\n", "Channel08_is_close\r\n"},
    {"Channel09_close\r\n", "Channel09_is_close\r\n"},
    {"Channel10_close\r\n", "Channel10_is_close\r\n"},
    {"Channel11_close\r\n", "Channel11_is_close\r\n"},
    {"Channel12_close\r\n", "Channel12_is_close\r\n"},
    {"Channel13_close\r\n", "Channel13_is_close\r\n"},
    {"Channel14_close\r\n", "Channel14_is_close\r\n"},
    {"Channel15_close\r\n", "Channel15_is_close\r\n"},
    {"Channel16_close\r\n", "Channel16_is_close\r\n"}};


void command_parsing(PC_Transmit_Buffer_t *PC_Transmit_Buffer, void (*Usart_Send_Data)(uint8_t *buf, uint8_t count));
