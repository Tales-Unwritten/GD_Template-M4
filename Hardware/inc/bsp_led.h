#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "gd32f4xx.h"
#include "systick.h"


#define RCU_LED1  	RCU_GPIOC   // GPIOA��ʱ��
#define RCU_LED2  	RCU_GPIOC   // GPIOA��ʱ��
#define RCU_LED3  	RCU_GPIOA   // GPIOA��ʱ��

#define PORT_LED1 	GPIOC				// GPIOA�Ķ˿�
#define PORT_LED2 	GPIOC				// GPIOA�Ķ˿�
#define PORT_LED3 	GPIOA				// GPIOA�Ķ˿�


#define PIN_LED1 		GPIO_PIN_5  // GPIOA������
#define PIN_LED2 		GPIO_PIN_4  // GPIOA������
#define PIN_LED3 		GPIO_PIN_7  // GPIOA������

void led_gpio_config(void); 	  // led gpio��������
void led_on(uint8_t led_num);   // ����LED
void led_off(uint8_t led_num);  // Ϩ��LED
void led_toggle(uint8_t led_num); // �л�LED״̬

#endif /* BSP_LED_H */