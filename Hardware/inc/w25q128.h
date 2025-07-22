#pragma once
#include <stdint.h>
#include "gd32f4xx.h"
#include "systick.h"



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

void w25qxx_css_enable(void);
void w25qxx_css_disable(void);
void w25qxx_wait_busy(void);
void w25qxx_write_enable(void);
uint8_t spi_read_write_byte(uint8_t dat);
void w25qxx_erase_sector(uint32_t addr);
void w25qxx_write(uint8_t* buffer, uint32_t addr, uint16_t numbyte);
void w25qxx_read(uint8_t* buffer, uint32_t read_addr, uint16_t read_length);

void w25qxx_spi_config(void);
