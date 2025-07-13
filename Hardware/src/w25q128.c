#include "../Hardware/inc/w25q128.h"

#define CSS_PIN   GPIO_PIN_6
#define CLOCK_PIN GPIO_PIN_7
#define MISO_PIN  GPIO_PIN_8
#define MOSI_PIN  GPIO_PIN_9

#define W25Qxx_RCU_SPI RCU_SPI4
#define W25Qxx_RCU_GPIO RCU_GPIOF
#define W25Qxx_RCU_GPIO_CSS RCU_GPIOF

#define W25Qxx_SPI SPI4
#define W25Qxx_PORT GPIOF

#define W25Qxx_AF_SPI GPIO_AF_5

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
	gpio_mode_set(W25Qxx_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CSS_PIN);
	gpio_output_options_set(W25Qxx_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CSS_PIN);

	w25qxx_css_disable(); // 确保CSS引脚初始状态为高电平

}


static void w25q128_spi_init(void)
{
	rcu_periph_clock_enable(W25Qxx_RCU_SPI); 
}



void w25qxx_css_enable(void)
{
	gpio_bit_reset(W25Qxx_PORT, CSS_PIN);
}
void w25qxx_css_disable(void)
{
	gpio_bit_set(W25Qxx_PORT, CSS_PIN);
}
	