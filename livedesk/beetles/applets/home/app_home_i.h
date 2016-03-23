/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: app_main_i.h
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#ifndef __APP_HOME_I_H_
#define  __APP_HOME_I_H_

#include "apps.h"
#include "beetles_app.h"
#include "home_uipara.h"
#include "main_menu.h"


//#define AC00280
#define AC00285


#define MAIN_MENU_ID  		(APP_HOME_ID +1)
#define SUB_MENU_ID			(APP_HOME_ID +2)
#define MSGBOX_ID			(APP_HOME_ID +3)
#define HOME_NODISK_DLG_ID	(APP_HOME_ID +4)
#define MANUEL_BAR_ID    	(APP_HOME_ID +5)
#define SPORTS_BAR_ID		(APP_HOME_ID +6)
#define PRESET_BAR_ID		(APP_HOME_ID +7)
#define SPORTS_HR_BAR_ID	(APP_HOME_ID +8)
#define	HR_BAR_ID			(APP_HOME_ID +9)
#define USER_BAR_ID		(APP_HOME_ID +10)
#define FACTORY_BAR_ID    (APP_HOME_ID +11)


#define MOVIE_ID_SET_SHOWERR		(GUI_MSG_USER_DEF + 1)

#define AGE_MAX   			99
#define AGE_MIN   			10
#define AGE_INIT			35

#define TIME_MAX			99
#define TIME_MIN			0
#define TIME_INIT			20

#define WEIGHT_MAX_M    	180
#define WEIGHT_MIN_M 		30
#define WEIGHT_INIT_M		70

#define WEIGHT_MAX_B    	400
#define WEIGHT_MIN_B 		40
#define WEIGHT_INIT_B		155

#define LEVEL_MAX			20
#define	LEVEL_MIN			1
#define	LEVEL_INIT			1
#define LEVEL_DEFAULT		5

#define INCLINE_MAX		20
#define	INCLINE_MIN		1
#define INCLINE_INIT		1

#define TARGHR_MAX			200
#define TARGHR_MIN			72
#define	TARGHR_INIT		111
#define TARGHR_INIT2		148



void  main_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2);

void main_cmd2manuel(H_WIN hwin, __s32 id, __s32 data1, __u16 *mmenu_attr);

H_LYR home_32bpp_layer_create(RECT *rect, __u8 pipe);


#endif//__APP_HOME_I_H_
