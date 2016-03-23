/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           ir key driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : DEV_KEY.c
* By      : Steven.ZGJ
* Version : V1.00
*********************************************************************************************************
*/
#include "drv_key_i.h"

extern __key_drv_t keydrv;

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
__hdle DEV_KEYC_Open(void * open_arg, __u32 mode)
{
    __s32           cpu_sr;
    __key_dev_t  *pKeycDev = (__key_dev_t *)open_arg;
    
    ENTER_CRITICAL(cpu_sr);
    if(pKeycDev->used)
    {
        EXIT_CRITICAL(cpu_sr);
        __wrn("keyboard dev open fail, used by someone else!\n");
    	return NULL;
    }
  	pKeycDev->used = 1;
    EXIT_CRITICAL(cpu_sr);
    
   	return (__hdle)pKeycDev;
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
__s32 DEV_KEYC_Close(__hdle hKey)
{
    __s32           cpu_sr;
    __key_dev_t  *pKeycDev = (__key_dev_t *)hKey;
    
    ENTER_CRITICAL(cpu_sr);
    if(pKeycDev->used == 0)
    {
        EXIT_CRITICAL(cpu_sr);
    	return EPDK_FAIL;
    }
  	pKeycDev->used = 0;
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
__u32 DEV_KEYC_Read(void *pdata, __u32 size, __u32 n, __hdle hKey)
{
    __wrn("you can't read keyboard device\n");
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
__u32 DEV_KEYC_Write(const void *pdata, __u32 size, __u32 n, __hdle hKey)
{
    __wrn("you can't write keyboard device\n");
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
__s32 DEV_KEYC_Ioctrl(__hdle hKey, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32       ret = EPDK_FAIL;

	//eDbug("--- cmd --- = %d\n", cmd);
    switch(cmd)
    {
		case DRV_KEY_CMD_PUTKEY:
		{   
			__s32 tmpVal;
			tmpVal = LRADC_DATA_VALUE;  
			tmpVal = tmpVal & 0x3f;
			//eLIBs_printf("tmpval = %d %d %d\n", tmpVal, __FILE__, __LINE__);
			
		   if(aux != 255)
		    {
    		    __key_msg_t *msg = (__key_msg_t *)pbuffer;
    		    if(msg == NULL)
    		    {
                    return ret;
    		    }		    
    		    
    		    /* send event to input system */
    		  //  __inf("keyborad send event to input system, "
    		   //       "[type:%d, code:%d, value:%d]\n",
    		   //       EV_KEY, msg->key_value, msg->flag);
    		          
    		    esINPUT_SendEvent(keydrv.pKeyc_dev,
    		                      EV_KEY,
    		                      msg->key_value,
    		                      msg->flag
    		                      );
    		    esINPUT_SendEvent(keydrv.pKeyc_dev,
        	                      EV_SYN,
        	                      SYN_REPORT,
        	                      0
        	                      );
        	 }
        	 else
        	 {
                 __u32 key_value = (__u32)pbuffer;          
                 
                 /* send event to input system */
                 __inf("virtual keyboard send event to input system, "
                       "[type:%d, code:%d, value:%d]\n",
                       EV_KEY, key_value, 1);
                       
                 esINPUT_SendEvent(keydrv.pKeyc_dev,
                                   EV_KEY,
                                   key_value,
                                   1 /*key down, no key up*/
                                   );
                 esINPUT_SendEvent(keydrv.pKeyc_dev,
                                   EV_SYN,
                                   SYN_REPORT,
                                   0
                                   );
        	 }        	 
             break;
		}

		case DRV_KEY_CMD_SET_FIRST_DEBOUNCE_TIME:
    	{
    	    __u32 first_time = (__u32)pbuffer;
            BSP_LRADCK_set_firstKeyTime(first_time);
    		break;
    	}

    	case DRV_KEY_CMD_SET_FIRST_RPT_TIME:
    	{
    	    __u32 first_delay = (__u32)pbuffer;             
            BSP_LRADCK_set_firstKeyDelay(first_delay);
    		break;
    	}

    	case DRV_KEY_CMD_SET_SBSEQ_RPT_TIME:
    	{
    	    __u32 repeat_time = (__u32)pbuffer;
            BSP_LRADCK_set_rpKeyTime(repeat_time); 
    		break;
    	}

    	case DRV_KEY_CMD_GET_FIRST_DEBOUNCE_TIME:
    	{
            ret = BSP_LRADCK_get_firstKeyTime();
     	    break;
    	}

    	case DRV_KEY_CMD_GET_FIRST_RPT_TIME:
    	{
            ret = BSP_LRADCK_get_firstKeyDelay();
    		break;
    	}

    	case DRV_KEY_CMD_GET_SBSEQ_RPT_TIME:
    	{
            ret = BSP_LRADCK_get_rpKeyTime();
    	    break;
    	} 
		default:
       {
		   ret = EPDK_FAIL;
		   break;
	   }
    }//end-of-switch
    
	return ret;
}

__dev_devop_t keyc_dev_ops =
{
    DEV_KEYC_Open,
    DEV_KEYC_Close,
    DEV_KEYC_Read,
    DEV_KEYC_Write,
    DEV_KEYC_Ioctrl
};
