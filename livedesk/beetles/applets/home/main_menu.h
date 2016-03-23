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
#ifndef __MAIN_MENU_H_
#define __MAIN_MENU_H_

#include "apps.h"
#include "beetles_app.h"

#include "com_ctrl.h"

typedef struct tag_mmenu_uipara_pos
{
	__s32 x;
	__s32 y;
	__s32 w;
	__s32 h;
	__s32 reserve;
}mmenu_uipara_pos_t;

#define MAX_PARTITION		11		//可以有11 个盘

#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    //wqx 20130325
#define MAX_ITEM_NUM	8//10    //8
#elif defined(HIDE_LINEIN)&&defined(NO_CALENDAR)    //wqx 20130415
#ifdef HIDE_RECORD
#define MAX_ITEM_NUM	7
#else
#define MAX_ITEM_NUM	8//10    //8
#endif
#elif defined(HIDE_EBOOK)&&defined(NO_CALENDAR)    //wqx 20130424
#define MAX_ITEM_NUM	8//10    //8
#else
#define MAX_ITEM_NUM	10    //8
#endif

#define MAX_ROW_NUM	2

#define ID_SWITCH_ITEM		0
#define ID_OP_SUB_UP			1
#define ID_OP_SUB_DOWN		2
#define ID_OP_SUB_ENTER	3

//按键定义

#define MANUAL_KEY  				0x20
#define HILL_KEY					0x24
#define	FATBURN_KEY				0x28
#define	CARDIO_KEY					0x2c
#define	STRENGTH_KEY				0x30
#define	INTERUAL_KEY				0x34
#define	USER1_KEY					0x38
#define	USER2_KEY					0x3c
#define	HR1_KEY						0x40
#define	HR2_KEY						0x44
#define UP_KEY						0x4c
#define	UP_LONG_KEY				0x4d
#define	UP_QUICK_LONG_KEY			0x4f
#define DOWN_KEY 					0x48
#define DOWN_LONG_KEY 				0x49
#define DOWN_QUICK_LONG_KEY 		0x4a
#define SLOW_KEY 					0x4
#define SLOW_LONG_KEY 				0x5
#define SLOW_LONG_QUICK_KEY 		0x6
#define FAST_KEY 					0x8
#define FAST_LONG_KEY				0x9
#define FAST_LONG_QUICK_KEY		0xa
#define ENTER_KEY					0x14
#define START_KEY					0xc
#define STOP_KEY					0x10
#define STOP_LONG_KEY				0x11
#define FAN_KEY						0x18
#define DISPLAY_KEY				0x1c

#define INCDEC_KEY					0x90
#define INCDEC_LONG_KEY			0x91
#define INCDEC_QUICK_LONG_KEY		0x92
#define INCPLUSE_KEY				0x94
#define INCPLUSE_LONG_KEY			0x95
#define	INCPLUSE_QUICK_LONG_KEY	0x96

#define LEVELDEC_KEY				0x8c
#define LEVELDEC_LONG_KEY			0x8d
#define LEVELDEC_QUICK_LONG_KEY	0x8e
#define LEVELPLUSE_KEY				0x88
#define LEVELPLUSE_LONG_KEY		0x89
#define	LEVELPLUSE_QUICK_LONG_KEY 0x8a

#define LEVEL_1_KEY				0x50
#define	LEVEL_3_KEY				0x54
#define	LEVEL_5_KEY				0x58
#define	LEVEL_7_KEY				0x5c
#define LEVEL_9_KEY				0x60
#define	LEVEL_12_KEY				0x64
#define	LEVEL_4_KEY				0x68

#define INCLINE_1_KEY				0x6c
#define INCLINE_3_KEY				0x70
#define INCLINE_5_KEY				0x74
#define INCLINE_7_KEY				0x78
#define INCLINE_9_KEY				0x7c
#define INCLINE_12_KEY				0x80
#define INCLINE_15_KEY				0x84

#define FACTORY_3_KEY				0xA0
#define FACTORY_2_KEY				0xA8
#define FACTORY_1_KEY				0xB4
#define DISPLAY_MODE_KEY			0xB0
#define UNLOCK_CHILD_KEY			0xAC

#define BT_DISCONNECT				0xFF

#define ID_MAIN        		4
#define ID_MANUAL 			5
#define ID_HILL				6
#define ID_FATBURN     	7
#define ID_CARDIO			8
#define ID_STRENGTH		9
#define ID_INTERVAL		10
#define	ID_USER1			11
#define ID_USER2			12
#define	ID_HR1				13
#define	ID_HR2				14

#define ID_SPORTS			15
#define ID_SPORTS_HR		16
#define ID_FACTORY			17

/*BT  CMD*/
#define BT_HEAD			0x5B
#define CONNECT_CMD	0xF0
#define BT_END			0x5D
#define MODE_CMD		0x03
#define SET_CMD			0x07
#define MODESET_CMD	0x08
#define TIME_CMD		0x04
#define WAIT_CMD		0x02
#define DATASEND_CMD	0x06
#define OK_CMD			0x0
#define ERRSTATUS_CMD	0x10
#define SUMMAY_CMD		0x32
#define MAXLEVEL_CMD	0x23
#define HRMODE_CMD		0x09
#define TARGHR_CMD		0x20
#define USER_CMD		0x26
#define INC_CMD			0x12
#define LEVEL_CMD		0x13




typedef struct tag_main_menu
{
	__u16  	HRFlag;
	__u8 	timeFlag;
	__s32 	focus_id;
	__u8    curID;
	__u16   HR0;
	__s32	age;
	__s32   weight;
	__s32   time;
	__s32   maxLevel;
	__s32	targHR;
	__u8    incPro;
	__bool  BTSET_Flag;
	__bool  E2Flag;
	H_WIN	layer;
	GUI_FONT *mmenu_font;
	__u8 *dot;
}mmenu_para_t;

typedef struct tag_res_id_t
{
	__s32 unfocus_bmp_id;
	__s32 focus_bmp_id;
}mm_res_id_t;

typedef struct manuel_set
{
	__bool manuel_focus;
	__s32 manuel_setNUM;
}manuel_set_para;

typedef struct profile_display //sports显示profile结构体
{
	__s32 time_unit; //每段时间
	__s32 level_value_unit;//level每段值
	__s32 incline_value_unit;//incline每段值
}profile_display_para;


typedef struct tag_mmenu_attr
{
	HTHEME focus_bmp[10];
	HTHEME unfocus_bmp[10];
	HTHEME settingnum_bmp[11];//10个设置数字
	HTHEME sports_time_bmp[11];//10个sports时间数字
	HTHEME sports_white_bmp[12];//10个白色数字
	HTHEME sports_blue_bmp[12];//10个蓝色数字
	HTHEME sports_yellow_bmp[10];
	HTHEME sports_hrprofile_bmp[10];//10个hr profile标示图
	HTHEME sports_profile_bmp[9];//9个显示profile
	HTHEME displayNum_bmp[12]; //10个display 数字
	HTHEME summary_num_bmp[12];   // 10个summary数字
	HTHEME summaryUnit_bmp[9];
	HTHEME USER_NAME[27];//26个字母
	HTHEME sel_focus_bmp;	//焦点图标
	HTHEME sports_incline_level_bmp[2];//level incline 切换图片
	HTHEME sports_rpm_bmp;//rpm 图片
	HTHEME bt_bmp;
	HTHEME inc_error_bmp;
	HTHEME sports_distance_bmp;//
	HTHEME sports_pace_bmp;//
	HTHEME sports_incline_bmp;//
	HTHEME sports_level_bmp;
	HTHEME sports_pulse_bmp;//
	HTHEME sports_hr_bmp;
	HTHEME sports_calories_bmp;//
	HTHEME sports_watts_bmp;//
	HTHEME sports_targHR_bmp[2];
	HTHEME user_save1_bmp;
	HTHEME user_save2_bmp;
	HTHEME user_title1_bmp;
	HTHEME user_title2_bmp;
	HTHEME initLogo_bmp;
	HTHEME initLogo2_bmp;

	HTHEME childLock_bmp;
	HTHEME initPOP_bmp;
	HTHEME E2_ERROR_bmp;

	HTHEME sports_hrProfile_1_bmp;
	HTHEME sports_hrProfile_2_bmp;
	HTHEME sports_hrWar_bmp;

	HTHEME HRProfile_bmp[5];

	HTHEME factoryUnit_bmp[2];
	HTHEME factoryUnit[2];

	HTHEME factoryInitPop_bmp[25];
	HTHEME factoryLine_bmp;

	HTHEME factoryMaxInc[2];

	HTHEME factoryTotalTime_bmp;
	HTHEME factoryTotalDistance_bmp;
	HTHEME factorySoftVer_bmp;

	HTHEME factoryCal_bmp;
	HTHEME factoryMaxinc_bmp;
	HTHEME factoryMininc_bmp;
	HTHEME factoryNum_bmp[13];
	HTHEME factoryBlueNum_bmp[10];
	HTHEME factoryCounterNum_bmp[12];
	HTHEME factory_ERR_bmp;
	HTHEME factoryJudg_bmp[2];
	HTHEME factoryTest_bmp[20];
	HTHEME factoryModeTest_focus[7];
	HTHEME factoryModeTest_unfocus[7];
	HTHEME factoryOnOff_bmp[2];
	HTHEME factoryOk_bmp[2];
	HTHEME facMotorTest_bmp;
	HTHEME facManualTest_bmp;
	HTHEME facExit_bmp;
	
	
	__s32 first_item;
	__s32 focus_item;
	__s32 item_nr;//总个数
	__s32 item_row_num;  //多少行   //cai++
	__s32 item_per_row; //一行多少个//cai++

	__s32 item_w;
	__s32 item_h;  //cai++

	GUI_FONT *font;
	H_LYR	layer;
    __bool  res_init;

	HTHEME title_bmp; //主界面title
	HTHEME message_bmp_1;
	HTHEME message_bmp_2;//主界面底部
	HTHEME error_message_bmp;
	HTHEME stop_bmp; // 停止画面
	HTHEME display_bmp; //display切换画面
	HTHEME pause_bmp;	//暂停画面
	HTHEME targTime_bmp; //时间倒记结束画面
	HTHEME summary_bmp;//summary画面
	HTHEME summary_message_bmp;
	HTHEME message_user1_bmp;
	HTHEME message_user2_bmp;
	HTHEME user_run_bmp[2];
	HTHEME user_delete_bmp[2];
	HTHEME curProfile_bmp;
	HTHEME levelProfile_bmp;
	HTHEME inclineProfile_bmp;
	HTHEME EnterUsername_bmp;
	HTHEME welcome_bmp;
	__u8 level_saved[18];

	__u8 incline_saved[18];

	__u8 curFocus;	//当前运行的段数
	__u8  curID; 		//当前模式
	//__s32 manuel_age;//Manuel 年龄
	//__s32 manuel_weight;//Manuel 体重
	//__s32 manuel_time;//manuel  时间
	//bool manuel_focus[4]; //焦点
	manuel_set_para manuel_set_p[4];

	/*
		0:rpm  1:distance  2:time  3:pace  4:level 
		5:incline 6:pulse 7:hr  8:calories  9:walls
	*/
	__u32 sports_data[10];

	profile_display_para sports_profile[18];//18段profile
		
	com_data_para_t *p_cmd_data;//通讯结构体
	//__u64 tmp[50];

}mmenu_attr_t;




__s32 main_menu_win_create(H_WIN h_parent, mmenu_para_t *para,com_data_para_t *com_data);
void main_menu_win_delete(H_WIN mm_win);
void main_menu_res_uninit(H_WIN hwin);
void main_menu_res_init(H_WIN hwin);

__s32 main_bar_win_create(H_WIN h_parent, mmenu_para_t *para);
__s32  mainbar_key_proc(__gui_msg_t *msg,com_data_para_t *com_data);


#endif
