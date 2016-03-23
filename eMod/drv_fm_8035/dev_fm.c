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

#include "drv_fm_i.h"
//#include "rda5820.h"
//#include "tea5767.h"
//#include "rda5807p.h"
#include "RDA_FM5807M_drv.h"

extern    __krnl_event_t  *fm_lock;
extern    __fm_drv_t       fm_drv;
ES_FILE   *fiic;
/*
**********************************************************************************************************************
*                                               FUNCTION
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
static void FM_OpLock(void)
{
    __u8  err;

    esKRNL_SemPend(fm_lock, 0, &err);
}
/*
**********************************************************************************************************************
*                                               FUNCTION
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
static void FM_OpUnlock(void)
{
    esKRNL_SemPost(fm_lock);
}
/*
****************************************************************************************************
*
*             DEV_FM_Open
*
*  Description:
*       DEV_FM_Open
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__hdle DEV_FM_Open(void * open_arg, __u32 mode)
{
    if(!fm_drv.fm_dev.iic_file)
    {
        return (__hdle)0;
    }
    else
    {
        fiic = fm_drv.fm_dev.iic_file;

        //eLIBs_fioctrl(fm_drv.fm_dev.power_file, DRV_POWER_CMD_VSTS_SET, POWER_VOL_LDO5, (void *)POWER_VOL_ON);
        //esKRNL_TimeDly(5);
        //fm_mute(DRV_FM_VOICE_OFF);

        return (__hdle)&(fm_drv.fm_dev);
    }
}
/*
****************************************************************************************************
*
*             DEV_FM_Close
*
*  Description:
*       DEV_FM_Close
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_FM_Close(__hdle hfm)
{
    //eLIBs_fioctrl(fm_drv.fm_dev.power_file, DRV_POWER_CMD_VSTS_SET, POWER_VOL_LDO5, (void *)POWER_VOL_OFF);
    //fm_mute(DRV_FM_VOICE_OFF);
    fiic = NULL;

	return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DEV_FM_Read
*
*  Description:
*       DEV_FM_Read
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_FM_Read(void *pdata, __u32 size, __u32 n, __hdle hPower)
{
    return 0;
}
/*
****************************************************************************************************
*
*             DEV_FM_Write
*
*  Description:
*       DEV_FM_Write
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_FM_Write(const void *pdata, __u32 size, __u32 n, __hdle hPower)
{
    return size * n;
}
/*
****************************************************************************************************
*
*             DEV_FM_Ioctrl
*
*  Description:
*       DEV_FM_Ioctrl
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_FM_Ioctrl(__hdle hPower, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32                ret;

	ret = EPDK_FAIL;

	if(!fiic)
	{
	    return ret;
	}

    FM_OpLock();

    switch(cmd)
    {
        case DRV_FM_CMD_INIT:
        {
            ES_FILE *paudio;
            fm_init();
            fm_drv.fm_dev.status = FM_DRV_STATUS_RECEIVE;
            paudio = eLIBs_fopen("b:\\AUDIO\\FM", "r+");
        	if(!paudio)
        	{
        		__wrn("Open Audio Dev File Failed\n");
        		return EPDK_FAIL;
        	}
            eLIBs_fioctrl(paudio, AUDIO_DEV_CMD_START, 0, 0);
            eLIBs_fclose(paudio);
            ret = fm_drv.fm_dev.status;

            break;
        }
        case DRV_FM_CMD_EXIT:
        {
            ES_FILE  *paudio;

            paudio = eLIBs_fopen("b:\\AUDIO\\FM", "r+");
        	if(paudio)
        	{
        	    eLIBs_fioctrl(paudio, AUDIO_DEV_CMD_STOP, 0, 0);
                eLIBs_fclose(paudio);
            }
            fm_exit();
            ret = EPDK_OK;
            break;
        }
        case DRV_FM_CMD_RECVE_INIT:
        {
            break;
       }
        case DRV_FM_CMD_RECVE_EXIT:
        {

            break;
        }
        case DRV_FM_CMD_RECVE_PLAY:
        {
            fm_play((__s32)pbuffer);

            ret = EPDK_OK;
            break;
        }
        case DRV_FM_CMD_RECVE_SEARCH:
        {
            if((aux & 0xff) == DRV_FM_AUTO_SEARCH)
            {
            	__wrn(" ret = %d,  aux & 0xff00 = 0x%x\n",ret,aux & 0xff00);
                ret = fm_auto_search((__s32)pbuffer, aux & 0xff00);
            }
            else if((aux & 0xff) == DRV_FM_MANUAL_SEARCH)
            {
            	__wrn(" ret = %d,  aux & 0xff00 = 0x%x\n",ret,aux & 0xff00);
                ret = fm_manual_search((__s32)pbuffer, aux & 0xff00);
            }

            break;
        }
        case DRV_FM_CMD_AREA_SET:
        {
            ret = fm_area_choose(aux, pbuffer);

            break;
        }
        case DRV_FM_CMD_STEREO_SET:
        {
            ret = fm_stereo_choose(aux);

            break;
        }

        case DRV_FM_CMD_MUTE:
        {
            ret = fm_mute(aux);

            break;
        }

        case DRV_FM_CMD_SLEVEL_SET:
        {
            ret = fm_signal_level(aux);

            break;
        }

        case DRV_FM_CMD_SEND_INIT:
        {
            ret = fm_send_on();
            if(ret == EPDK_OK)
            {
                fm_drv.fm_dev.status = FM_DRV_STATUS_SEND;
            }
            break;
        }
        case DRV_FM_CMD_SEND_EXIT:
        {
            ret = fm_send_off();

            break;
        }
        case DRV_FM_CMD_SEND_START:
        {
            ret = fm_send((__s32)pbuffer);

            break;
        }
        case DRV_FM_CMD_SEND_PAGAIN_SET:
        {
            ret = fm_pa_gain(aux);

            break;
        }

        case DRV_FM_CMD_QUERY_STATUS:
        {
            //ret = fm_drv.fm_dev.status;
            fm_drv.fm_dev.status = fm_get_status();
            ret = fm_drv.fm_dev.status;

            break;
        }

    	default:
		{
			__wrn("Unkonwn Command...\n");

			break;
		}
	}

	FM_OpUnlock();

	return ret;
}

__dev_devop_t fm_dev_ops =
{
    DEV_FM_Open,
    DEV_FM_Close,
    DEV_FM_Read,
    DEV_FM_Write,
    DEV_FM_Ioctrl
};
