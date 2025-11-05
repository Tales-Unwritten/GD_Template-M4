//备注：拷贝代码请加上作者信息
//作者：王海涛
//邮箱：1126471088@qq.com
//版本：V1.0.0
#include "gpio_bsp.h"

__attribute__((weak)) uint32_t __clz(uint32_t value)
{
    uint32_t count = 0;
    while ((value & 0x80000000) == 0 && count < 32)
    {
        count++;
        value <<= 1;
    }
    return count;
}


/*GPIO端口时钟*/
static void WHT_GPIO_BSP_Set_Clock(WHT_GPIO_Port_enum portx, ControlStatus state)
{
    unsigned int RCC_APB2Periph;

    switch (portx)
    {
    case PortA:RCC_APB2Periph = RCU_GPIOA; break;
    case PortB:RCC_APB2Periph = RCU_GPIOB; break;
    case PortC:RCC_APB2Periph = RCU_GPIOC; break;
    case PortD:RCC_APB2Periph = RCU_GPIOD; break;
    case PortE:RCC_APB2Periph = RCU_GPIOE; break;
    case PortF:RCC_APB2Periph = RCU_GPIOF; break;
    case PortG:RCC_APB2Periph = RCU_GPIOG; break;
    case PortH:RCC_APB2Periph = RCU_GPIOH; break;
    case PortI:RCC_APB2Periph = RCU_GPIOI; break;
    default:
        return;
    }
	if (state == ENABLE)
		rcu_periph_clock_enable((rcu_periph_enum)RCC_APB2Periph);
	else
		rcu_periph_clock_disable((rcu_periph_enum)RCC_APB2Periph);
}
/*GPIO引脚工作模式*/
static void WHT_GPIO_BSP_Set_Mode(WHT_GPIO_Port_enum portx, uint16_t pinx, WHT_GPIO_Mode_enum mode)
{
    unsigned int GPIO_Mode;
    unsigned int GPIO_PuPd;
    unsigned int GPIO_OType;

    switch (mode)
    {
    case Mode_Ain:        //模拟输入
        GPIO_Mode = GPIO_MODE_ANALOG;
        GPIO_OType = GPIO_OTYPE_PP;
        GPIO_PuPd = GPIO_PUPD_NONE;
        break;
    case Mode_In_FLOATING://浮空输入
        GPIO_Mode = GPIO_MODE_INPUT;
        GPIO_OType = GPIO_OTYPE_PP;
        GPIO_PuPd = GPIO_PUPD_NONE;
        break;
    case Mode_IPD:        //下拉输入
        GPIO_Mode = GPIO_MODE_INPUT;
        GPIO_OType = GPIO_OTYPE_PP;
        GPIO_PuPd = GPIO_PUPD_PULLDOWN;
        break;
    case Mode_IPU:        //上拉输入
        GPIO_Mode = GPIO_MODE_INPUT;
        GPIO_OType = GPIO_OTYPE_PP;
        GPIO_PuPd = GPIO_PUPD_PULLUP;
        break;
    case Mode_Out_OD:     //开漏输出
        GPIO_Mode = GPIO_MODE_OUTPUT;
        GPIO_OType = GPIO_OTYPE_OD;
        GPIO_PuPd = GPIO_PUPD_PULLUP;
        break;
    case Mode_Out_PP:     //推挽输出
        GPIO_Mode = GPIO_MODE_OUTPUT;
        GPIO_OType = GPIO_OTYPE_PP;
        GPIO_PuPd = GPIO_PUPD_NONE;
        break;
    case Mode_OPD_PP:     //下拉推挽输出
        GPIO_Mode = GPIO_MODE_OUTPUT;
        GPIO_OType = GPIO_OTYPE_PP;
        GPIO_PuPd = GPIO_PUPD_PULLDOWN;
        break;
    case Mode_OPU_PP:     //上拉推挽输出
        GPIO_Mode = GPIO_MODE_OUTPUT;
        GPIO_OType = GPIO_OTYPE_PP;
        GPIO_PuPd = GPIO_PUPD_PULLUP;
        break;
    case Mode_AF_OD:      //开漏复用输出
        GPIO_Mode = GPIO_MODE_AF;
        GPIO_OType = GPIO_OTYPE_OD;
        GPIO_PuPd = GPIO_PUPD_PULLUP;
        break;
    case Mode_AF_PP:      //推挽复用输出
        GPIO_Mode = GPIO_MODE_AF;
        GPIO_OType = GPIO_OTYPE_PP;
        GPIO_PuPd = GPIO_PUPD_NONE;
        break;
    default:
        break;
    }
    gpio_mode_set(portx, GPIO_Mode, GPIO_PuPd, pinx);
    gpio_output_options_set(portx, GPIO_OType, GPIO_OSPEED_50MHZ, pinx);
}
/*GPIO引脚复用功能*/
static void WHT_GPIO_BSP_Set_AFIO(WHT_GPIO_Port_enum portx, WHT_GPIO_Pin_enum pinx, WHT_GPIO_AFIO_enum AFIO)
{
    gpio_af_set(portx, AFIO, pinx);
}

/*设置GPIO引脚状态*/
static void WHT_GPIO_BSP_Set_State(WHT_GPIO_Port_enum portx, uint16_t pinx, WHT_GPIO_State_enum state)
{
    gpio_bit_write(portx, pinx, (bit_status)state);
}
/*获取GPIO引脚状态*/
static WHT_GPIO_State_enum WHT_GPIO_BSP_Get_State(WHT_GPIO_Port_enum portx, uint16_t pinx)
{
    return (WHT_GPIO_State_enum)gpio_input_bit_get(portx, pinx);
}
/*获取GPIO端口状态*/
static uint16_t WHT_GPIO_BSP_Get_Prot_Value(WHT_GPIO_Port_enum portx)
{
    return gpio_input_port_get(portx);
}
/*设置GPIO端口状态*/
static void WHT_GPIO_BSP_Set_Port_Value(WHT_GPIO_Port_enum portx, uint16_t port_value)
{
    gpio_port_write(portx, port_value);
}

/*注册位带操作读取*/
static void WHT_GPIO_BSP_Register_Bit_Output(WHT_GPIO_Port_enum portx, WHT_GPIO_Pin_enum pinx, volatile unsigned int** output)
{
    unsigned int GPIO_ODR_Address;

    //原理为：#define WHT_Set_GPIO_Pin_State(WHT_GPIO_Port_enum,WHT_GPIO_Pin_Number_enum)    (*((volatile unsigned int*)(((WHT_GPIO_Port_enum+0x0c)&0xf0000000)+0x02000000+(((WHT_GPIO_Port_enum+0x0c)&0x00ffffff)<<5)+(WHT_GPIO_Pin_Number_enum<<2))))
    GPIO_ODR_Address =  ((portx + GPIO_ODR_Offset_Address) & 0xf0000000) + 0x02000000;
    GPIO_ODR_Address += ((portx + GPIO_ODR_Offset_Address) & 0x00ffffff) << 5;
    GPIO_ODR_Address += (31 - __clz(pinx)) << 2;
    *output = (volatile unsigned int*)GPIO_ODR_Address;
}
/*注册未带操作写*/
static void WHT_GPIO_BSP_Register_Bit_Input(WHT_GPIO_Port_enum portx, WHT_GPIO_Pin_enum pinx, volatile unsigned int** input)
{
    unsigned int GPIO_IDR_Address;
    //原理为：#define WHT_Get_GPIO_Pin_State(WHT_GPIO_Port_enum,WHT_GPIO_Pin_Number_enum)    (*((volatile unsigned int*)(((WHT_GPIO_Port_enum+0x08)&0xf0000000)+0x02000000+(((WHT_GPIO_Port_enum+0x08)&0x00ffffff)<<5)+(WHT_GPIO_Pin_Number_enum<<2))))
    GPIO_IDR_Address =  ((portx + GPIO_IDR_Offset_Address) & 0xf0000000) + 0x02000000;
    GPIO_IDR_Address += ((portx + GPIO_IDR_Offset_Address) & 0x00ffffff) << 5;
    GPIO_IDR_Address += (31 - __clz(pinx)) << 2;
    *input = (volatile unsigned int*)GPIO_IDR_Address;
}


/*全局常量*/
const WHT_GPIO_BSP_t WHT_GPIO_BSP =
{
    .WHT_Set_Clock = WHT_GPIO_BSP_Set_Clock,
    .WHT_Set_Mode  = WHT_GPIO_BSP_Set_Mode,
    .WHT_Set_AFIO  = WHT_GPIO_BSP_Set_AFIO,
    .WHT_Get_Pin   = WHT_GPIO_BSP_Get_State,
    .WHT_Set_Pin   = WHT_GPIO_BSP_Set_State,
    .WHT_Get_Prot  = WHT_GPIO_BSP_Get_Prot_Value,
    .WHT_Set_Prot  = WHT_GPIO_BSP_Set_Port_Value,
    .WHT_Register_Bit_Output = WHT_GPIO_BSP_Register_Bit_Output,
    .WHT_Register_Bit_Input  = WHT_GPIO_BSP_Register_Bit_Input,
};
