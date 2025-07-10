/* bitband.c */
#include "../Hardware/inc/bitband.h"



#define IS_VALID_GPIO_PERIPH(p) \
    ((p) == GPIOA || (p) == GPIOB || (p) == GPIOC || (p) == GPIOD || \
     (p) == GPIOE || (p) == GPIOF || (p) == GPIOG || (p) == GPIOH)

#define BITBAND_ALIAS(addr, bitnum) \
    ((volatile uint32_t*)(BITBAND_BASE + (((uint32_t)(addr) - PERIPH_BASE) * 32U) + ((bitnum) * 4U)))

void bitband_gpio_set(uint32_t gpio_periph, uint32_t pin) {
    if (IS_VALID_GPIO_PERIPH(gpio_periph) && pin <= GPIO_PIN_MAX) {
        *BITBAND_ALIAS((uint32_t*)(gpio_periph + 0x14), pin) = 1; // ODR
    }
}

void bitband_gpio_clear(uint32_t gpio_periph, uint32_t pin) {
    if (IS_VALID_GPIO_PERIPH(gpio_periph) && pin <= GPIO_PIN_MAX) {
        *BITBAND_ALIAS((uint32_t*)(gpio_periph + 0x14), pin) = 0; // ODR
    }
}

void bitband_gpio_write(uint32_t gpio_periph, uint32_t pin, bool value) {
    if (IS_VALID_GPIO_PERIPH(gpio_periph) && pin <= GPIO_PIN_MAX) {
        *BITBAND_ALIAS((uint32_t*)(gpio_periph + 0x14), pin) = value ? 1 : 0; // ODR
    }
}

bool bitband_gpio_read(uint32_t gpio_periph, uint32_t pin) {
    if (IS_VALID_GPIO_PERIPH(gpio_periph) && pin <= GPIO_PIN_MAX) {
        return (*BITBAND_ALIAS((uint32_t*)(gpio_periph + 0x10), pin) & 0x1); // IDR
    }
    return false;
}
