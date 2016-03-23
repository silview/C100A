#ifndef	__APP_DIGITAL_I_H__
#define	__APP_DIGITAL_I_H__
#include "digital_db.h"

//ui position
typedef struct tag_APP_DIGITAL_UI_POS
{
	__pos_t title;
	__pos_t content;
	__pos_t btn_start;
	__pos_t icon;
}APP_DIGITAL_UI_POS_T;

//ui gui size
typedef struct tag_APP_DIGITAL_UI_SIZE
{
	__rectsz_t title;
	__rectsz_t content;
	__rectsz_t btn;
	__rectsz_t btn_sp;
	__rectsz_t icon;
}APP_DIGITAL_UI_SIZE_T;

//ui操作控制参数
typedef struct tag_APP_DIGITAL_UI_CTRL_PAPA 
{
	__s32 focus_id;
}APP_DIGITAL_UI_CTRL_PAPA_T;

typedef struct tag_APP_DIGITAL_UI_COLOUR
{
	GUI_COLOR txt_n;
	GUI_COLOR txt_f;
}APP_DIGITAL_UI_COLOUR_T;

//ui data
typedef struct tag_APP_DIALOG_UI
{
	RECT                       rt;   //dialog screen rect
	APP_DIGITAL_UI_POS_T        pos;
	APP_DIGITAL_UI_SIZE_T       size;
	APP_DIGITAL_UI_CTRL_PAPA_T  ctrl;
	APP_DIGITAL_UI_COLOUR_T     colour;
}APP_DIGITAL_UI_T;

#define ADLG_STR_BTN_MEM_SIZE        (4 * 3)
#define ADLG_MAX_BTN_COUNT           3
//dialog resource
typedef struct tag_APP_DIGITAL_RES
{
	load_bmp_res_t bmp_bg;
	load_bmp_res_t bmp_btn_n;
	load_bmp_res_t bmp_btn_f;
	load_bmp_res_t bmp_icon;
	char           str_title[ADLG_STR_TITLE_MEM_SIZE];
	char           str_content[ADLG_STR_CONTENT_MEM_SIZE];
	char           str_btn[ADLG_MAX_BTN_COUNT][ADLG_STR_BTN_MEM_SIZE];
	__s32          btn_result[ADLG_MAX_BTN_COUNT];
	__s16          btn_count;
	__s16          icon_enable;
	__s32          txt_clr;
}APP_DIGITAL_RES_T;


//dialog window parameter
typedef struct tag_APP_DIGITAL_PARA 
{
	H_WIN               parent;
	H_WIN               hwnd;
	H_LYR               hlyr;
	__s32               ID;
	__s32               result;
	__u32               style;
	__u32               timeout;
	APP_DIGITAL_UI_T     ui;
	APP_DIGITAL_RES_T    res;
}APP_DIGITAL_PARA_T;

typedef APP_DIGITAL_PARA_T digital_wnd_t;
