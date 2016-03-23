/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: main_menu.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "app_home_i.h"
#include "main_menu.h"
#include "com_ctrl.h"
#include "apps.h"
#include "beetles_app.h"

#if 0
    #define __here__            eLIBs_printf("@L%d(%s)\n", __LINE__, __FILE__);
	#define __msg(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
	#define __wrn(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
	#define __err(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
#else
    #define __msg(...)
#endif

//32位图层与8位图层颜色定义不同  自己添加定义
#define MAIN_32_WHITE         0xffFFFFFF
#define MAIN_32_YELLOW		0xff00FFFF

#define SEL_ICON_WIDTH		22
#define SEL_ICON_HEIGH		150

static H_WIN h_deswin_old;

static H_LYR   lyr;

static H_LYR	E2_Lyr;

static __s32 main_menu_timmer_id = 11;

static __s32 key_status = 0;

static __u16 longkeyTime_1 = 0;
static __u16 longkeyTime_2 = 0;
static __u16 longkeyTime_3 = 0;
static __u16 longkeyTime_4 = 0;
static __u16 longkeyTime_5 = 0;
static __u16 longkeyTime_6 = 0;

static __u8 errorCode;

static __bool unitFlag ;
static __bool resetFlag = EPDK_FALSE;;
static __u8 ProgramRDData[32]={0};
static __u8 ProgramWRData[32]={0};
static __u8 ShareData[32] = {0};
static __u8 pageData[2];

static __u8 fanFlag = 0;
static __bool E2ErrorFlag = EPDK_FALSE;
static __bool LevelError = EPDK_FALSE;

static __u8 curRpm;

static com_data_para_t *p_data = NULL;

//mmenu_attr_t *mmenu_attr;
#define DISPLAY_MODE_ID 		0x100

static __bool displayModeFlag = EPDK_FALSE;

static __bool ScreenCloseFlag = EPDK_FALSE;

static __u16 displayModeTime = 0;

static __u16 fanTime = 0;

static __bool childModeFlag = EPDK_FALSE; //

static __bool keyTonFlag;

static __bool CLOCK_flag = EPDK_FALSE; //是否已锁

static __u16 loopCount = 0;

static __u8 InitINCCount = 0;

static __u16 TotalDist = 0;

static __u16 TotalTime = 0;

static __u16 SP_Data[6] = {0};



enum
{
	MMENU_PAINT_ALL,
	MMENU_PAINT_ITEM
};

static __s32 FactoryNUM_ID[] = {
	ID_HOME_TV_FACTORY_0_BMP,
	ID_HOME_TV_FACTORY_1_BMP,
	ID_HOME_TV_FACTORY_2_BMP,
	ID_HOME_TV_FACTORY_3_BMP,
	ID_HOME_TV_FACTORY_4_BMP,
	ID_HOME_TV_FACTORY_5_BMP,
	ID_HOME_TV_FACTORY_6_BMP,
	ID_HOME_TV_FACTORY_7_BMP,
	ID_HOME_TV_FACTORY_8_BMP,
	ID_HOME_TV_FACTORY_9_BMP,
	ID_HOME_TV_FACTORY_COLON_BMP,
	ID_HOME_TV_FACTORY_POINT_BMP,
	ID_HOME_TV_FACTORY_V_BMP,
};


static __s32 INCInit_ID[] = {
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_01_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_02_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_03_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_04_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_05_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_06_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_07_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_08_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_09_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_10_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_11_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_12_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_13_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_14_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_15_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_16_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_17_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_18_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_19_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_20_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_21_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_22_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_23_BMP,
	ID_HOME_IMG_POP_INITIALIZE_CIRCLE_24_BMP,	
};


#ifdef FOR_800X480_UI

static mm_res_id_t ui_res_800_480[] =
{
	{ID_HOME_BTN_HOME_MANUAL_1_BMP,ID_HOME_BTN_HOME_MANUAL_2_BMP},
	{ID_HOME_BTN_HOME_HILL_1_BMP,ID_HOME_BTN_HOME_HILL_2_BMP},
	{ID_HOME_BTN_HOME_FATBURN_1_BMP,ID_HOME_BTN_HOME_FATBURN_2_BMP},
	{ID_HOME_BTN_HOME_CARDIO_1_BMP,ID_HOME_BTN_HOME_CARDIO_2_BMP},
	{ID_HOME_BTN_HOME_STRENGTH_1_BMP,ID_HOME_BTN_HOME_STRENGTH_2_BMP},
	{ID_HOME_BTN_HOME_INTERVAL_1_BMP,ID_HOME_BTN_HOME_INTERVAL_2_BMP},
	{ID_HOME_BTN_HOME_USER1_1_BMP,ID_HOME_BTN_HOME_USER1_2_BMP},
	{ID_HOME_BTN_HOME_USER2_1_BMP,ID_HOME_BTN_HOME_USER2_2_BMP},
	{ID_HOME_BTN_HOME_HR1_1_BMP,ID_HOME_BTN_HOME_HR1_2_BMP},
	{ID_HOME_BTN_HOME_HR2_1_BMP,ID_HOME_BTN_HOME_HR2_2_BMP},
};
 
#endif
static mm_res_id_t* main_get_ui_res(__s32 rotate)
{
#if 0
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
				return uipara_400_240;	
			else if((screen_width == 480 )&&( screen_height == 272))
			{
				__msg("在此处返回图片ID和文字ID??\n");
				return uipara_480_272;	
			}
			else if((screen_width == 800 )&&( screen_height == 480))
			{
				__msg("return ui_res_800_480\n");
				return ui_res_800_480;	
			}
			
		}		
	case GUI_SCNDIR_ROTATE90:
	case GUI_SCNDIR_ROTATE270:
		
		break;
	}	
__here__;
	return uipara_400_240;	
#else
	return ui_res_800_480;	
#endif

}


__s32 g_fsize;
ES_FILE*g_f;
unsigned char* g_p;
__s32 g_ret = EPDK_FAIL;

static __u32 seedtbl[]=
    { 16381163, 902867507, 703006358, 825601098, 702943101, 600362569, 824180876, 928050656,
     500242512, 600364801, 928023969, 680398149, 672661988, 825601527, 825601527, 825613410,
     823090756, 526599359,   8342805, 631233585, 302865607, 926952182, 232272027, 824145274,
     926933440, 919200125, 631206441, 631245671, 726297021, 726289272,  85829891, 631270516,
     726269366, 727066702, 726269355, 750013275, 726212604, 672729451,  85809036, 825660596,
     417735750,  18473775, 631290206};
static __u8         index = 0;
static __u32        ramdomcnt = 15;          /* used by ramdom function              */
static __u32        ramdomseed = 16;

static __u32 my_random(__u32 max)
{
    __u32 ret;

    if(index > sizeof(seedtbl)/sizeof(seedtbl[0]))
    {
        index = 0;
    }
    ramdomcnt = ramdomcnt * 29 + 314;

    ramdomseed += ramdomcnt;

    ret = ramdomcnt;

    ret = ret + seedtbl[index++];

    ret = ret - (ret / max) * max;

    return ret;
}

static __s32 test_read_nor_open(__s32* fsize, ES_FILE**f, unsigned char** p)
{
    __s32 ret;
    __s32 i, j;
    __s32 cur_data;
    __s32 page_num;
    __s32 once_count;    
    __s32 start_pos;
    __s32 file_size;
    unsigned char * pbuf;
    char path[]=BEETLES_APP_ROOT"apps\\nor_reading_test.bin";
    ES_FILE* fp;
   
    //open file
    fp = eLIBs_fopen(path, "rb");
    if(!fp)
    {        
        eLIBs_printf("open file fail, path=%s\n", path);
        return EPDK_FAIL;
    }

    //get file size
    eLIBs_fseek(fp, 0, SEEK_END);
    file_size = eLIBs_ftell(fp);
    eLIBs_printf("file_size=%d\n", file_size);

    //cal page size
    page_num = (file_size+1023)>>10;
    eLIBs_printf("page_num=%d\n", page_num);

    //alloc page mem
    pbuf = esMEMS_Palloc(page_num, 0);
    if(!pbuf)
    {        
        eLIBs_printf("esMEMS_Palloc fail\n");
        eLIBs_fclose(fp);
        return EPDK_FAIL;
    }

    *fsize = file_size;
    *f = fp;
    *p = pbuf;
    return EPDK_OK;
}

static __s32 test_read_nor_read(__s32 fsize, ES_FILE*f, unsigned char* p)
{
    __s32 i,j;
    __s32 start_pos,once_count; 
    __s32 file_size;
    __s32 cur_data;
    __s32 result;
    ES_FILE* fp;
    __s32 ret;
    unsigned char* pbuf;

    file_size = fsize;
    fp = f;
    pbuf = p;

    result = EPDK_TRUE;
    
    //begin test
    for(i = 0 ; i < 1; i++)
    {
        //generate start reading pos
        start_pos = my_random(file_size-2);                       
        
        //generate reading count
        once_count = my_random(file_size-start_pos);
               
        if(0 == once_count)
        {
            once_count = 1;
        }

        //seek and read
        
        eLIBs_fseek(fp, start_pos, SEEK_SET);
        
        ret = eLIBs_fread(pbuf, 1,once_count, fp);
        
        if(ret != once_count)
        {
            result = EPDK_FALSE;
            eLIBs_printf("eLIBs_fread fail, once_count=%d, ret=%d\n", once_count, ret);
            break;
        }

        //veryfy data
        cur_data = start_pos%256;
        for(j = 0 ; j < once_count ; j++)
        {
            if(cur_data != pbuf[j])
            {
                break;
            }
            cur_data++;
            if(cur_data >= 256)
            {
                cur_data = 0;
            }
        }

        if(j < once_count)//has err data
        {
            result = EPDK_FALSE;
            
            eLIBs_printf("***************err encounter, ****************\n");
            eLIBs_printf("test bad, i=%d, start_pos=%d, once_count=%d, cur_data=%d, pbuf[%d]=%d, err_pos=%d, (start_pos+j)/1024=%f\n"
                , i, start_pos, once_count, cur_data, j, pbuf[j], start_pos+j, (start_pos+j)/1024.0);

            if(0){//dump data
                __s32 k;
                eLIBs_printf("***************err encounter, begin dump****************\n");

                for(k = 0 ; k < once_count ; k++)
                {
                    eLIBs_printf("pbuf[%.6d]=%X\n", k, pbuf[k]);
                }
                eLIBs_printf("***************end dump****************\n");
            }

            __wait__;
            //break;
        }
        else//test alright
        {
            eLIBs_printf("test alright, i=%d, start_pos=%d, once_count=%d\n"
                , i, start_pos, once_count);
        }
        
        esKRNL_TimeDly(10);
    }
        
    return EPDK_OK;
}


/*
	申请资源
*/
static __s32 init_mmenu_res(mmenu_attr_t *mmenu_attr)
{
    __u32 	i;    
    home_uipara_t* home_ui_para;
    mm_res_id_t* main_ui_res;

    if(EPDK_TRUE == mmenu_attr->res_init)
    {
        return EPDK_OK;
    }

    home_ui_para = home_get_ui_para(GUI_GetScnDir());
    if(!home_ui_para)
    {
        __err("home_ui_para is null..\n");
        return EPDK_FAIL;
    }    

    main_ui_res = main_get_ui_res(GUI_GetScnDir());
    if(!main_ui_res)
    {
        __err("main_ui_para is null...\n");
        return EPDK_FAIL;
    }

	mmenu_attr->item_nr = 10; 
	for(i = 0; i < mmenu_attr->item_nr; i++)
	{       				
		mmenu_attr->unfocus_bmp[i] = dsk_theme_open(main_ui_res[i].unfocus_bmp_id);				

		mmenu_attr->focus_bmp[i] = dsk_theme_open(main_ui_res[i].focus_bmp_id);	
	}    

	for(i = 0 ; i < 25; i++)
	{
		mmenu_attr->factoryInitPop_bmp[i] = dsk_theme_open(INCInit_ID[i]);
	}

	for(i = 0 ; i < 13; i++)
	{
		mmenu_attr->factoryNum_bmp[i] = dsk_theme_open(FactoryNUM_ID[i]);
	}
	
	mmenu_attr->message_bmp_1 = dsk_theme_open(ID_HOME_TV_MESSAGE_HOME_BMP);
	mmenu_attr->message_bmp_2 = dsk_theme_open(ID_HOME_TV_MESSAGE_CONSOLE_LOCKED_BMP);
	mmenu_attr->error_message_bmp = dsk_theme_open(ID_HOME_TV_MESSAGE_ERROR_BMP);
	mmenu_attr->childLock_bmp = dsk_theme_open(ID_HOME_IMG_POP_CONSOLE_LOCKED_BMP);
	mmenu_attr->title_bmp = dsk_theme_open(ID_HOME_TV_TITLE_HOME_BMP);
	mmenu_attr->initPOP_bmp = dsk_theme_open(ID_HOME_IMG_POP_INITIALIZE_BMP);
	mmenu_attr->initLogo_bmp = dsk_theme_open(ID_HOME_BK_1_BMP);
	mmenu_attr->initLogo2_bmp = dsk_theme_open(ID_HOME_BK_2_BMP);
	mmenu_attr->factoryTotalDistance_bmp = dsk_theme_open(ID_HOME_TV_FACTORY_TOTAL_DISTANCE_BMP);
	mmenu_attr->factoryTotalTime_bmp= dsk_theme_open(ID_HOME_TV_FACTORY_TOTAL_TIME_BMP);
	mmenu_attr->factorySoftVer_bmp= dsk_theme_open(ID_HOME_TV_FACTORY_SOFTWARE_VER_BMP);
	mmenu_attr->factoryLine_bmp = dsk_theme_open(ID_HOME_IMG_FACTORY_LINE_BMP);
	mmenu_attr->E2_ERROR_bmp = dsk_theme_open(ID_HOME_IMG_POP_ERROR_2_BMP);
	
    mmenu_attr->res_init = EPDK_TRUE;
	 
	return EPDK_OK;
}

/*
	释放资源
*/
static __s32 uninit_mmenu_res(mmenu_attr_t *mmenu_attr)
{
	  __u32 	i; 

    if(EPDK_FALSE == mmenu_attr->res_init)
    {
        return EPDK_OK;
    }
#if 1	  
	for (i=0; i<mmenu_attr->item_nr; i++)
	{
		__msg("i = %d\n", i);
		dsk_theme_close(mmenu_attr->unfocus_bmp[i]);
		dsk_theme_close(mmenu_attr->focus_bmp[i]);		
	}

	for (i = 0; i < 13; i++)
	{
		dsk_theme_close(mmenu_attr->factoryNum_bmp[i]);
	}
	//dsk_theme_close(mmenu_attr->sel_focus_left_bmp);
	//dsk_theme_close(mmenu_attr->sel_focus_right_bmp);
	dsk_theme_close(mmenu_attr->message_bmp_1);
#endif	
	dsk_theme_close(mmenu_attr->title_bmp);
	dsk_theme_close(mmenu_attr->message_bmp_2);
	dsk_theme_close(mmenu_attr->childLock_bmp);
	dsk_theme_close(mmenu_attr->initPOP_bmp);
	dsk_theme_close(mmenu_attr->initLogo_bmp);
	dsk_theme_close(mmenu_attr->initLogo2_bmp);
	dsk_theme_close(mmenu_attr->factorySoftVer_bmp);
	dsk_theme_close(mmenu_attr->factoryTotalDistance_bmp);
	dsk_theme_close(mmenu_attr->factoryTotalTime_bmp);
	dsk_theme_close(mmenu_attr->factoryLine_bmp);
	dsk_theme_close(mmenu_attr->E2_ERROR_bmp);
	dsk_theme_close(mmenu_attr->error_message_bmp);
	for (i=0; i<25; i++) {
		dsk_theme_close(mmenu_attr->factoryInitPop_bmp[i]);
	}

    mmenu_attr->res_init = EPDK_FALSE;
    
	return EPDK_OK;
}


//Paint Focus Select Icon Only;  Ethan;
static void paint_mmain_item_focus(mmenu_attr_t *mmenu_attr, __s32 index, __s32 bfocus, __s32 row)
{   
#if 0
    GUI_RECT gui_rect;
    __u32   i; 
    void* pbmp;
	void* sel_pbmp[2];
	
    __s32 bmp_width;
    __s32 bmp_height;
    HBMP hbmp;
    __s32 bmp_x;
    __s32 bmp_y;
    __s32 first_item_x;
   __s32 first_item_y;
    home_uipara_t* home_ui_para;
    mm_res_id_t* main_ui_res;

    if(EPDK_FALSE == mmenu_attr->res_init)
    {
        init_mmenu_res(mmenu_attr);
    }

    home_ui_para = home_get_ui_para(GUI_GetScnDir());
    if(!home_ui_para)
    {
        __err("home_ui_para is null..\n");
        return ;
    }    
    main_ui_res = main_get_ui_res(GUI_GetScnDir());
    if(!main_ui_res)
    {
        __err("main_ui_res is null...\n");
        return ;
    }
	if (index < mmenu_attr->first_item || index >= mmenu_attr->item_nr)
	{
		__err("invalid para...\n");
		return ;
	}   
        
    GUI_LyrWinSel(mmenu_attr->layer);
    GUI_SetFont(mmenu_attr->font);
    GUI_UC_SetEncodeUTF8();
    GUI_SetBkColor(0);

    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
    if(bfocus)
    {
        pbmp = dsk_theme_hdl2buf(mmenu_attr->focus_bmp[index]);
#if 0		
		sel_pbmp[1] = dsk_theme_hdl2buf(mmenu_attr->sel_focus_right_bmp);
		if(sel_pbmp[0] == NULL || sel_pbmp[1] == NULL)
		{
	        __msg("dsk_theme_hdl2buf fail\n");
	        return ;
		}
#else
		sel_pbmp[0] = dsk_theme_hdl2buf(mmenu_attr->sel_focus_bmp);
#endif
    }
    else
    {
        pbmp = dsk_theme_hdl2buf(mmenu_attr->unfocus_bmp[index]);
    }

    if(NULL == pbmp)
    {
        __msg("dsk_theme_hdl2buf fail\n");
        return ;
    }

    hbmp = bmp_open(pbmp);
    if(NULL == hbmp)
    {
        __msg("open bmp fail\n");
        return;
    }

    bmp_width = bmp_get_width(hbmp);
    bmp_height = bmp_get_height(hbmp);
    __msg("bmp_width = %d, bmp_height = %d\n", bmp_width, bmp_height);
    bmp_close(hbmp);
    hbmp = 0;
    if(bmp_width > home_ui_para->max_main_bmp_width|| bmp_height > home_ui_para->max_main_bmp_height)
    {
        __msg("width or height of bmp too big..\n");
        return ;
    }

	//Clear Left Side; 
    first_item_x = (home_ui_para->lyr_main_menu.w - (home_ui_para->max_main_bmp_width * mmenu_attr->item_per_row))/(mmenu_attr->item_per_row + 1);
    first_item_y = 2*((home_ui_para->lyr_main_menu.h-mmenu_attr->item_row_num*home_ui_para->max_main_bmp_height)/(mmenu_attr->item_row_num + 3));
#if 0
    gui_rect.x0 = (first_item_x  + (index%home_ui_para->item_per_row*first_item_x) + (home_ui_para->max_main_bmp_width * (index%home_ui_para->item_per_row))) - SEL_ICON_WIDTH;
    gui_rect.y0 = first_item_y/2 + row*(first_item_y+ home_ui_para->max_main_bmp_height);  // first_item_y + row*(first_item_y/2+ home_ui_para->max_main_bmp_height);
    gui_rect.x1 = gui_rect.x0 + SEL_ICON_WIDTH;
    gui_rect.y1 = gui_rect.y0+home_ui_para->max_main_bmp_height;

    GUI_ClearRectEx(&gui_rect);

	//Clear Right Side; 
    gui_rect.x0 = gui_rect.x0 + home_ui_para->max_main_bmp_width + SEL_ICON_WIDTH;
    gui_rect.y0 = first_item_y/2 + row*(first_item_y+ home_ui_para->max_main_bmp_height);  // first_item_y + row*(first_item_y/2+ home_ui_para->max_main_bmp_height);
    gui_rect.x1 = gui_rect.x0 + SEL_ICON_WIDTH;
    gui_rect.y1 = gui_rect.y0+home_ui_para->max_main_bmp_height;

    GUI_ClearRectEx(&gui_rect);
#endif

    //Draw bmp
	bmp_x = (first_item_x  + (index%home_ui_para->item_per_row*first_item_x) + (home_ui_para->max_main_bmp_width * (index%home_ui_para->item_per_row)));
    bmp_y = first_item_y/2 + row*(first_item_y+ home_ui_para->max_main_bmp_height);	//gui_rect.y0+(home_ui_para->max_main_bmp_height-bmp_height)/2;
    if(bfocus)
    {
#if 0    
    	//Left
    	GUI_BMP_Draw(sel_pbmp[0],bmp_x-SEL_ICON_WIDTH, bmp_y -10);
		//Right
		GUI_BMP_Draw(sel_pbmp[1],bmp_x+(bmp_width), bmp_y - 10);	
#else
		GUI_BMP_Draw_Trans(sel_pbmp[0],bmp_x-5, bmp_y-5, 32, 0);
	    GUI_BMP_Draw(pbmp,bmp_x, bmp_y);
#endif
	}
	else
	{
		gui_rect.x0 = bmp_x -5;
	    gui_rect.y0 = bmp_y -5;
	    gui_rect.x1 = gui_rect.x0+home_ui_para->max_main_bmp_width+10;
	    gui_rect.y1 = gui_rect.y0+home_ui_para->max_main_bmp_height+10;
	    GUI_ClearRectEx(&gui_rect);	
		
	    GUI_BMP_Draw(pbmp,bmp_x, bmp_y);
	}

    //Draw text
    if(bfocus)
    {
    	__msg("焦点字体颜色\n");
        GUI_SetColor(mmenu_attr->focus_txt_color);
    }
    else
    {
    	__msg("非焦点字体颜色\n");
        GUI_SetColor(mmenu_attr->unfocus_txt_color);
    }
	
	gui_rect.x0 = bmp_x -10;
    gui_rect.y0 = bmp_y + home_ui_para->max_main_bmp_height+6;
    gui_rect.x1 = gui_rect.x0+home_ui_para->max_main_bmp_width+10;
    gui_rect.y1 = gui_rect.y0+ 30;
    GUI_ClearRectEx(&gui_rect);

	dsk_langres_get_menu_text(main_ui_res[index].lang_id, mmenu_attr->item_str[index], GUI_TITLE_MAX);
    GUI_DispStringInRect(mmenu_attr->item_str[index], &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);    
	#endif
}

//Paint Focus

static void paint_mmain_item_ex(mmenu_attr_t *mmenu_attr, __s32 index
    , __s32 bfocus, __s32 row)
{    
#if 0
    GUI_RECT gui_rect;
    __u32   i; 
    void* pbmp;
	void* sel_pbmp[2];
	
    __s32 bmp_width;
    __s32 bmp_height;
    HBMP hbmp;
    __s32 bmp_x;
    __s32 bmp_y;
    __s32 first_item_x;
   __s32 first_item_y;
    home_uipara_t* home_ui_para;
    mm_res_id_t* main_ui_res;

    if(EPDK_FALSE == mmenu_attr->res_init)
    {
        init_mmenu_res(mmenu_attr);
    }

    home_ui_para = home_get_ui_para(GUI_GetScnDir());
    if(!home_ui_para)
    {
        __err("home_ui_para is null..\n");
        return ;
    }    
    main_ui_res = main_get_ui_res(GUI_GetScnDir());
    if(!main_ui_res)
    {
        __err("main_ui_res is null...\n");
        return ;
    }
	if (index < mmenu_attr->first_item || index >= mmenu_attr->item_nr)
	{
		__err("invalid para...\n");
		return ;
	}   
        
    GUI_LyrWinSel(mmenu_attr->layer);
    GUI_SetFont(mmenu_attr->font);
    GUI_UC_SetEncodeUTF8();
    GUI_SetBkColor(0);
    //GUI_SetBkColor(TB_BACKCOLOR);

    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	//GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
    if(bfocus)
    {
        pbmp = dsk_theme_hdl2buf(mmenu_attr->focus_bmp[index]);
#if 0		
		sel_pbmp[0] = dsk_theme_hdl2buf(mmenu_attr->sel_focus_left_bmp);
		sel_pbmp[1] = dsk_theme_hdl2buf(mmenu_attr->sel_focus_right_bmp);
		if(sel_pbmp[0] == NULL || sel_pbmp[1] == NULL)
		{
	        __msg("dsk_theme_hdl2buf fail\n");
	        return ;
		}
#else
		sel_pbmp[0] = dsk_theme_hdl2buf(mmenu_attr->sel_focus_bmp);
		if(sel_pbmp[0] == NULL)
		{
	        __msg("dsk_theme_hdl2buf fail\n");
	        return ;
		}

#endif
    }
    else
    {
        pbmp = dsk_theme_hdl2buf(mmenu_attr->unfocus_bmp[index]);
    }

    if(NULL == pbmp)
    {
        __msg("dsk_theme_hdl2buf fail\n");
        return ;
    }

    hbmp = bmp_open(pbmp);
    if(NULL == hbmp)
    {
        __msg("open bmp fail\n");
        return;
    }

    bmp_width = bmp_get_width(hbmp);
    bmp_height = bmp_get_height(hbmp);
    bmp_close(hbmp);
    hbmp = 0;
    if(bmp_width > home_ui_para->max_main_bmp_width|| bmp_height > home_ui_para->max_main_bmp_height)
    {
        __msg("width or height of bmp too big..\n");
        return ;
    }

    //clear bg rect
    first_item_x = (home_ui_para->lyr_main_menu.w - (home_ui_para->max_main_bmp_width * mmenu_attr->item_per_row))/(mmenu_attr->item_per_row + 1);
    first_item_y = 2*((home_ui_para->lyr_main_menu.h-mmenu_attr->item_row_num*home_ui_para->max_main_bmp_height)/(mmenu_attr->item_row_num + 3));
    gui_rect.x0 = (first_item_x  + (index%home_ui_para->item_per_row*first_item_x) + (home_ui_para->max_main_bmp_width * (index%home_ui_para->item_per_row))) - 22;
    gui_rect.y0 = first_item_y/2 + row*(first_item_y+ home_ui_para->max_main_bmp_height);  
    gui_rect.x1 = gui_rect.x0+home_ui_para->max_main_bmp_width + 44;
    gui_rect.y1 = gui_rect.y0+home_ui_para->max_main_bmp_height;
    GUI_ClearRectEx(&gui_rect);

    //draw bmp
	bmp_x = (first_item_x  + (index%home_ui_para->item_per_row*first_item_x) + (home_ui_para->max_main_bmp_width * (index%home_ui_para->item_per_row)));
    bmp_y = gui_rect.y0+(home_ui_para->max_main_bmp_height-bmp_height)/2;
    if(bfocus)
    {
		GUI_BMP_Draw_Trans(sel_pbmp[0],bmp_x-5, bmp_y-5, 32, 0);
	    GUI_BMP_Draw(pbmp,bmp_x, bmp_y);
    }
	else
	    GUI_BMP_Draw(pbmp,bmp_x, bmp_y);
		

    //draw text
    if(bfocus)
    {
    	__msg("焦点字体颜色\n");
        GUI_SetColor(mmenu_attr->focus_txt_color);
    }
    else
    {
    	__msg("非焦点字体颜色\n");
        GUI_SetColor(mmenu_attr->unfocus_txt_color);
    }
	
	gui_rect.x0 = bmp_x -10;
	gui_rect.y0 = bmp_y + home_ui_para->max_main_bmp_height+6;
	gui_rect.x1 = gui_rect.x0+home_ui_para->max_main_bmp_width+10;
#ifdef FOR_800X480_UI
    gui_rect.y1 = gui_rect.y0+ 30;
#else
    gui_rect.y1 = gui_rect.y0+ 16;
#endif
    GUI_ClearRectEx(&gui_rect);

	dsk_langres_get_menu_text(main_ui_res[index].lang_id, mmenu_attr->item_str[index], GUI_TITLE_MAX);
    GUI_DispStringInRect(mmenu_attr->item_str[index], &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER); 
	#endif
}

static void draw_child_lock(mmenu_attr_t *mmenu_attr)
{
	RECT	rect;
	void *pbmp;
	
	rect.x = 0;
	rect.y = 0;
	rect.width = 1024;
	rect.height = 600;
	lyr = home_32bpp_layer_create(&rect, 2);
	
	if( !lyr )
	{
		__err("new layer create error !\n");
	}
	GUI_LyrWinSetSta(lyr, GUI_LYRWIN_STA_ON);
	GUI_LyrWinSel(lyr);
	GUI_LyrWinSetTop(lyr);
	GUI_SetBkColor(0xEE000000);
	GUI_ClearRect(0, 0, 1024, 600);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->childLock_bmp);
	GUI_BMP_Draw(pbmp,123, 150);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_2);
	GUI_BMP_Draw(pbmp,0, 504);

	CLOCK_flag = EPDK_TRUE;
	
}

/*
	显示主界面
*/

static void paint_mmain_all(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	GUI_RECT gui_rect;
	__s32 pbmp_x = 106;
	__s32 pbmp_y = 167;
	__s32 i;

	__msg("huang### paint all \n");
//	if(EPDK_FALSE == mmenu_attr->res_init)
   // {
    //    init_mmenu_res(mmenu_attr);
 //   }
	
	GUI_LyrWinSel(mmenu_attr->layer);
	GUI_SetBkColor(0x00000000);

    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	
	gui_rect.x0 = 0;
    gui_rect.y0 = 0;  
    gui_rect.x1 = 1024;
    gui_rect.y1 = 600;
    GUI_ClearRectEx(&gui_rect);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
	
	 GUI_BMP_Draw(pbmp,0, 70);

	 for (i=0; i<10; i++)
	 {
	 	pbmp = dsk_theme_hdl2buf(mmenu_attr->unfocus_bmp[i]);
		
		GUI_BMP_Draw(pbmp,pbmp_x,pbmp_y);

		pbmp_x += 165;
		if (i == 4){
			pbmp_x = 106;
			pbmp_y = 325;
		}	
	 }

	pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_1);
	GUI_BMP_Draw(pbmp,0,504);
	 
	
}

static void draw_loop_Init(mmenu_attr_t *mmenu_attr) 
{
	void *pbmp;

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryInitPop_bmp[loopCount]);
	GUI_BMP_Draw(pbmp,454,220);
	
}

static void draw_INC_init(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;

	pbmp = dsk_theme_hdl2buf(mmenu_attr->initPOP_bmp);
	GUI_BMP_Draw(pbmp,123,150);
}


static void draw_totalTimeData(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	__u8 hour;
	__u8 min;
	__u8 sec;

	hour = TotalTime / 3600;
	min = TotalTime % 3600 / 60;
	sec = TotalTime % 60;

	if (hour > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[hour/10]);
		GUI_BMP_Draw(pbmp,526,72);
		
	} else {
		GUI_SetBkColor(0);
		GUI_ClearRect(526, 72, 526+16, 72+61);
	}
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[hour%10]);
	GUI_BMP_Draw(pbmp,526+1+15,72);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[10]);
	GUI_BMP_Draw(pbmp,526+1+15*2,72);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[min/10]);
	GUI_BMP_Draw(pbmp,526+1+15*2+8,72);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[min%10]);
	GUI_BMP_Draw(pbmp,526+1+15*3+8,72);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[10]);
	GUI_BMP_Draw(pbmp,526+1+15*4+8,72);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[sec/10]);
	GUI_BMP_Draw(pbmp,526+1+15*4+16,72);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[sec%10]);
	GUI_BMP_Draw(pbmp,526+1+15*5+16,72);
	
}

static void draw_totalDistanceData(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	__u16 dist_int; //整数部分
	__u8 dist_dec;//小数部分

	if (TotalDist > 99999) {
		TotalDist = 99999;
	}
	dist_int = TotalDist/10;
	dist_dec = TotalDist%10;
	if (TotalDist > 9999) {
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[dist_int/1000]);
		GUI_BMP_Draw(pbmp,526,141);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[dist_int/100%10]);
		GUI_BMP_Draw(pbmp,526+15,141);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[dist_int%100/10]);
		GUI_BMP_Draw(pbmp,526+15*2,141);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[dist_int%100%10]);
		GUI_BMP_Draw(pbmp,526+15*3,141);		
		
	} else if (TotalDist > 999 && TotalDist < 10000) {
		GUI_SetBkColor(0);
		GUI_ClearRect(526, 141, 526+16, 141+61);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[dist_int/100]);
		GUI_BMP_Draw(pbmp,526+15,141);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[dist_int%100/10]);
		GUI_BMP_Draw(pbmp,526+15*2,141);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[dist_int%100%10]);
		GUI_BMP_Draw(pbmp,526+15*3,141);		
	} else if (TotalDist > 99 && TotalDist < 1000) {
		GUI_SetBkColor(0);
		GUI_ClearRect(526, 141, 526+16*2, 141+61);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[dist_int/10]);
		GUI_BMP_Draw(pbmp,526+15*2,141);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[dist_int%10]);
		GUI_BMP_Draw(pbmp,526+15*3,141);
	} else if (TotalDist < 100) {
		GUI_SetBkColor(0);
		GUI_ClearRect(526, 141, 526+16*3, 141+61);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[dist_int]);
		GUI_BMP_Draw(pbmp,526+15*3,141);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[11]);
	GUI_BMP_Draw(pbmp,526+15*4,141);

	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[dist_dec]);
	GUI_BMP_Draw(pbmp,526+15*4+8,141);
	

	
	
}

static void draw_softVerData(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[2]);
	GUI_BMP_Draw(pbmp,526,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[0]);
	GUI_BMP_Draw(pbmp,526+15,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[1]);
	GUI_BMP_Draw(pbmp,526+15*2,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[6]);
	GUI_BMP_Draw(pbmp,526+15*3,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[0]);
	GUI_BMP_Draw(pbmp,526+15*4,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[3]);
	GUI_BMP_Draw(pbmp,526+15*5,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[1]);
	GUI_BMP_Draw(pbmp,526+15*6,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[4]);
	GUI_BMP_Draw(pbmp,526+15*7,209);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[12]);
	GUI_BMP_Draw(pbmp,670,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[1]);
	GUI_BMP_Draw(pbmp,670+15,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[11]);
	GUI_BMP_Draw(pbmp,670+15*2,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[0]);
	GUI_BMP_Draw(pbmp,670+15*3,209);
	
	
}


static void draw_E2_Error(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	RECT	rect;

	rect.x = 0;
	rect.y = 0;
	rect.width = 1024;
	rect.height = 600;
	E2_Lyr = home_32bpp_layer_create(&rect, 2);
	
	if( !E2_Lyr )
	{
		__err("new layer create error !\n");
	}
	GUI_LyrWinSetSta(E2_Lyr, GUI_LYRWIN_STA_ON);
	GUI_LyrWinSel(E2_Lyr);
	GUI_LyrWinSetTop(E2_Lyr);
	GUI_SetBkColor(0xEE000000);
	
	GUI_ClearRect(0, 0, 1024, 600);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->E2_ERROR_bmp);
	GUI_BMP_Draw(pbmp,123,150);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->error_message_bmp);
	GUI_BMP_Draw(pbmp,0,504);


	
}


static void draw_pre_logo(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	reg_root_para_t *root_para;
	GUI_RECT gui_rect;

	pageData[0] = 0x40;
	readPage(ProgramRDData,pageData);
	
	
	TotalDist = ProgramRDData[3]*256+ProgramRDData[4];
	TotalTime= ProgramRDData[0]*65536+ProgramRDData[1]*256+ProgramRDData[2];

	GUI_LyrWinSel(mmenu_attr->layer);
	GUI_SetBkColor(0x00000000);

    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	
	gui_rect.x0 = 0;
    gui_rect.y0 = 0;  
    gui_rect.x1 = 1024;
    gui_rect.y1 = 600;
    GUI_ClearRectEx(&gui_rect);
	

	

	pbmp= dsk_theme_hdl2buf(mmenu_attr->factoryTotalTime_bmp);
	GUI_BMP_Draw(pbmp,124,72);

	pbmp= dsk_theme_hdl2buf(mmenu_attr->factoryLine_bmp);
	GUI_BMP_Draw(pbmp,124,137);

	pbmp= dsk_theme_hdl2buf(mmenu_attr->factoryTotalDistance_bmp);
	GUI_BMP_Draw(pbmp,124,140);

	pbmp= dsk_theme_hdl2buf(mmenu_attr->factoryLine_bmp);
	GUI_BMP_Draw(pbmp,124,203);

	pbmp= dsk_theme_hdl2buf(mmenu_attr->factorySoftVer_bmp);
	GUI_BMP_Draw(pbmp,124,208);

	pbmp= dsk_theme_hdl2buf(mmenu_attr->factoryLine_bmp);
	GUI_BMP_Draw(pbmp,124,270);

	draw_totalTimeData(mmenu_attr);

	draw_totalDistanceData(mmenu_attr);

	draw_softVerData(mmenu_attr);
	gscene_bgd_set_state(BGD_STATUS_SHOW);
}


/*
	显示全部ITEM
*/

static void paint_mmain_item_all(mmenu_attr_t *mmenu_attr)
{	
    __s32 i;
    __s32 j;
    //__s32 max_paint_index;

	

    if(EPDK_FALSE == mmenu_attr->res_init)
    {
        init_mmenu_res(mmenu_attr);
    }
	
	for(i = 0; i < mmenu_attr->item_row_num; i++)
	{
		for(j=mmenu_attr->first_item+i*mmenu_attr->item_per_row; j<mmenu_attr->item_per_row+i*mmenu_attr->item_per_row; j++)
		{
#if (defined(HIDE_LINEIN)&&defined(HIDE_RECORD)&&defined(NO_CALENDAR))
			if((i==1)&&(j==(mmenu_attr->item_per_row+mmenu_attr->item_per_row-1)))
				break;
#endif
			if (j == mmenu_attr->focus_item)
			{
	            		paint_mmain_item_ex(mmenu_attr, j, 1, i);			
			}
			else
			{
				paint_mmain_item_ex(mmenu_attr, j, 0, i);
			}
		}
	}

	
}

static __s32  mmenu_key_proc(__gui_msg_t *msg)
{
	static __s32 last_key = -1;
	mmenu_attr_t *mmenu_attr;
	
	mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);
	if (!mmenu_attr)
	{
		__err("invalid para...\n");
		return EPDK_FAIL;
	}

	if (KEY_UP_ACTION == msg->dwAddData2)
	{
		if (GUI_MSG_KEY_ENTER == last_key)
		{
			main_cmd2parent(msg->h_deswin, ID_OP_SUB_ENTER, mmenu_attr->focus_item, mmenu_attr->first_item);
		}
	}
	else
	{
		switch( msg->dwAddData1 )
		{
			case GUI_MSG_KEY_IR_UP:
			case GUI_MSG_KEY_LONGIR_UP:
			case GUI_MSG_KEY_IR_LEFT:
			case GUI_MSG_KEY_LONGIR_LEFT:
			case GUI_MSG_KEY_LEFT:
			case GUI_MSG_KEY_LONGLEFT:
			{
				if(mmenu_attr->focus_item <= 0)
				{
					paint_mmain_item_focus(mmenu_attr, mmenu_attr->focus_item, 0,mmenu_attr->focus_item/mmenu_attr->item_per_row);
					mmenu_attr->focus_item = mmenu_attr->item_nr - 1;
					paint_mmain_item_focus(mmenu_attr, mmenu_attr->focus_item, 1,mmenu_attr->focus_item/mmenu_attr->item_per_row);
				}
				else
				{
					paint_mmain_item_focus(mmenu_attr, mmenu_attr->focus_item, 0,mmenu_attr->focus_item/mmenu_attr->item_per_row);
					mmenu_attr->focus_item--;
					paint_mmain_item_focus(mmenu_attr, mmenu_attr->focus_item, 1,mmenu_attr->focus_item/mmenu_attr->item_per_row);

				}
			}
			break;
			
			case GUI_MSG_KEY_IR_DOWN:
			case GUI_MSG_KEY_LONGIR_DOWN:
			case GUI_MSG_KEY_IR_RIGHT:
			case GUI_MSG_KEY_LONGIR_RIGHT:
			case GUI_MSG_KEY_RIGHT:
			case GUI_MSG_KEY_LONGRIGHT:
			{
				__here__;
				if(mmenu_attr->focus_item >= mmenu_attr->item_nr - 1)
				{
					paint_mmain_item_focus(mmenu_attr, mmenu_attr->focus_item, 0,mmenu_attr->focus_item/mmenu_attr->item_per_row);
					mmenu_attr->focus_item = 0;
					paint_mmain_item_focus(mmenu_attr, mmenu_attr->focus_item, 1,mmenu_attr->focus_item/mmenu_attr->item_per_row);
				}
				else
				{
					paint_mmain_item_focus(mmenu_attr, mmenu_attr->focus_item, 0,mmenu_attr->focus_item/mmenu_attr->item_per_row);
					mmenu_attr->focus_item++;
					paint_mmain_item_focus(mmenu_attr, mmenu_attr->focus_item, 1,mmenu_attr->focus_item/mmenu_attr->item_per_row);
				}
			}
			break;
			
			case GUI_MSG_KEY_UP:
			case GUI_MSG_KEY_LONGUP:
			{
				main_cmd2parent(msg->h_deswin, ID_OP_SUB_UP, mmenu_attr->focus_item, mmenu_attr->first_item);
				break;
			}
			
			case GUI_MSG_KEY_MENU:
			case GUI_MSG_KEY_LONGDOWN:
			case GUI_MSG_KEY_DOWN:
			{
				main_cmd2parent(msg->h_deswin, ID_OP_SUB_DOWN, mmenu_attr->focus_item, mmenu_attr->first_item);
				break;	
			}
					 
			case GUI_MSG_KEY_LONGVDEC:
			case GUI_MSG_KEY_VDEC:
			case GUI_MSG_KEY_IR_VDEC:
			case GUI_MSG_KEY_LONGIR_VDEC:
			{
				dsk_volume_dec();
				break;
			}
			
			case GUI_MSG_KEY_LONGVADD:
			case GUI_MSG_KEY_VADD:
			case GUI_MSG_KEY_IR_VADD:
			case GUI_MSG_KEY_LONGIR_VADD:
			{
				dsk_volume_inc();
				break;
			}
					
		}
	}	

	if (KEY_UP_ACTION == msg->dwAddData2)
	{
		last_key = -1;	
	}
	else
	{
		last_key = msg->dwAddData1;	
	}

	return EPDK_OK;
}

/*
	主界面按键响应
*/
__s32  mainbar_key_proc(__gui_msg_t *msg,com_data_para_t *com_data)
{
	__u32 keycode;
	mmenu_attr_t *mmenu_attr;
	
	mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(h_deswin_old);
	if (!mmenu_attr)
	{
		__err("invalid para...\n");
		return EPDK_FAIL;
	}
	curRpm = com_data->curRpm;
	if (msg->dwAddData1 != 4096 || curRpm > 0) {
		if (ScreenCloseFlag) {
			
			ScreenCloseFlag = EPDK_FALSE;
			displayModeTime = 0;
			//GUI_SetTimer(msg->h_deswin, DISPLAY_MODE_ID, 100, NULL);
			buffer_time(p_data,50);
			
			gscene_bgd_set_state(BGD_STATUS_HIDE);
			
			GUI_LyrWinSel(mmenu_attr->layer);
			GUI_SetBkColor(0x00000000);
			GUI_ClearRect(0, 0, 1024, 600);
			dsk_display_lcd_on();
			InitINCCount = 0;
			resetFlag = EPDK_TRUE;
			SetFanOnOff(p_data,2);
			main_cmd2manuel(h_deswin_old,ID_MAIN,mmenu_attr->focus_item,SP_Data);
			return EPDK_OK;
			
		} else {
			displayModeTime = 0;
		}
	}

	if (msg->dwAddData1 != 4096){
		fanTime = 0;
	}

	
	keycode = msg->dwAddData1;
	if (longkeyTime_3>0 && keycode!=FACTORY_3_KEY) {
		longkeyTime_3 = 0;
	}

	if (longkeyTime_2>0 && keycode!=FACTORY_2_KEY) {
		longkeyTime_2 = 0;
	}

	if (longkeyTime_1>0 && keycode!=FACTORY_1_KEY) {
		longkeyTime_1 = 0;
	}

	if (longkeyTime_4>0 && keycode!=STOP_LONG_KEY) {
		longkeyTime_4 = 0;
	}

	if (longkeyTime_5>0 && keycode!=DISPLAY_MODE_KEY) {
		longkeyTime_5 = 0;
	}

	if (longkeyTime_6>0 && keycode!=UNLOCK_CHILD_KEY) {
		longkeyTime_6 = 0;
	}
	errorCode = com_data->error_code;
	if (!com_data->E2) {
		if ((errorCode & 0x0F) == 0x01 || (errorCode & 0x0F) == 0x02) {
			if (!LevelError) {
				E2ErrorFlag = EPDK_TRUE;
				LevelError = EPDK_TRUE;
				draw_E2_Error(mmenu_attr);
				com_data->E2 = LevelError;
			}
		} else {
			E2ErrorFlag = EPDK_FALSE;
			LevelError = EPDK_FALSE;
		}
	}
	
	if (keycode != UNLOCK_CHILD_KEY && CLOCK_flag == EPDK_TRUE) { //锁住后除解锁键其他案件全部屏蔽
		return EPDK_OK;
	} 

	switch(keycode)
	{
		case FAN_KEY:
			fanFlag = !fanFlag;
			open_buffer(p_data,1,keyTonFlag);
			//__msg("huang#####  keyTonFlag = %d\n", keyTonFlag);
			SetFanOnOff(p_data,fanFlag);
			break;
		case MANUAL_KEY:
			if (E2ErrorFlag) {
				break;
			}
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_MANUAL,mmenu_attr->focus_item,SP_Data);
		
			break;
		#if 0
		case ENTER_KEY:
			if (E2ErrorFlag) {
				break;
			}
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_MANUAL,mmenu_attr->focus_item,SP_Data);
		
			break;
		#endif
		case HILL_KEY:
			if (E2ErrorFlag) {
				break;
			}
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_HILL,mmenu_attr->focus_item,SP_Data);
	
			break;
		case FATBURN_KEY:
			if (E2ErrorFlag) {
				break;
			}
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_FATBURN,mmenu_attr->focus_item,SP_Data);
		
			break;
		case CARDIO_KEY:
			if (E2ErrorFlag) {
				break;
			}
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_CARDIO,mmenu_attr->focus_item,SP_Data);
			
			break;
		case STRENGTH_KEY:
			if (E2ErrorFlag) {
				break;
			}
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_STRENGTH,mmenu_attr->focus_item,SP_Data);
			
			break;
		case INTERUAL_KEY:
			if (E2ErrorFlag) {
				break;
			}
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_INTERVAL,mmenu_attr->focus_item,SP_Data);
			
			break;
		case HR1_KEY:
			if (E2ErrorFlag) {
				break;
			}
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_HR1,mmenu_attr->focus_item,SP_Data);
		
			break;
		case HR2_KEY:
			if (E2ErrorFlag) {
				break;
			}
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_HR2,mmenu_attr->focus_item,SP_Data);
		
			break;
		case USER1_KEY:
			if (E2ErrorFlag) {
				break;
			}
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_USER1,mmenu_attr->focus_item,SP_Data);
			
			break;
		case USER2_KEY:
			if (E2ErrorFlag) {
				break;
			}
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_USER2,mmenu_attr->focus_item,SP_Data);
		
			break;
		case START_KEY:
			if (E2ErrorFlag) {
				break;
			}
			//mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_INIT;
			//mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_M;
			//mmenu_attr->manuel_set_p[2].manuel_setNUM = 0;
			//mmenu_attr->manuel_set_p[3].manuel_setNUM = LEVEL_MAX;
			SP_Data[0] = AGE_INIT;
			SP_Data[1] = WEIGHT_INIT_M;
			SP_Data[2] = 0;
			SP_Data[3] = LEVEL_MAX;
	
			open_buffer(p_data,1,keyTonFlag);
			main_cmd2manuel(h_deswin_old,ID_SPORTS,1,SP_Data);
			break;
		case STOP_KEY:
			if (E2ErrorFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
				E2ErrorFlag = EPDK_FALSE;
				GUI_LyrWinSel(E2_Lyr);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinDelete(E2_Lyr);
				E2_Lyr = 0;
				break;
			}
			break;
		case FACTORY_1_KEY:
			if (E2ErrorFlag) {
				break;
			}
			longkeyTime_1++;
			if (longkeyTime_1 == 40) {
				open_buffer(p_data,1,keyTonFlag);
				main_cmd2manuel(h_deswin_old,ID_FACTORY,1,SP_Data);
			}
			break;
		case FACTORY_2_KEY:
			if (E2ErrorFlag) {
				break;
			}
			longkeyTime_2++;
			if (longkeyTime_2 == 40) {
				open_buffer(p_data,1,keyTonFlag);
				main_cmd2manuel(h_deswin_old,ID_FACTORY,2,SP_Data);
			}
			break;
		case FACTORY_3_KEY:
			if (E2ErrorFlag) {
				break;
			}
			longkeyTime_3++;
			if (longkeyTime_3 == 80) {
				open_buffer(p_data,1,keyTonFlag);	
				main_cmd2manuel(h_deswin_old,ID_FACTORY,3,SP_Data);
			}	
			break;
		case UNLOCK_CHILD_KEY:
			if (E2ErrorFlag) {
				break;
			}
			if (CLOCK_flag) {
				longkeyTime_6++;
				if (longkeyTime_6 == 50) {
					open_buffer(p_data,1,keyTonFlag);
					GUI_SetBkColor(0x00000000);
					GUI_ClearRect(0, 0, 1024, 600);
					GUI_LyrWinDelete(lyr);
					CLOCK_flag = EPDK_FALSE;
					gscene_bgd_set_state(BGD_STATUS_SHOW);
    				paint_mmain_all(mmenu_attr);
					SetRunLevel(p_data,1);
				}
			}
			break;	 
		case STOP_LONG_KEY:
			if (E2ErrorFlag) {
				break;
			}
			longkeyTime_4++;
			if (longkeyTime_4 == 30) {
				//open_buffer(p_data,1,keyTonFlag);
				//esKSRV_Reset();
				buffer_time(p_data,50);
				
				gscene_bgd_set_state(BGD_STATUS_HIDE);
				GUI_LyrWinSel(mmenu_attr->layer);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				InitINCCount = 0;
				resetFlag = EPDK_TRUE;
				SetFanOnOff(p_data,2);
				com_data->E2 = EPDK_FALSE;
				SeclectMac(p_data , 1);
				main_cmd2manuel(h_deswin_old,ID_MAIN,mmenu_attr->focus_item,SP_Data);
			}
			break;
		case DISPLAY_MODE_KEY:
			if (E2ErrorFlag) {
				break;
			}
			longkeyTime_5++;
			if (longkeyTime_5 == 40) {
				open_buffer(p_data,1,keyTonFlag);
				main_cmd2manuel(h_deswin_old,ID_FACTORY,4,SP_Data);
			}
			break;
		default:
			break;

	}

	return EPDK_OK;
}
	
/*
	
*/
static __s32 _main_menu_Proc(__gui_msg_t *msg)
{
    home_uipara_t* home_ui_para;
 
    
    home_ui_para = home_get_ui_para(GUI_GetScnDir());
    if(!home_ui_para)
    {
    __here__;
        __err("home_ui_para is null..\n");
        return ;
    }   
	//__msg("huang######                                     \n ");
 //   __msg("_main_menu_Proc msg->id=%d, msg->h_deswin=%x, msg->dwAddData1=%d, msg->dwAddData2=%d\n"
   //     ,msg->id, msg->h_deswin, msg->dwAddData1, msg->dwAddData2);
	//__msg("huang######                                     \n ");

	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
		#if 0
			mmenu_attr_t *mmenu_attr;
			mmenu_para_t *mmenu_para;

			mmenu_para = (mmenu_para_t *)GUI_WinGetAttr(msg->h_deswin);
			
			mmenu_attr = (mmenu_attr_t *)esMEMS_Balloc(sizeof(mmenu_attr_t));
            if(!mmenu_attr)
            {
                return EPDK_FAIL;                
            }
			eLIBs_memset((void *)mmenu_attr, 0, sizeof(mmenu_attr));
			//init_mmenu_res(mmenu_attr);
			
			mmenu_attr->font = mmenu_para->mmenu_font;
			mmenu_attr->layer = mmenu_para->layer;
			mmenu_attr->item_w = home_ui_para->item_width;
			mmenu_attr->focus_item = mmenu_para->focus_id;
			mmenu_attr->first_item = 0;
			//mmenu_attr->focus_txt_color = MAIN_32_YELLOW;//APP_COLOR_YELLOW;
			//mmenu_attr->unfocus_txt_color =   MAIN_32_WHITE;//APP_COLOR_BLACK; //cai modify//APP_COLOR_WHITE;
            mmenu_attr->res_init = EPDK_FALSE;
		
			//init_mmenu_res(mmenu_attr);
			//init_cmd_data(mmenu_attr);
			GUI_WinSetAddData(msg->h_deswin, (__s32)mmenu_attr);
			h_deswin_old = msg->h_deswin;
			
		
			__here__;            
            //GUI_SetTimer(msg->h_deswin, main_menu_timmer_id, 200, NULL);

           // g_ret = test_read_nor_open(&g_fsize, &g_f, &g_p); 
			#endif
			
		}
		return EPDK_OK;
	case GUI_MSG_CLOSE:
		{
			
			GUI_FrmWinDelete(msg->h_deswin);
		}
		return EPDK_OK;
	case GUI_MSG_DESTROY:
		{
			mmenu_para_t *mmenu_para;
			mmenu_attr_t *mmenu_attr;  
          
			__u8 i;

			if( GUI_IsTimerInstalled(msg->h_deswin, DISPLAY_MODE_ID))		
				GUI_KillTimer(msg->h_deswin, DISPLAY_MODE_ID);
			
			mmenu_para = (mmenu_para_t *)GUI_WinGetAttr(msg->h_deswin);
			mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			uninit_mmenu_res(mmenu_attr);
			
			esMEMS_Bfree(mmenu_attr, sizeof(mmenu_attr_t));
			esMEMS_Bfree(mmenu_para, sizeof(mmenu_para_t));//esMEMS_Bfree(mmenu_para, sizeof(mmenu_para));//内存释放问题
			
		}
		return EPDK_OK;
	case GUI_MSG_PAINT:
        {	
		   mmenu_attr_t *mmenu_attr;
		   mmenu_para_t *mmenu_para;
		   void *pbmp;
		   __u8 i;
		   GUI_RECT gui_rect;
		   RECT	rect;
		   reg_root_para_t *para;
		   
			mmenu_para = (mmenu_para_t *)GUI_WinGetAttr(msg->h_deswin);
			
			mmenu_attr = (mmenu_attr_t *)esMEMS_Balloc(sizeof(mmenu_attr_t));
            if(!mmenu_attr)
            {
                return EPDK_FAIL;                
            }
			eLIBs_memset((void *)mmenu_attr, 0, sizeof(mmenu_attr));
			//init_mmenu_res(mmenu_attr);
			E2ErrorFlag = EPDK_FALSE;	
			LevelError	 = EPDK_FALSE;
			
			mmenu_attr->font = mmenu_para->mmenu_font;
			mmenu_attr->layer = mmenu_para->layer;
			mmenu_attr->item_w = home_ui_para->item_width;
			mmenu_attr->focus_item = mmenu_para->focus_id;
			mmenu_attr->first_item = 0;
			
            mmenu_attr->res_init = EPDK_FALSE;
			
			//displayModeFlag = EPDK_FALSE;
			displayModeTime = 0;
			fanTime = 0;
			longkeyTime_1 = 0;
			longkeyTime_2 = 0;
			longkeyTime_3 = 0;
			longkeyTime_4 = 0;
			longkeyTime_5 = 0;
			longkeyTime_6 = 0;
			
			GUI_WinSetAddData(msg->h_deswin, (__s32)mmenu_attr);
			h_deswin_old = msg->h_deswin;
			
       

            if (GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(GUI_WinGetLyrWin(msg->h_deswin)))
            {
                __msg("layer not on, not need paint...\n");
    			return EPDK_OK;
            }

            mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);

            if (!mmenu_attr)
    		{
    			__err("invalid para...\n");
    			return EPDK_FAIL;
    		}
			para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);
			
            init_mmenu_res(mmenu_attr);
			
			//__msg("huang#####   init  res   \n");
			if (!InitINCCount) {
				if (resetFlag) {
					pbmp = dsk_theme_hdl2buf(mmenu_attr->initLogo2_bmp);
			 		GUI_BMP_Draw(pbmp,0, 0);
					resetFlag = EPDK_FALSE;
					esKRNL_TimeDly(150);
				}
				pbmp = dsk_theme_hdl2buf(mmenu_attr->initLogo_bmp);
		
		 		GUI_BMP_Draw(pbmp,0, 0);
				if (!para->childFlag) {
					pageData[0] = 0x20;
					ShareData[0] = AGE_INIT; //通用年龄
					ShareData[1] = TIME_INIT;//通用时间
					ShareData[2] = WEIGHT_INIT_M;//通用公制体重
					ShareData[3] = (WEIGHT_INIT_B>>8)&0xFF;//通用英制体重高8位
					ShareData[4] = WEIGHT_INIT_B&0xFF;//通用英制低8位
					writePage(ShareData,pageData);
					esKRNL_TimeDly(3);
					pageData[0] = 0x40;
					ProgramRDData[0] = 0;
					ProgramRDData[1] = 0;
					ProgramRDData[2] = 0;
					ProgramRDData[3] = 0;
					ProgramRDData[4] = 0;
					writePage(ProgramRDData,pageData);
					esKRNL_TimeDly(3);
					pageData[0] = 0x60;
					ProgramWRData[0] = 0; //公英制 默认英制
					ProgramWRData[1] = 0; //display MODE 
					ProgramWRData[2] = 0; //PAUSE MODE
					ProgramWRData[3] = 0; //KEY TONE
					ProgramWRData[4] = 0; //CHILD LOCK
					ProgramWRData[5] = 221; // MAX ADC
					ProgramWRData[6] = 21; //MIN ADC
					ProgramWRData[7] = 0; // user1 Flag
					ProgramWRData[8] = 0; //user2 Flag
					writePage(ProgramWRData,pageData);
				}
				esKRNL_TimeDly(100);
				draw_pre_logo(mmenu_attr);
				para->childFlag = EPDK_TRUE;
				dsk_reg_flush();
				InitINCCount = 1;
				
			/*
				检测是否INC归零，没有归零开始归零
			*/
			
			#ifdef AC00280
				
				//__msg("huang#########   inc   init  %d \n",p_data->run_mode);
				GUI_SetBkColor(0x00000000);
				gui_rect.x0 = 0;
			    gui_rect.y0 = 0;  
			    gui_rect.x1 = 1024;
			    gui_rect.y1 = 600;
			    GUI_ClearRectEx(&gui_rect);
				pageData[0] = 0x60;
				readPage(ProgramRDData,pageData);
				if (ProgramRDData[6]) {
					GetRunIncAdc(p_data);
					GetRunIncMINAdc(p_data);
					//__msg("huang#########   curInc_ADC = %d  \n",p_data->curInc_ADC);
					//__msg("huang#########   minInc_ADC = %d  \n",p_data->minInc_ADC);
					if ((p_data->curInc_ADC-2) > ProgramRDData[6]  || (ProgramRDData[6]-2)>p_data->curInc_ADC) {
						//__msg("huang#########   inc  not init  \n");
						InitINC(p_data);
						gscene_bgd_set_state(BGD_STATUS_HIDE);
						draw_INC_init(mmenu_attr);
						
						rect.x = 0;
						rect.y = 0;
						rect.width = 1024;
						rect.height = 600;
						lyr = home_32bpp_layer_create(&rect, 2);
						if( !lyr )
						{
							__err("new layer create error !\n");
						}
						GUI_LyrWinSetSta(lyr, GUI_LYRWIN_STA_ON);
						GUI_LyrWinSel(lyr);
						GUI_LyrWinSetTop(lyr);
						GUI_SetBkColor(0x00000000);
						GUI_ClearRect(0, 0, 1024, 600);
						
						GetINCstatus(p_data);
						esKRNL_TimeDly(12);
						
						esKRNL_TimeDly(5);
						while (p_data->INCstatus != 0)   {
							//__msg("huang#######  INC  initing  \n");
							esKRNL_TimeDly(1);
							loopCount ++;
							if (loopCount == 24) {
								loopCount = 1;
							}
							draw_loop_Init(mmenu_attr);
							GetINCstatus(p_data);

							if (p_data->INCstatus == 1){
								break;
							}
						}
						GUI_SetBkColor(0x00000000);
						GUI_ClearRect(0, 0, 1024, 600);
						GUI_LyrWinDelete(lyr);
					}
				}
			#else ifdef AC00285
				GUI_SetBkColor(0x00000000);
				gui_rect.x0 = 0;
			    gui_rect.y0 = 0;  
			    gui_rect.x1 = 1024;
			    gui_rect.y1 = 600;
			    GUI_ClearRectEx(&gui_rect);
				GetRunIncAdc(p_data);
				if (p_data->curInc_ADC > 29) {
						gscene_bgd_set_state(BGD_STATUS_HIDE);
						draw_INC_init(mmenu_attr);
						
						rect.x = 0;
						rect.y = 0;
						rect.width = 1024;
						rect.height = 600;
						lyr = home_32bpp_layer_create(&rect, 2);
						if( !lyr )
						{
							__err("new layer create error !\n");
						}
						GUI_LyrWinSetSta(lyr, GUI_LYRWIN_STA_ON);
						GUI_LyrWinSel(lyr);
						GUI_LyrWinSetTop(lyr);
						GUI_SetBkColor(0x00000000);
						GUI_ClearRect(0, 0, 1024, 600);
						
						SetRunSTRDIE(p_data,0);
						esKRNL_TimeDly(12);
						GetINCstatus(p_data);
						esKRNL_TimeDly(5);
						while (p_data->INCstatus != 0)   {
							//__msg("huang#######  INC  initing  \n");
							esKRNL_TimeDly(5);
							loopCount ++;
							if (loopCount == 24) {
								loopCount = 1;
							}
							draw_loop_Init(mmenu_attr);
							GetINCstatus(p_data);

							if (p_data->INCstatus == 1){
								break;
							}
						}
						GUI_SetBkColor(0x00000000);
						GUI_ClearRect(0, 0, 1024, 600);
						GUI_LyrWinDelete(lyr);
					
				}
			#endif
				gscene_bgd_set_state(BGD_STATUS_SHOW);
				if (childModeFlag) {
					draw_child_lock(mmenu_attr);
					SetRunLevel(p_data,20);
				}
			}
			
			gscene_bgd_set_state(BGD_STATUS_SHOW);
    		paint_mmain_all(mmenu_attr);
			
			if (displayModeFlag || !unitFlag) {
				__msg("huang  displayModeFlag \n");
				GUI_SetTimer(msg->h_deswin, DISPLAY_MODE_ID, 100, NULL);
			}
			
			SetCMDLevel(p_data,2);//拉线器归零
    		return EPDK_OK;
    		//break;
	    }
	case GUI_MSG_KEY:
    {
	
		return EPDK_OK;
	}
    case GUI_MSG_TOUCH:	
	{
		//mmenu_touch_proc(msg);
        return EPDK_OK;
	}			
    case GUI_MSG_TIMER:
    {  
		
		mmenu_attr_t *mmenu_attr;

		 mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);

		if (msg->dwAddData1 == DISPLAY_MODE_ID) {
			if (curRpm) {
				displayModeTime = 0;
			} else {
				displayModeTime++;
			}
			fanTime++;
			if (!unitFlag) {
				//__msg("huang    english   \n");
				if(fanTime == 300) {
					//__msg("set off   \n");
					SetFanOnOff(mmenu_attr->p_cmd_data,2);
					fanFlag = 0;
					fanTime=0;
					//if(!displayModeFlag){
					//GUI_KillTimer(msg->h_deswin, DISPLAY_MODE_ID);
					//}
				}
				
			}

			if(displayModeFlag){
				if (displayModeTime >= 1800) {
					ScreenCloseFlag = EPDK_TRUE;
					SetRunLevel(mmenu_attr->p_cmd_data,1);
				#ifdef AC00280
					SetRunINC(mmenu_attr->p_cmd_data,1);
				#else ifdef AC00285
					SetRunSTRDIE(mmenu_attr->p_cmd_data,0);
				#endif
					SetFanOnOff(mmenu_attr->p_cmd_data,2);
					fanFlag = 0;
					dsk_display_lcd_off();	
					GUI_KillTimer(msg->h_deswin, DISPLAY_MODE_ID);
					
				}
			}
		}
		
		
        return EPDK_OK;
    }
	case GUI_MSG_COMMAND:
		return EPDK_OK;
	case GUI_MSG_WIN_WAKEUP:
		GUI_InvalidateRect(msg->h_deswin, NULL, EPDK_TRUE);
		return EPDK_OK;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

__s32 main_menu_win_create(H_WIN h_parent, mmenu_para_t *para,com_data_para_t *com_data)
{
	__gui_framewincreate_para_t framewin_para;
	mmenu_para_t *mmenu_para;
	FB fb;
	
	
	GUI_LyrWinGetFB(para->layer, &fb);
    __here__;

	mmenu_para = (mmenu_para_t *)esMEMS_Balloc(sizeof(mmenu_para_t));
    if(!mmenu_para)
    {
    __here__;
        return EPDK_FAIL;
    }
	p_data = com_data;

	eLIBs_memset((void*)mmenu_para, 0, sizeof(mmenu_para_t));
	mmenu_para->mmenu_font = para->mmenu_font;
	mmenu_para->layer = para->layer;
	mmenu_para->focus_id = para->focus_id;
	pageData[0] = 0x60;
	readPage(ProgramRDData,pageData);
	childModeFlag = ProgramRDData[4];
	keyTonFlag = ProgramRDData[3];
	//__msg("huang#####   childModeLock =%d  \n",childModeFlag);
	displayModeFlag = ProgramRDData[1];
	unitFlag = ProgramRDData[0];
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	"main_menu_win",
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = MAIN_MENU_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_main_menu_Proc);
	framewin_para.rect.x =0;
	framewin_para.rect.y = 0;
	framewin_para.rect.width= fb.size.width;
	framewin_para.rect.height = fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red = 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue = 0;
	framewin_para.attr =  (void *)mmenu_para;
	framewin_para.hLayer = para->layer;
    __here__;

	return (GUI_FrmWinCreate(&framewin_para));
}

void main_menu_win_delete(H_WIN mm_win)
{
	GUI_FrmWinDelete(mm_win);
}

void main_menu_res_uninit(H_WIN hwin)
{    
    mmenu_attr_t * mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(hwin);
	
    if(mmenu_attr)
    {
        uninit_mmenu_res(mmenu_attr);
    }
}

void main_menu_res_init(H_WIN hwin)
{
    mmenu_attr_t * mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(hwin);
    if(mmenu_attr)
    {
        init_mmenu_res(mmenu_attr);
    }
}

