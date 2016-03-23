/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               hello world sample
*
*                                (c) Copyright 2006-2007, Steven.ZGJ China
*                                            All    Rights Reserved
*
* File    : drv_fm.h
* By      : jerry
* Version : V1.00
* Date    : 2009-5
*********************************************************************************************************
*/
#ifndef  _DRV_MCU_H_
#define  _DRV_MCU_H_


typedef enum  __DRV_MCU_CMD_SET
{
    DRV_MCU_CMD_NONE              = 0,
    DRV_MCU_CMD_INIT                 ,
    DRV_MCU_CMD_EXIT                 ,

    DRV_MCU_CMD_CHECK          ,
    DRV_MCU_CMD_CHECK_HXN

}__drv_mcu_cmd_set_t;



#endif   /*_DRV_FM_H_*/
