//备注：拷贝代码请加上作者信息
//作者：王海涛
//邮箱：1126471088@qq.com
//版本：V1.0.0
#include "dma_bsp.h"
#include <string.h>

/*DMA总管理结构体*/
typedef struct
{
    WHT_DMA_Periph_enum Periph;//DMA名字
    WHT_DMA_Stream_enum Stream;//DMA通道名字
    WHT_DMA_Info_t Info;       //所有标志,提供给外部
    void (*IT_Callback)(void); //通道回调函数
}WHT_Data_t;

static WHT_Data_t WHT_DMA1_Stream_0 = {.Periph = (WHT_DMA_Periph_enum)DMA0, .Stream = (WHT_DMA_Stream_enum)DMA_CH0};
static WHT_Data_t WHT_DMA1_Stream_1 = {.Periph = (WHT_DMA_Periph_enum)DMA0, .Stream = (WHT_DMA_Stream_enum)DMA_CH1};
static WHT_Data_t WHT_DMA1_Stream_2 = {.Periph = (WHT_DMA_Periph_enum)DMA0, .Stream = (WHT_DMA_Stream_enum)DMA_CH2};
static WHT_Data_t WHT_DMA1_Stream_3 = {.Periph = (WHT_DMA_Periph_enum)DMA0, .Stream = (WHT_DMA_Stream_enum)DMA_CH3};
static WHT_Data_t WHT_DMA1_Stream_4 = {.Periph = (WHT_DMA_Periph_enum)DMA0, .Stream = (WHT_DMA_Stream_enum)DMA_CH4};
static WHT_Data_t WHT_DMA1_Stream_5 = {.Periph = (WHT_DMA_Periph_enum)DMA0, .Stream = (WHT_DMA_Stream_enum)DMA_CH5};
static WHT_Data_t WHT_DMA1_Stream_6 = {.Periph = (WHT_DMA_Periph_enum)DMA0, .Stream = (WHT_DMA_Stream_enum)DMA_CH6};
static WHT_Data_t WHT_DMA1_Stream_7 = {.Periph = (WHT_DMA_Periph_enum)DMA0, .Stream = (WHT_DMA_Stream_enum)DMA_CH7};
#if defined(GD32F450) || defined(GD32F470) || defined(GD32F405) || defined(GD32F425) || defined(GD32F407) || defined(GD32F427)
static WHT_Data_t WHT_DMA2_Stream_0 = {.Periph = (WHT_DMA_Periph_enum)DMA1, .Stream = (WHT_DMA_Stream_enum)DMA_CH0};
static WHT_Data_t WHT_DMA2_Stream_1 = {.Periph = (WHT_DMA_Periph_enum)DMA1, .Stream = (WHT_DMA_Stream_enum)DMA_CH1};
static WHT_Data_t WHT_DMA2_Stream_2 = {.Periph = (WHT_DMA_Periph_enum)DMA1, .Stream = (WHT_DMA_Stream_enum)DMA_CH2};
static WHT_Data_t WHT_DMA2_Stream_3 = {.Periph = (WHT_DMA_Periph_enum)DMA1, .Stream = (WHT_DMA_Stream_enum)DMA_CH3};
static WHT_Data_t WHT_DMA2_Stream_4 = {.Periph = (WHT_DMA_Periph_enum)DMA1, .Stream = (WHT_DMA_Stream_enum)DMA_CH4};
static WHT_Data_t WHT_DMA2_Stream_5 = {.Periph = (WHT_DMA_Periph_enum)DMA1, .Stream = (WHT_DMA_Stream_enum)DMA_CH5};
static WHT_Data_t WHT_DMA2_Stream_6 = {.Periph = (WHT_DMA_Periph_enum)DMA1, .Stream = (WHT_DMA_Stream_enum)DMA_CH6};
static WHT_Data_t WHT_DMA2_Stream_7 = {.Periph = (WHT_DMA_Periph_enum)DMA1, .Stream = (WHT_DMA_Stream_enum)DMA_CH7};
#endif

#define WHT_DMA     ((WHT_Data_t*)config->Private_Data)//简化操作


/*DMA中断优先级配置*/
static void NVIC_Config_uration(unsigned char NVIC_IRQStream)
{
	NVIC_SetPriority((IRQn_Type)NVIC_IRQStream, 15);
	NVIC_EnableIRQ((IRQn_Type)NVIC_IRQStream);
}
/*DMA注册*/
static ErrStatus WHT_DMA_BSP_Register(const WHT_DMA_Config_t* config)
{
    if ((WHT_DMA != NULL) && (config->Info != NULL))//之前注册过了
        return SUCCESS;
    
    if (config->Periph == DMA0)
    {
        switch ((unsigned int)config->Stream)
        {
        case DMA_CH0:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA1_Stream_0; NVIC_Config_uration(DMA0_Channel0_IRQn); break;
        case DMA_CH1:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA1_Stream_1; NVIC_Config_uration(DMA0_Channel1_IRQn); break;
        case DMA_CH2:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA1_Stream_2; NVIC_Config_uration(DMA0_Channel2_IRQn); break;
        case DMA_CH3:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA1_Stream_3; NVIC_Config_uration(DMA0_Channel3_IRQn); break;
        case DMA_CH4:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA1_Stream_4; NVIC_Config_uration(DMA0_Channel4_IRQn); break;
        case DMA_CH5:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA1_Stream_5; NVIC_Config_uration(DMA0_Channel5_IRQn); break;
        case DMA_CH6:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA1_Stream_6; NVIC_Config_uration(DMA0_Channel6_IRQn); break;
        case DMA_CH7:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA1_Stream_7; NVIC_Config_uration(DMA0_Channel7_IRQn); break;
        default:return ERROR;
        }
    }
    else if (config->Periph == DMA1)
    {
        switch ((unsigned int)config->Stream)
        {
    #if defined(GD32F450) || defined(GD32F470) || defined(GD32F405) || defined(GD32F425) || defined(GD32F407) || defined(GD32F427)
        case DMA_CH0:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA2_Stream_0; NVIC_Config_uration(DMA1_Channel0_IRQn); break;
        case DMA_CH1:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA2_Stream_1; NVIC_Config_uration(DMA1_Channel1_IRQn); break;
        case DMA_CH2:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA2_Stream_2; NVIC_Config_uration(DMA1_Channel2_IRQn); break;
        case DMA_CH3:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA2_Stream_3; NVIC_Config_uration(DMA1_Channel3_IRQn); break;
        case DMA_CH4:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA2_Stream_4; NVIC_Config_uration(DMA1_Channel4_IRQn); break;
        case DMA_CH5:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA2_Stream_5; NVIC_Config_uration(DMA1_Channel5_IRQn); break;
        case DMA_CH6:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA2_Stream_6; NVIC_Config_uration(DMA1_Channel6_IRQn); break;
        case DMA_CH7:((WHT_DMA_Config_t*)config)->Private_Data = &WHT_DMA2_Stream_7; NVIC_Config_uration(DMA1_Channel7_IRQn); break;
    #endif
        default:return ERROR;
        }
    }
    
    ((WHT_DMA_Config_t*)config)->Info = &WHT_DMA->Info;
    WHT_DMA->Info.Mount_Count++;
    rcu_periph_clock_enable(WHT_DMA->Periph == DMA0 ? RCU_DMA0 : RCU_DMA1);
    return SUCCESS;
}
/*DMA配置*/
static void WHT_DMA_BSP_Config(const WHT_DMA_Config_t* config)
{
    if ((WHT_DMA == NULL) || (config->Info == NULL))
        return;

    WHT_DMA->Info.State = TRx_TS;
    WHT_DMA->IT_Callback = config->IT_Callback;

    dma_deinit(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream);                                          //Reset DMA
    dma_single_data_mode_init(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, ((WHT_DMA_Config_t*)config)->DMA_InitStruct);//Init Config DMA Stream
    if (config->DMA_InitStruct->circular_mode == DMA_CIRCULAR_MODE_ENABLE)                 //循环传输模式
    {
        dma_interrupt_disable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_FTF);//Off TC HT DMA Interrupt
        dma_interrupt_disable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_HTF);
        dma_interrupt_enable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_TAE);//Open TE DMA Interrupt
        dma_interrupt_enable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_SDE);
        dma_interrupt_enable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_FEE);
    }
    else
    {
        dma_interrupt_enable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_FTF);//open All DMA IT Stream
        dma_interrupt_enable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_HTF);//open All DMA IT Stream
        dma_interrupt_enable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_TAE);//open All DMA IT Stream
        dma_interrupt_enable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_SDE);//open All DMA IT Stream
        dma_interrupt_enable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_FEE);//open All DMA IT Stream
    }
    
    dma_channel_subperipheral_select(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, (dma_subperipheral_enum )config->Channel);
    dma_channel_enable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream);//Open DMA Stream
}

/*DMA获取当前数据个数*/
static unsigned short WHT_DMA_BSP_Get_Curr_Data_Count(const WHT_DMA_Config_t* config)
{
    if (WHT_DMA == NULL)
        return (unsigned short)65535;
    else
        return dma_transfer_number_get(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream);
}
/*DMA关闭*/
static void WHT_DMA_BSP_Close(const WHT_DMA_Config_t* config)
{
    if (WHT_DMA)
    {
        dma_interrupt_disable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_FTF);//Off TC HT DMA Interrupt
        dma_interrupt_disable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_HTF);
        dma_interrupt_disable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_TAE);//Open TE DMA Interrupt
        dma_interrupt_disable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_SDE);
        dma_interrupt_disable(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream, DMA_INT_FEE);
        dma_deinit(WHT_DMA->Periph, (dma_channel_enum)WHT_DMA->Stream);//Reset DMA
        WHT_DMA->Info.State = TRx_TC;
        WHT_DMA->Info.BSP_Mutex = RESET;
    }
}

/*DMA检查空闲数据流并上锁 STM32F1/F4 DMA1不支持内存拷贝*/
static ErrStatus WHT_DMA_BSP_Lock_Idle_Streamx(const WHT_DMA_Config_t* config)
{
    unsigned char Number;
    WHT_Data_t* DMAx_Streamx[] =
    {
        &WHT_DMA1_Stream_0,
        &WHT_DMA1_Stream_1,
        &WHT_DMA1_Stream_2,
        &WHT_DMA1_Stream_3,
        &WHT_DMA1_Stream_4,
        &WHT_DMA1_Stream_5,
        &WHT_DMA1_Stream_6,
        &WHT_DMA1_Stream_7,
#if defined(GD32F450) || defined(GD32F470) || defined(GD32F405) || defined(GD32F425) || defined(GD32F407) || defined(GD32F427)
        &WHT_DMA2_Stream_0,
        &WHT_DMA2_Stream_1,
        &WHT_DMA2_Stream_2,
        &WHT_DMA2_Stream_3,
        &WHT_DMA2_Stream_4,
        &WHT_DMA2_Stream_5,
        &WHT_DMA2_Stream_6,
        &WHT_DMA2_Stream_7,
#endif
    };

    for (Number = 0; Number < sizeof(DMAx_Streamx) / sizeof(DMAx_Streamx[0]); Number++)//寻找未被注册过并且未使用过的
    {
        if ((DMAx_Streamx[Number]->Info.Mount_Count == RESET) && (DMAx_Streamx[Number]->Info.BSP_Mutex == RESET))
        {
            DMAx_Streamx[Number]->Info.BSP_Mutex = SET;
            goto Game_Over;
        }
    }
    for (Number = 0; Number < sizeof(DMAx_Streamx) / sizeof(DMAx_Streamx[0]); Number++)//寻找所有的
    {
        if (DMAx_Streamx[Number]->Info.BSP_Mutex == RESET)
        {
            DMAx_Streamx[Number]->Info.BSP_Mutex = SET;
            goto Game_Over;
        }
    }
    return ERROR;

    Game_Over:
        DMAx_Streamx[Number]->Info.Mount_Count++;//由调用者释放!!!
        ((WHT_DMA_Config_t*)config)->Periph = DMAx_Streamx[Number]->Periph;
        ((WHT_DMA_Config_t*)config)->Stream = DMAx_Streamx[Number]->Stream;
        ((WHT_DMA_Config_t*)config)->Channel = DMA_SUBPERI0;
        ((WHT_DMA_Config_t*)config)->IT_Callback = NULL;
        ((WHT_DMA_Config_t*)config)->Info = &DMAx_Streamx[Number]->Info;
        ((WHT_DMA_Config_t*)config)->Private_Data = DMAx_Streamx[Number];
        
        rcu_periph_clock_enable(config->Periph == DMA0 ? RCU_DMA0 : RCU_DMA1);//Open DMA Clock

        if (config->Periph == DMA0)
        {
            switch ((unsigned int)config->Stream)
            {
            case DMA_CH0:NVIC_Config_uration(DMA0_Channel0_IRQn); break;
            case DMA_CH1:NVIC_Config_uration(DMA0_Channel1_IRQn); break;
            case DMA_CH2:NVIC_Config_uration(DMA0_Channel2_IRQn); break;
            case DMA_CH3:NVIC_Config_uration(DMA0_Channel3_IRQn); break;
            case DMA_CH4:NVIC_Config_uration(DMA0_Channel4_IRQn); break;
            case DMA_CH5:NVIC_Config_uration(DMA0_Channel5_IRQn); break;
            case DMA_CH6:NVIC_Config_uration(DMA0_Channel6_IRQn); break;
            case DMA_CH7:NVIC_Config_uration(DMA0_Channel7_IRQn); break;
            default:return ERROR;
            }
        }
        else if (config->Periph == DMA1)
        {
            switch ((unsigned int)config->Stream)
            {
        #if defined(GD32F450) || defined(GD32F470) || defined(GD32F405) || defined(GD32F425) || defined(GD32F407) || defined(GD32F427)
            case DMA_CH0:NVIC_Config_uration(DMA1_Channel0_IRQn); break;
            case DMA_CH1:NVIC_Config_uration(DMA1_Channel1_IRQn); break;
            case DMA_CH2:NVIC_Config_uration(DMA1_Channel2_IRQn); break;
            case DMA_CH3:NVIC_Config_uration(DMA1_Channel3_IRQn); break;
            case DMA_CH4:NVIC_Config_uration(DMA1_Channel4_IRQn); break;
            case DMA_CH5:NVIC_Config_uration(DMA1_Channel5_IRQn); break;
            case DMA_CH6:NVIC_Config_uration(DMA1_Channel6_IRQn); break;
            case DMA_CH7:NVIC_Config_uration(DMA1_Channel7_IRQn); break;
        #endif
            default:return ERROR;
            }
        }
        return SUCCESS;
}
/*DMA内存拷贝*/
void WHT_DMA_BSP_Memcpy(void* _Dst, void const* _Src, unsigned int _Size)
{
    WHT_DMA_Config_t Config;
    unsigned int Data_Size_4x_Number;

	if (_Size > 65535 * sizeof(unsigned int) || (_Size < sizeof(unsigned int)))//for max 262140 Byte
	{
		memcpy(_Dst, _Src, _Size);
		return;
	}
    if (WHT_DMA_BSP_Lock_Idle_Streamx(&Config) == ERROR)
    {
        memcpy(_Dst, _Src, _Size);
        return;
    }

	Data_Size_4x_Number = _Size >> 2;//4倍的个数
	if (_Size & 0x03)                //4倍的余数
		memcpy(((unsigned int*)_Dst) + Data_Size_4x_Number, ((unsigned int*)_Src) + Data_Size_4x_Number, _Size & 0x03);
	
 	dma_single_data_parameter_struct DMA_InitStruct =
	{
        .periph_addr            = (unsigned int)_Dst,         //目的基地址
        .memory0_addr           = (unsigned int)_Src,         //源基地址
        .direction              = DMA_MEMORY_TO_MEMORY,       //M->M
        .number                 = Data_Size_4x_Number,        //传输个数
        .circular_mode          = DMA_CIRCULAR_MODE_DISABLE,  //单次模式
        .periph_inc             = DMA_PERIPH_INCREASE_ENABLE, //目的地址递增
        .memory_inc             = DMA_MEMORY_INCREASE_ENABLE, //源地址递增
        .periph_memory_width    = DMA_PERIPH_WIDTH_32BIT,     //目的数据大小为字
        .priority               = DMA_PRIORITY_LOW,           //优先级最低
	};

    Config.Info->State = TRx_TS;
    Config.Info->Mount_Count--;//这里给它手动减去

    dma_single_data_mode_init(Config.Periph, (dma_channel_enum)Config.Stream, &DMA_InitStruct);     //Init Config DMA Stream
    dma_channel_subperipheral_select(Config.Periph, (dma_channel_enum)Config.Stream, (dma_subperipheral_enum )Config.Channel);
    dma_interrupt_enable(Config.Periph, (dma_channel_enum)Config.Stream, DMA_INT_FTF);//Open TC TE DMA Interrupt
    dma_interrupt_enable(Config.Periph, (dma_channel_enum)Config.Stream, DMA_INT_TAE);//Open TC TE DMA Interrupt
    dma_channel_enable(Config.Periph, (dma_channel_enum)Config.Stream);               //Open DMA Channel
}
/*全局常量*/
const WHT_DMA_BSP_t WHT_DMA_BSP =
{
    .WHT_Register            = WHT_DMA_BSP_Register,
    .WHT_Config              = WHT_DMA_BSP_Config,
    .WHT_Get_Curr_Data_Count = WHT_DMA_BSP_Get_Curr_Data_Count,
    .WHT_Close               = WHT_DMA_BSP_Close,
};



typedef struct
{
    unsigned int DMA_IT_TCIFx; //流传输完成中断
    unsigned int DMA_IT_HTIFx; //流传输过半完成中断
    unsigned int DMA_IT_TEIFx; //流传输错误中断
    unsigned int DMA_IT_DMEIFx;//流直接模式错误中断
    unsigned int DMA_IT_FEIFx; //流FIFO错误中断
}WHT_DMA_IT_Event_t;


/*DMA中断回调函数*/
static void WHT_DMA_BSP_DMA_Stream_IRQHandler(WHT_Data_t* dma_stream, const WHT_DMA_IT_Event_t* dma_it_event)
{
    if (dma_interrupt_flag_get(dma_stream->Periph, (dma_channel_enum)dma_stream->Stream, dma_it_event->DMA_IT_HTIFx) == SET)
    {
        dma_stream->Info.State = TRx_HT;
        dma_interrupt_flag_clear(dma_stream->Periph, (dma_channel_enum)dma_stream->Stream, dma_it_event->DMA_IT_HTIFx);
    }
    else if (dma_interrupt_flag_get(dma_stream->Periph, (dma_channel_enum)dma_stream->Stream, dma_it_event->DMA_IT_TCIFx) == SET)
    {
        dma_stream->Info.State = TRx_TC;
        goto GameOver;
    }
    else //错误了
    {
        dma_stream->Info.State = TRx_TE;
        goto GameOver;
    }
    return;
    GameOver:
    //对于向外设发送数据,注意最后一个数据此时在发送过程中
    dma_interrupt_flag_clear(dma_stream->Periph, (dma_channel_enum)dma_stream->Stream, dma_it_event->DMA_IT_TCIFx);
    dma_interrupt_flag_clear(dma_stream->Periph, (dma_channel_enum)dma_stream->Stream, dma_it_event->DMA_IT_TEIFx);
    dma_interrupt_flag_clear(dma_stream->Periph, (dma_channel_enum)dma_stream->Stream, dma_it_event->DMA_IT_DMEIFx);
    dma_interrupt_flag_clear(dma_stream->Periph, (dma_channel_enum)dma_stream->Stream, dma_it_event->DMA_IT_FEIFx);
    dma_channel_disable(dma_stream->Periph, (dma_channel_enum)dma_stream->Stream);

    if (dma_stream->IT_Callback != NULL)
    {
        dma_stream->IT_Callback();
        dma_stream->IT_Callback = NULL;
    }
    dma_stream->Info.BSP_Mutex = RESET;//自解锁
}

/*DMA1数据流0中断服务程序*/
void DMA0_Channel0_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA1_Stream_0, &WHT_DMA_IT_Event);
}
/*DMA1数据流1中断服务程序*/
void DMA0_Channel1_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA1_Stream_1, &WHT_DMA_IT_Event);
}
/*DMA1数据流2中断服务程序*/
void DMA0_Channel2_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA1_Stream_2, &WHT_DMA_IT_Event);
}
/*DMA1数据流3中断服务程序*/
void DMA0_Channel3_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA1_Stream_3, &WHT_DMA_IT_Event);
}
/*DMA1数据流4中断服务程序*/
void DMA0_Channel4_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA1_Stream_4, &WHT_DMA_IT_Event);
}
/*DMA1数据流5中断服务程序*/
void DMA0_Channel5_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA1_Stream_5, &WHT_DMA_IT_Event);
}
/*DMA1数据流6中断服务程序*/
void DMA0_Channel6_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA1_Stream_6, &WHT_DMA_IT_Event);
}
/*DMA1数据流7中断服务程序*/
void DMA0_Channel7_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA1_Stream_7, &WHT_DMA_IT_Event);
}

#if defined(GD32F450) || defined(GD32F470) || defined(GD32F405) || defined(GD32F425) || defined(GD32F407) || defined(GD32F427)
/*DMA2数据流0中断服务程序*/
void DMA1_Channel0_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA2_Stream_0, &WHT_DMA_IT_Event);
}
/*DMA2数据流1中断服务程序*/
void DMA1_Channel1_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA2_Stream_1, &WHT_DMA_IT_Event);
}
/*DMA2数据流2中断服务程序*/
void DMA1_Channel2_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA2_Stream_2, &WHT_DMA_IT_Event);
}
/*DMA2数据流3中断服务程序*/
void DMA1_Channel3_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA2_Stream_3, &WHT_DMA_IT_Event);
}
/*DMA2数据流4中断服务程序*/
void DMA1_Channel4_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA2_Stream_4, &WHT_DMA_IT_Event);
}
/*DMA2数据流5中断服务程序*/
void DMA1_Channel5_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA2_Stream_5, &WHT_DMA_IT_Event);
}
/*DMA2数据流6中断服务程序*/
void DMA1_Channel6_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA2_Stream_6, &WHT_DMA_IT_Event);
}
/*DMA2数据流7中断服务程序*/
void DMA1_Channel7_IRQHandler(void)
{
    static const WHT_DMA_IT_Event_t WHT_DMA_IT_Event =
    {
        .DMA_IT_TCIFx = DMA_INT_FLAG_FTF,
        .DMA_IT_HTIFx = DMA_INT_FLAG_HTF,
        .DMA_IT_TEIFx = DMA_INT_FLAG_TAE,
        .DMA_IT_DMEIFx = DMA_INT_FLAG_SDE,
        .DMA_IT_FEIFx = DMA_INT_FLAG_FEE,
    };
    WHT_DMA_BSP_DMA_Stream_IRQHandler(&WHT_DMA2_Stream_7, &WHT_DMA_IT_Event);
}
#endif
