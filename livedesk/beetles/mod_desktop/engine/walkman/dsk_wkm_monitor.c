/*
************************************************************************************************************************
*                                                        dsk_walkman
*
*                             Copyright(C), 2009-2010, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_wkm_monitor.c
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.11.19
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
* Gary.Wang      2009.11.19       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __dsk_wkm_monitor_c
#define  __dsk_wkm_monitor_c


#include "dsk_walkman_i.h"


#define MNT_FILE_LEN_MAX                511


static volatile __s32  monitor_id  = 0; 
static volatile __bool forbid_autoplay = EPDK_TRUE;
static __bool monitor_has_run = EPDK_FALSE;


static void   monitor( void *arg_p );
static __bool _is_filter_file( const char *file );
static __s32 _request_monitor_res( void );
static void  _release_monitor_res( void );



__s32  dsk_wkm_monitor_create( __u8 prio )
{
	/* create monitor thread */

	return 0;
}



void   dsk_wkm_monitor_delete( void )
{
	/* delete monitor thread */

}



__s32  __dsk_wkm_monitor_enable( void )
{
    forbid_autoplay = EPDK_FALSE;

    return 0;
}



__s32  __dsk_wkm_monitor_disable( void )
{
    forbid_autoplay = EPDK_TRUE;

    return 0;
}



static void monitor( void *arg_p )
{

}



static __bool _is_filter_file( const char *file )
{
    __sw_audio_format_e audio_format;
    
    eLIBs_GetAudioFormat( file, (__s32 *)&audio_format );
    if(    audio_format == SW_AUDIO_FORMAT_APE 
        || audio_format == SW_AUDIO_FORMAT_FLAC 
        || audio_format == SW_AUDIO_FORMAT_OGG )
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    }
}



static __s32  _request_monitor_res( void )
{

    return -1;
}



static void  _release_monitor_res( void )
{

}




#endif     //  ifndef __dsk_wkm_monitor_c

/* end of dsk_wkm_monitor.c */
