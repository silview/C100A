/*
************************************************************************************************************************
*                                                     display misc
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_display_misc.c
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.09.02
*
* Description :
* 
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2010.09.02        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __DSK_DISPLAY_MISC_C__
#define  __DSK_DISPLAY_MISC_C__


#include "dsk_display_i.h"

/*
****************************************************************************************************
*Name        : dsk_display_hdmi_audio_enable
*Prototype   : __s32 dsk_display_hdmi_audio_enable( __bool flag )
*Arguments   : flag       input. true or false.
*Return      : EPDK_OK     succeed
*              EPDK_FAIL   fail
*Description : enable or disable the audio output through HDMI.
*Other       :
****************************************************************************************************
*/
__s32 dsk_display_hdmi_audio_enable( __bool flag )
{
    ES_FILE		 * p_hdmi;
    __s32          ret;
    
    /*open display driver handle*/
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {   
        __err("open hdmi device fail!\n");
        return EPDK_FAIL;
    }
    ret = eLIBs_fioctrl(p_hdmi, HDMI_CMD_AUDIO_ENABLE, flag, 0);
    if( ret == EPDK_FAIL )
    {
	    __wrn("fail in setting hdmi audio para.!\n");
		eLIBs_fclose(p_hdmi);
        return EPDK_FAIL;
    }
    
    eLIBs_fclose(p_hdmi);
    return EPDK_OK;
}



/*
****************************************************************************************************
*Name        : dsk_display_get_size
*Prototype   : __s32 dsk_display_get_size(__s32 *p_width, __s32 *p_height)
*Arguments   : p_width     output. width
*              p_height    output. height
*Return      : EPDK_OK     succeed
*              EPDK_FAIL   fail
*Description : get the current screen size.
*Other       :
****************************************************************************************************
*/
__s32 dsk_display_get_size(__s32 *p_width, __s32 *p_height)
{
	ES_FILE 			 *p_disp = NULL;
	
	/* open display driver */
	p_disp = eLIBs_fopen(DISP_DRV_PATH, "r+");
	if( p_disp == NULL )
	{
	    __err("Error in opening display driver.\n");
	    return EPDK_FAIL;
	}
	
	*p_width  = eLIBs_fioctrl(p_disp, DISP_CMD_SCN_GET_WIDTH, 0, 0);
	*p_height = eLIBs_fioctrl(p_disp, DISP_CMD_SCN_GET_HEIGHT, 0, 0);
	
	
	eLIBs_fclose(p_disp);
	
	return EPDK_OK;
}



/*
****************************************************************************************************
*Name        : dsk_display_set_backcolor
*Prototype   : __s32 dsk_display_set_backcolor(__u32 de_color)
*Arguments   : de_color    input. color format is DE format "ARGB".
*Return      : EPDK_OK     succeed
*              EPDK_FAIL   fail
*Description : set the backcolor of the display system.
*Other       :
****************************************************************************************************
*/
__s32 dsk_display_set_backcolor(__u32 de_color)
{
	ES_FILE 			 *p_disp = NULL;
	__u32 arg[3];

	/* open display driver */
	p_disp = eLIBs_fopen(DISP_DRV_PATH, "r+");
	if( p_disp == NULL )
	{
	    __err("Error in opening display driver.\n");
	    return EPDK_FAIL;
	}
	
	arg[0] = (__u32)&de_color;
	arg[1] = 0;
	arg[2] = 0;
	eLIBs_fioctrl(p_disp,DISP_CMD_SET_BKCOLOR,0,(void *)arg);	
	
	eLIBs_fclose(p_disp);
	
	return EPDK_OK;
}



/*
****************************************************************************************************
*Name        : dsk_display_get_backcolor
*Prototype   : __u32 dsk_display_get_backcolor(void)
*Arguments   : void
*Return      : the backcolor of the display system.
*Description : get the backcolor of the display system. color format is DE format "ARGB".
*Other       :
****************************************************************************************************
*/
__u32 dsk_display_get_backcolor(void)
{
	ES_FILE 			 *p_disp = NULL;
	__s32				  color;
	__u32 arg[3];

	/* open display driver */
	p_disp = eLIBs_fopen(DISP_DRV_PATH, "r+");
	if( p_disp == NULL )
	{
	    __err("Error in opening display driver.\n");
	    return 0;
	}	
					
	arg[0] = (__u32)&color;
	arg[1] = 0;
	arg[2] = 0;	
	eLIBs_fioctrl(p_disp, DISP_CMD_GET_BKCOLOR, 0, (void *)arg);	
	
	eLIBs_fclose(p_disp);
	
	return color;
}



#endif     //  ifndef __DSK_DISPLAY_MISC_C__

/* end of dsk_display_misc.c */
