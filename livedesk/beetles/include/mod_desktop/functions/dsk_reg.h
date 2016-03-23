/*
************************************************************************************************************************
*                                             information in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_reg.h
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.09.01
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
* Gary.Wang      2010.09.01        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __DSK_REG_H__
#define  __DSK_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "reg\\reg_audio_output.h"
#include "reg\\reg_display.h"
#include "reg\\reg_lcd_bright.h"

typedef enum
{
	REG_APP_SYSTEM=0,		
	REG_APP_INIT,		
	REG_APP_ROOT,		
	REG_APP_MOVIE,		
	REG_APP_MUSIC,		
	REG_APP_PHOTO,	
	REG_APP_EBOOK,
	REG_APP_FM,
	REG_APP_RECORD,
	REG_APP_CALENDAR,
	REG_APP_MAX
}reg_app_e;

typedef struct
{		
	__u32 	audio_output;		
	__u32 	channel;	
	__u32 	charset;		
	__u32 	detect_gate;	
	__u32 	volume;	
    __u32 	language;	    //语言
	__u32 	style;			//主题风格
	__u32 	backlight;		//背光亮度
	__u32 	backlightoff;	//
	__u32 	poweroff;
	__u32	keytone;		//按键音
	__u32	echo_delay;	
	__u32 	gamma_r_value;
	__u32 	gamma_g_value;
	__u32 	gamma_b_value;
	__u32	output;
    __u32   pal_ntsc;//0代表ntsc, 1代表pal
	__u32	time_set;  //系统启动后是否要设置时间
	__date_t	date;
	__time_t	time;

#if 1
	__s32	constract;
	__s32	brightness;
	__s32	saturation;
	__s32 	hue;
	__u32 	vcom;
#endif	
//	__u32		tp_adjust_finish_flag;
    __u32   reserverd[4];  //保留参数设置
}reg_system_para_t;

typedef struct
{
	__u32 	default_bgd_index;		
	char 	default_bgd_path[768];	
	char	setting_bgd_path[768];		
	__u32 	bgd_flag;//当前的背景状态: 0代表设置背景，1代表默认背景
    __u32 	bgd_flag_in_main;//从主界面进入其它ap之前，背景状态:0代表设置背景，1代表默认背景	
    __u32   reserverd[4]; //保留参数设置
}reg_init_para_t;

typedef struct
{
	char	last_app_name[128];		// 1最近一次运行的应用名称
	
	__u32	last_movie_index_sd	;//	  2最后一次在SD卡播放的视频编号
	char	last_movie_path_sd[768];//  3最后一次在SD卡播放的视频路径
	__u32	last_movie_start_index_sd	;//	 在浏览器中的起始索引号
	
	__u32	last_movie_index_ud;	//  4最后一次在U盘播放的视频编号
	char	last_movie_path_ud[768];//  5最后一次在U盘播放的视频路径
	__u32	last_movie_start_index_ud	;//	在浏览器中的起始索引号
	
	__u32	last_music_index_sd	;//  6最后一次在SD卡播放的音乐编号
	char	last_music_path_sd[768];	//  7最后一次在SD卡播放的音乐径
	__u32	last_music_start_index_sd	;//	在浏览器中的起始索引号
	
	__u32	last_music_index_ud;//  8最后一次在U盘播放的音乐编号
	char	last_music_path_ud[768];	//  9最后一次在U盘播放的音乐路径
	__u32	last_music_start_index_ud	;//	在浏览器中的起始索引号
	
	__u32	last_photo_index_sd;//  10最后一次在SD卡播放的图片编号
	char	last_photo_path_sd[768];//  11最后一次在SD卡播放的图片路径
	__u32	last_photo_start_index_sd;//
	
	__u32	last_photo_index_ud;//  12最后一次在U盘播放的图片编号
	char	last_photo_path_ud[768];	//  13最后一次在U盘播放的图片路径
	__u32	last_photo_start_index_ud;//
	
	__u32	last_ebook_index_sd;//  14最后一次在SD卡播放的电子书编号
	char	last_ebook_path_sd[768];//  15最后一次在SD卡播放的电子书路径
	__u32	last_ebook_start_index_sd;
	
	__u32	last_ebook_index_ud;//  16最后一次在U盘播放的电子书编号
	char	last_ebook_path_ud[768];//  17最后一次在U盘播放的电子书路径
	__u32	last_ebook_start_index_ud;//


	__u32	last_app_root_type;

	__u32   reserverd[4]; //保留参数设置

	__u8	level_1[18];
	__u8	level_2[18];
	
	__u8 	age_1;
	__u8    age_2;
	__u16    weight_1;
	__u16	 weight_2;
	__u8	time_1;
	__u8    time_2;
	__u8	max_level_1;
	__u8	max_level_2;
	__u8	incline_1[18];
	__u8	incline_2[18];

	__u8    BT_level[18];

	__u8	username_1[7];
	__u8	username_2[7];

	__bool	user1In_flag;

	__bool  user2In_flag;

	__bool UnitFlag; //单位标志
	__bool BufferFlag; //蜂鸣器标志
	__u8   maxINC;
	__u8   maxADC;
	__u8   minADC;
	__u32  totalTime;
	__u32  totalDistance;
	__u16	weightShare_m;
	__u16   weightShare_b;
	__u8 	ageShare;

	__bool displayFlag;
	__bool pauFlag;
	__bool keyTFlag;
	__bool childFlag;

		
}reg_root_para_t;

/**************************************************************
*模块 movie				
*名称	变量名称	类型	范围	备注
*循环模式	rotate_mode	__u32	0-4	
*缩放模式	zoom_mode	__u32	0-4	
*字幕状态	sub_state	__u32	0-1	
*字幕颜色	sub_color	__u32	0xffffff	rgb值
*字幕位置	sub_pos	__u32	0-2	
****************************************************************/

typedef struct
{
	__u32	rotate_mode	;	
	__u32	zoom_mode	;
	__u32	sub_state	;	
	__u32 	sub_color;	
	__u32	sub_pos;	//0:bottom 1:mid 2:top
	__u32   sub_stream;
    __u32   reserverd[4]; //保留参数设置
}reg_movie_para_t;

/**************************************************************
*模块 photo
*背景音乐开关	bgmusic	__s32	0-1	预设为0
*播放速度	speedflag	__s32	0-4	预设为1
*自定义播放速度(时间间隔)	interval_time	__s32	0xffffff	预设为5
*显示比例	ratio	__s32	0-4	预设为0
*播放特效	effect	__s32	0-16	预设为0
*设置图片为桌面	bg_file_name	char[]	768字节	预设为0
*
****************************************************************/

typedef struct
{
	__u32	bgmusic	;	
	__u32	speedflag	;
	__u32	interval_time	;	
	__u32 	ratio;	
	__u32	effect;	
	__u32   reserverd[4]; //保留参数设置
}reg_photo_para_t;

typedef struct	
{
	__u32 start_item_id;	//上一次浏览器中起始条目号
	__u32 focus_item_id;	 //上一次浏览器中焦点条目号
	__u32 play_mode; 	//上一次设置的播放模式
	__u32 EQ_mode;	//上一次设置的EQ模式
	__u32 reserverd[4]; //保留参数设置
}reg_music_para_t;

typedef struct
{
	__u32	mode;			//FM,AM 模式
	__u32	channel_id;	 	//上一次收听的频道号
	__u32	FM_channel[50];	//FM频段保存
	//__u32	AM_channel[50];	//AM频段保存
	__u16	curFM1_3AM1_2_id;
	__u16	 curFM1_3AM1_2_index;
	__u32	FM1_3_AM1_2_freq[5][6];	//频段保存//add by Kingvan
	__u32	reserverd[4];	//保留参数设置 reserverd[0]:电台个数
	__u32     FM1_3_BAND_Index[3];
	__u32 	manual_seach_freq;//手动搜索到的电台频段保存值//by	 cky 
}reg_fm_para_t;

typedef struct
{
	__u32	text_colour;
	__u32	switch_time;
	__u32   bg_music_switch;	//背景音乐开关
    __u32	switch_time_flag;//是用户自定义时间还是程序预定义时间(0-3)	
	__u32   reserverd[4]; //保留参数设置
	
}reg_ebook_para_t;

typedef struct
{
	__u32   reserverd[8]; //保留参数设置
	
}reg_record_para_t;

/*
typedef struct
{
	__u32   test[2];
	__u32   sport_value;
}reg_sport_para_t;

*/
typedef struct
{
	__u32   reserverd[8]; //保留参数设置
	__u32   time_format;//时制，0代表24小时制，1代表12小时制
}reg_calendar_para_t;




extern __s32 dsk_reg_init_para(void);

extern __s32 dsk_reg_deinit_para(void);

extern __s32 dsk_reg_flush(void);

extern void* dsk_reg_get_para_by_app(reg_app_e eApp);

extern void* dsk_reg_get_default_para_by_app(reg_app_e eApp);

extern __bool dsk_reg_get_app_restore_flag(reg_app_e eApp);

extern __s32 dsk_reg_set_app_restore_flag(reg_app_e eApp, __bool flag);

extern __s32 dsk_reg_save_cur_play_info(reg_app_e eApp, __s32 index, char* filename, rat_root_t root_type);

extern __s32 dsk_reg_set_app_restore_all(void);


#ifdef __cplusplus
}
#endif

#endif     //  ifndef __DSK_REG_H__

/* end of dsk_reg.h */
