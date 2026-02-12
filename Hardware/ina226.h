#pragma once

#include "gd32f4xx.h"
#include "systick.h"

#include "../Hardware/inc/soft_i2c.h"
#include "../Hardware/inc/bsp_led.h"


//MAX_Current = Full-scale range / R·SHUNT = 81.92mV / 0.1R = 819.2mA
//Current_LSB = 819.2mA / 2^15 = 0.025mA 
//选择接近值：0.05mA = 0.00005A 手册推荐1mA 但是便于CAL计算和实际调试则重新选择为 0.05mA
//(手册：While this value yields the highest resolution, it is common to select a value for the Current_LSB to the nearest round number above this value to simplify the conversion of the Current Register (04h) and Power Register (03h) to amperes and watts respectively.)
//(虽然该值产生最高分辨率，但通常为Current_LSB选择一个值，使其与高于该值的最接近的整数相匹配，以简化电流寄存器（04h）和功率寄存器（03h）分别转换为安培和瓦的过程。)
//CAL = 0.00512 / (Current_LSB * R·SHUNT) = 0.00512 / (0.00005A * 0.1R) = 1024 = 0x0400
//Power_LSB = Current_LSB * 25 = 0.05mA * 25 = 1.25mW
//(手册：The power LSB has a fixed ratio to the Current_LSB of 25)
//(power LSB与Current_LSB的固定比率为25)


//实测发现电流偏大，采用手册校准公式 Corrected_Full_Scale_Cal = (Cal * MeasShuntCurrent) / INA226_Current 
//5V 测试条件下 MeasShuntCurrent = 490 mV   INA226_Current = 520mV  Corrected_Full_Scale_Cal = 490 * 1024 / 520 = 964.9 = 965 = 0x03C5


/*
 * =====================================================================================
 * INA226 配置参数计算方法
 *
 * 本注释块详细说明了如何根据电路参数来计算INA226的校准值(CAL)和
 * 其他相关参数。这些计算是保证测量精度的关键。
 * =====================================================================================
 *
 * --- 1. 设定前提参数 ---
 *
 * I_MAX   : 设计中预期的最大电流 (单位: 安培 A)。
 * R_SHUNT : 电路中使用的采样电阻的阻值 (单位: 欧姆 Ω)。
 *
 *
 * --- 2. 核心计算步骤 ---
 *
 * // 步骤 1: 计算电流分辨率 (Current_LSB)
 * // 目的:  确定电流寄存器中每一位(bit)代表的实际电流量。
 * // 公式:  Current_LSB = I_MAX / 32768    //  15/32768=0.00045776367  
 * // 重要:  Current_LSB计算结果必须取整 (舍弃小数部分) 后才能写入寄存器，但取整更方便计算可以取500uA，或1mA
 *
 * // 步骤 2: 计算校准值 (CAL)
 * // 目的:  生成需写入校准寄存器的核心值，芯片将依据它进行内部运算。
 * // 公式:  CAL = 0.00512 / (Current_LSB * R_SHUNT)    //CAL=0.00512/（0.001 * 0.001）=5120 = 0x1400
 * // 重要:  计算结果必须取整 (舍弃小数部分) 后才能写入寄存器。
 *
 * // 步骤 3: 计算功率分辨率 (Power_LSB)
 * // 目的:  确定功率寄存器中每一位(bit)代表的实际功率。
 * // 公式:  Power_LSB = 25 * Current_LSB
 *
 *
 * --- 3. 最终读数转换方法 ---
 * //current     = (Shunt Register × Calibration Register) / 2048   如Current =(8000 × 1024) / 2048 = 3900mA = 3.9A
 * //power       = (Power Register × Power_LSB) 如 Power = (4000 × 0.025) = 100W
 * //bus_voltage = (Bus Voltage Register × 1.25) 如 Bus Voltage = (9584 × 1.25) = 11980mV = 11.98V
 * 
 * 	//注意：以上计算均为示例，实际应用中需根据具体情况进行调整。
 * =====================================================================================
 */


typedef enum
{
	Ina226_7bit_address0 = 0x40, // A1：0 A0：0
	Ina226_7bit_address1 = 0x41, // A1：0 A0：1
	Ina226_7bit_address3 = 0x44, // A1：1 A0：0
	Ina226_7bit_address4 = 0x45  // A1：1 A0：1
} INA226_Addr_enum;              // INA226 7-bit address enum

typedef enum
{
	Cmd_Config_Register =            0x00,			// Configuration register   (Read_Write)      配置寄存器
	Cmd_Shunt_Voltage_Register =     0x01,			// Shunt voltage register   (Only Read)       分流电压寄存器
	Cmd_Bus_Voltage_Register =       0x02,			// Bus voltage register     (Only Read)       总线电压寄存器
	Cmd_Power_Register =             0x03,			// Power register           (Only Read)       功率寄存器
	Cmd_Current_Register =           0x04,			// Current register         (Only Read)       电流寄存器
	Cmd_Calibration_Register =       0x05,			// Calibration register     (Read_Write)      校准寄存器
	Cmd_Mask_Enable_Register =       0x06,			// Mask/Enable register     (Read_Write)      掩码/使能寄存器
	Cmd_Alert_Limit_Register =       0x07,			// Alert limit register     (Read_Write)      警报限制寄存器
	Cmd_Manufacturer_ID_Register =   0xFE,			// Manufacturer ID register (Only Read)       制造商ID寄存器
	Cmd_Die_ID_Register =            0xFF			// Die ID register          (Only Read)       芯片ID寄存器
} INA226_Reg_enum;   


// Configuration register bit field________________________________________________________________________________
typedef enum
{
	Rst_Def_Status = 0x00, // No reset
	Rst_Soft_Reset = 0x01, // Software reset
} INA226_Rst_enum;         // INA226 System RST enum

typedef enum
{
	Ina226_Reservation = 0x04, // Reservation  保留位
} INA226_Reservation_enum;

typedef enum
{ 
	Sample_Averaging_Mode_1              = 0x00, // 1     samples
	Sample_Averaging_Mode_4              = 0x01, // 4     samples
	Sample_Averaging_Mode_16             = 0x02, // 16    samples
	Sample_Averaging_Mode_64             = 0x03, // 64    samples
	Sample_Averaging_Mode_128            = 0x04, // 128   samples
	Sample_Averaging_Mode_256            = 0x05, // 256   samples
	Sample_Averaging_Mode_512            = 0x06, // 512   samples
	Sample_Averaging_Mode_1024           = 0x07  // 1024  samples
} INA226_Sample_Averaging_Mode_enum;


typedef enum
{
	Bus_Voltage_Conversion_Time_140us    = 0x00,  // 140us
	Bus_Voltage_Conversion_Time_204us    = 0x01,  // 204us
	Bus_Voltage_Conversion_Time_332us    = 0x02,  // 332us
	Bus_Voltage_Conversion_Time_588us    = 0x03,  // 588us
	Bus_Voltage_Conversion_Time_1100us   = 0x04,  // 1100us
	Bus_Voltage_Conversion_Time_2116us   = 0x05,  // 2116us
	Bus_Voltage_Conversion_Time_4156us   = 0x06,  // 4156us
	Bus_Voltage_Conversion_Time_8244us   = 0x07   // 8244us
} INA226_Bus_Voltage_ConvTime_enum;               //Bus Voltage Conversion Time enum

typedef enum
{
	Shunt_Voltage_Conversion_Time_140us  = 0x00, // 140us
	Shunt_Voltage_Conversion_Time_204us  = 0x01, // 204us
	Shunt_Voltage_Conversion_Time_332us  = 0x02, // 332us
	Shunt_Voltage_Conversion_Time_588us  = 0x03, // 588us
	Shunt_Voltage_Conversion_Time_1100us = 0x04, // 1100us
	Shunt_Voltage_Conversion_Time_2116us = 0x05, // 2116us
	Shunt_Voltage_Conversion_Time_4156us = 0x06, // 4156us
	Shunt_Voltage_Conversion_Time_8244us = 0x07  // 8244us
} INA226_Shunt_Voltage_ConvTime_enum;            // Shunt Voltage Conversion Time enum

typedef enum
{
	Power_Down_Mode                      = 0x00, // Power down mode
	Shunt_Voltage_Mode_Trigger           = 0x01, // Shunt voltage Trigger mode
	Bus_Voltage_Mode_Trigger             = 0x02, // Bus voltage Trigger mode
	Shunt_Bus_Voltage_Mode_Trigger       = 0x03, // Shunt and bus voltage Trigger mode
	Power_Shutdown_Mode                  = 0x04, // Power shutdown mode
	Shunt_Voltage_Mode_Continuous        = 0x05, // Shunt voltage continuous mode
	Bus_Voltage_Mode_Continuous          = 0x06, // Bus voltage continuous mode
	Shunt_Bus_Voltage_Mode_Continuous    = 0x07  // Shunt and bus voltage continuous mode
} INA226_Operating_Mode_enum;                    // Operating Mode enum
//______________________________________________________________________________________________________________

//Mask/Enable Register fountion bit field Alert 默认高电平______________________________________________________________________

typedef enum
{
	Mask_Over_Current_Lock_H  = 0x8001, //	 Over Current Lock
	Mask_Over_Power_Lock_H    = 0x2001, //	 Over Power Lock
	Mask_Over_Voltage_Lock_H  = 0x0801, //	 Over Voltage Lock
	Mask_Under_Current_Lock_H = 0x4001, //	 Under Voltage Lock
	Mask_Under_Voltage_Lock_H = 0x1001, //	 Under Voltage Lock

	Mask_Over_Current_NotLock_H  = 0x8000, //  Over Current Not Lock
	Mask_Over_Power_NotLock_H    = 0x2000, //  Over Power Not Lock
	Mask_Over_Voltage_NotLock_H  = 0x0800, //  Over Voltage Not Lock
	Mask_Under_Current_NotLock_H = 0x4000, //  Under Current Not Lock
	Mask_Under_Voltage_NotLock_H = 0x1000, //  Under Voltage Not Lock
} INA226_Mask_Enable_Mode_enum_H; // Mask/Enable Register function bit field enum

// Mask/Enable Register function bit field end__________________________________

//Mask/Enable Register fountion bit field ，Alert 默认低电平______________________________________________________________________

typedef enum
{
	Mask_Over_Current_Lock_L  = 0x8003, //	 Over Current Lock
	Mask_Over_Power_Lock_L    = 0x2003, //	 Over Power Lock
	Mask_Over_Voltage_Lock_L  = 0x0803, //	 Over Voltage Lock
	Mask_Under_Current_Lock_L = 0x4003, //	 Under Voltage Lock
	Mask_Under_Voltage_Lock_L = 0x1003, //	 Under Voltage Lock

	Mask_Over_Current_NotLock_L  = 0x8002, //  Over Current Not Lock
	Mask_Over_Power_NotLock_L    = 0x2002, //  Over Power Not Lock
	Mask_Over_Voltage_NotLock_L  = 0x0802, //  Over Voltage Not Lock
	Mask_Under_Current_NotLock_L = 0x4002, //  Under Current Not Lock
	Mask_Under_Voltage_NotLock_L = 0x1002, //  Under Voltage Not Lock
} INA226_Mask_Enable_Mode_enum_L;

// Mask/Enable Register function bit field end__________________________________________________________________

typedef struct
{
	INA226_Addr_enum I2C_Add;								                // 7-bit address
	INA226_Reg_enum Reg;									                // Register address
	INA226_Reservation_enum Reservation;					                // Reserved
	INA226_Rst_enum Rst;									                // Reset
	INA226_Sample_Averaging_Mode_enum Sample_Averaging_Mode;                // Sample Averaging Mode
	INA226_Bus_Voltage_ConvTime_enum Bus_Voltage_ConvTime;	                // Bus Voltage Conversion Time
	INA226_Shunt_Voltage_ConvTime_enum Shunt_Voltage_ConvTime;              // Shunt Voltage Conversion Time
	INA226_Operating_Mode_enum Operating_Mode;				                // Operating Mode
	INA226_Mask_Enable_Mode_enum_H Mask_Enable_H;					        // Mask/Enable register
	INA226_Mask_Enable_Mode_enum_L Mask_Enable_L;					        // Mask/Enable register
	uint16_t Alert_Limit;
	uint16_t Calibration_Value;								                // Calibration register
	float Current_LSB;									                    // Alert Limit register
} INA226_Info_t;											                // Configuration register structure


typedef struct
{
	void(*CHG_Ina226_Init)(uint32_t GPIOx, uint16_t SCL_Pin, uint16_t SDA_Pin, INA226_Addr_enum Addr); 
	float(*CHG_Ina226_Get_Current)(void);
	float(*CHG_Ina226_Get_Power)(void);
	float(*CHG_Ina226_Get_Bus_Voltage)(void);
	float(*CHG_Ina226_Get_Shunt_Voltage)(void);
	uint16_t(*CHG_Ina226_Get_Manufacturer_ID)(void);
	uint16_t(*CHG_Ina226_Get_Die_ID)(void);
	void(*CHG_Ina226_Unlock_Alert)(void);
} INA226_Device_Info_t;

void text_INA226_I2C(void);
void INA226_Alert_detection(void); // Detect Alert pin status, if low, alert triggered

extern const INA226_Device_Info_t INA226_Device_Info;

