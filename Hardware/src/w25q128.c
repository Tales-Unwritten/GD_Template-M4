#include "../Hardware/inc/w25q128.h"

#define CSS_PIN                GPIO_PIN_6
#define CLOCK_PIN              GPIO_PIN_7
#define MISO_PIN               GPIO_PIN_8
#define MOSI_PIN               GPIO_PIN_9

#define W25Qxx_RCU_SPI         RCU_SPI4
#define W25Qxx_RCU_GPIO        RCU_GPIOF
#define W25Qxx_RCU_GPIO_CSS    RCU_GPIOF

#define W25Qxx_SPI             SPI4
#define W25Qxx_PORT            GPIOF
#define W25Qxx_CSS_PORT        GPIOF

#define W25Qxx_AF_SPI          GPIO_AF_5




static void w25q128_gpio_init(void)
{
	rcu_periph_clock_enable(W25Qxx_RCU_GPIO);

	gpio_af_set(W25Qxx_PORT, W25Qxx_AF_SPI, CLOCK_PIN); 
	gpio_af_set(W25Qxx_PORT, W25Qxx_AF_SPI, MISO_PIN); 
	gpio_af_set(W25Qxx_PORT, W25Qxx_AF_SPI, MOSI_PIN); 

	gpio_mode_set(W25Qxx_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, CLOCK_PIN);
	gpio_mode_set(W25Qxx_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, MISO_PIN);
	gpio_mode_set(W25Qxx_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, MOSI_PIN);

	gpio_output_options_set(W25Qxx_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CLOCK_PIN);
	gpio_output_options_set(W25Qxx_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MISO_PIN);
	gpio_output_options_set(W25Qxx_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MOSI_PIN);

	rcu_periph_clock_enable(W25Qxx_RCU_GPIO_CSS);
	gpio_mode_set(W25Qxx_CSS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CSS_PIN);
	gpio_output_options_set(W25Qxx_CSS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CSS_PIN);

	w25qxx_css_disable(); // 确保CSS引脚初始状态为高电平

}

static void w25q128_spi_init(void)
{
	rcu_periph_clock_enable(W25Qxx_RCU_SPI); 
	spi_parameter_struct spi_init_struct;
	spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;  	//传输模式全双工
	spi_init_struct.device_mode          = SPI_MASTER;                	//配置为主机
	spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;        	//8位数据
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;		//极性相位  
	spi_init_struct.nss                  = SPI_NSS_SOFT;              	//软件cs
	spi_init_struct.prescale             = SPI_PSC_2;                 	//SPI时钟预分频为2
	spi_init_struct.endian               = SPI_ENDIAN_MSB;            	//高位在前
	//将参数填入SPI4
	spi_init(SPI4, &spi_init_struct);
	//使能SPI
	spi_enable(SPI4);
}

void w25qxx_css_enable(void)
{
	gpio_bit_reset(W25Qxx_PORT, CSS_PIN);
}
void w25qxx_css_disable(void)
{
	gpio_bit_set(W25Qxx_PORT, CSS_PIN);
}


void W25Q64_wait_busy(void)   
{   
    unsigned char byte = 0;
    do
     { 
        w25qxx_css_enable();                            
        spi_read_write_byte(0x05);                 
        byte = spi_read_write_byte(0Xff);       
        w25qxx_css_disable();         
     }while( ( byte & 0x01 ) == 1 );  
}  


void W25Q64_write_enable(void)   
{
    w25qxx_css_enable();                           
    spi_read_write_byte(0x06);                  
    w25qxx_css_disable(); 
}                            	      


void W25Q64_erase_sector(uint32_t addr)   
{
	addr *= 4096;
	W25Q64_write_enable();  //写使能
	W25Q64_wait_busy();     //判断忙
	w25qxx_css_enable();
	spi_read_write_byte(0x20);
	spi_read_write_byte((uint8_t)((addr)>>16));
	spi_read_write_byte((uint8_t)((addr)>>8));
	spi_read_write_byte((uint8_t)addr);
	w25qxx_css_disable();
	//等待擦除完成
	W25Q64_wait_busy();
}         				                      	      



void W25Q64_write(uint8_t* buffer, uint32_t addr, uint16_t numbyte)
{    //0x02e21
    unsigned int i = 0;
    W25Q64_erase_sector(addr/4096);//擦除扇区数据
    W25Q64_write_enable();//写使能    
    W25Q64_wait_busy(); //忙检测    
    //写入数据
    w25qxx_css_enable();
    spi_read_write_byte(0x02);
    spi_read_write_byte((uint8_t)((addr)>>16));
    spi_read_write_byte((uint8_t)((addr)>>8));   
    spi_read_write_byte((uint8_t)addr);   
    for(i=0;i<numbyte;i++)
    {
        spi_read_write_byte(buffer[i]);  
    }
    w25qxx_css_disable();
    W25Q64_wait_busy(); //忙检测      
}


void W25Q64_read(uint8_t* buffer,uint32_t read_addr,uint16_t read_length)   
{ 
	uint16_t i;   		
	w25qxx_css_enable();            
	spi_read_write_byte(0x03);                           
	spi_read_write_byte((uint8_t)((read_addr)>>16));           
	spi_read_write_byte((uint8_t)((read_addr)>>8));   
	spi_read_write_byte((uint8_t)read_addr);   
	for(i=0;i<read_length;i++)
	{ 
		buffer[i]= spi_read_write_byte(0XFF);  
	}
	w25qxx_css_disable();
} 


void w25q128_spi_config(void)
{
	w25q128_gpio_init(); // 初始化GPIO
	w25q128_spi_init();  // 初始化SPI
}


