#include "../Hardware/inc/can.h"


FlagStatus receive_flag;                      // CAN接收标志位，用于指示是否有新的CAN消息接收
uint8_t transmit_number = 0x0;                // CAN发送计数器，用于跟踪发送的消息数量
can_trasnmit_message_struct transmit_message; // CAN发送消息结构体，用于存储待发送的CAN帧数据

/* enable can clock */
static void can_gpio_init(void)
{
    /* enable can clock */
    rcu_periph_clock_enable(RCU_GPIOB); // 使能GPIOB端口时钟

#ifdef DEV_CAN0_USED
    /* configure CAN0 GPIO */
    rcu_periph_clock_enable(RCU_CAN0);  // 使能CAN0外设时钟

    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8); // 配置PB8推挽输出，50MHz速率
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);               // 配置PB8为复用功能模式，无上下拉
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_8);                                    // 设置PB8复用为CAN0_RX功能

    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9); // 配置PB9推挽输出，50MHz速率
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);               // 配置PB9为复用功能模式，无上下拉
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_9);                                    // 设置PB9复用为CAN0_TX功能
#else
    /* configure CAN1 GPIO */
    rcu_periph_clock_enable(RCU_CAN1);  // 使能CAN1外设时钟

    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12); // 配置PB5推挽输出，50MHz速率
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12);               // 配置PB5为复用功能模式，无上下拉
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_12);                                    // 设置PB5复用为CAN1_RX功能

    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13); // 配置PB6推挽输出，50MHz速率
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);               // 配置PB6为复用功能模式，无上下拉
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_13);                                    // 设置PB6复用为CAN1_TX功能
#endif
}

static void can_networking_init(void)
{
    can_parameter_struct can_parameter;     // CAN参数配置结构体
    can_filter_parameter_struct can_filter; // CAN过滤器参数配置结构体

    can_struct_para_init(CAN_INIT_STRUCT, &can_parameter); // 初始化CAN参数结构体为默认值
    can_struct_para_init(CAN_FILTER_STRUCT, &can_filter);  // 初始化CAN过滤器结构体为默认值

    /* initialize CAN register */
    can_deinit(CANX); // 复位CAN寄存器到默认状态

    /* initialize CAN */
    can_parameter.time_triggered = DISABLE;           // 禁用时间触发通信模式
    can_parameter.auto_bus_off_recovery = ENABLE;     // 启用自动离线恢复功能
    can_parameter.auto_wake_up = DISABLE;             // 禁用自动唤醒功能
    can_parameter.auto_retrans = ENABLE;              // 启用自动重传功能
    can_parameter.rec_fifo_overwrite = DISABLE;       // 禁用接收FIFO覆盖模式
    can_parameter.trans_fifo_order = DISABLE;         // 禁用发送FIFO按顺序发送
    can_parameter.working_mode = CAN_NORMAL_MODE;     // 设置为正常工作模式
    can_parameter.resync_jump_width = CAN_BT_SJW_1TQ; // 重同步跳跃宽度为1个时间量子
    can_parameter.time_segment_1 = CAN_BT_BS1_7TQ;    // 时间段1为7个时间量子
    can_parameter.time_segment_2 = CAN_BT_BS2_2TQ;    // 时间段2为2个时间量子
    /* baudrate 1Mbps */
    can_parameter.prescaler = 5;    // 预分频器设置为5，配合时间段设置实现1Mbps波特率
    can_init(CANX, &can_parameter); // 应用CAN参数配置

    /* initialize filter */
#ifdef DEV_CAN0_USED
    /* CAN0 filter number */
    can_filter.filter_number = 0; // CAN0使用过滤器0
#else
    /* CAN1 filter number */
    can_filter.filter_number = 15; // CAN1使用过滤器15
#endif
    /* initialize filter */
    can_filter.filter_mode = CAN_FILTERMODE_MASK;  // 设置过滤器为掩码模式
    can_filter.filter_bits = CAN_FILTERBITS_32BIT; // 设置过滤器为32位模式
    can_filter.filter_list_high = 0x0000;          // 过滤器标识符高16位（接受所有ID）
    can_filter.filter_list_low = 0x0000;           // 过滤器标识符低16位（接受所有ID）
    can_filter.filter_mask_high = 0x0000;          // 过滤器掩码高16位（不关心任何位）
    can_filter.filter_mask_low = 0x0000;           // 过滤器掩码低16位（不关心任何位）
    can_filter.filter_fifo_number = CAN_FIFO1;     // 过滤后的消息存储到FIFO1
    can_filter.filter_enable = ENABLE;             // 启用过滤器
    can_filter_init(&can_filter);                  // 应用过滤器配置
}

static void nvic_config(void)
{
#ifdef DEV_CAN0_USED
    /* configure CAN0 NVIC */
    nvic_irq_enable(CAN0_RX1_IRQn, 0, 0); // 启用CAN0接收FIFO1中断，抢占优先级0，子优先级0
#else
    /* configure CAN1 NVIC */
    nvic_irq_enable(CAN1_RX1_IRQn, 0, 0); // 启用CAN1接收FIFO1中断，抢占优先级0，子优先级0
#endif
}

/**
 * @brief CAN发送数据函数
 *
 * 该函数用于发送CAN数据帧，检查数据长度是否符合要求，并填充发送消息结构体。
 *
 * @param data 指向要发送的数据缓冲区的指针
 * @param len 数据长度，最大为8字节
 */
void can_transmit_data(uint8_t *data, uint8_t len)
{
    /* 检查数据长度是否超过8字节 */
    if (data == NULL || len > 8)
    {
        return; // 数据长度超过CAN帧限制，直接返回
    }

    /* 填充发送消息结构体 */
    transmit_message.tx_sfid = 0x0000;        // 标准帧ID
    transmit_message.tx_efid = 0x00000000;    // 扩展帧ID
    transmit_message.tx_ff = CAN_FF_STANDARD; // 标准帧格式
    transmit_message.tx_ft = CAN_FT_DATA;     // 数据帧类型
    transmit_message.tx_dlen = len;           // 数据长度
    for (uint8_t i = 0; i < len; i++)
    {
        transmit_message.tx_data[i] = data[i]; // 填充数据
    }

    /* 发送CAN消息 */
    can_message_transmit(CANX, &transmit_message);
}

/**
 * @brief CAN接收数据处理函数
 *
 * @param out_data 输出缓冲区，长度至少为8字节
 * @return 实际接收到的数据字节数，若无数据则返回0
 *
 *    使用示例：
 *    uint8_t buffer[8];
 *    uint8_t len = can_receive_data(buffer);
 *    if (len > 0)
 *   {
 *         // 处理 buffer 中的 len 字节数据
 *   }
 */
uint8_t can_receive_data(uint8_t *out_data)
{
    can_receive_message_struct receive_message;

    if (out_data == NULL)
    {
        return 0; // 输出缓冲区无效，直接返回
    }

    if (receive_flag)
    {
        receive_flag = RESET;
        can_message_receive(CANX, CAN_FIFO1, &receive_message);

        // 检查接收长度是否合法（CAN最大8字节）
        uint8_t len = receive_message.rx_dlen;
        if (len > 8)
        {
            len = 8;
        }
        memcpy(out_data, receive_message.rx_data, len);
        return len;
    }
    else
    {
        return 0; // 无新数据可接收，返回0字节
    }
}

/**
 * @brief 通用CAN接收FIFO1中断处理函数
 *
 * 该函数用于处理CAN接收FIFO1非空中断，设置接收标志位，并可扩展为错误处理等功能。
 * 增加了基本的安全性和健壮性检查。
 */
static void CANx_RX_IRQHandler(void)
{
    /* 检查接收FIFO1非空中断标志 */
    if (can_interrupt_flag_get(CANX, CAN_INT_FLAG_RFL1) == SET) 
    {
        /* 可选：检查FIFO溢出错误 */
        if (can_interrupt_flag_get(CANX, CAN_INT_FLAG_RFO1) == SET)
        {
            can_interrupt_flag_clear(CANX, CAN_INT_FLAG_RFO1); // 清除FIFO溢出标志
            // 可在此处添加错误处理或日志记录
        }

        /* 可选：检查错误警告标志 */
        if (can_flag_get(CANX, CAN_FLAG_WERR) == SET)
        {
            // 可在此处添加错误处理或报警
        }

        can_interrupt_flag_clear(CANX, CAN_INT_FLAG_RFL1); // 清除接收FIFO1非空中断标志
        receive_flag = SET; // 设置接收标志位，表示有新数据可用
    }
}


/**
 * @brief CAN配置函数
 *
 * 该函数用于初始化CAN模块，包括GPIO引脚配置、CAN网络配置和中断向量配置。
 */
void can_config(void)
{
    can_gpio_init();                           // 配置GPIO引脚
    can_networking_init();                     // 初始化CAN网络配置
    nvic_config();                             // 配置中断向量
    can_interrupt_enable(CANX, CAN_INT_RFNE1); // 启用接收FIFO1非空中断
}


#ifdef DEV_CAN0_USED
void CAN0_RX_IRQHandler(void)
{
    CANx_RX_IRQHandler(); // 调用通用的接收中断处理函数
}
#else
void CAN1_RX_IRQHandler(void)
{
    CANx_RX_IRQHandler(); // 调用通用的接收中断处理函数
}
#endif

