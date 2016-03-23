/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               hello world sample
*
*                                (c) Copyright 2006-2007, Steven.ZGJ China
*                                            All    Rights Reserved
*
* File    : drv_tcon.h
* By      : jerry
* Version : V1.00
* Date    : 2009-5
*********************************************************************************************************
*/
#ifndef  _DRV_TCON_H_
#define  _DRV_TCON_H_

#define  TCON_SEARCH_CHN_MIN_FREQ         87000
#define  TCON_SEARCH_CHN_MAX_FREQ        108000

#define  TCON_SEARCH_JAP_MIN_FREQ         76000
#define  TCON_SEARCH_JAP_MAX_FREQ         91000

#define  TCON_SEARCH_STEP                   100

#define  TCON_DRV_STATUS_RECEIVE         0x8000
#define  TCON_DRV_STATUS_SEND            0x8001

typedef enum  __DRV_TCON_CMD_SET
{
    DRV_TCON_CMD_NONE              = 0,
    DRV_TCON_CMD_INIT                 ,
    DRV_TCON_CMD_EXIT                 ,
    DRV_TCON_CMD_AUTO_SEARCH		,

    DRV_TCON_CMD_PAD_TYPE_SETUP,
    

    DRV_TCON_CMD_QUERY_STATUS,
    DRV_TCON_CMD_WRITE_TEST,
    DRV_TCON_CMD_POWERON_INIT,

	DRV_TCON_CMD_SET_OSD_LANG,
	DRV_TCON_CMD_OSD_OFF,
	DRV_TCON_CMD_SHOW_ATV_SCH_MENU,
	DRV_TCON_CMD_SET_BLUE_SCREEN


}__drv_tcon_cmd_set_t;



#endif   /*_DRV_TCON_H_*/
