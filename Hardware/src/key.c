#include "../Hardware/inc/key.h"


static void Key_GPIO_Init(void)
{
	rcu_periph_clock_enable(KEY1_RCU); // Enable GPIOA clock
	rcu_periph_clock_enable(KEY2_RCU); // Enable GPIOC clock
	gpio_mode_set(KEY1_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEY1_PIN); // Set PA0 as input with pull-down
	gpio_mode_set(KEY2_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY2_PIN); // Set PC13 as input with pull-up
}

void Key_Configuration(void)
{
	Key_GPIO_Init(); // Initialize GPIO for keys
	// Key_EXTI_Init(); // Initialize EXTI for keys
}


void Key_Scan(void)
{
	if (gpio_input_bit_get(KEY1_PORT, KEY1_PIN) == SET) // Check if KEY1 is pressed
	{
		led_on(1); // Turn on LED1
	}
	else if (gpio_input_bit_get(KEY1_PORT, KEY1_PIN) == RESET) // Check if KEY2 is pressed
	{
		led_off(1); // Turn on LED2
	}

}

