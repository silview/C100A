/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									          ir keyboard driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : drv_rocker_i.h
* By      : jerry
* Version : V1.00
*********************************************************************************************************
*/
#ifndef	_DRV_ROCKER_I_H_
#define	_DRV_ROCKER_I_H_

#include "epdk.h"
#include "bsp_rk.h"
#include "drv_rocker.h"
#include "bitops.h"


#define   RK_CALLBAKC_FUNC_MAX       (16)

/* key message */
#define DRV1_KEY_LEFT          KMSG_USR_KEY_LEFT
#define DRV1_KEY_RIGHT         KMSG_USR_KEY_RIGHT
#define DRV1_KEY_UP            KMSG_USR_KEY_UP
#define DRV1_KEY_DOWN          KMSG_USR_KEY_DOWN


typedef __s32  (*rk_callback)(void *buff);

typedef struct STRUCT_ROCKER_DEV
{
    __u32           mid;
	__hdle			hReg;
	__u32           main_tid;
	__s32           cb_index;    //总共函数id
	rk_callback     cb;
    rk_callback     cbr[RK_CALLBAKC_FUNC_MAX];         //向驱动注册的回调函数，仅限注册一次
    rk_callback     cb_bk;
}__rocker_dev_t;

typedef struct STRUCT_ROCKER_DRV
{
    __u32            mid;
    __u8             used;
    __u8             ndev;
    __rocker_dev_t   rk_dev;
    __input_dev_t *  p_input_dev;
}__rocker_drv_t;

//此数据结构保留
typedef struct RK_PRIVATE_DATA_SET
{
    __u8              data_tid;           //采集数据线程的id
    __krnl_event_t   *data_sem;           //用于通知数据采集已经ok
    
    __s32             crt_x[16];           //当前的xAD
    __s32             crt_y[16];           //当前的yAD
    __s32             temp_x[4];
    __s32             temp_y[4];
   __s32             times;
}__rk_private_data_set_t;

extern  __rocker_drv_t              rocker_drv;
extern  __dev_devop_t               rocker_dev_ops;
extern  __rk_private_data_set_t     rk_private;

extern __s32 g_echo_level;
extern __s32 g_echo_chk_flag;

extern  __s32  rocker_init(void);
extern  __s32  rocker_exit(void);

__s32 DRV_RK_MInit(void);
__s32 DRV_RK_MExit(void);
__mp* DRV_RK_MOpen(__u32 mid, __u32 mode);
__s32 DRV_RK_MClose(__mp *mp);
__u32 DRV_RK_MRead(void *pdata, __u32 size, __u32 n, __mp *mp);
__u32 DRV_RK_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
__s32 DRV_RK_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);

#endif	/* _DRV_ROCKER_I_H_ */
