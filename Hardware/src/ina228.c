#include "../Hardware/inc/ina228.h"

I2C_Info INA228_i2C;


static void INA228_Init(uint32_t GPIOx, uint16_t SCL_Pin, uint16_t SDA_Pin, uint16_t Addr)
{   
	Soft_I2C_Init(&INA228_i2C, GPIOx, SCL_Pin, SDA_Pin, Addr);                                                                                                                                             
}


static void Ina228_WriteRegister(I2C_Info *i2c, uint16_t Device_Add, uint16_t RegAdd, uint16_t Data)
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

static uint16_t Ina228_ReadRegister(I2C_Info *i2c, uint16_t Device_Add, uint16_t RegAddress)
{
	static uint32_t Data_Temp = 0;
	Soft_I2C_Start(i2c);
	Soft_I2C_WriteByte(i2c, Device_Add << 1);
	Soft_I2C_ReceiveAck(i2c);
	Soft_I2C_WriteByte(i2c, RegAddress);
	Soft_I2C_ReceiveAck(i2c);

	Soft_I2C_Start(i2c);
	Soft_I2C_WriteByte(i2c, Device_Add << 1 | 0x01);
	Soft_I2C_ReceiveAck(i2c);
	Data_Temp = Soft_I2C_ReceiveByte(i2c);
	Soft_I2C_WriteAck(i2c, 0);
	Data_Temp = (Data_Temp << 8) | Soft_I2C_ReceiveByte(i2c);
	Soft_I2C_Stop(i2c);
	return Data_Temp;
}




void text_online_INA228(void)
{
    uint16_t Die_ID=Ina228_ReadRegister(&INA228_i2C,0x40,0x3F);
    uint16_t MANUFACTURER_ID=Ina228_ReadRegister(&INA228_i2C,0x40,0x3E);
    printf("INA228  Die_ID: 0x%04X\r\n", Die_ID);
    printf("INA228  MANUFACTURER_ID: 0x%04X\r\n", Die_ID);
}


void INA228_Config(uint32_t GPIOx, uint16_t SCL_Pin, uint16_t SDA_Pin, uint16_t Addr)
{
    INA228_Init(GPIOx, SCL_Pin, SDA_Pin, Addr);
}
