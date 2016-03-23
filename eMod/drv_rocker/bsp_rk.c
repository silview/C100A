/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           ir key driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : bsp_rk.c
* By      : 
* Version : V1.00
*********************************************************************************************************
*/

#include "epdk.h"
#include "bsp_rk.h"

#ifndef __REG
    #define __REG(x)    (*(volatile unsigned int *)(x))
#endif

#define  RKC_REGS_BASE                0xF1C24800
#define  RK_REG_o_CTRL                0x00
#define  RK_REG_o_INT                 0x10
#define  RK_REG_o_FIFOC               0x14
#define  RK_REG_o_DATA                0x18

#define  RK_READ_REG(Nb)              __REG(RKC_REGS_BASE + (Nb))
#define  RK_WRITE_REG(Nb, value)      (__REG(RKC_REGS_BASE + (Nb)) = (value))

static   __u32      save_ctrl;
static   __u32      save_int;
static   __u32      save_fifo;


__s32   RK_Init(void)
{
    __u32       reg_value;
    
	// 初始化REG_CTRL
	reg_value = RK_READ_REG(RK_REG_o_CTRL);
	save_ctrl = reg_value;
	reg_value = reg_value & ~(0x01 << 0);       // set tp disable
	reg_value = reg_value | (0x01 << 1);        // set ADC enable
	reg_value = reg_value | (0x01 << 2);        // set ADC select
	reg_value = reg_value | (0x04 << 3);        // set 4 channel select robin round
	reg_value = reg_value | (0x0f << 13);       // set CLK_IN/32
	reg_value = reg_value | (0x01 << 17);       // HOSC(24MHz)
	reg_value = reg_value & ~(0x01 << 20);      // ADC CLK_IN (CLK/2)

	RK_WRITE_REG(RK_REG_o_CTRL, reg_value);
	
	// 初始化REG_INT
	reg_value = RK_READ_REG(RK_REG_o_INT);
	save_int  = reg_value;
	reg_value = reg_value & ~(RK_DATA_IRQ | RK_LTOUCH_IRQ | RK_FTOUCH_IRQ | RK_FIFO_OV_IRQ);  // disable int
	RK_WRITE_REG(RK_REG_o_INT, reg_value);

	// 初始化REG_FIFOC
	reg_value = RK_READ_REG(RK_REG_o_FIFOC);
	save_fifo = reg_value;
	reg_value = reg_value & (0x01 << 0);        // FIRST Touch IRQ DISABLE
	reg_value = reg_value & (0x01 << 1);        // LAST Touch IRQ DISABLE
	reg_value = reg_value | (0x01 << 4);        // FIFO DATA IRQ ENABLE
	reg_value = reg_value | (0x01 << 5);        // FIFO OVER RUN IRQ ENABLE
	RK_WRITE_REG(RK_REG_o_FIFOC, reg_value);
	
	return EPDK_OK;
}

__s32   RK_Exit(void)
{
	RK_WRITE_REG(RK_REG_o_CTRL , save_ctrl);
	RK_WRITE_REG(RK_REG_o_INT  , save_int);
	RK_WRITE_REG(RK_REG_o_FIFOC, save_fifo);
    
    return EPDK_OK;
}

void    RK_ClearFifo(void)
{
    __s32       reg_value;
    
	reg_value = RK_READ_REG(RK_REG_o_INT);
	reg_value = reg_value | (0x01 << 4);        // clear fifo
	RK_WRITE_REG(RK_REG_o_INT, reg_value);
}

__s32   RK_EnableInt(__u32 int_no)
{
    __s32       reg_value;
    
	reg_value = RK_READ_REG(RK_REG_o_INT);
	reg_value = reg_value | int_no;
	RK_WRITE_REG(RK_REG_o_INT, reg_value);
    
    return EPDK_OK;
}

__s32   RK_DisableInt(__u32 int_no)
{
    __s32       reg_value;
    
	reg_value = RK_READ_REG(RK_REG_o_INT);
	reg_value = reg_value & (~int_no);
	RK_WRITE_REG(RK_REG_o_INT, reg_value);

    return EPDK_OK;
}

__s32   RK_GetData(__s32 *data, __u32 data_cnt)
{
    __s32       i;

    for(i = 0; i < data_cnt; i++)
    {
        data[i] = RK_READ_REG(RK_REG_o_DATA) & 0x7ff;   // 读取数据  

        if(data[i] < 2000)
        {
            //__msg("data[%d]=%d\n",i, data[i]);
        }
    }
    
    return EPDK_OK;
}

__s32   RK_QueryIntPending(void)
{
    __s32       ret;
    
    ret = RK_READ_REG(RK_REG_o_FIFOC);
	
	return ret;
}

void    RK_ClearIntPending(__s32 pending)
{
    RK_WRITE_REG(RK_REG_o_FIFOC, pending);
}

__s32   RK_SetSampleRate(__rk_sample_rate_e rate)
{
    __s32       reg_value;
    
    switch(rate)
    {
        case RK_SAMPLE_RATE_01K:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL);
            reg_value = reg_value & ~(0x0f << 13);
            reg_value = reg_value | (0x05 << 13);
            RK_WRITE_REG(RK_REG_o_CTRL, reg_value);
            
            break;
        }
        case RK_SAMPLE_RATE_02K:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL);
            reg_value = reg_value & ~(0x0f << 13);
            reg_value = reg_value | (0x06 << 13);
            RK_WRITE_REG(RK_REG_o_CTRL, reg_value);
            
            break;
        }
        case RK_SAMPLE_RATE_04K:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL);
            reg_value = reg_value & ~(0x0f << 13);
            reg_value = reg_value | (0x07 << 13);
            RK_WRITE_REG(RK_REG_o_CTRL, reg_value);
            
            break;
        }
        case RK_SAMPLE_RATE_08K:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL);
            reg_value = reg_value & ~(0x0f << 13);
            reg_value = reg_value | (0x08 << 13);
            RK_WRITE_REG(RK_REG_o_CTRL, reg_value);
            
            break;
        }
        case RK_SAMPLE_RATE_16K:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL);
            reg_value = reg_value & ~(0x0f << 13);
            reg_value = reg_value | (0x09 << 13);
            RK_WRITE_REG(RK_REG_o_CTRL, reg_value);
            
            break;
        }
        default:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL);
            reg_value = reg_value & ~(0x0f << 13);
            reg_value = reg_value | (0x07 << 13);
            RK_WRITE_REG(RK_REG_o_CTRL, reg_value);
            
            break;
        }
    }
    
    return EPDK_OK;
}

__s32   RK_SetFifoDepth(__rk_fifo_depth_e depth)
{
    __s32       reg_value;
    
    switch(depth)
    {
        case RK_FIFO_DEPTH_04:
        {
            reg_value = RK_READ_REG(RK_REG_o_FIFOC);
            reg_value = reg_value & ~(0x3f << 7);
            reg_value = reg_value | (0x01 << 2);
            RK_WRITE_REG(RK_REG_o_FIFOC, reg_value);
            
            break;
        }
        case RK_FIFO_DEPTH_08:
        {
            reg_value = RK_READ_REG(RK_REG_o_FIFOC);
            reg_value = reg_value & ~(0x3f << 7);
            reg_value = reg_value | (0x01 << 3);
            RK_WRITE_REG(RK_REG_o_FIFOC, reg_value);
            
            break;
        }
        case RK_FIFO_DEPTH_16:
        {
            reg_value = RK_READ_REG(RK_REG_o_FIFOC);
            reg_value = reg_value & ~(0x3f << 7);
            reg_value = reg_value | (0x01 << 4);
            RK_WRITE_REG(RK_REG_o_FIFOC, reg_value);
            
            break;
        }
        case RK_FIFO_DEPTH_32:
        {
            reg_value = RK_READ_REG(RK_REG_o_FIFOC);
            reg_value = reg_value & ~(0x3f << 7);
            reg_value = reg_value | (0x01 << 5);
            RK_WRITE_REG(RK_REG_o_FIFOC, reg_value);
            
            break;
        }
        default:
        {
            reg_value = RK_READ_REG(RK_REG_o_FIFOC);
            reg_value = reg_value & ~(0x3f << 7);
            reg_value = reg_value | (0x01 << 2);
            RK_WRITE_REG(RK_REG_o_FIFOC, reg_value);
            
            break;
        }
    }
    
    return EPDK_OK;
}

__s32   RK_SelectChannel(__rk_channel_e channel)
{
    __u32       reg_value;

    switch(channel)
    {
        case RK_CHANNEL_X1:
        {
        	reg_value = RK_READ_REG(RK_REG_o_CTRL);
        	reg_value = reg_value & ~(0x07 << 3);
        	reg_value = reg_value | (0x00 << 3);        // select x1 channel
        	RK_WRITE_REG(RK_REG_o_CTRL, reg_value);
              esKRNL_TimeDly(2);
            
            break;
        }
        case RK_CHANNEL_X2:
        {
        	reg_value = RK_READ_REG(RK_REG_o_CTRL);
        	reg_value = reg_value & ~(0x07 << 3);
        	reg_value = reg_value | (0x01 << 3);        // select x2 channel
        	RK_WRITE_REG(RK_REG_o_CTRL, reg_value);
              esKRNL_TimeDly(2);

           
            break;
        }
        case RK_CHANNEL_Y1:
        {
        	reg_value = RK_READ_REG(RK_REG_o_CTRL);
        	reg_value = reg_value & ~(0x07 << 3);
        	reg_value = reg_value | (0x02 << 3);        // select y1 channel
        	RK_WRITE_REG(RK_REG_o_CTRL, reg_value);
			 esKRNL_TimeDly(2);
            //__inf("rk_channel_y1-----------\n");
            break;
        }
        case RK_CHANNEL_Y2:
        {
        	reg_value = RK_READ_REG(RK_REG_o_CTRL);
        	reg_value = reg_value & ~(0x07 << 3);
        	reg_value = reg_value | (0x03 << 3);        // select y2 channel
        	RK_WRITE_REG(RK_REG_o_CTRL, reg_value);
			 esKRNL_TimeDly(2);
           // __inf("rk_channel_y2-----------\n");
            break;
        }
        default:
        {
            break;
        }
    }
    
    return EPDK_OK;
}

