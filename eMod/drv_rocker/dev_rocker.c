/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           touchpanel driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : dev_rocker.c
* By      : Jerry
* Version : V1.00
*********************************************************************************************************
*/

#include "drv_rocker_i.h"


/*
****************************************************************************************************
*
*             DEV_KEY_Open
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
__hdle DEV_RK_Open(void * open_arg, __u32 mode)
{
	return (__hdle)&(rocker_drv.rk_dev);
}
/*
****************************************************************************************************
*
*             DEV_KEY_Close
*
*  Description:
*       DRV_irkey_MClose
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_RK_Close(__hdle hrocker)
{
	return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DEV_KEY_Read
*
*  Description:
*       DRV_irkey_MRead
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_RK_Read(void *pdata, __u32 size, __u32 n, __hdle hrocker)
{
    return 0;
}
/*
****************************************************************************************************
*
*             DEV_KEY_Write
*
*  Description:
*       DRV_irkey_MWrite
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_RK_Write(const void *pdata, __u32 size, __u32 n, __hdle hrocker)
{
    return size * n;
}
/*
****************************************************************************************************
*
*             DEV_KEY_Ioctrl
*
*  Description:
*       DRV_irkey_MIoctrl
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_RK_Ioctrl(__hdle hrocker, __u32 cmd, __s32 aux, void *pbuffer)
{
    __rocker_dev_t  *hRK = (__rocker_dev_t *)hrocker;
    __s32           ret  = EPDK_OK;
    __s32           cpu_sr;

    switch(cmd)
    {
        case DRV_RK_CMD_REG:
        {
            if(hRK->cb_index >= (RK_CALLBAKC_FUNC_MAX-1))
            {
                __wrn("RK_ERROR: rk callback functions full\n");

                return EPDK_FAIL;
            }
            ENTER_CRITICAL(cpu_sr);
            hRK->cb_index ++;
            hRK->cbr[hRK->cb_index] = (rk_callback)esKRNL_GetCallBack((rk_callback)pbuffer);
            hRK->cb = hRK->cbr[hRK->cb_index];
            EXIT_CRITICAL(cpu_sr);
            __wrn("cb function %d successfull = %x\n", hRK->cb_index, hRK->cbr[hRK->cb_index]);

            break;
        }
        case DRV_RK_CMD_UNREG:
        {
            if(!(hRK->cb_index <= -1))
            {
                hRK->cbr[hRK->cb_index] = hRK->cb_bk;
                hRK->cb_index --;
                if(hRK->cb_index == -1)
                {
                    hRK->cb = hRK->cb_bk;
                }
                else
                {
                    hRK->cb = hRK->cbr[hRK->cb_index];
                }
            }

            break;
        }  

		case DRV_KEY_CMD_SET_ECHO_FLAG:
			g_echo_chk_flag = aux;
			break;
			
		case DRV_KEY_CMD_GET_ECHO_LEVEL:
			ret = g_echo_level;
			break;
		
        default:
        {
            __wrn("Unknown RK Command\n");
            break;
        }
    }

	return ret;
}

__dev_devop_t   rocker_dev_ops =
{
    DEV_RK_Open,
    DEV_RK_Close,
    DEV_RK_Read,
    DEV_RK_Write,
    DEV_RK_Ioctrl
};
