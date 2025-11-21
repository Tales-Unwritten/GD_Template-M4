#include "../Hardware/inc/ina228.h"

I2C_Info INA228_i2C;

static void INA228_Init(uint32_t GPIOx, uint16_t SCL_Pin, uint16_t SDA_Pin, INA228_Addr_enum Addr)
{   
	Soft_I2C_Init(&INA228_i2C, GPIOx, SCL_Pin, SDA_Pin, Addr);                                                                                                                                             
}

static void Ina228_WriteRegister(I2C_Info *i2c, INA228_Addr_enum Device_Add, INA228_Cmd_enum RegAdd, uint64_t Data)
{
	Soft_I2C_Start(i2c);
	Soft_I2C_WriteByte(i2c, Device_Add << 1);
	Soft_I2C_ReceiveAck(i2c);
	Soft_I2C_WriteByte(i2c, RegAdd);
	Soft_I2C_ReceiveAck(i2c);
	Soft_I2C_WriteByte(i2c, Data >> 8);
	Soft_I2C_ReceiveAck(i2c);
	Soft_I2C_WriteByte(i2c, Data);
	Soft_I2C_ReceiveAck(i2c);
	Soft_I2C_Stop(i2c);
}

uint64_t Ina228_ReadRegister(I2C_Info *i2c, INA228_Addr_enum Device_Add, uint16_t RegAddress)
{
    uint64_t Data = 0;

    //----------------------------------------------------------------------
    // ① 判断是否是【2 字节寄存器】
    //----------------------------------------------------------------------
    if (RegAddress != Cmd_Voltage_Shunt_Register &&
        RegAddress != Cmd_Bus_Voltage_Register1 &&
        RegAddress != Cmd_Current_Register1 &&
        RegAddress != Cmd_Power_Register1 &&
        RegAddress != Cmd_Energt_Register &&
        RegAddress != Cmde_Charge_Register)
    {
        // 2 byte read
        Soft_I2C_Start(i2c);
        Soft_I2C_WriteByte(i2c, Device_Add << 1);
        Soft_I2C_ReceiveAck(i2c);
        Soft_I2C_WriteByte(i2c, RegAddress);
        Soft_I2C_ReceiveAck(i2c);

        Soft_I2C_Start(i2c);
        Soft_I2C_WriteByte(i2c, (Device_Add << 1) | 0x01);
        Soft_I2C_ReceiveAck(i2c);

        uint8_t b1 = Soft_I2C_ReceiveByte(i2c);
        Soft_I2C_WriteAck(i2c, 0);   // ACK
        uint8_t b2 = Soft_I2C_ReceiveByte(i2c);
        Soft_I2C_WriteAck(i2c, 1);   // NACK

        Soft_I2C_Stop(i2c);

        Data = ((uint16_t)b1 << 8) | b2;
        return Data;
    }

    //----------------------------------------------------------------------
    // ②【3 字节寄存器】
    //----------------------------------------------------------------------
    else if (RegAddress == Cmd_Voltage_Shunt_Register ||
             RegAddress == Cmd_Bus_Voltage_Register1 ||
             RegAddress == Cmd_Current_Register1 ||
             RegAddress == Cmd_Power_Register1)
    {
        // 3 byte read
        Soft_I2C_Start(i2c);
        Soft_I2C_WriteByte(i2c, Device_Add << 1);
        Soft_I2C_ReceiveAck(i2c);
        Soft_I2C_WriteByte(i2c, RegAddress);
        Soft_I2C_ReceiveAck(i2c);

        Soft_I2C_Start(i2c);
        Soft_I2C_WriteByte(i2c, (Device_Add << 1) | 0x01);
        Soft_I2C_ReceiveAck(i2c);

        uint8_t b1 = Soft_I2C_ReceiveByte(i2c); Soft_I2C_WriteAck(i2c, 0);
        uint8_t b2 = Soft_I2C_ReceiveByte(i2c); Soft_I2C_WriteAck(i2c, 0);
        uint8_t b3 = Soft_I2C_ReceiveByte(i2c); Soft_I2C_WriteAck(i2c, 1);   // last NACK

        Soft_I2C_Stop(i2c);

        Data = ((uint32_t)b1 << 16) | ((uint32_t)b2 << 8) | b3;
        return Data;
    }

    //----------------------------------------------------------------------
    // ③【5 字节寄存器：Energy/Charge】
    //----------------------------------------------------------------------
    else
    {
        // 5 byte read
        Soft_I2C_Start(i2c);
        Soft_I2C_WriteByte(i2c, Device_Add << 1);
        Soft_I2C_ReceiveAck(i2c);
        Soft_I2C_WriteByte(i2c, RegAddress);
        Soft_I2C_ReceiveAck(i2c);

        Soft_I2C_Start(i2c);
        Soft_I2C_WriteByte(i2c, (Device_Add << 1) | 0x01);
        Soft_I2C_ReceiveAck(i2c);

        uint8_t b1 = Soft_I2C_ReceiveByte(i2c); Soft_I2C_WriteAck(i2c, 0);
        uint8_t b2 = Soft_I2C_ReceiveByte(i2c); Soft_I2C_WriteAck(i2c, 0);
        uint8_t b3 = Soft_I2C_ReceiveByte(i2c); Soft_I2C_WriteAck(i2c, 0);
        uint8_t b4 = Soft_I2C_ReceiveByte(i2c); Soft_I2C_WriteAck(i2c, 0);
        uint8_t b5 = Soft_I2C_ReceiveByte(i2c); Soft_I2C_WriteAck(i2c, 1); // last NACK

        Soft_I2C_Stop(i2c);

        Data = ((uint64_t)b1 << 32) |
               ((uint64_t)b2 << 24) |
               ((uint64_t)b3 << 16) |
               ((uint64_t)b4 << 8)  |
               b5;

        return Data;
    }
}

INA228_Config_Info_t INA228_Config_Info =
{
	.Config_Reset              = Reset_No_Reset,
	.Config_ResACC             = Normal_Operation,
	.Config_Init_Delay_Conver  = Init_Delay_Time_0s,
	.Config_Temperature        = Temper_Compern_Disable,
	.Config_Range              = Range_40_96mV,
	.Config_Reserved           = Reserved_Config_Bit,
};

INA228_ADC_Config_Info_t INA228_ADC_Config_Info =
{
	.ADC_Mode_Config           = Mode_Continuous_Shunt_Bus_Voltage,
	.ADC_Vbus_Conv_Time        = Vbus_Conv_Time_50us,
	.ADC_Vshunt_Conv_Time      = Vshunt_Conv_Time_50us,
	.ADC_Temp_Conv_Time        = Temp_Conv_Time_50us,
	.ADC_Avg_Sample            = Avg_Mode_256_Samples,	
};

INA228_Diag_Alert_Info_t INA228_Diag_Alert_Info=
{
	.Alert_Clock_Config        = Alert_Clock_Enable,
	.Alert_Ready_Flag_Config   = Alert_Ready_Flag_Disable,
	.Alert_Asserted_Config     = Alert_Asserted_Disable,
	.Alert_Polarity_Config     = Alert_Active_High,
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
	Config_Register |= (Info->Config_Reset << 15);                        // Reset bit
	Config_Register |= (Info->Config_ResACC << 14);                       // Reserved for ACC bit
	Config_Register |= (Info->Config_Init_Delay_Conver << 6);             // Initial ADC conversion delay bits
	Config_Register |= (Info->Config_Temperature << 5);                   // Shunt Temperature Compensation bit
	Config_Register |= (Info->Config_Range << 4);                         // Shunt full scale range selection bits
	Config_Register |= (Info->Config_Reserved << 0);                      // Reserved bits
	return Config_Register;
}

static uint16_t INA228_BuildADCConfigRegister(INA228_ADC_Config_Info_t *Info)
{
	uint16_t ADC_Config_Register = 0;
	ADC_Config_Register |= (Info->ADC_Mode_Config << 12);         // ADC operating mode bits
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
	Alert_Register |= (Info->Temperature_Over_Limit << 7);      // Temperature over-limit status bit
	Alert_Register |= (Info->Shunt_Voltage_Over_Limit << 6);    // Shunt voltage over-limit status bit
	Alert_Register |= (Info->Shunt_Voltage_Under_Limit << 5);   // Shunt voltage under-limit status bit
	Alert_Register |= (Info->Bus_Voltage_Over_Limit << 4);      // Bus voltage over-limit status bit
	Alert_Register |= (Info->Bus_Voltage_Under_Limit << 3);     // Bus voltage under-limit status bit
	Alert_Register |= (Info->Power_Over_Limit << 2);            // Power over-limit status bit
	Alert_Register |= (Info->Conversion_Ready << 1);            // Conversion ready status bit
	Alert_Register |= (Info->Checksum_Status << 0);             // Checksum status bit
	return Alert_Register;
};

void text_online_INA228(void)
{
    uint16_t Die_ID=Ina228_ReadRegister(&INA228_i2C,Ina228_7bit_address0,Cmd_Device_ID);
    uint16_t MANUFACTURER_ID=Ina228_ReadRegister(&INA228_i2C,Ina228_7bit_address0,Cmd_Manufacturer_ID);
    printf("INA228  Die_ID: 0x%04X\r\n", Die_ID);
    printf("INA228  MANUFACTURER_ID: 0x%04X\r\n", MANUFACTURER_ID);
}


void INA228_Config(uint32_t GPIOx, uint16_t SCL_Pin, uint16_t SDA_Pin, INA228_Addr_enum Addr)
{
    INA228_Init(GPIOx, SCL_Pin, SDA_Pin, Addr);
}
