#include "../Hardware/inc/key.h"

key_struct_t  key_struct; // 定义按键状态结构体
static uint16_t key1;
static uint16_t key2;
/**
 * @brief       基本定时器TIMER6定时中断初始化
 * @note
 *              基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为60M, 所以定时器TIMER6时钟 = 120Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值
 * @param       psc: 时钟预分频数
 * @retval      无
 */
static void timer6_int_init(uint16_t arr, uint16_t psc)
{
	timer_parameter_struct timer_initpara;               /* timer_initpara用于存放定时器的参数 */

	/* 使能RCU相关时钟 */ 
	rcu_periph_clock_enable(RCU_TIMER6);                 /* 使能TIMER6的时钟 */

	/* 复位TIMER6 */
	timer_deinit(TIMER6);                                /* 复位TIMER6 */
	timer_struct_para_init(&timer_initpara);             /* 初始化timer_initpara为默认值 */

	/* 配置TIMER6 */
	timer_initpara.prescaler         = psc;              /* 设置预分频值 */
	timer_initpara.counterdirection  = TIMER_COUNTER_UP; /* 设置向上计数模式 */
	timer_initpara.period            = arr;              /* 设置自动重装载值 */
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1; /* 设置时钟分频因子 */
	timer_init(TIMER6, &timer_initpara);                 /* 根据参数初始化定时器 */

	/* 使能定时器及其中断 */
	timer_interrupt_enable(TIMER6, TIMER_INT_UP);        /* 使能定时器的更新中断 */
	nvic_irq_enable(TIMER6_IRQn, 1, 3);                  /* 配置NVIC设置优先级，抢占优先级1，响应优先级3 */
	timer_enable(TIMER6);                                /* 使能定时器TIMER6 */
}

static void Key_GPIO_Init(void)
{
	rcu_periph_clock_enable(KEY1_RCU); // Enable GPIOA clock
	rcu_periph_clock_enable(KEY2_RCU); // Enable GPIOC clock
	gpio_mode_set(KEY1_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEY1_PIN); // Set PA0 as input with pull-down
	gpio_mode_set(KEY2_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY2_PIN);   // Set PC13 as input with pull-up
}

uint16_t key1_get_status(void)
{
	static uint16_t status = 0;
	status = gpio_input_bit_get(KEY1_PORT, KEY1_PIN); // Check if KEY1 is pressed
	return status;
}

uint16_t key2_get_status(void)
{
	static uint16_t status = 0;
	status = gpio_input_bit_get(KEY2_PORT, KEY2_PIN); // Check if KEY1 is pressed
	return !status;
}


void key_logic_handle(void)
{
	// Handle Key1 logic
	if (key_struct.Key1_Status == 0xFFFF)
	{
		if (key_struct.Key1_Count >= 258 && key_struct.Key1_Status == 0xFFFF)
		{
			key_struct.Key1_State = key_long_press;
			led_on(3);
			key_struct.Key1_Count = 0;
		}
		else
		{
			key_struct.Key1_State = key_single_click;
			led_on(1);
		}
	}
	else
	{
		key_struct.Key1_State = key_none;
		led_off(1);
	}

	// Handle Key2 logic
	if (key_struct.Key2_Status == 0xFFFF)
	{
		key_struct.Key2_State = key_single_click;
		led_on(2);
	}
	else
	{
		key_struct.Key2_State = key_none;
		led_off(2);
	}
}

/**
 * @brief       配置1ms定时器中断
 * @note        定时器时钟为120MHz，5ms定时器中断需要计算arr和psc
 * @retval      无
 */
void configure_key_init(void)
{
	uint16_t psc = 120   - 1;   // 预分频器将时钟分频到1MHz (120MHz / 120 = 1MHz)
	uint16_t arr = 5000  - 1;   // 自动重装载值设置为1000 (1MHz / 5000 = 200Hz = 5ms)
	timer6_int_init(arr, psc);
	Key_GPIO_Init();
}

/**
 * @brief       基本定时器TIMER6中断服务函数
 * @param       无
 * @retval      无
 */
void TIMER6_IRQHandler(void)
{
	if (timer_interrupt_flag_get(TIMER6, TIMER_INT_FLAG_UP) == SET) /* 判断定时器更新中断是否发生 */
	{
		key_struct.Key1_Count++;
		if (key_struct.Key1_Count > 400)
		{
			key_struct.Key1_Count=0;
		}
		
		key1 = key1_get_status(); // 获取按键1状态
		key_struct.Key1_Status = (key_struct.Key1_Status << 1) | key1;
		key2 = key2_get_status(); // 获取按键2状态
		key_struct.Key2_Status = (key_struct.Key2_Status << 1) | key2;
		timer_interrupt_flag_clear(TIMER6, TIMER_INT_FLAG_UP); /* 清除定时器更新中断标志 */
	}
}
