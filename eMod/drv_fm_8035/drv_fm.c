/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : drv_fm.c
* By      : xueli
* Version : V1.00
*********************************************************************************************************
*/
#include "drv_fm_i.h"

__fm_drv_t       fm_drv;
__krnl_event_t  *fm_lock;

/*
****************************************************************************************************
*
*             DRV_FM_MInit
*
*  Description:
*       DRV_FM_MInit
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/

__s32 DRV_FM_MInit(void)
{
    fm_lock = esKRNL_SemCreate(1);
    fm_drv.used = 0;

    return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DRV_FM_MExit
*
*  Description:
*       DRV_FM_MExit
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_FM_MExit(void)
{
    __u8 err;

    esKRNL_SemDel(fm_lock, OS_DEL_ALWAYS, &err);

	return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DRV_FM_MOpen
*
*  Description:
*       DRV_FM_MOpen
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__mp *DRV_FM_MOpen(__u32 mid, __u32 mod)
{
    fm_drv.mid = mid;

	return (__mp *)&fm_drv;
}
/*
****************************************************************************************************
*
*             DRV_FM_MClose
*
*  Description:
*       DRV_FM_MClose
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_FM_MClose(__mp *mp)
{
	return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DRV_FM_MRead
*
*  Description:
*       DRV_FM_MRead
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DRV_FM_MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}
/*
****************************************************************************************************
*
*             DRV_FM_MWrite
*
*  Description:
*       DRV_FM_MWrite
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DRV_FM_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return 0;
}
/*
****************************************************************************************************
*
*             DRV_FM_MIoctrl
*
*  Description:
*       DRV_FM_MIoctrl
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_FM_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    switch(cmd)
    {
		case DRV_CMD_PLUGIN:
		{
		    if(fm_drv.used)
		    {
                __inf("fm already used\n");

                return EPDK_FAIL;
		    }

            fm_drv.fm_dev.iic_file = eLIBs_fopen("b:\\BUS\\TWI0", "r+");
        	if(!fm_drv.fm_dev.iic_file)
        	{
        		__wrn("Open TWI Dev File Failed\n");

        		return EPDK_FAIL;
        	}

            fm_drv.hReg = esDEV_DevReg("USER", "FM", &fm_dev_ops, 0);
    		if(!fm_drv.hReg)
    		{
    			__wrn("user fm registered Error!\n");
    			eLIBs_fclose(fm_drv.fm_dev.iic_file);

    		    return EPDK_FAIL;
    		}

            fm_drv.used = 1;
            fm_drv.fm_dev.status = FM_DRV_STATUS_RECEIVE;
            __wrn("fm drv install OK\n");

    		return EPDK_OK;
		}

    	case DRV_CMD_PLUGOUT:
    	{
    	    if(fm_drv.used == 1)
    	    {
        		if(fm_drv.fm_dev.iic_file)
        		{
        			eLIBs_fclose(fm_drv.fm_dev.iic_file);
        		}

        		if(!fm_drv.hReg)
    			{
    			    __wrn("Dev not plugin!\n");
    			    return EPDK_FAIL;
    	    	}

    		    esDEV_DevUnreg(fm_drv.hReg);
    		}

    		fm_drv.used = 0;

		    return EPDK_OK;
    	}

		case DRV_CMD_GET_STATUS:
		{
		    if(fm_drv.used)
		    {
		        return DRV_STA_BUSY;
		    }
		    else
		    {
		        return DRV_STA_FREE;
		    }
		}
		default:
		    return EPDK_FAIL;
    }
}

