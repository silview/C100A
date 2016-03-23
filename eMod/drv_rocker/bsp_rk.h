/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           ir key driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : bsp_rk.h
* By      : 
* Version : V1.00
*********************************************************************************************************
*/

#ifndef __BSP_RK_H__
#define __BSP_RK_H__

/* interrupt control */
#define RK_DATA_IRQ           (0x01 << 5)
#define RK_LTOUCH_IRQ         (0x01 << 1)
#define RK_FTOUCH_IRQ         (0x01 << 0)
#define RK_FIFO_OV_IRQ        (0x01 << 6)


/* interrupt status */
#define RK_FTOUCH_PENDING      (0x1 << 0)
#define RK_LTOUCH_PENDING      (0x1 << 1)
#define RK_DATA_PENDING        (0x1 << 5)
#define RK_FIFO_OV_PENDING     (0x1 << 6)

typedef enum 
{
    RK_FIFO_DEPTH_04,
    RK_FIFO_DEPTH_08,
    RK_FIFO_DEPTH_16,
    RK_FIFO_DEPTH_32,
}__rk_fifo_depth_e;

typedef enum 
{
    RK_SAMPLE_RATE_01K,
    RK_SAMPLE_RATE_02K,
    RK_SAMPLE_RATE_04K,
    RK_SAMPLE_RATE_08K,
    RK_SAMPLE_RATE_16K,
}__rk_sample_rate_e;

typedef enum
{
    RK_CHANNEL_X1,
    RK_CHANNEL_X2,
    RK_CHANNEL_Y1,
    RK_CHANNEL_Y2,
}__rk_channel_e;

__s32   RK_Init(void);

__s32   RK_Exit(void);

void    RK_ClearFifo(void);

__s32   RK_EnableInt(__u32 int_no);

__s32   RK_DisableInt(__u32 int_no);

__s32   RK_GetData(__s32 *data, __u32 data_cnt);

__s32   RK_QueryIntPending(void);

void    RK_ClearIntPending(__s32 pending);

__s32   RK_SetSampleRate(__rk_sample_rate_e rate);

__s32   RK_SetFifoDepth(__rk_fifo_depth_e depth);

__s32   RK_SelectChannel(__rk_channel_e channel);

#endif /* #ifndef __BSP_RK_H__ */

