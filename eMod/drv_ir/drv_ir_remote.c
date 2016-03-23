/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                             audio driver Module
*
*                                    (c) Copyright 2009-2010, kevin China
*                                             All Rights Reserved
*
* File    : drv_ir_remote.c
* By      : victor
* Version : V1.0
* Date    : 2010-12-31
*********************************************************************************************************
*/

#include "drv_ir_remote_i.h"
#include "irkey_bitops.h"

extern __u32 *ir_solution_keymapping;

__ir_key_drv_t    irkey_drv;

extern __dev_devop_t irkey_dev_ops;

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
__s32 DRV_IR_KEY_MInit(void)
{
	irkey_drv.used = 0;

  return Ir_key_Init();
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
__s32 DRV_IR_KEY_MExit(void)
{
    irkey_drv.used = 0;
    Ir_key_Exit();
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
__mp *DRV_IR_KEY_MOpen(__u32 mid, __u32 mod)
{
    __s32 cpu_sr;

    ENTER_CRITICAL(cpu_sr);
    if(irkey_drv.used)
    {
        __wrn("ir key driver already openned by someone else!\n");
        EXIT_CRITICAL(cpu_sr);
        return (__mp *)0;
    }
    irkey_drv.used = 1;
    EXIT_CRITICAL(cpu_sr);

    irkey_drv.mid = mid;
	return (__mp *)&irkey_drv;
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
__s32 DRV_IR_KEY_MClose(__mp *mp)
{
    __ir_key_drv_t * drv = (__ir_key_drv_t *)mp;
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
__u32 DRV_IR_KEY_MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
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
__u32 DRV_IR_KEY_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
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
__s32 DRV_IR_KEY_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32 cpu_sr = 0;

    switch(cmd)
    {
		case DRV_CMD_PLUGIN:
		{
		    __u32 i = 0;
		    __input_dev_t   *pIrkey_Dev = NULL;

		    ENTER_CRITICAL(cpu_sr);
		    if(irkey_drv.status)
		    {
		        __wrn("Driver cannot support more than one ir key board!\n");
		        EXIT_CRITICAL(cpu_sr);
		        return EPDK_FAIL;
		    }
		    irkey_drv.status = 1;
		    EXIT_CRITICAL(cpu_sr);

		    pIrkey_Dev = (__input_dev_t *)eLIBs_malloc(sizeof(__input_dev_t));
		    if(NULL == pIrkey_Dev)
		    {
                __wrn("allocate memory for input device hanle fail!\n");
                return EPDK_FAIL;
		    }
		    eLIBs_memset(pIrkey_Dev, 0, sizeof(__input_dev_t));
		    irkey_drv.ir_key_dev.used = 0;/* for device manager */

            /* general managment system information */
		    pIrkey_Dev->classname = INPUT_CLASS_NAME;
		    pIrkey_Dev->name      = IR_KEY_DEV_NAME;
		    pIrkey_Dev->ops       = &irkey_dev_ops;
		    pIrkey_Dev->parg      = &(irkey_drv.ir_key_dev);

		    pIrkey_Dev->id.product = 0x0001;
		    pIrkey_Dev->id.version = 0x0001;

            /* KEYBOARD DEVICE */
		    pIrkey_Dev->device_class = INPUT_KEYBOARD_CLASS;
            /* set keyboard support event types */
		    pIrkey_Dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);

            /* set keyboard support event codes */
            for(i = 0; i < IR_KEY_CODE_TABLE_SIZE; i++)
            {
                __set_bit(ir_solution_keymapping[i], pIrkey_Dev->keybit);
            }
		    /* set key event handler, handle the feedback events */
		    pIrkey_Dev->event = drv_ir_key_event;

		    /* register keyboard device to INPUT managemant system */
		    if(esINPUT_RegDev(pIrkey_Dev) != EPDK_OK)
		    {
                __wrn("register ir keyboard device to input system failed\n");
                eLIBs_free(pIrkey_Dev);
	        	return EPDK_FAIL;
		    }
		    /* attach pIR_KEYDev to keydrv */
		    irkey_drv.pIrKey_dev = pIrkey_Dev;

		    __inf("IR_KEY device initialize succeeded!\n");
		    return EPDK_OK;

		}
		case DRV_CMD_PLUGOUT:
		{
		    /* unregister keyboard deivce from INPUT manage system,
             * free keyboard device handle.
             */
		    if (irkey_drv.used)
            {
                __wrn("IR_KEY is using now\n");
                return EPDK_FAIL;
            }
	        esINPUT_UnregDev(irkey_drv.pIrKey_dev);
	        eLIBs_free(irkey_drv.pIrKey_dev);
            return EPDK_OK;

		}
		case DRV_CMD_GET_STATUS:
		{
		    if(irkey_drv.status)
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
__s32 drv_ir_key_event(__input_dev_t *dev, __u32 type, __u32 code, __s32 value)
{
    __msg("ir keyborad feedback event [type:%d, code:%d, value:%d]\n",
                type, code, value);
 	return EPDK_OK;
}

