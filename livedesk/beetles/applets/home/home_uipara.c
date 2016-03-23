/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: main_uipara.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#include "home_uipara.h"
#include "main_menu.h"
//#include "sub_menu.h"

static home_uipara_t uipara_400_240 =
{
    {
        0, 171, 400, 240-171
    },
    {
        0, 40, 118, 123
    },
    400/5,//一屏5个
    8, //7
    5,//一屏5个
    45,//最大宽度
    45,//最大高度
        
    118,//item width
    24,//item height
    118,//line width
    3,//line height
    118,//top width
    2,//top height
    118,//bottom width
    6//bottom height
};

#if 0
static home_uipara_t uipara_480_272 =
{
    {
        0, 185, 480, 272-185
    },
    {
        0, 56, 118, 123
    },
    480/5,//一屏5个
    8,//总共7个图标//cai++  加到8个图标
    5,//一屏5个
    45,//最大图标宽度
    45,//最大图标高度
    
    118,//item width
    24,//item height
    118,//line width
    3,//line height
    118,//top width
    2,//top height
    118,//bottom width
    6//bottom height
};
#else
  //主界面使用2x4方案
  static home_uipara_t uipara_480_272 =
{
    {
        0, 42, 480, 230
    },
    {
        0, 0, 480, 272
    },
    480/5,//一屏5个
    230/2,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    8,//总共7个图标//cai++  加到8个图标
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
#ifdef HIDE_RECORD
    7,//总共7个图标
#else
    8,//总共7个图标//cai++  加到8个图标
#endif    
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
    8,//总共7个图标//cai++  加到8个图标
#else
    10,//总共7个图标//cai++  加到8个图标
#endif
    2,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    4,//一屏5个
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
	4,
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
	4,
#else
    5,//一屏5个
#endif
    65,//最大图标宽度
    55,//最大图标高度

  //条目坐标  //dsk_type_string
    118,//item width
    24,//item height
    118,//line width
    3,//line height
    118,//top width
    2,//top height
    118,//bottom width
    6//bottom height
};
#endif
#if (MAX_ROW_NUM==1)
  static home_uipara_t uipara_800_480 =
{
    {
        0, 240, 800, 240
    },
    {
        0, 0, 200, 480
    },
    800/10,//一屏5个
    480/4,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    8,//总共7个图标//cai++  加到8个图标
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
#ifdef HIDE_RECORD
    7,//总共7个图标
#else
    8,//总共7个图标//cai++  加到8个图标
#endif    
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
    8,//总共7个图标//cai++  加到8个图标
#else
    10,//总共7个图标//cai++  加到8个图标
#endif
    1,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    4,//一屏5个
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
	5,
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
	4,
#else
    5,//一屏5个
#endif
    100,//最大图标宽度
    100,//最大图标高度

//sub menu
  //条目坐标  //dsk_type_string
    118,//item width
    24,//item height
    118,//line width
    3,//line height
    118,//top width
    2,//top height
    118,//bottom width
    6//bottom height
};
#else
  static home_uipara_t uipara_800_480 =
{
    {
        0, 0, 1024, 600
    },
    {
        0, 0, 1024, 600
    },
    800/10,//一屏5个
    480/3,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    8,//总共7个图标//cai++  加到8个图标
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
#ifdef HIDE_RECORD
    7,//总共7个图标
#else
    8,//总共7个图标//cai++  加到8个图标
#endif    
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
    8,//总共7个图标//cai++  加到8个图标
#else
    10,//总共7个图标//cai++  加到8个图标
#endif
    2,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    4,//一屏5个
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
	4,
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
	4,
#else
    5,//一屏5个
#endif
    130,//最大图标宽度
    130,//最大图标高度

//sub menu
  //条目坐标  //dsk_type_string
    118,//item width
    30,//item height
    118,//line width
    3,//line height
    118,//top width
    2,//top height
    118,//bottom width
    6//bottom height
};
#endif
home_uipara_t* home_get_ui_para(__s32 rotate)
{
    __s32           screen_width;
    __s32 			screen_height;
	
	/* get lcd size*/
	dsk_display_get_size(&screen_width, &screen_height);
    
	switch(rotate)
	{
	case GUI_SCNDIR_NORMAL:
	case GUI_SCNDIR_ROTATE180:
		{
			if((screen_width == 400 )&&( screen_height == 240))
				return &uipara_400_240;	
			else if((screen_width == 480 )&&( screen_height == 272))
				return &uipara_480_272;	
			else if((screen_width == 800 )&&( screen_height == 480))
				return &uipara_800_480;	
			
		}		
	case GUI_SCNDIR_ROTATE90:
	case GUI_SCNDIR_ROTATE270:
		
		break;
	}	

#ifdef FOR_800X480_UI
	return &uipara_800_480;		
#else
	return &uipara_480_272;		
#endif
}



