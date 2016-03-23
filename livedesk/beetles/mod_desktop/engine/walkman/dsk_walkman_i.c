/*
************************************************************************************************************************
*                                                       walkman
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_walkman_i.c
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.06.14
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
* Gary.Wang      2010.06.14        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __DSK_WALKMAN_I_C__
#define  __DSK_WALKMAN_I_C__


#include "dsk_walkman_i.h"


G_FILE      *volatile  	dsk_wkm_horc       		= NULL;
HNPL        volatile  	dsk_wkm_hnpl       		= NULL;
__mp        *volatile  	dsk_wkm_hced       		= NULL;
g_Queue     volatile  	dsk_wkm_cedar_msgQ 		= NULL;
g_mutex_t   volatile  	dsk_wkm_res_mutex  		= NULL;

__pCBK_t 	volatile  	cb_auto_play_next  		= NULL;
void 		*volatile 	cb_ctx_auto_play_next 	= NULL;
__pCBK_t 	volatile  	cb_no_item_to_play 		= NULL;
void 		*volatile 	cb_ctx_no_item_to_play 	= NULL;

void 		*volatile 	dsk_wkm_attr 			= NULL;

__bool   	volatile  	to_quit_dsk_wkm    		= EPDK_FALSE;

char 					now_playing_file[FS_FULL_PATH_LEN_MAX];

static __u8   volatile  mid_ced = 0;
static __u8   volatile  mid_wlo = 0;
static __bool volatile  walkman_is_open    = EPDK_FALSE;
static __bool volatile  walkman_is_playing = EPDK_FALSE;
static __bool volatile  walkman_is_pause = EPDK_FALSE;
static __bool volatile  npl_is_dirty = EPDK_FALSE;


/*
****************************************************************************************************
*Name        : __dsk_wkm_orchid_open
*Prototype   : __s32 __dsk_wkm_orchid_open( void )
*Arguments   : void
*Return      : ==0    succeed
*              !=0    fail
*Description : open orchid
*Other       :
****************************************************************************************************
*/
__s32 __dsk_wkm_orchid_open( void )
{

	return 0;
}



/*
****************************************************************************************************
*Name        : __dsk_wkm_orchid_close
*Prototype   : void  __dsk_wkm_orchid_close( void )
*Arguments   : void
*Return      : void
*Description : close orchid
*Other       :
****************************************************************************************************
*/
void  __dsk_wkm_orchid_close( void )
{

}



/*
****************************************************************************************************
*Name        : __dsk_wkm_npl_create
*Prototype   : __s32 __dsk_wkm_npl_create( orc_media_type_e media_type, orc_npl_work_mode_e  work_mode )
*Arguments   : media_type     input. media type, such as audio, video and so on
*              work_mode      input. work mode.
*Return      : ==0    succeed
*              !=0    fail
*Description : create now playlist
*Other       :
****************************************************************************************************
*/
__s32 __dsk_wkm_npl_create( orc_media_type_e  media_type, orc_npl_work_mode_e  work_mode )
{

	return 0;
}



/*
****************************************************************************************************
*Name        : __dsk_wkm_npl_delete
*Prototype   : void  __dsk_wkm_npl_delete( void )
*Arguments   : void
*Return      : void
*Description : delete now playlist
*Other       :
****************************************************************************************************
*/
void  __dsk_wkm_npl_delete( void )
{

}



/*
****************************************************************************************************
*Name        : __dsk_wkm_cedar_open
*Prototype   : __s32 __dsk_wkm_cedar_open( void )
*Arguments   : void
*Return      : ==0    succeed
*              !=0    fail
*Description : open cedar
*Other       :
****************************************************************************************************
*/
__s32 __dsk_wkm_cedar_open( void )
{

	return 0;
}



/*
****************************************************************************************************
*Name        : __dsk_wkm_cedar_close
*Prototype   : void  __dsk_wkm_cedar_close( void )
*Arguments   : void
*Return      : void
*Description : close cedar
*Other       :
****************************************************************************************************
*/
void  __dsk_wkm_cedar_close( void )
{

}



/***************************************************************************************************
*Name        : __dsk_wkm_get_feedback_msgQ
*Prototype   : __s32  __dsk_wkm_get_feedback_msgQ( void )
*Arguments   : void
*Return      : ==0    succeed
*              !=0    fail
*Description : get the feedback message queue handle.
*Other       :
***************************************************************************************************/
__s32  __dsk_wkm_get_feedback_msgQ( void )
{

	return 0;
}



/***************************************************************************************************
*Name        : __dsk_wkm_res_mutex_create
*Prototype   : __s32 __dsk_wkm_res_mutex_create( void )
*Arguments   : void
*Return      : ==0    succeed
*              !=0    fail
*Description : create cedar mutex
*Other       :
***************************************************************************************************/
__s32 __dsk_wkm_res_mutex_create( void )
{

	
	return 0;
}



/***************************************************************************************************
*Name        : __dsk_wkm_res_mutex_delete
*Prototype   : void  __dsk_wkm_res_mutex_delete( void )
*Arguments   : void
*Return      : void
*Description : delete cedar mutex
*Other       :
***************************************************************************************************/
void  __dsk_wkm_res_mutex_delete( void )
{

}



/*
****************************************************************************************************
*Name        : __dsk_wkm_set_open_state
*Prototype   : void  __dsk_wkm_set_open_state( __bool is_open )
*Arguments   : is_open  input. the walkman's open state.
*Return      : void
*Description : record the open state.
*Other       :
****************************************************************************************************
*/
void  __dsk_wkm_set_open_state( __bool is_open )
{
    walkman_is_open = is_open;
}



/*
****************************************************************************************************
*Name        : __dsk_wkm_get_open_state
*Prototype   : __bool  __dsk_wkm_get_open_state( void )
*Arguments   : void
*Return      : the walkman's open state
*Description : get the open state.
*Other       :
****************************************************************************************************
*/
__bool  __dsk_wkm_get_open_state( void )
{
    return walkman_is_open;
}



/*
****************************************************************************************************
*Name        : __dsk_wkm_set_playing_state
*Prototype   : void  __dsk_wkm_set_pause_state( __bool is_pause )
*Arguments   : is_pause  input. the walkman's pause state.
*Return      : void
*Description : record the pause state.
*Other       :
****************************************************************************************************
*/
void  __dsk_wkm_set_pause_state( __bool is_pause )
{
    walkman_is_pause = is_pause;
}

/*
****************************************************************************************************
*Name        : __dsk_wkm_get_open_state
*Prototype   : __bool  __dsk_wkm_get_pause_state( void )
*Arguments   : void
*Return      : the walkman's open state
*Description : get the pause state.
*Other       :
****************************************************************************************************
*/
__bool  __dsk_wkm_get_pause_state( void )
{
    return walkman_is_pause;
}


/*
****************************************************************************************************
*Name        : __dsk_wkm_set_playing_state
*Prototype   : void  __dsk_wkm_set_playing_state( __bool is_playing )
*Arguments   : is_playing  input. the walkman's play state.
*Return      : void
*Description : record the play state.
*Other       :
****************************************************************************************************
*/
void  __dsk_wkm_set_playing_state( __bool is_playing )
{
    walkman_is_playing = is_playing;
}



/*
****************************************************************************************************
*Name        : __dsk_wkm_get_playing_state
*Prototype   : __bool  __dsk_wkm_get_playing_state( void )
*Arguments   : void
*Return      : the walkman's play state
*Description : get the play state.
*Other       :
****************************************************************************************************
*/
__bool  __dsk_wkm_get_playing_state( void )
{
    return walkman_is_playing;
}

__s32 __dsk_wkm_set_attr(void * attr)
{
	dsk_wkm_attr = attr;

	return 0;
}

void * __dsk_wkm_get_attr(void)
{
	return dsk_wkm_attr;
}

void  __dsk_wkm_set_npl_dirty_flag( __bool is_dirty )
{
    npl_is_dirty = is_dirty;
}


__bool  __dsk_wkm_get_npl_dirty_flag( void )
{
    return npl_is_dirty;
}
#endif     //  ifndef __DSK_WALKMAN_I_C__

/* end of dsk_walkman_i.c */
