/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               hello world sample
*
*                                (c) Copyright 2006-2007, Steven.ZGJ China
*                                            All    Rights Reserved
*
* File    : drv_tv.h
* By      : jerry
* Version : V1.00
* Date    : 2009-5
*********************************************************************************************************
*/
#ifndef  _DRV_TV_H_
#define  _DRV_TV_H_


typedef enum  __DRV_TV_CMD_SET
{
    DRV_TV_CMD_NONE              = 0,
    DRV_TV_CMD_INIT                 ,
    DRV_TV_CMD_EXIT                 ,
    DRV_TV_CMD_SEARCH_START_INIT,///CAI_MODIFY_FOR_C500_ATV
    DRV_TV_CMD_AUTO_SEARCH,
    DRV_TV_CMD_AUTO_SEARCH_END,
    DRV_TV_CMD_SET_FREQ		,
    DRV_TV_CMD_SET_TV_SYSTEM		,
    DRV_TV_CMD_GET_TV_SYSTEM,
    DRV_TV_CMD_I2C_TEST		,
	DRV_TV_CMD_SLEEP_CTRL,

}__drv_tv_cmd_set_t;



#endif   /*_DRV_TV_H_*/
