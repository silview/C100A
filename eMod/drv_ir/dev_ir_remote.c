/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                             audio driver Module
*
*                                    (c) Copyright 2009-2010, kevin China
*                                             All Rights Reserved
*
* File    : dev_ir_remote.c
* By      : victor
* Version : V1.0
* Date    : 2010-12-31
*********************************************************************************************************
*/

#include "drv_ir_remote_i.h"

extern __ir_key_setting_t *pIrkey;   //cai++ for ir power

extern __ir_key_drv_t    irkey_drv;

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
__hdle DEV_IR_KEY_Open(void * open_arg, __u32 mode)
{
    __s32           cpu_sr;
    __ir_key_dev_t  *pIrkeyDev = (__ir_key_dev_t *)open_arg;
    
    ENTER_CRITICAL(cpu_sr);
    if(pIrkeyDev->used)
    {
        EXIT_CRITICAL(cpu_sr);
        __wrn("keyboard dev open fail, used by someone else!\n");
    	return NULL;
    }
  	pIrkeyDev->used = 1;
    EXIT_CRITICAL(cpu_sr);
    
   	return (__hdle)pIrkeyDev;
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
__s32 DEV_IR_KEY_Close(__hdle hKey)
{
    __s32           cpu_sr;
    __ir_key_dev_t  *pIrkeyDev = (__ir_key_dev_t *)hKey;
    
    ENTER_CRITICAL(cpu_sr);
    if(pIrkeyDev->used == 0)
    {
        EXIT_CRITICAL(cpu_sr);
    	return EPDK_FAIL;
    }
  	pIrkeyDev->used = 0;
    EXIT_CRITICAL(cpu_sr);
    
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
__u32 DEV_IR_KEY_Read(void *pdata, __u32 size, __u32 n, __hdle hKey)
{
    __wrn("you can't read ir keyboard device\n");
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
__u32 DEV_IR_KEY_Write(const void *pdata, __u32 size, __u32 n, __hdle hKey)
{
    __wrn("you can't write ir keyboard device\n");
    return 0;
}

/*
****************************************************************************************************
*
*             DEV_KEY_Ioctrl
*
*  Description:
*       DRV_irkey_MIoctrl 句柄没有得到实际的用处，可用作判断。
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_IR_KEY_Ioctrl(__hdle hKey, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32       ret = EPDK_FAIL;
    
    switch(cmd)
    {
		case DRV_IRKEY_CMD_PUTKEY:
		{   

		    __ir_key_msg_t *msg = (__ir_key_msg_t *)pbuffer;
		    if(msg == NULL)
		    {
                return ret;
		    }		    
		    
		    /* send event to input system */
		    __inf("ir keyborad send event to input system, "
		          "[type:%d, code:%d, value:%d]\n",
		          EV_KEY, msg->key_value, msg->flag);
		          
		    esINPUT_SendEvent(irkey_drv.pIrKey_dev,
		                      EV_KEY,
		                      msg->key_value,
		                      msg->flag
		                      );
		    esINPUT_SendEvent(irkey_drv.pIrKey_dev,
    	                      EV_SYN,
    	                      SYN_REPORT,
    	                      0
    	                      );
             break;
        }
    	case DRV_IRKEY_CMD_SET_SBSEQ_RPT_TIME:
    	{
    	    __u32 rp_keytime = (__u32)pbuffer;             
            BSP_IR_key_set_rp_keytime(rp_keytime);
    		break;
    	}

    	case DRV_IRKEY_CMD_GET_SBSEQ_RPT_TIME:
    	{    	    
            ret = BSP_IR_key_get_rp_keytime();
    		break;
    	}

    	case DRV_IRKEY_CMD_DISPLAY_SCANCODE:
    	{
    	    __u32 on_off = (__u32)pbuffer;
    	    ret = BSP_IR_key_display_scancode(on_off);
            break;
    	}
	case DRV_KEY_CMD_GET_IRPOWERVALUE:
	{
		ret = 30;
		break;
	}	
	case DRV_KEY_CMD_GET_IRMASK:
	{
		ret = pIrkey->user_code;
		break;
	}
    	/*
        case DRV_IRKEY_CMD_CHG_REMOTER:
        {
            __u32 remoter = (__u32)pbuffer;
            BSP_IR_key_set_remoter_usercode(user_code);
            break;
        }*/
		default:
       {
		   ret = EPDK_FAIL;
		   break;
	   }
    }//end-of-switch
    
	return ret;
}

__dev_devop_t irkey_dev_ops =
{
    DEV_IR_KEY_Open,
    DEV_IR_KEY_Close,
    DEV_IR_KEY_Read,
    DEV_IR_KEY_Write,
    DEV_IR_KEY_Ioctrl
};
