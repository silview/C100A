/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : drv_irkey.c
* By      : jerry
* Version : V1.00
*********************************************************************************************************
*/
#include "drv_key_i.h"
#include "keyc_bitops.h"

__key_drv_t    keydrv;
__key_dev_t    keydev;

extern LRADC_KEY_SCANCODE *solution_keymapping;
extern __dev_devop_t keyc_dev_ops;
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
__s32 DRV_KEYC_MInit(void)
{
	keydrv.used = 0;
	eLIBs_printf("line=%d file=%s\n", __LINE__, __FILE__);
	Lradc_key_Init();
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
__s32 DRV_KEYC_MExit(void)
{
    keydrv.used = 0;
    Lradc_key_Exit();
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
__mp *DRV_KEYC_MOpen(__u32 mid, __u32 mod)
{
    __s32 cpu_sr;

    ENTER_CRITICAL(cpu_sr);
    if(keydrv.used)
    {
        __wrn("driver already openned by someone else!\n");
        EXIT_CRITICAL(cpu_sr);
        return (__mp *)0;
    }
    keydrv.used = 1;
    EXIT_CRITICAL(cpu_sr);

    keydrv.mid = mid;
	return (__mp *)&keydrv;
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
__s32 DRV_KEYC_MClose(__mp *mp)
{
    __key_drv_t * drv = (__key_drv_t *)mp;
    if(drv->used == 0)
    {
        return EPDK_FAIL;
    }
    drv->used = 0;

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
__u32 DRV_KEYC_MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
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
__u32 DRV_KEYC_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
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
__s32 DRV_KEYC_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32 cpu_sr = 0;
	//eDbug(" srtart key ioctl\n");
    switch(cmd)
    {
		case DRV_CMD_PLUGIN:
		{
		    __u32 keyCodeNum = 0;
		    __u32 i = 0;
		    __input_dev_t   *pKeycDev = NULL;
			
			//eDbug(" plugin key ioctl\n");
			
		    ENTER_CRITICAL(cpu_sr);
		    if(keydrv.status)
		    {
		        __wrn("Driver cannot support more than one key board!\n");
		        EXIT_CRITICAL(cpu_sr);
		        return EPDK_FAIL;
		    }
		    keydrv.status = 1;
		    EXIT_CRITICAL(cpu_sr);

		    pKeycDev = (__input_dev_t *)eLIBs_malloc(sizeof(__input_dev_t));
		    if(NULL == pKeycDev)
		    {
                __wrn("allocate memory for input device hanle fail!\n");
                return EPDK_FAIL;
		    }
		    eLIBs_memset(pKeycDev, 0, sizeof(__input_dev_t));
		    keydev.used = 0;/* for device manager */

            /* general managment system information */
		    pKeycDev->classname = INPUT_CLASS_NAME;
		    pKeycDev->name      = LRADC_KEY_DEV_NAME;
		    pKeycDev->ops       = &keyc_dev_ops;
		    pKeycDev->parg      = &keydev;

		    pKeycDev->id.product = 0x0001;
		    pKeycDev->id.version = 0x0001;

            /* KEYBOARD DEVICE */
		    pKeycDev->device_class = INPUT_KEYBOARD_CLASS;
            /* set keyboard support event types */
		    pKeycDev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);

            /* set keyboard support event codes */
            keyCodeNum = KEY_CODE_TABLE_SIZE;
            for(i = 0; i < keyCodeNum; i++)
            {
                __set_bit(solution_keymapping[i].keyCode, pKeycDev->keybit);
            }
		    /* set key event handler, handle the feedback events */
		    pKeycDev->event = drv_keyc_event;

		    /* register keyboard device to INPUT managemant system */
		    if(esINPUT_RegDev(pKeycDev) != EPDK_OK)
		    {
                __wrn("register keyboard device to input system failed\n");
                eLIBs_free(pKeycDev);
	        	return EPDK_FAIL;
		    }
		    /* attach pKeycDev to keydrv */
		    keydrv.pKeyc_dev = pKeycDev;

		    __inf("keyc device initialize succeeded!\n");
		    return EPDK_OK;

		}
		case DRV_CMD_PLUGOUT:
		{
			//eDbug(" plugout key ioctl\n");
		    /* unregister keyboard deivce from INPUT manage system,
             * free keyboard device handle.
             */
		    if (keydev.used)
            {
                __wrn("keyc is using now\n");
                return EPDK_FAIL;
            }
	        esINPUT_UnregDev(keydrv.pKeyc_dev);
	        eLIBs_free(keydrv.pKeyc_dev);
            return EPDK_OK;

		}
		case DRV_CMD_GET_STATUS:
		{
			//eDbug(" get status key ioctl\n");
		    if(keydrv.status)
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

/*
*********************************************************************************************************
*                                       KEYBOARD FEEDBACK EVENT
*
* Description:  deal with keyboard feedback event.
*
* Arguments  : dev      handle of keyboard device;
*              type     type of event;
               code     code of event;
               value    value of event;
*
* Returns    : keyboard device feedback event handle result;
*              EPDK_OK     -   handle successed.
*              EPDK_FAIL   -   handle failed.
*********************************************************************************************************
*/
__s32 drv_keyc_event(__input_dev_t *dev, __u32 type, __u32 code, __s32 value)
{
    __msg("keyborad feedback event [type:%d, code:%d, value:%d]\n",
                type, code, value);
 	return EPDK_OK;
}

