#include "../Hardware/inc/ina228.h"

I2C_Info INA228_i2C;

static void INA228_Init(uint32_t GPIOx, uint16_t SCL_Pin, uint16_t SDA_Pin, INA228_Addr_enum Addr)
{   
	Soft_I2C_Init(&INA228_i2C, GPIOx, SCL_Pin, SDA_Pin, Addr);                                                                                                                                             
}



// 寄存器长度表（按 INA228 手册）
static uint8_t INA228_RegLen(uint16_t reg)
{
    switch (reg)
    {
    case Cmd_Voltage_Shunt_Register:
    case Cmd_Bus_Voltage_Register1:
    case Cmd_Current_Register1:
    case Cmd_Power_Register1:
        return 3;   // 24-bit

    case Cmd_Energt_Register:
    case Cmde_Charge_Register:
        return 5;   // 40-bit

    default:
        return 2;   // default 16-bit registers
    }
}

void Ina228_WriteRegister(I2C_Info *i2c, INA228_Addr_enum Device_Add, 
                          INA228_Cmd_enum RegAdd, uint64_t Data)
{
    uint8_t len = INA228_RegLen(RegAdd);

    Soft_I2C_Start(i2c);
    Soft_I2C_WriteByte(i2c, Device_Add << 1);    // Write mode
    Soft_I2C_ReceiveAck(i2c);

    Soft_I2C_WriteByte(i2c, RegAdd);             // Register address
    Soft_I2C_ReceiveAck(i2c);

    // 从高位字节开始写（和手册一致）
    for (int8_t i = len - 1; i >= 0; i--)
    {
        uint8_t byte_to_write = (Data >> (8 * i)) & 0xFF;

        Soft_I2C_WriteByte(i2c, byte_to_write);
        Soft_I2C_ReceiveAck(i2c);
    }

    Soft_I2C_Stop(i2c);
}


uint64_t Ina228_ReadRegister(I2C_Info *i2c, INA228_Addr_enum Device_Add, uint16_t RegAddress)
{
    uint64_t Data = 0;
    uint8_t len = INA228_RegLen(RegAddress);

    //----------------- 写寄存器地址 -----------------
    Soft_I2C_Start(i2c);
    Soft_I2C_WriteByte(i2c, Device_Add << 1);
    Soft_I2C_ReceiveAck(i2c);
    Soft_I2C_WriteByte(i2c, RegAddress);
    Soft_I2C_ReceiveAck(i2c);

    //----------------- 读取数据 -----------------
    Soft_I2C_Start(i2c);
    Soft_I2C_WriteByte(i2c, (Device_Add << 1) | 0x01);
    Soft_I2C_ReceiveAck(i2c);

    for (uint8_t i = 0; i < len; i++)
    {
        uint8_t rx = Soft_I2C_ReceiveByte(i2c);

        // shift and append
        Data = (Data << 8) | rx;

        // 最后 1 字节 NACK；其他 ACK
        // 向I2C总线写入应答信号，如果是最后一个字节则写入NACK(1)，否则写入ACK(0)
        Soft_I2C_WriteAck(i2c, (i == (len - 1)) ? 1 : 0);
    }

    Soft_I2C_Stop(i2c);

    return Data;
}


INA228_Config_Info_t INA228_Config_Info =
{
	.Config_Reset              = Reset_No_Reset,
	.Config_ResACC             = Normal_Operation,
	.Config_Init_Delay_Conver  = Init_Delay_Time_2ms,
	.Config_Temperature        = Temper_Comperm_Enable,
	.Config_Range              = Range_163_84mV,
	.Config_Reserved           = Reserved_Config_Bit,
    .Calibration_Value         = 0x2000,          // Calibration register
    .Alert_Limit               = 0x0FA0,          // Alert Limit register
    .Current_LSB               = 1.0f,            // Current LSB value

};

INA228_ADC_Config_Info_t INA228_ADC_Config_Info =
{
	.ADC_Mode_Config           = Mode_Continuous_Shunt_Bus_Voltage,
	.ADC_Vbus_Conv_Time        = Vbus_Conv_Time_280us,
	.ADC_Vshunt_Conv_Time      = Vshunt_Conv_Time_540us,
	.ADC_Temp_Conv_Time        = Temp_Conv_Time_280us,
	.ADC_Avg_Sample            = Avg_Mode_64_Samples,	
};

INA228_Diag_Alert_Info_t INA228_Diag_Alert_Info=
{
	.Alert_Clock_Config        = Alert_Clock_Enable,
	.Alert_Ready_Flag_Config   = Alert_Ready_Flag_Disable,
	.Alert_Asserted_Config     = Alert_Asserted_Disable,
	.Alert_Polarity_Config     = Alert_Active_Low,
	.Eenrgy_Status             = Energy_Normal,
	.Charge_Status             = Charge_Normal,
	.Arithmetic_Overflow       = Arithmetic_Normal,
	.Reserved_Bit              = Reserved_Bit,
	.Temperature_Over_Limit    = Temp_Over_Normal,
	.Shunt_Voltage_Over_Limit  = Shunt_Voltage_Over_Normal,
	.Shunt_Voltage_Under_Limit = Shunt_Voltage_Under_Normal,
	.Bus_Voltage_Over_Limit    = Bus_Voltage_Over_Normal,
	.Bus_Voltage_Under_Limit   = Bus_Voltage_Under_Normal,
	.Power_Over_Limit          = Power_Normal,
	.Conversion_Ready          = Conversion_Not_Ready,
	.Checksum_Status           = Checksum_Normal,
};


static uint16_t INA228_BuildConfigRegister(INA228_Config_Info_t *Info)
{
	uint16_t Config_Register = 0;
	Config_Register |= (Info->Config_Reset << 15);               // Reset bit
	Config_Register |= (Info->Config_ResACC << 14);              // Reserved for ACC bit
	Config_Register |= (Info->Config_Init_Delay_Conver << 6);    // Initial ADC conversion delay bits
	Config_Register |= (Info->Config_Temperature << 5);          // Shunt Temperature Compensation bit
	Config_Register |= (Info->Config_Range << 4);                // Shunt full scale range selection bits
	Config_Register |= (Info->Config_Reserved << 0);             // Reserved bits
	return Config_Register;
}

static uint16_t INA228_BuildADCConfigRegister(INA228_ADC_Config_Info_t *Info)
{
	uint16_t ADC_Config_Register = 0;
	ADC_Config_Register |= (Info->ADC_Mode_Config << 12);        // ADC operating mode bits
	ADC_Config_Register |= (Info->ADC_Vbus_Conv_Time << 8);      // Bus voltage conversion time bits
	ADC_Config_Register |= (Info->ADC_Vshunt_Conv_Time << 4);    // Shunt voltage conversion time bits
	ADC_Config_Register |= (Info->ADC_Temp_Conv_Time << 2);      // Temperature conversion time bits
	ADC_Config_Register |= (Info->ADC_Avg_Sample << 0);          // Sample averaging mode bits
	return ADC_Config_Register;
};

static uint16_t INA228_BuildAlertRegister(INA228_Diag_Alert_Info_t *Info)
{
	uint16_t Alert_Register = 0;
	Alert_Register |= (Info->Alert_Clock_Config << 15);          // Alert Clock configuration bit
	Alert_Register |= (Info->Alert_Ready_Flag_Config << 14);     // Conversion Ready Flag configuration bit
	Alert_Register |= (Info->Alert_Asserted_Config << 13);       // Alert Asserted configuration bit
	Alert_Register |= (Info->Alert_Polarity_Config << 12);       // Alert Polarity configuration bit
	Alert_Register |= (Info->Eenrgy_Status << 11);               // Energy register status bit
	Alert_Register |= (Info->Charge_Status << 10);               // Charge register status bit
	Alert_Register |= (Info->Arithmetic_Overflow << 9);          // Arithmetic overflow status bit
	Alert_Register |= (Info->Reserved_Bit << 8);                 // Reserved bit
	Alert_Register |= (Info->Temperature_Over_Limit << 7);       // Temperature over-limit status bit
	Alert_Register |= (Info->Shunt_Voltage_Over_Limit << 6);     // Shunt voltage over-limit status bit
	Alert_Register |= (Info->Shunt_Voltage_Under_Limit << 5);    // Shunt voltage under-limit status bit
	Alert_Register |= (Info->Bus_Voltage_Over_Limit << 4);       // Bus voltage over-limit status bit
	Alert_Register |= (Info->Bus_Voltage_Under_Limit << 3);      // Bus voltage under-limit status bit
	Alert_Register |= (Info->Power_Over_Limit << 2);             // Power over-limit status bit
	Alert_Register |= (Info->Conversion_Ready << 1);             // Conversion ready status bit
	Alert_Register |= (Info->Checksum_Status << 0);              // Checksum status bit
	return Alert_Register;
};


static void INA228_BuildConfiguration(void)
{
    uint16_t Config_Reg = INA228_BuildConfigRegister(&INA228_Config_Info);
    Ina228_WriteRegister(&INA228_i2C, Ina228_7bit_address0, Cmd_Config_Register1, Config_Reg);

    uint16_t ADC_Config_Reg = INA228_BuildADCConfigRegister(&INA228_ADC_Config_Info);
    Ina228_WriteRegister(&INA228_i2C, Ina228_7bit_address0, Cmd_ADC_Config_Register, ADC_Config_Reg);

    uint16_t Alert_Reg = INA228_BuildAlertRegister(&INA228_Diag_Alert_Info);
    Ina228_WriteRegister(&INA228_i2C, Ina228_7bit_address0, Cmd_Diagnostic_Alert_Register, Alert_Reg);
}




static float INA228_Get_Shunt_Voltage(void)
{
    float raw = 0.00f;
    int16_t shunt_val;
    shunt_val = Ina228_ReadRegister(&INA228_i2C, Ina228_7bit_address0, Cmd_Voltage_Shunt_Register);
    // Conversion factor: 312.5nV/LSB
    raw = (float)shunt_val * 0.0000003125f;
    return raw;
}

// static float INA228_Get_Bus_Voltage(void)
// {
//     float raw = 0.00f;
//     uint32_t bus_val;
//     bus_val = Ina228_ReadRegister(&INA228_i2C, Ina228_7bit_address0, Cmd_Bus_Voltage_Register1);
//     printf("%llu\r\n", bus_val);
//     // Extract upper 24 bits of valid data
//     bus_val = (bus_val>>4) & 0xFFFFF;
    
//     // Conversion factor: 195.3125 µV/LSB (1 LSB = 195.3125 µV)
//     raw = (float)bus_val * 0.0001953125f;
    
//     return raw;
// }

float INA228_Get_Bus_Voltage(void)
{
    uint32_t reg = Ina228_ReadRegister(&INA228_i2C,
                                       Ina228_7bit_address0,
                                       Cmd_Bus_Voltage_Register1);

    // 移除低 4 位保留位
    uint32_t val20 = reg >> 4;

    // 20-bit 两补码符号扩展到 32bit
    if (val20 & (1 << 19)) {
        val20 |= 0xFFF00000;   // 111111111111 0000 0000 0000 0000 0000
    }

    // LSB = 195.3125 uV
    return ((float)((int32_t)val20)) * 0.0001953125f;
}


//获取温度 7.8125m°C/LSB
static float INA228_Get_Temperature(void)
{
    float raw = 0.00f;
    int16_t temp_val;
    temp_val = Ina228_ReadRegister(&INA228_i2C, Ina228_7bit_address0, Cmd_Temperature_Register);
    // Conversion factor: 7.8125m°C/LSB
    raw = (float)temp_val * 0.0078125f;
    return raw;
}


static float INA228_Get_Bus_Current(void)
{
    float raw = 0.00f;
    uint32_t curr_val;
    curr_val = Ina228_ReadRegister(&INA228_i2C, Ina228_7bit_address0, Cmd_Current_Register1);
    raw = curr_val>>4; // 取高20位有效数据
    // Conversion factor: 19.073486µA /LSB (1 LSB = 19.073486µA)
    if (raw & (1 << 19)) {
        raw |= 0xFFF00000;   // 111111111111 0000 0000 0000 0000 0000
    }
    return ((float)((int32_t)raw)) * 0.000019073486f;
}


//获取电能 电源 LSB x 16 = 976.5625µJ/LSB
static float INA228_Get_Energy(void)
{
    float raw =0.00f;
    uint64_t energy_val;
    energy_val = Ina228_ReadRegister(&INA228_i2C, Ina228_7bit_address0, Cmd_Energt_Register);
    // Conversion factor: 976.5625µJ/LSB
    raw = (float)energy_val * 0.0009765625f;
    return raw;
}


//获取电荷 电流 LSB = 19.073486µC/LSB
static float INA228_Get_Charge(void)
{
    float raw =0.00f;
    uint64_t charge_val;
    charge_val = Ina228_ReadRegister(&INA228_i2C, Ina228_7bit_address0, Cmde_Charge_Register);
    // Conversion factor: 19.073486µC/LSB
    raw = (float)charge_val * 0.000019073486f;
    return raw;
}


static float INA228_Get_Power(void)
{
    float raw =0.00f;
    uint16_t power_val;
    power_val = Ina228_ReadRegister(&INA228_i2C, Ina228_7bit_address0, Cmd_Power_Register1);
    // Conversion factor: 61.035156µW/LSB
    raw = (float)power_val * 0.000061035156f;
    return raw;
}

static uint16_t INA228_Get_Manufacturer_ID(void)
{
    uint16_t MANUFACTURER_ID=Ina228_ReadRegister(&INA228_i2C,Ina228_7bit_address0,Cmd_Manufacturer_ID);
    return MANUFACTURER_ID;
}

static uint16_t INA228_Get_Die_ID(void)
{
    uint16_t Die_ID=Ina228_ReadRegister(&INA228_i2C,Ina228_7bit_address0,Cmd_Device_ID);
    return Die_ID;
}


void INA228_Unlock_Alert(void)
{
	uint16_t Mask_Enable_Register = 0;
	Mask_Enable_Register = Ina228_ReadRegister(&INA228_i2C, Ina228_7bit_address0, Cmd_Diagnostic_Alert_Register);
}



static void INA228_Text_Online(void)
{
    uint16_t Die_ID=INA228_Device_Func.CHG_INA228_Get_Device_ID();
    uint16_t MANUFACTURER_ID=INA228_Device_Func.CHG_INA228_Get_Manufacturer_ID();
    float Bus_Voltage=INA228_Device_Func.CHG_INA228_Get_Bus_Voltage();
    float Bus_Current=INA228_Device_Func.CHG_INA228_Get_Current();
    printf("INA228  Die_ID: 0x%04X\r\n", Die_ID);
    printf("INA228  MANUFACTURER_ID: 0x%04X\r\n", MANUFACTURER_ID);
    printf("INA228  Bus_Voltage: %.6f V\r\n", Bus_Voltage);
    printf("INA228  Bus_Current: %.6f A\r\n", Bus_Current);
}

static void INA228_Config(uint32_t GPIOx, uint16_t SCL_Pin, uint16_t SDA_Pin, INA228_Addr_enum Addr)
{
    INA228_Init(GPIOx, SCL_Pin, SDA_Pin, Addr);
    INA228_BuildConfiguration();
}
const INA228_Device_Func_t INA228_Device_Func =
{
    .CHG_INA228_Config               = INA228_Config,
    .CHG_INA228_Get_Manufacturer_ID  = INA228_Get_Manufacturer_ID,
    .CHG_INA228_Get_Device_ID        = INA228_Get_Die_ID,
    .CHG_INA228_Get_Shunt_Voltage    = INA228_Get_Shunt_Voltage,
    .CHG_INA228_Get_Bus_Voltage      = INA228_Get_Bus_Voltage,
    .CHG_INA228_Get_Temperature      = INA228_Get_Temperature,
    .CHG_INA228_Unlock_Alert         = INA228_Unlock_Alert,
    .CHG_INA228_Get_Current          = INA228_Get_Bus_Current,    
    .CHG_INA228_Get_Energy           = INA228_Get_Energy,
    .CHG_INA228_Get_Charge           = INA228_Get_Charge,
    .CHG_INA228_Get_Power            = INA228_Get_Power,
    .CHG_INA228_Text_Online          = INA228_Text_Online,
};
