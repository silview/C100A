/*
************************************************************************************************************************
*                                                        CUCKOO
*                                                   the Audio Player
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   :ResDecode.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2008.11.08
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
* Gary.Wang      2008.11.08       1.1.0        build the file
*
*/

#ifndef __BEETLES_APP_H_
#define __BEETLES_APP_H_

#include "apps.h"
#include "lib/common/palette.h"
#include "lib/common/misc.h"
#include "lib/common/mem_dev.h"
#include "lib/sset/toolbar/GlideCurve.h"
#include "lib/sset/toolbar/tbar_com.h"
#include "lib/sset/sset_e.h"
#include "lib/sset/sset_uipara.h"
#include "lib/app_dialog/app_dialog.h"
#include "lib/fifo_db/fifo_db.h"

//#define __here__  eLIBs_printf("@L%d(%s)\n", __LINE__, __FILE__);test_flush_nor(0);//112350
//#define __msg(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
//							     eLIBs_printf(__VA_ARGS__));test_flush_nor(0);//112350									        )


extern GUI_FONT    *SWFFont;

#define APP_ROOT		"beetles"
#define APP_HOME		"app_home"
#define APP_EXPLORER	"app_explorer"
#define APP_MOVIE		"app_movie"
#define APP_MUSIC		"app_music"
#define APP_PHOTO		"app_photo"
#define APP_FM			"app_fm"
#define APP_SETTING		"app_setting"
#define APP_EBOOK		"app_ebook"
#define APP_CALENDAR	"app_calendar"
#define APP_RECORD	    "app_record"
#define APP_LINEIN		"app_linein"


#define APP_HOME_ID			0x1001
#define APP_EXPLORER_ID		0x2001
#define APP_MOVIE_ID		0x3001
#define APP_MUSIC_ID		0x4001
#define APP_PHOTO_ID		0x5001
#define APP_FM_ID			0x6001
#define APP_SETTING_ID		0x7001
#define APP_EBOOK_ID		0x8001
#define APP_CALENDAR_ID		0x9001
#define APP_RECORD_ID		0x9002
#define APP_LINEIN_ID		0x9100
#define APP_ROOT_ID			0x9200  //cai++


typedef enum
{    
    SWITCH_TO_OTHER_APP = 0,
    SWITCH_TO_MMENU,
    SWITCH_TO_EXPLORER,
    OPEN_BG_MUSIC,
    CLOSE_BG_MUSIC,
    SWITCH_TO_LINE_IN,
}app2root_cmd_t;

#define BMP_LINE_ROUND(x)	(((__u32)x+3)/4*4)

#define BEETLES_TBL_SIZE(a)	(sizeof(a)/sizeof(a[0]))

#if 0
typedef enum
{	
	ID_HOME_FM = 0,
        
    ID_MEDIA_START,//记录movie photo music ebook、ID_HOME_RECORD类型的开始下标
	ID_HOME_RECORD=ID_MEDIA_START,
	ID_HOME_MOVIE,
	ID_HOME_PHOTO,
	ID_HOME_MUSIC,
	ID_HOME_EBOOK, 
	ID_HOME_LINEIN,
	ID_HOME_EXPLORER,  //cai++ for change main_menu
	ID_HOME_CALENDAR,   //cai++ for chang main_menu
	ID_HOME_SETTING,    //cai++ for change main_menu
    ID_MEDIA_END,
	
	ID_HOME_OTHERS = ID_MEDIA_END,	
	ID_MAX_NUM,
}root_home_id_t;
#elif defined(HIDE_LINEIN)
#ifdef HIDE_RECORD
typedef enum
{	  
    ID_MEDIA_START = 0,//记录movie photo music ebook、ID_HOME_RECORD类型的开始下标
	ID_HOME_MUSIC=ID_MEDIA_START,
	ID_HOME_MOVIE,
	ID_HOME_PHOTO,
	ID_HOME_FM,
	ID_HOME_EBOOK, 
#ifndef NO_EXPLORE	
	ID_HOME_EXPLORER,  //cai++ for change main_menu
#endif	
#ifndef NO_CALENDAR	
	ID_HOME_CALENDAR,   //cai++ for chang main_menu
#endif	
	ID_HOME_SETTING,    //cai++ for change main_menu
    ID_MEDIA_END,
	ID_HOME_LINEIN,
	ID_HOME_RECORD,
	
	ID_HOME_OTHERS = ID_MEDIA_END,	
	ID_MAX_NUM,
}root_home_id_t;
#else
typedef enum
{	  
    ID_MEDIA_START = 0,//记录movie photo music ebook、ID_HOME_RECORD类型的开始下标
	ID_HOME_MUSIC=ID_MEDIA_START,
	ID_HOME_MOVIE,
	ID_HOME_PHOTO,
	ID_HOME_FM,
	ID_HOME_RECORD,
	ID_HOME_EBOOK, 
#ifndef NO_EXPLORE	
	ID_HOME_EXPLORER,  //cai++ for change main_menu
#endif	
#ifndef NO_CALENDAR	
	ID_HOME_CALENDAR,   //cai++ for chang main_menu
#endif	
	ID_HOME_SETTING,    //cai++ for change main_menu
    ID_MEDIA_END,
	ID_HOME_LINEIN,
	
	ID_HOME_OTHERS = ID_MEDIA_END,	
	ID_MAX_NUM,
}root_home_id_t;
#endif
#elif defined(HIDE_EBOOK)
typedef enum
{	  
    ID_MEDIA_START = 0,//记录movie photo music ebook、ID_HOME_RECORD类型的开始下标
	ID_HOME_MUSIC=ID_MEDIA_START,
	ID_HOME_MOVIE,
	ID_HOME_PHOTO,
	ID_HOME_FM,
	ID_HOME_RECORD,
	ID_HOME_LINEIN,
#ifndef NO_EXPLORE	
	ID_HOME_EXPLORER,  //cai++ for change main_menu
#endif	
#ifndef NO_CALENDAR	
	ID_HOME_CALENDAR,   //cai++ for chang main_menu
#endif	
	ID_HOME_SETTING,    //cai++ for change main_menu
    ID_MEDIA_END,
	ID_HOME_EBOOK, 
	
	ID_HOME_OTHERS = ID_MEDIA_END,	
	ID_MAX_NUM,
}root_home_id_t;
#else
typedef enum
{	  
    ID_MEDIA_START = 0,//记录movie photo music ebook、ID_HOME_RECORD类型的开始下标
	ID_HOME_MUSIC=ID_MEDIA_START,
	ID_HOME_MOVIE,
	ID_HOME_PHOTO,
	ID_HOME_FM,
	ID_HOME_RECORD,
	ID_HOME_EBOOK, 
	ID_HOME_LINEIN,
#ifndef NO_EXPLORE	
	ID_HOME_EXPLORER,  //cai++ for change main_menu
#endif	
#ifndef NO_CALENDAR	
	ID_HOME_CALENDAR,   //cai++ for chang main_menu
#endif	
	ID_HOME_SETTING,    //cai++ for change main_menu
    ID_MEDIA_END,
	
	ID_HOME_OTHERS = ID_MEDIA_END,	
	ID_MAX_NUM,
}root_home_id_t;
#endif

typedef enum
{
	ID_FM_FMPLAY=0,
	ID_FM_FMSRH,
}root_fm_id_t;

typedef enum
{
	ID_EXPLORER_ALL=0,
	ID_EXPLORER_MOVIE,
	ID_EXPLORER_PHOTO,	
	ID_EXPLORER_MUSIC,
	ID_EXPLORER_EBOOK,
	ID_EXPLORER_RESTORE,
}root_explorer_id_t;

typedef enum
{
	EXPLR_SW_TO_MAIN=0,
	EXPLR_SW_TO_MOVIE,
	EXPLR_SW_TO_PHOTO,	
	EXPLR_SW_TO_MUSIC,
	EXPLR_SW_TO_EBOOK
}explorer_sw_to_t;

typedef enum
{
	ID_OTHERS_EXPLORER=0,
	ID_OTHERS_EBOOK,
	ID_OTHERS_CALENDAR,
}root_others_id_t;

typedef enum
{
	ID_SETTING_COMMON=0,
	ID_SETTING_POWER,
	ID_SETTING_TIME,
	ID_SETTING_MULTIMEDIA,
}root_setting_id_t;

enum{
	MENU_PAINT_ALL,//draw all
	MENU_PAINT_ITEMSW,//item switch
	MENU_PAINT_VALUESW,//value change
};

typedef enum
{
	SETTING_SW_TO_MAIN=0,
	SETTING_SW_TO_PROMPT_UPGRADE,
	SETTING_SW_TO_PROMPT_DEFAULT,
	EXPLR_SW_TO_UPGRADE,
	SETTING_SW_TO_MULTIMEDIA,
}setting_sw_to_t;

typedef enum
{
	FM_SW_TO_RECORD=0,
}fm_sw_to_t;

typedef enum
{
	RECORD_SW_TO_FM=0,
}record_sw_to_t;


//
//MUSICPLAYER_NORMAL: 		正常进入音乐播放器
//MUSICPLAYER_BACKGROUND: 	音乐播放器作为后台播放
typedef enum 
{
	MUSICPLAYER_NORMAL = 0,
	MUSICPLAYER_BACKGROUND = 1,	
}musicplayer_mode_e;

typedef enum   //cai++
{
	KEY_SW_TO_APP  = 0x11,
	KEY_SW_TO_FM,
	KEY_SW_TO_RECORD,
	KEY_SW_TO_MOVIE,
	KEY_SW_TO_MUSIC,
	KEY_SW_TO_EBOOK,
	KEY_SW_TO_PHOTO,
	KEY_SW_TO_SETTING,
}key_sw_to_app;

typedef enum 
{
	LINEIN_MODE_NORMAL = 0,
	LINEIN_MODE_BACKGROUND,
	LINEIN_MODE_INVALID
}linein_mode_e;

typedef struct tag_root_para
{
	GUI_FONT *font;
	H_WIN	h_parent;
	H_WIN	src_win;			//
	__u32	root_type;
    rat_root_t explr_root;
	__u32 setting_opt;//为0代表通用设置，为1代表电源管理
    __u32 record_opt;//为0代表正常进入录音，为1代表fm录音
	__s32	data;   
    __s32  explorer_play_music;
    char  music_file_name[RAT_MAX_FULL_PATH_LEN];

   __u32 late_root_type;   //cai++ for main_menu_change
	__s32 amplify_open_delay_cnt;//延时打开的次数
}root_para_t;

#define H_THEME HTHEME
#define get_menu_text   dsk_langres_get_menu_text
#define get_menu_len    dsk_langres_get_menu_len
#define theme_open      dsk_theme_open
#define theme_hdl2buf   dsk_theme_hdl2buf
#define theme_hdl2size  dsk_theme_hdl2size
#define theme_close     dsk_theme_close

extern  __u32 SetCurrentDevTypeEx(__u32 curdev);
extern __u32 GetCurrentDevTypeEx(void);

#endif// __BEETLES_APP_H_
