/*
*********************************************************************************************************
*											        eMOD
*						                    the Easy Module System
*                                          drv_touchpanel sub-system
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File   : drv_rocker.h
* Version: V1.0
* By     : steven.zgj
*********************************************************************************************************
*/
#ifndef _DRV_ROCKER_H_
#define _DRV_ROCKER_H_

typedef struct __EV_RK_MSG
{
    __s32       x_value;
    __s32       y_value;
}__ev_rk_msg_t;

typedef enum __DRV_ROCKER_CMD
{
    DRV_RK_CMD_NULL       =0,
    DRV_RK_CMD_REG          ,          //挂载回调函数，如果前面存在回调函数，会自动替换前一个回调函数
    DRV_RK_CMD_UNREG        ,          //卸载回调函数，卸载后，如果前面存在回调函数，会自动使用前一个回调函数
    DRV_RK_CMD_

}__drv_rocker_cmd_t;


#endif  /*_DRV_ROCKER_H_*/
