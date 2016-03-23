/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                             audio driver Module
*
*                                    (c) Copyright 2009-2010, kevin China
*                                             All Rights Reserved
*
* File    : ir_remote.h
* By      : victor
* Version : V1.0
* Date    : 2010-12-31
*********************************************************************************************************
*/
#ifndef  _IR_REMOTE_H_
#define  _IR_REMOTE_H_


#define IR_IRQ_NUM              (6)//ir模块中断号
#define IR_KEY_CODE_TABLE_SIZE   256

/* NEC remoter user code ,defined by user */
#define NEC_IR_ADDR_CODE1   (0x7f80) /* 银色学习型的红外遥控  */
#define NEC_IR_ADDR_CODE2   (0xfe01) /* 黑色pvp方案的红外遥控 */
#define NEC_IR_ADDR_CODE3   (0xff00) 
#define NEC_IR_ADDR_CODE4  (0xbf00)	//平安行

extern  __s32  Ir_key_Init(void);
extern  __s32  Ir_key_Exit(void);

#endif  /*_IR_REMOTE_H_*/

