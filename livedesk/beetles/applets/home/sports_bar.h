/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: main_menu.h
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#ifndef __SPORTS_BAR_H_
#define __SPORTS_BAR_H_

#include "app_home_i.h"
#include "main_menu.h"




__s32 sports_bar_win_create(H_WIN h_parent, mmenu_para_t *para);
void sports_bar_win_delete(H_WIN mm_win);
void sports_bar_res_uninit(H_WIN hwin);
void sports_bar_res_init(H_WIN hwin);
__s32  sportsbar_key_proc(__gui_msg_t *msg, com_data_para_t *com_data);



#endif
