/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                             audio driver Module
*
*                                    (c) Copyright 2008-2009, Jerry China
*                                             All Rights Reserved
*
* File    : lradc_i.h
* By      : Jerry
* Version : V1.0
* Date    : 2009-4-13
*********************************************************************************************************
*/

#ifndef  _LRADC_K_H_
#define  _LRADC_K_H_


#define LRADC_IRQ_NUM           (22)//lradc模块中断号

#define LRADC_NOTUSED_ADC01     (0x0)//不使用任何一路lradc
#define LRADC_USED_ADC0         (0x1)//使用lradc0
#define LRADC_USED_ADC1         (0x2)//使用lradc1

extern  __s32  Lradc_key_Init(void);
extern  __s32  Lradc_key_Exit(void);

#endif  /*_LRADC_K_I_H_*/
