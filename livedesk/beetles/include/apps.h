/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eMOD Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : lion
* File    : apps.h
*
* By      : andy.zhang
* Version : v1.0
* Date    : 2008-10-4 11:28:56
**********************************************************************************************************************
*/
#ifndef __APPS_H__
#define __APPS_H__

#define BEETLES_APP_ROOT   "d:\\"//"f:\\rootfs\\"//程序和资源的所在盘符.如果非调试，则定义为"d:\\"
#define BEETLES_RTC_EXIST	0

#if !(BEETLES_RTC_EXIST)	//wqx 20130325
#define NO_CALENDAR
//#define NO_EXPLORE
#define HIDE_LINEIN		//WQX 20130415
//#define HIDE_EBOOK		//WQX 20130424
//#define HIDE_RECORD		//WQX 20130918
#endif

#define FOR_800X480_UI		//WQX 20131018
#define SUPPORT_DIGIT_KEY	//WQX 20130330
//#define KEY_VOICE			//WQX 20131028
#define AB_D_AMP_CTL		//WQX 20131102

#define NO_KEYTONE			//WQX 20130410
//#define ECHO_SETTING		//WQX 20130503
#define VOL_SETTING			//WQX 20130701
#define SUPPORT_FM_VOLUME	//wqx 20130902

#define SET_OUTPUT_SUPPORT			0

#define AUTO_SAVE_BREAK_POINT			//Ethan

#define FM_SEARCH_PREVIEW				//Ethan FM搜台预览播放

#define RECORD_RAISE_GAIN			1	//Ethan 增大录音增益; K27用到, 其他案子看实际情况;

#define ECHO_CTRL_SUPPORT			1	//Ethan 软件混响旋钮检测;

#define BEETLES_SW_VCOM				1	//Ethan		软件VCOM电压调节


#define BEETLES_ENABLE_WKM			0	//

#define BEETLES_C100_AUTO_UPDATE	1	//Ethan	C100自动升级固件;

#define BEETLES_TV_OUT				0	//Ethan Tvout


#define BEETLES_MODIFY_TOOL			1	//Ethan 修改工具支持;

#define BEETLES_RESET_LCD_PARAM		1	//Ethan 恢复出厂设置同时恢复屏参;


#define BEETLES_VCOM_PATH		"e:\\Vcom.txt"
#define VCOM_MAX_LEVEL		30
#define VCOM_DEFAULT_LEVEL	16

#define BEETLES_CONSTRACT_PATH	"e:\\Constract.txt"

#define BEETLES_BRIGHTNESS_PATH	"e:\\Brightness.txt"

#define BEETLES_HUE_PATH		"e:\\Hue.txt"

#define BEETLES_SATURATION_PATH	"e:\\Saturation.txt"

#define LCD_PARAM_MAX_LEVEL		99
#define LCD_PARAM_DEFAULT		50


#define BEETLES_DEFAULT_VOLUME		30

#define BEETLES_BLACK_COLOR	0x99000000

#define BEETLES_GRAY_COLOR	0x99A0A0A0

//Io Port Definition

//Mic Mute
#define BEETLES_MIC_MUTE_CTRL		0
#define MIC_MUTE_IO_PORT			3
#define MIC_MUTE_IO_PIN				4

//Mic Det
#define BEETLES_MIC_DET_CTRL		0
#define MIC_DET_IO_PORT				3
#define MIC_DET_IO_PIN				9

//Pa AB/D
#define BEETLES_AB_D_CTRL			0
#define AB_D_IO_PORT				3
#define AB_D_IO_PIN					11

//Pa Mute
#define BEETLES_PA_MUTE_CTRL		1
#define PA_HIGH_OR_LOW_MUTE			1			//1: High Mute  0: Low Mute
#define PA_MUTE_IO_PORT				1
#define PA_MUTE_IO_PIN				0

//Line In Det
#define BEETLES_LINE_IN_CTRL		0			//multi with TX;
#define LINE_IN_IO_PORT				1
#define LINE_IN_IO_PIN				2

#define BEETLES_BOOST_IC_CTRL	0
#define BOOST_IC_PORT			3
#define BOOST_IC_PORTNUM		10


#include "epdk.h"
#include "mod_orange.h"
#include "mod_orange\orange_api_entry.h"
//#include "mod_flash.h"
#include "applets/app_load_para.h"
#include "applets/tv_visible_area.h"

#include "elibs\lib_ex.h"

#include "mod_desktop\desktop.h"

#include "init\init.h"

#include "libapps\libapps_i.h"

#include "res\lang.h"
#include "res\theme.h"

#endif
