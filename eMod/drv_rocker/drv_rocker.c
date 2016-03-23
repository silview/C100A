/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : drv_rocker.c
* By      : jerry
* Version : V1.00
*********************************************************************************************************
*/
#include "drv_rocker_i.h"
/*#define DebugPrintf(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)		)
#define __msg(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
									 eLIBs_printf(__VA_ARGS__)		)
#define __here__			eLIBs_printf("@L%d(%s)\n", __LINE__, __FILE__);

*/
__rocker_drv_t          rocker_drv;

static __u32 RockerKeyDevCodes[] = 
{ 
	KPAD_MOVIE,
	KPAD_MUSIC,
	KPAD_RECORD,
	KPAD_PHOTO,
	KPAD_FM,	
	KPAD_SCR_ONOFF,
	KPAD_MUTE,

	KPAD_LEFT,
	KPAD_MENU,
	KPAD_RIGHT,
	KPAD_ENTER,
	KPAD_RETURN,
	KPAD_VOICEDOWN,
	KPAD_VOICEUP,

	KPAD_NUM0,
	KPAD_NUM1,
	KPAD_NUM2,
	KPAD_NUM3,
	KPAD_NUM4, 
	KPAD_NUM5,
	KPAD_NUM6,
	KPAD_NUM7,
	KPAD_NUM8,
	KPAD_NUM9,
	
};

/*
****************************************************************************************************
*
*             DRV_IRKEY_MInit
*
*  Description:
*       DRV_IRKEY_MInit
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_RK_MInit(void)
{
    __wrn("RK MInit\n");
	rocker_drv.used = 0;
    return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DRV_IRKEY_MExit
*
*  Description:
*       DRV_IRKEY_MExit
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_RK_MExit(void)
{
    __wrn("RK MExit\n");
    rocker_drv.used = 0;
    return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DRV_IRKEY_MOpen
*
*  Description:
*       DRV_MOpen
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__mp *DRV_RK_MOpen(__u32 mid, __u32 mod)
{
    __wrn("RK MOpen = %d\n", mid);
    rocker_drv.mid = mid;
	return (__mp *)&rocker_drv;
}
/*
****************************************************************************************************
*
*             DRV_IRKEY_MClose
*
*  Description:
*       DRV_IRKEY_MClose
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_RK_MClose(__mp *mp)
{
    __wrn("RK MClose\n");
	return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DRV_IRKEY_MRead
*
*  Description:
*       DRV_IRKEY_MRead
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DRV_RK_MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}
/*
****************************************************************************************************
*
*             DRV_IRKEY_MWrite
*
*  Description:
*       DRV_IRKEY_MWrite
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DRV_RK_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}

__s32 RockerKeyDevInit(void)
{
    /* 分配电源键输入设备句柄，并向输入子系统注册.*/
    __s32               i;
    __u32               KeyCodeNum;
    __input_dev_t *     p_input_dev = NULL;
    p_input_dev = (__input_dev_t *)eLIBs_malloc(sizeof(__input_dev_t));
    if (p_input_dev == NULL)
    {
    	__msg("not enough memory for INPUT physical device handle\n");
    	return EPDK_FAIL;
    }
    eLIBs_memset(p_input_dev, 0, sizeof(__input_dev_t));
    __here__;
    /* 用于向设备管理系统注册一个管理设备节点 */
    p_input_dev->classname = INPUT_CLASS_NAME;
    p_input_dev->name      = "RockerKey";
    p_input_dev->ops       = &rocker_dev_ops;
    p_input_dev->parg      = NULL;
    
    p_input_dev->id.product = 0x0001;
    p_input_dev->id.version = 0x0001;
    
    /* 属于按键类设备 */
    p_input_dev->device_class = INPUT_KEYBOARD_CLASS;
    __here__;

    p_input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);
    
    /* 设置ADC设备支持的按键事件 */
    KeyCodeNum = sizeof(RockerKeyDevCodes) / sizeof(RockerKeyDevCodes[0]);
    for (i = 0; i < KeyCodeNum; i++)
    {
    	__set_bit(RockerKeyDevCodes[i], p_input_dev->keybit);
    }
    
    /* 设备电源键设备的事件反馈回掉函数为NULL,不支持事件的回馈 */
    p_input_dev->event = NULL;
    __here__;
    /* 向输入子系统注册电源键输入设备 */
    if (esINPUT_RegDev(p_input_dev) != EPDK_OK)
    {
        __msg("register powerkey device to INPUT system failed\n");
        eLIBs_free(p_input_dev);
    	return EPDK_FAIL;
    }
    rocker_drv.p_input_dev = p_input_dev;    
    __msg("PowerKey device initialize succeeded\n");
    return EPDK_OK;
}

void PockerKeyDevExit(void)
{
    if (rocker_drv.p_input_dev)
    {
        /* 注销设备并释放内存 */
        esINPUT_UnregDev(rocker_drv.p_input_dev);
        eLIBs_free(rocker_drv.p_input_dev);
    }
}
/*
****************************************************************************************************
*
*             DRV_IRKEY_MIoctrl
*
*  Description:
*       DRV_IRKEY_MIoctrl
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_RK_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32 cpu_sr = 0;

    switch(cmd)
    {
		case DRV_CMD_PLUGIN:
		{
		    if(rocker_init() == EPDK_FAIL)
		    {
		        return EPDK_FAIL;
		    }
            __here__;
		    ENTER_CRITICAL(cpu_sr);
		    if(rocker_drv.ndev)
		    {
		        __msg("Driver cannot support more than one tp board!\n");
		    }
		    rocker_drv.ndev = 1;
		    EXIT_CRITICAL(cpu_sr);
            __here__;
		    rocker_drv.rk_dev.hReg = esDEV_DevReg("HID", "RK", &rocker_dev_ops, 0);
		    __here__;
		__msg("11111111111111111111111\n");
            if(!rocker_drv.rk_dev.hReg)
            {
            	__msg("touch panel registered Error!\n");
            	rocker_exit();

            	return EPDK_FAIL;
            }
            __here__;
            ++ rocker_drv.used;
            RockerKeyDevInit();
            __msg("RK Plugin OK\n");

            return EPDK_OK;
		}
		case DRV_CMD_PLUGOUT:
		{
            PockerKeyDevExit();
		    rocker_exit();
		    if(rocker_drv.used)
		    {
		        -- rocker_drv.used;
		    }
		    if(!rocker_drv.rk_dev.hReg)
		    {
		        __wrn("Dev not plugin!\n");
		        return EPDK_FAIL;
		    }

		    esDEV_DevUnreg(rocker_drv.rk_dev.hReg);
		    rocker_drv.ndev = 0;

		    __wrn("RK Plugout OK\n");

            return EPDK_OK;
		}
		case DRV_CMD_GET_STATUS:
		{
		    if(rocker_drv.ndev)
		    {
		        return DRV_STA_BUSY;
		    }
		    else
		    {
		        return DRV_STA_FREE;
		    }
		}
		default:
			break;
	}

	return EPDK_FAIL;
}
