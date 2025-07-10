#include "../Hardware/inc/ina226.h"

I2C_Info INA226_i2C;

static void INA226_Alert_Config(void)
{
	rcu_periph_clock_enable(RCU_GPIOG); 
	gpio_mode_set(GPIOG, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_14); 
}

static void Ina226_WriteRegister(I2C_Info *i2c, INA226_Info_t *Device_Add, INA226_Reg_enum RegAdd, uint16_t Data)
{
	Soft_I2C_Start(i2c);
	Soft_I2C_WriteByte(i2c, Device_Add->I2C_Add << 1);
	Soft_I2C_ReceiveAck(i2c);
	Soft_I2C_WriteByte(i2c, RegAdd);
	Soft_I2C_ReceiveAck(i2c);
	Soft_I2C_WriteByte(i2c, Data >> 8);
	Soft_I2C_ReceiveAck(i2c);
	Soft_I2C_WriteByte(i2c, Data);
	Soft_I2C_ReceiveAck(i2c);
	Soft_I2C_Stop(i2c);
}

static uint16_t Ina226_ReadRegister(I2C_Info *i2c, INA226_Info_t *Device_Add, INA226_Reg_enum RegAddress)
{
	static uint32_t Data_Temp = 0;
	Soft_I2C_Start(i2c);
	Soft_I2C_WriteByte(i2c, Device_Add->I2C_Add << 1);
	Soft_I2C_ReceiveAck(i2c);
	Soft_I2C_WriteByte(i2c, RegAddress);
	Soft_I2C_ReceiveAck(i2c);

	Soft_I2C_Start(i2c);
	Soft_I2C_WriteByte(i2c, Device_Add->I2C_Add << 1 | 0x01);
	Soft_I2C_ReceiveAck(i2c);
	Data_Temp = Soft_I2C_ReceiveByte(i2c);
	Soft_I2C_WriteAck(i2c, 0);
	Data_Temp = (Data_Temp << 8) | Soft_I2C_ReceiveByte(i2c);
	Soft_I2C_Stop(i2c);
	return Data_Temp;
}

INA226_Info_t INA226_Info =
	{
		.Rst = Rst_Def_Status,										   // 0x00
		.Reservation = Ina226_Reservation,							   // 0x04
		.Sample_Averaging_Mode = Sample_Averaging_Mode_128,			   // 0x03
		.Bus_Voltage_ConvTime = Bus_Voltage_Conversion_Time_588us,	   // 588us
		.Shunt_Voltage_ConvTime = Shunt_Voltage_Conversion_Time_588us, // 140us
		.Operating_Mode = Shunt_Bus_Voltage_Mode_Continuous,		   // 0x07
		.I2C_Add = Ina226_7bit_address0,							   // 0x40
		.Alert_Limit = 0x07D0,										   // Alert Limit register (to be set later)
		.Current_LSB = 1,											   // Current LSB value (to be set later)
		.Calibration_Value = 0x03EC,								   // Calibration register (to be set later)
		.Reg = Cmd_Config_Register,									   // Register address for configuration
		.Mask_Enable_L = Mask_Over_Current_Lock_L,					   // Mask/Enable register for low alerts
		.Mask_Enable_H = Mask_Over_Current_Lock_H,					   // Mask/Enable register for high and low alerts
};

static uint16_t INA226_BuildConfigRegister(INA226_Info_t *Info)
{
	uint16_t Config_Register = 0;
	Config_Register |= (Info->Rst << 15);					             // Reset bit
	Config_Register |= (Info->Reservation << 12);			             // Reserved bits
	Config_Register |= (Info->Sample_Averaging_Mode  << 9);	             // Sample Averaging Mode bits
	Config_Register |= (Info->Bus_Voltage_ConvTime   << 6);	             // Bus Voltage Conversion Time bits
	Config_Register |= (Info->Shunt_Voltage_ConvTime << 3);              // Shunt Voltage Conversion Time bits
	Config_Register |= (Info->Operating_Mode << 0);			             // Operating Mode bits
	return Config_Register;
}

static float INA226_Get_Current(void)
{
	static float Current = 0;
	int16_t Current_Register_Data = 0;
	Current_Register_Data = Ina226_ReadRegister(&INA226_i2C,&INA226_Info, Cmd_Current_Register);
	Current = (Current_Register_Data * INA226_Info.Current_LSB); // Convert to mA
	return Current/1000; // Return the current value in A
}

static float INA226_Get_Power(void)
{
	static float Power = 0.00f;
	int16_t Power_Register_Data = 0;
	Power_Register_Data = Ina226_ReadRegister(&INA226_i2C,&INA226_Info, Cmd_Power_Register);
	Power = Power_Register_Data * (INA226_Info.Current_LSB * 25); // Convert to mW
	return Power;														 // Return the power value in milliwatts
}

static float INA226_Get_Bus_Voltage(void)
{
	static float Bus_Voltage = 0.00f;
	uint16_t Bus_Voltage_Register_Data = 0;
	Bus_Voltage_Register_Data = Ina226_ReadRegister(&INA226_i2C, &INA226_Info, Cmd_Bus_Voltage_Register);
	Bus_Voltage = (float)Bus_Voltage_Register_Data * 1.25f; // Convert to mV
	return Bus_Voltage/1000;										// Return the bus voltage value in volts
}

static float INA226_Get_Shunt_Voltage(void)
{
	float Shunt_Voltage = 0.00f;
	int16_t Shunt_Voltage_Register_Data = 0;
	Shunt_Voltage_Register_Data = Ina226_ReadRegister(&INA226_i2C, &INA226_Info, Cmd_Shunt_Voltage_Register);
	Shunt_Voltage = ((Shunt_Voltage_Register_Data) +1) * 0.0025f; // Convert to mV
	return Shunt_Voltage;						  // Return the shunt voltage value in millivolts
}

static uint16_t INA226_Get_Manufacturer_ID(void)
{
	uint16_t Manufacturer_ID = 0;
	Manufacturer_ID = Ina226_ReadRegister(&INA226_i2C, &INA226_Info, Cmd_Manufacturer_ID_Register) ; // Read Manufacturer ID
	return Manufacturer_ID;																	// Return the manufacturer ID
}

static uint16_t INA226_Get_Die_ID(void)
{
	uint16_t Die_ID = 0;
	Die_ID = Ina226_ReadRegister(&INA226_i2C, &INA226_Info, Cmd_Die_ID_Register) ; // Read Die ID
	return Die_ID;														  // Return the die ID
}

static void INA226_Unlock_Alert(void)
{
	uint16_t Mask_Enable_Register = 0;
	Mask_Enable_Register = Ina226_ReadRegister(&INA226_i2C, &INA226_Info, Cmd_Mask_Enable_Register);
}

void text_INA226_I2C(void)
{	
	uint16_t Config_Register = Ina226_ReadRegister(&INA226_i2C, &INA226_Info, Cmd_Config_Register); // Read the configuration register
	uint16_t Shunt_Voltage_Register = Ina226_ReadRegister(&INA226_i2C, &INA226_Info, Cmd_Shunt_Voltage_Register); // Read the shunt voltage register
	uint16_t Bus_Voltage_Register = Ina226_ReadRegister(&INA226_i2C, &INA226_Info, Cmd_Bus_Voltage_Register); // Read the bus voltage register
	uint16_t Power_Register = Ina226_ReadRegister(&INA226_i2C, &INA226_Info, Cmd_Power_Register); // Read the power register
	uint16_t Current_Register = Ina226_ReadRegister(&INA226_i2C, &INA226_Info, Cmd_Current_Register); // Read the current register

	printf("Config Register: 0x%04X\r\n", Config_Register); // Print the configuration register value
	printf("Shunt Voltage Register: 0x%04X\r\n", Shunt_Voltage_Register); // Print the shunt voltage register value
	printf("Bus Voltage Register: 0x%04X\r\n", Bus_Voltage_Register); // Print the bus voltage register value
	printf("Power Register: 0x%04X\r\n", Power_Register); // Print the power register value
	printf("Current Register: 0x%04X\r\n", Current_Register); // Print the current register value	
}

//如果Alter引脚为低电平，表示有警报触发
void INA226_Alert_detection(void)
{
	delay_ns(100);
	if (gpio_input_bit_get(GPIOG, GPIO_PIN_14) == RESET) // Check if alert pin is low
	{
		led_on(1);	// Turn on LED2 to indicate alert
	}
	else
	{
		led_off(1); // Turn off LED2 if no alert
	}
}

static void INA226_Init(uint32_t GPIOx, uint16_t SCL_Pin, uint16_t SDA_Pin, INA226_Addr_enum Addr)
{   
	Soft_I2C_Init(&INA226_i2C, GPIOx, SCL_Pin, SDA_Pin, Addr); // Initialize I2C with address 0x40
	uint16_t Config_Register = INA226_BuildConfigRegister(&INA226_Info);

	INA226_Alert_Config(); // Configure alert pin as input

	Ina226_WriteRegister(&INA226_i2C, &INA226_Info, Cmd_Config_Register, Config_Register);
	Ina226_WriteRegister(&INA226_i2C, &INA226_Info, Cmd_Calibration_Register, INA226_Info.Calibration_Value);
	Ina226_WriteRegister(&INA226_i2C, &INA226_Info, Cmd_Mask_Enable_Register, INA226_Info.Mask_Enable_H); // Set Mask/Enable register
	Ina226_WriteRegister(&INA226_i2C, &INA226_Info, Cmd_Alert_Limit_Register, INA226_Info.Alert_Limit); // Set Alert Limit register
}

const INA226_Device_Info_t INA226_Device_Info =
	{
		.CHG_Ina226_Init = INA226_Init,
		.CHG_Ina226_Get_Current = INA226_Get_Current,
		.CHG_Ina226_Get_Power = INA226_Get_Power,
		.CHG_Ina226_Get_Bus_Voltage = INA226_Get_Bus_Voltage,
		.CHG_Ina226_Get_Shunt_Voltage = INA226_Get_Shunt_Voltage,
		.CHG_Ina226_Get_Manufacturer_ID = INA226_Get_Manufacturer_ID,
		.CHG_Ina226_Get_Die_ID = INA226_Get_Die_ID,
		.CHG_Ina226_Unlock_Alert = INA226_Unlock_Alert,
};
