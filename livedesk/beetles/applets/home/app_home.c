/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: app_main.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "beetles_app.h"
#include "app_home.h"

#include "manuel_bar.h"
#include "sports_bar.h"
#include "home_uipara.h"
#include "msgbox.h"
#include "app_home_i.h"
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

#define GET_NORMAL_DATA_ID  0x1000

#define READ_BT_DATA		0x1100

#define WRITE_BT_DATA		0x1110

#define NORMAL_DATA_CHECK_TIME 	5

#define READ_BT_TIME				1

#define WRITE_BT_TIME              1

static __s32 key_status = 0;

static __bool KEYAction = EPDK_TRUE;
static __bool StartAction =EPDK_FALSE;

static __s32 currentMode = 4;
static __u8 curMode;  //BT所处模式
static __u8 BTMode;
static __u8 BTTime;
static __u8 BTMAXLevel;
static __u8 BTTARGHR;
static __u16 errSendCount = 0;
static __u16 ERRStatusCount = 0;

static __u8 BTLen = 0;

static __u8 ProgramRDData[32]={0};
static __u8 ProgramWRData[32]={0};
static __u8 pageData[2];

static __bool readFlag = EPDK_FALSE;

static __bool BTConnectFlag = EPDK_FALSE;

static __bool E2Flag = EPDK_FALSE;

static __u8 isConFlag = 0;

static H_WIN h_deswin_current;

static __u8 HRFlag = 0;

static __u8 HR0 = 0;

static __u8 HRW = 0;

static __u8 HRMode;

static __u8 readLen = 0;

static __u16 fanFlag = 0;

static __u16 SP_Data[6];

static __u16 BT_Weight;
static __u16 BT_Age;

static __u8 ReadBTbuf[30];
static __u8 ReadBTData[10];
static __u8 SendBTData[30];
static __u8 readLen;
static __u8 levelDot[18];
static __u8 levelDotMax;
static __u8 levelDotMin;

static __bool unitFlag = EPDK_FALSE;

static __bool keyTonFlag;

static __u8 unitCode ;

static __bool readBTFlag = EPDK_FALSE;

static __bool writeBTFlag = EPDK_FALSE;

 //280  285
 #if 1
static __u16 counter[20] = {
	0,20,39,58,77,96,115,134,153,172,
	191,210,229,248,267,286,305,324,343,362,
};
#endif
 // 282 286
#if 0 
static __u16 counter[20] = {
	0,3,28,33,36,40,44,48,51,55,
	60,64,69,74,80,87,93,98,103,105,
};
#endif

static __u16 maxCounter = 20;

typedef struct tag_home_para
{
	H_WIN	h_app_main;
	H_WIN 	h_mmenu;
	H_WIN 	h_submenu;
	H_WIN	h_calendar;
	H_WIN	h_msgbox;

	H_WIN	lyr_mmenu;
	H_WIN	lyr_smenu;
	H_WIN no_disk_sence;//sun++ for disk

	__s32	focus_id;
	__s32   first_item;
	GUI_FONT *main_font;

    __s32   focus_smenu_item;
	void		*core;
	__u32	root_type;

	__u32	disk_type;  //cai++ for main_menu_change

	__u8  timeFlag;
}home_para_t;
/******************************************************************************************************************************************************
				主界面尝试使用32位图层  //cai++

*******************************************************************************************************************************************************/
 H_LYR home_32bpp_layer_create(RECT *rect, __u8 pipe)
{
	H_LYR layer = NULL;


	FB  fb =
	{
	    {0, 0},                                   		/* size      */
	    {0, 0, 0},                                      /* buffer    */
	    //{FB_TYPE_RGB, {PIXEL_MONO_8BPP, 0, (__rgb_seq_t)0}},    /* fmt       */
	    {FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, (__rgb_seq_t)0}},    /* fmt       */
	    //{FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, 0}},    /* fmt       */
	};

	__disp_layer_para_t lstlyr =
	{
	    //DISP_LAYER_WORK_MODE_PALETTE,                    /* mode      */
	    DISP_LAYER_WORK_MODE_NORMAL,
	    0,                                              /* ck_mode   */
	    0,                                              /* alpha_en  */
	    0,                                              /* alpha_val */
	    1,                                              /* pipe      */
	    0xff,                                           /* prio      */
	    {0, 0, 0, 0},                           		/* screen    */
	    {0, 0, 0, 0},                               	/* source    */
	    DISP_LAYER_OUTPUT_CHN_DE_CH1,                   /* channel   */
	   NULL                                            /* fb        */
    };	                                      
    __layerwincreate_para_t lyrcreate_info =  
    {                                         
        "main menu layer",                       
      	NULL,                                 
      	GUI_LYRWIN_STA_SUSPEND,               
      	GUI_LYRWIN_NORMAL                     
    };	
	
    fb.size.width		= rect->width;            
    fb.size.height		= rect->height;	        
    fb.fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888; //PIXEL_MONO_8BPP; //PIXEL_COLOR_ARGB8888;
	
	lstlyr.src_win.x  		= 0;
	lstlyr.src_win.y  		= 0;
	lstlyr.src_win.width 	= rect->width;
	lstlyr.src_win.height 	= rect->height;
	
	lstlyr.scn_win.x		= rect->x;
	lstlyr.scn_win.y		= rect->y;
	lstlyr.scn_win.width  	= rect->width;
	lstlyr.scn_win.height 	= rect->height;

	lstlyr.pipe = pipe;	
	lstlyr.fb = &fb;
	lyrcreate_info.lyrpara = &lstlyr;
	
	layer = GUI_LyrWinCreate(&lyrcreate_info);
	
	if( !layer )
	{
		__err("app music layer create error !\n");
	}
	return layer;	
}



//sun++	for 创建对话框提示无磁盘
static __s32 __home_nodisk_dlg_scene_create(home_para_t *home_para)
{
	__s32 ret;	
	
	if (NULL == home_para)
	{
		__err("invalid para...\n");
		return EPDK_FAIL;
	}
	
 	{        
        if (NULL == home_para->no_disk_sence)
        {
            __s32 lang_id[]={STRING_HOME_CUE, STRING_HOME_NULL_DISK};

            default_dialog(home_para->no_disk_sence, home_para->h_app_main, HOME_NODISK_DLG_ID, ADLG_OK, lang_id);
        }
                
 		if (NULL == home_para->no_disk_sence)
 		{
 			__msg("default_dialog fail...\n");
 			return EPDK_FAIL;
 		}
 	}
	
	return EPDK_OK;
}

static __s32 __home_nodisk_dlg_scene_delete(home_para_t *home_para)
{
	__s32 ret;	
	
	if (NULL == home_para)
	{
		__err("invalid para...\n");
		return EPDK_FAIL;
	}
	
 	{
 		if (home_para->no_disk_sence)
 		{
 			app_dialog_destroy(home_para->no_disk_sence);
 			home_para->no_disk_sence = NULL;
 		}
 	}
	
	return EPDK_OK;
}


//应用退出到桌面时设置回当前背景
void __app_home_change_to_home_bg(void)
{
    reg_init_para_t* para;
    para = dsk_reg_get_para_by_app(REG_APP_INIT);
    if(para)
    {
        
#if 0 // Dragon 130422 Temperary solution
		gscene_bgd_set_flag(para->bgd_flag_in_main);
		gscene_bgd_refresh();													   
#else
		__s32 ret;
		
		ret = gscene_bgd_get_flag();

		if(ret != para->bgd_flag_in_main)
        {
            gscene_bgd_set_flag(para->bgd_flag_in_main);
            gscene_bgd_refresh();                                                      
        }   
        else
        {
            __msg("ret == para->bgd_flag_in_main, not need change bg...\n");
        }
#endif //Dragon 130422
	}
    else
    {
        __msg("dsk_reg_get_para_by_app fail...\n");
    }

    gscene_bgd_set_state(BGD_STATUS_SHOW);
    
}

void  main_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
{
	__gui_msg_t msg;

	msg.h_deswin = GUI_WinGetParent(hwin);
	msg.h_srcwin = NULL;
	msg.id = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hwin), id);
	msg.dwAddData2 = data1;
	msg.dwReserved = data2;

	GUI_SendNotifyMessage(&msg);
}

/*
	主界面到Manuel
*/

void  main_cmd2manuel(H_WIN hwin, __s32 id, __s32 data1, __u16 *sportsData)
{
	__gui_msg_t msg;
	__u8 i;
	//__msg("huang###################    main_cmd2manuel    \n");

	h_deswin_current = hwin;
	msg.h_deswin = GUI_WinGetParent(hwin);
	msg.h_srcwin = NULL;
	msg.id = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hwin), id);
	msg.dwAddData2 = data1;
	//msg.dwReserved = data2;
	for (i = 0; i < 6; i++) {
		SP_Data[i] = sportsData[i];
	}
	

	GUI_SendNotifyMessage(&msg);
}


static void check_disk(home_para_t *home_para)
{
    __s32 i;
    __s32 ret;
    __s32 cnt;
    char diskname[RAT_MAX_PARTITION][4];

     {
        __target_para_t target;
        esKSRV_GetTargetPara(&target);
        if(target.debugenable == EPDK_TRUE)////如果是调试，则默认为有外设//112350
        {
            home_para->root_type = RAT_TF<<8;
            return ;        
        }
    }   

	__msg("**********check_disk**********\n");
    cnt = 0;
    home_para->root_type = RAT_UNKNOWN;    

    ret = rat_get_partition_name(RAT_USB_DISK, diskname,1);
    if(EPDK_OK == ret)
    {
    	i = 0;    
    	//while (i < RAT_MAX_PARTITION)
    	{
    		if (diskname[i][0])
    		{
    			__msg("SD : diskname[%d]=%s\n", i, diskname[i]);
                home_para->root_type += RAT_USB;
                cnt++;
    		}						
    		i++;
    	}
    }

    ret = rat_get_partition_name(RAT_SD_CARD, diskname,1);
    if(EPDK_OK == ret)
    {
    	i = 0;    
    	//while (i < RAT_MAX_PARTITION)
    	{
    		if (diskname[i][0])
    		{
    			__msg("SD : diskname[%d]=%s\n", i, diskname[i]);
                home_para->root_type += RAT_TF<<8;
                cnt++;
    		}						
    		i++;
    	}
    }
    
}

#if 0
static void app_sub_menu_create(home_para_t *home_para)
{
	smenu_para_t smenu_para;
	RECT	rect;
	home_uipara_t * home_ui_para;

    home_ui_para = home_get_ui_para(GUI_GetScnDir());
	__msg("经过这里\n");
    if(!home_ui_para)
    {
        __err("home_ui_para is null...\n");
        return ;
    }
		
	rect.x = (home_para->focus_id-home_para->first_item)*home_ui_para->item_width;
	rect.y = home_ui_para->lyr_sub_item.y;
	rect.width = home_ui_para->lyr_sub_item.w;
	rect.height = home_ui_para->lyr_sub_item.h;

	if (rect.x+rect.width >= home_ui_para->lyr_main_menu.w)
	{
		rect.x = home_ui_para->lyr_main_menu.w-rect.width;
	}
	
	home_para->lyr_smenu = home_8bpp_layer_create(&rect, 1);   
	
	smenu_para.layer = home_para->lyr_smenu;
	smenu_para.sub_menu_id = home_para->focus_id;
	smenu_para.smenu_font = home_para->main_font;
	smenu_para.root_type = home_para->root_type;
    smenu_para.focus_item = home_para->focus_smenu_item;
    __msg("home_para->focus_smenu_item=%d\n", home_para->focus_smenu_item);
	
	home_para->h_submenu = sub_menu_win_create(home_para->h_app_main, &smenu_para);	
    __msg("home_para->h_submenu=%x\n", home_para->h_submenu);
}
#endif

static Write_ToBT_OK(__u8 cmd) 
{
	SendBTData[0]= BT_HEAD;
	SendBTData[1]= 0x04;
	SendBTData[2] = 0x00;
	SendBTData[3] = cmd;
	SendBTData[4] = 0x4F;
	SendBTData[5] = 0x4B;
	SendBTData[6] = BT_END;
	bluetooth_writebuf(SendBTData,7);
}

static void Write_OK() 
{
	SendBTData[0]= BT_HEAD;
	SendBTData[1]= 0x04;
	SendBTData[2] = 0x00;
	SendBTData[3] = 0x09;
	SendBTData[4] = 0x4F;
	SendBTData[5] = 0x4B;
	SendBTData[6] = BT_END;
	bluetooth_writebuf(SendBTData,7);
}

static void write_error_status()
{
	SendBTData[0]= BT_HEAD;
	SendBTData[1]= 0x02;
	SendBTData[2] = ERRSTATUS_CMD;
	SendBTData[3] = 0x00;
	SendBTData[4] = BT_END;

	bluetooth_writebuf(SendBTData,5);
}

static void app_main_menu_create(home_para_t *home_para,com_data_para_t *com_data)
{
	RECT	rect;
    mmenu_para_t mmenu_para;
    home_uipara_t * home_ui_para;

	__msg("huang####  app_main_menu_create \n");
    home_ui_para = home_get_ui_para(GUI_GetScnDir());
    if(!home_ui_para)
    {
        __err("home_ui_para is null...\n");
        return ;
    }

	rect.x = home_ui_para->lyr_main_menu.x;
	rect.y = home_ui_para->lyr_main_menu.y;
	rect.width = home_ui_para->lyr_main_menu.w;
	rect.height = home_ui_para->lyr_main_menu.h;

	home_para->lyr_mmenu = home_32bpp_layer_create(&rect, 1);//home_8bpp_layer_create(&rect, 1);

	mmenu_para.mmenu_font = home_para->main_font;
	mmenu_para.focus_id = home_para->focus_id;
	mmenu_para.layer = home_para->lyr_mmenu;
	home_para->h_mmenu = main_menu_win_create(home_para->h_app_main, &mmenu_para,com_data);
    __msg("home_para->h_mmenu=%x\n", home_para->h_mmenu);
	
	GUI_WinSetFocusChild(home_para->h_mmenu);
}

static void main_menu_bar_creat(home_para_t *home_para)
{
	mmenu_para_t mmenu_para;
   com_data_para_t *com_data = NULL;

	//__msg("huang####  manuel_bar_creat \n");


	mmenu_para.mmenu_font = home_para->main_font;
	mmenu_para.focus_id = home_para->focus_id;
	mmenu_para.layer = home_para->lyr_mmenu;
	home_para->h_mmenu = main_menu_win_create(home_para->h_app_main, &mmenu_para,com_data);
   // __msg("home_para->h_mmenu=%x\n", home_para->h_mmenu);
	
	GUI_WinSetFocusChild(home_para->h_mmenu);
}

static void manuel_bar_creat(home_para_t *home_para)
{
    mmenu_para_t mmenu_para;
   

	__msg("huang####  manuel_bar_creat \n");


	mmenu_para.mmenu_font = home_para->main_font;
	mmenu_para.focus_id = home_para->focus_id;
	mmenu_para.layer = home_para->lyr_mmenu;

	if (BTConnectFlag) {
		mmenu_para.age = BT_Age;
		mmenu_para.weight = BT_Weight;
		mmenu_para.time = BTTime;
		mmenu_para.BTSET_Flag = EPDK_TRUE;
	}else {
		//mmenu_para.age = BT_Age;
		//mmenu_para.weight = BT_Weight;
		//mmenu_para.time = BTTime;
		mmenu_para.BTSET_Flag = EPDK_FALSE;
	}
	home_para->h_mmenu = manuel_bar_win_create(home_para->h_app_main, &mmenu_para);
    __msg("home_para->h_mmenu=%x\n", home_para->h_mmenu);
	
	GUI_WinSetFocusChild(home_para->h_mmenu);

}

static void preset_bar_creat(home_para_t *home_para, __u8 id)
{
    mmenu_para_t mmenu_para;
   

	__msg("huang####  manuel_bar_creat \n");

	if (BTConnectFlag) {
		mmenu_para.age = BT_Age;
		mmenu_para.weight = BT_Weight;
		mmenu_para.time = BTTime;
		mmenu_para.maxLevel = BTMAXLevel;
		mmenu_para.BTSET_Flag = EPDK_TRUE;
	}else {
		//mmenu_para.age = BT_Age;
		//mmenu_para.weight = BT_Weight;
		//mmenu_para.time = BTTime;
		mmenu_para.BTSET_Flag = EPDK_FALSE;
	}
	mmenu_para.mmenu_font = home_para->main_font;
	mmenu_para.focus_id = home_para->focus_id;
	mmenu_para.layer = home_para->lyr_mmenu;
	home_para->h_mmenu = preset_bar_win_create(home_para->h_app_main, &mmenu_para,id);
    __msg("home_para->h_mmenu=%x\n", home_para->h_mmenu);
	
	GUI_WinSetFocusChild(home_para->h_mmenu);

}

static void hr_bar_creat(home_para_t *home_para, __u8 id)
{
    mmenu_para_t mmenu_para;
   

	__msg("huang####  manuel_bar_creat \n");

	if (BTConnectFlag) {
		mmenu_para.age = BT_Age;
		mmenu_para.weight = BT_Weight;
		mmenu_para.time = BTTime;
		mmenu_para.targHR = BTTARGHR;
		mmenu_para.BTSET_Flag = EPDK_TRUE;
	}else {
		//mmenu_para.age = BT_Age;
		//mmenu_para.weight = BT_Weight;
		//mmenu_para.time = BTTime;
		mmenu_para.BTSET_Flag = EPDK_FALSE;
	}
	mmenu_para.mmenu_font = home_para->main_font;
	mmenu_para.focus_id = home_para->focus_id;
	mmenu_para.layer = home_para->lyr_mmenu;
	home_para->h_mmenu = hr_bar_win_create(home_para->h_app_main, &mmenu_para,id);
    __msg("home_para->h_mmenu=%x\n", home_para->h_mmenu);
	
	GUI_WinSetFocusChild(home_para->h_mmenu);

}

static void user_bar_creat(home_para_t *home_para, __u8 id)
{
    mmenu_para_t mmenu_para;
   

	__msg("huang####  manuel_bar_creat \n");

	if (BTConnectFlag) {
		mmenu_para.age = BT_Age;
		mmenu_para.weight = BT_Weight;
		mmenu_para.time = BTTime;
		mmenu_para.targHR = BTTARGHR;
		mmenu_para.dot = levelDot;
		mmenu_para.BTSET_Flag = EPDK_TRUE;
	}else {
		//mmenu_para.age = BT_Age;
		//mmenu_para.weight = BT_Weight;
		//mmenu_para.time = BTTime;
		mmenu_para.BTSET_Flag = EPDK_FALSE;
	}

	mmenu_para.mmenu_font = home_para->main_font;
	mmenu_para.focus_id = home_para->focus_id;
	mmenu_para.layer = home_para->lyr_mmenu;
	home_para->h_mmenu = user_bar_win_create(home_para->h_app_main, &mmenu_para,id);
    __msg("home_para->h_mmenu=%x\n", home_para->h_mmenu);
	
	GUI_WinSetFocusChild(home_para->h_mmenu);

}


static void factory_bar_creat(home_para_t *home_para)
{
    mmenu_para_t mmenu_para;
   

	__msg("huang####  factory_bar_creat \n");


	mmenu_para.mmenu_font = home_para->main_font;
	mmenu_para.focus_id = home_para->focus_id;
	mmenu_para.layer = home_para->lyr_mmenu;
	home_para->h_mmenu = factory_bar_win_create(home_para->h_app_main, &mmenu_para);
    __msg("home_para->h_mmenu=%x\n", home_para->h_mmenu);
	
	GUI_WinSetFocusChild(home_para->h_mmenu);

}


static void sports_bar_creat(home_para_t *home_para)
{
    mmenu_para_t mmenu_para;
   

	__msg("huang####  sports_bar_creat \n");


	mmenu_para.mmenu_font = home_para->main_font;
	mmenu_para.timeFlag = home_para->timeFlag;
	mmenu_para.layer = home_para->lyr_mmenu;
	mmenu_para.age = SP_Data[0];//mmenu_attr_tmp->manuel_set_p[0].manuel_setNUM;
	mmenu_para.weight = SP_Data[1];//mmenu_attr_tmp->manuel_set_p[1].manuel_setNUM;
	mmenu_para.time  = SP_Data[2];//mmenu_attr_tmp->manuel_set_p[2].manuel_setNUM;
	mmenu_para.maxLevel = SP_Data[3];//mmenu_attr_tmp->manuel_set_p[3].manuel_setNUM;
	mmenu_para.curID = SP_Data[4];//mmenu_attr_tmp->curID; 
	mmenu_para.incPro= SP_Data[5];
	mmenu_para.BTSET_Flag = BTConnectFlag;
	__msg("huang####  sports_bar_creat  time =  %d \n", mmenu_para.time);
	
	home_para->h_mmenu = sports_bar_win_create(home_para->h_app_main, &mmenu_para);
    __msg("home_para->h_mmenu=%x\n", home_para->h_mmenu);
	
	GUI_WinSetFocusChild(home_para->h_mmenu);

	

}

static void sports_hr_bar_creat(home_para_t *home_para)
{
    mmenu_para_t mmenu_para;
   

	__msg("huang####  sports_hr_bar_creat \n");


	mmenu_para.mmenu_font = home_para->main_font;
	mmenu_para.timeFlag = home_para->timeFlag;
	mmenu_para.layer = home_para->lyr_mmenu;
	mmenu_para.age = SP_Data[0];//mmenu_attr_tmp->manuel_set_p[0].manuel_setNUM;
	mmenu_para.weight = SP_Data[1];//mmenu_attr_tmp->manuel_set_p[1].manuel_setNUM;
	mmenu_para.time  = SP_Data[2];//mmenu_attr_tmp->manuel_set_p[2].manuel_setNUM;
	mmenu_para.targHR = SP_Data[3];//mmenu_attr_tmp->manuel_set_p[3].manuel_setNUM;
	mmenu_para.curID = SP_Data[4];//mmenu_attr_tmp->curID; 
	mmenu_para.BTSET_Flag = BTConnectFlag;
	mmenu_para.HRFlag = HRFlag;
	mmenu_para.HR0 = HR0;
	
	__msg("huang####  sports_bar_creat  tarhr =  %d \n", mmenu_para.targHR);
	
	home_para->h_mmenu = sports_hr_bar_win_create(home_para->h_app_main, &mmenu_para);
    __msg("home_para->h_mmenu=%x\n", home_para->h_mmenu);
	
	GUI_WinSetFocusChild(home_para->h_mmenu);

	

}


static __s32 init_cmd_data(mmenu_attr_t *mmenu_attr)
{
	com_data_para_t *com_data;

	com_data = (com_data_para_t *)esMEMS_Balloc(sizeof(com_data_para_t));
    if(!com_data)
    {
        return EPDK_FAIL;                
    }
	eLIBs_memset((void *)com_data, 0, sizeof(com_data_para_t));

	com_data->com_func = COM_FUNC_IDLE;
	com_data->crtl_com = RUN_CMD_RUN;
	//com_data->direction_now = 1;
	//com_data->direction_set = 1;
	com_data->E2 = EPDK_FALSE;

	
	mmenu_attr->p_cmd_data = com_data;

	 return EPDK_OK;
}


static __s32 app_home_proc(__gui_msg_t *msg)
{
	
	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
			home_para_t *home_para;
			root_para_t *root_para;	
			mmenu_attr_t *mmenu_attr;
			
			__u8 i;
			__u16 maxInc;

			__msg("huang####   app_main_menu_create \n");
			open_uart2();
			//esKRNL_TimeDly(5);
			open_uart0();//初始化串口
			//init_buffer();
			#if 1
			silview_i2c_init();
			while(!GT02_Check()) {
			}
			GT02_Init();
			while(!AppAuthority());
			//silview_i2c_deinit();
			#endif
			root_para = (root_para_t *)GUI_WinGetAttr(msg->h_deswin);
			home_para = (home_para_t *)esMEMS_Balloc(sizeof(home_para_t));
            if(!home_para)
            {
                return EPDK_FAIL;
            }
			//mmenu_attr_tmp = (mmenu_attr_t *)esMEMS_Balloc(sizeof(mmenu_attr_t));
		   // if(!mmenu_attr_tmp)
		   // {
		    //    return ;                
		   // }
		
			//eLIBs_memset((void *)mmenu_attr_tmp, 0, sizeof(mmenu_attr_t));
			for (i = 0; i< 6; i++) {
				SP_Data[i] = 0;
			}
		
			eLIBs_memset((void *)home_para, 0, sizeof(home_para_t));
			home_para->main_font = root_para->font;
			home_para->h_app_main = msg->h_deswin;
			home_para->focus_id = 0;			// 	wqx 20130329	
			home_para->first_item = 0;

			home_para->disk_type = root_para->late_root_type; //cai++ for main_menu_change

			
			com_set_palette_by_id(ID_HOME_PAL_BMP);

            //background
          //  gscene_bgd_set_state(BGD_STATUS_SHOW);           
			GUI_WinSetAddData(msg->h_deswin, (__u32)home_para); 
		
			mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			if (!mmenu_attr)
			{
				__err("mmenu_attr is null...\n");
                return EPDK_OK;
			}
			init_cmd_data(mmenu_attr);

			SeclectMac(mmenu_attr->p_cmd_data , 1);//椭圆机
			//SeclectMac(mmenu_attr->p_cmd_data , 2);//脚踏车

			//SetPWM(mmenu_attr->p_cmd_data,75);
			SetCounter(mmenu_attr->p_cmd_data,counter,maxCounter);
			SetCMDLevel(mmenu_attr->p_cmd_data,2);//拉线器归零
		#ifdef AC00280
			
			maxInc = 20;
			pageData[0] = 0x60;
			readPage(ProgramRDData,pageData);
			unitFlag = ProgramRDData[0];
			keyTonFlag = ProgramRDData[3];
			//__msg("huang#####  keyTonFlag = %d\n", keyTonFlag);
			if (unitFlag) {
				unitCode = 0x00;
			} else {
				unitCode = 0x01;
			}
			SetRunMAXINC(mmenu_attr->p_cmd_data,maxInc);
			if (ProgramRDData[6]) {
			//__msg("huang######  maxadc = %d  \n", ProgramRDData[5]);
				SetMaxADC(mmenu_attr->p_cmd_data,ProgramRDData[5]);
			//__msg("huang######  maxadc = %d  \n", ProgramRDData[6]);
				SetMinADC(mmenu_attr->p_cmd_data,ProgramRDData[6]);
			} 	

			//InitINC(mmenu_attr->p_cmd_data);
		#else ifdef AC00285
			SetRunMAXINC(mmenu_attr->p_cmd_data,13);
			SetMaxADC(mmenu_attr->p_cmd_data,222);
			SetMinADC(mmenu_attr->p_cmd_data,29);
		#endif
			GUI_SetTimer(msg->h_deswin, GET_NORMAL_DATA_ID, NORMAL_DATA_CHECK_TIME, NULL); 
			//主菜单
			app_main_menu_create(home_para,mmenu_attr->p_cmd_data);

			GUI_LyrWinSetSta(home_para->lyr_mmenu, GUI_LYRWIN_STA_ON);
		
            //gscene_hbar_set_state(HBAR_ST_SHOW);

        //    gscene_bgd_set_bottom();
            GUI_WinSetFocusChild(home_para->h_mmenu);
			#if 0
			GUI_WinSetAddData(msg->h_deswin, (__u32)home_para); 
		
			mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			if (!mmenu_attr)
			{
				__err("mmenu_attr is null...\n");
                return EPDK_OK;
			}
			#endif
			mmenu_attr->p_cmd_data->E2 = EPDK_FALSE;
			
			eLIBs_memset(ReadBTData, 0, 10);
			
			//GUI_SetTimer(msg->h_deswin, READ_BT_DATA, READ_BT_TIME, NULL); 
			GUI_SetTimer(msg->h_deswin, WRITE_BT_DATA, WRITE_BT_TIME, NULL); 
		}
	
			
		return EPDK_OK;
	case GUI_MSG_CLOSE:
		GUI_ManWinDelete(msg->h_deswin);
		return EPDK_OK;
	case GUI_MSG_DESTROY:
		{
			home_para_t *home_para;

			home_para = (home_para_t *)GUI_WinGetAddData(msg->h_deswin);
			
			GUI_LyrWinDelete(home_para->lyr_mmenu);
			//GUI_LyrWinDelete(home_para->lyr_smenu);

			home_para->lyr_mmenu = 0;
			//home_para->lyr_smenu = 0;
            esMEMS_Bfree(home_para, sizeof(home_para_t));
			//esMEMS_Bfree(mmenu_attr_tmp, sizeof(mmenu_attr_t));
		}
		return EPDK_OK;
	case GUI_MSG_TIMER:
	{
			mmenu_attr_t *mmenu_attr;
			home_para_t *home_para;
			__u8 i;
			
			

			mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			home_para = (home_para_t *)GUI_WinGetAddData(msg->h_deswin);
			if (!mmenu_attr)
			{
				__err("mmenu_attr is null...\n");
                return EPDK_OK;
			}

		if (msg->dwAddData1 == GET_NORMAL_DATA_ID) {
      
			getNormalData(mmenu_attr->p_cmd_data);
			HR0 = mmenu_attr->p_cmd_data->WirHr;
	  		if (BTConnectFlag && !KEYAction){
				if (StartAction) {
					if (mmenu_attr->p_cmd_data->key_value == START_KEY || mmenu_attr->p_cmd_data->key_value == STOP_KEY) {
						KEYAction = EPDK_TRUE;
					} else {
						mmenu_attr->p_cmd_data->key_value = 0;
					}
				} else {
					mmenu_attr->p_cmd_data->key_value = 0;
				}
				
			} 
			//__msg("huang  key = 0x%x  \n", mmenu_attr->p_cmd_data->key_value);
			if(mmenu_attr->p_cmd_data->key_value != 0x5 && mmenu_attr->p_cmd_data->key_value != 0x6 &&
			   mmenu_attr->p_cmd_data->key_value != 0x9 && mmenu_attr->p_cmd_data->key_value != 0xa &&
			   mmenu_attr->p_cmd_data->key_value != 0x49 && mmenu_attr->p_cmd_data->key_value != 0x4a && 
			   mmenu_attr->p_cmd_data->key_value !=0x4d && mmenu_attr->p_cmd_data->key_value != 0x4f &&
			   mmenu_attr->p_cmd_data->key_value != FACTORY_1_KEY  &&  mmenu_attr->p_cmd_data->key_value != FACTORY_2_KEY &&
			   mmenu_attr->p_cmd_data->key_value != FACTORY_3_KEY  &&   mmenu_attr->p_cmd_data->key_value != DISPLAY_MODE_KEY &&
			   mmenu_attr->p_cmd_data->key_value != STOP_LONG_KEY && mmenu_attr->p_cmd_data->key_value != UNLOCK_CHILD_KEY &&
			   mmenu_attr->p_cmd_data->key_value != LEVELPLUSE_QUICK_LONG_KEY && mmenu_attr->p_cmd_data->key_value != LEVELPLUSE_LONG_KEY &&
			   mmenu_attr->p_cmd_data->key_value != LEVELDEC_LONG_KEY && mmenu_attr->p_cmd_data->key_value != LEVELDEC_QUICK_LONG_KEY &&
			   mmenu_attr->p_cmd_data->key_value != INCPLUSE_LONG_KEY && mmenu_attr->p_cmd_data->key_value != INCPLUSE_QUICK_LONG_KEY &&
			   mmenu_attr->p_cmd_data->key_value != INCDEC_LONG_KEY && mmenu_attr->p_cmd_data->key_value != INCDEC_QUICK_LONG_KEY )
			{
				if (mmenu_attr->p_cmd_data->key_value > 0)  {
					key_status ++;
					if (key_status == 1) {//按下
				  		//__msg("huang  key = 0x%x  \n", mmenu_attr->p_cmd_data->key_value);//上报键值
				  		msg->dwAddData1 = mmenu_attr->p_cmd_data->key_value;
						if (currentMode == ID_MAIN) {
							
				  			//mainbar_key_proc(msg,mmenu_attr->p_cmd_data);
						}
					
						//__msg("huang#####   hr flag = %d\n", mmenu_attr->p_cmd_data->HrFlag);
						//__msg("huang#####   hr hand = %d\n", mmenu_attr->p_cmd_data->HandHr);
						//__msg("huang#####   hr wir = %d\n", mmenu_attr->p_cmd_data->WirHr);
						//__msg("huang#####   cur rpm = %d\n", mmenu_attr->p_cmd_data->curRpm);
						//__msg("huang#####   err = %d\n", mmenu_attr->p_cmd_data->error_code);
					}	 

					
				} else {
					key_status = 0; //弹起
				}			 
			} else {
				msg->dwAddData1 = mmenu_attr->p_cmd_data->key_value;
				if (currentMode == ID_MAIN) {
				  //	mainbar_key_proc(msg,mmenu_attr->p_cmd_data);
				}
			}
			HRFlag =  mmenu_attr->p_cmd_data->HrFlag;
			HRW = mmenu_attr->p_cmd_data->WirHr;
			
			switch(currentMode) {
							case ID_MAIN:
								mainbar_key_proc(msg,mmenu_attr->p_cmd_data);
								break;
							case ID_MANUAL:
								msg->h_deswin = home_para->h_mmenu;
								manuelbar_key_proc(msg);
								break;
							case ID_HILL:
								msg->h_deswin = home_para->h_mmenu;
								presetbar_key_proc(msg);
								break;
							case ID_FATBURN:
								msg->h_deswin = home_para->h_mmenu;
								presetbar_key_proc(msg);
								break;
							case ID_CARDIO:
								msg->h_deswin = home_para->h_mmenu;
								presetbar_key_proc(msg);
								break;
							case ID_STRENGTH:
								msg->h_deswin = home_para->h_mmenu;
								presetbar_key_proc(msg);
								break;
							case ID_INTERVAL:
								msg->h_deswin = home_para->h_mmenu;
								presetbar_key_proc(msg);
								break;
		
							case ID_HR1:
								msg->h_deswin = home_para->h_mmenu;
								hrbar_key_proc(msg);
								break;
							case ID_HR2:
								msg->h_deswin = home_para->h_mmenu;
								hrbar_key_proc(msg);
								break;
							case ID_USER1:
								msg->h_deswin = home_para->h_mmenu;
								userbar_key_proc(msg);
								break;
							case ID_USER2:
								msg->h_deswin = home_para->h_mmenu;
								userbar_key_proc(msg);
								break;
							case ID_SPORTS:
								msg->h_deswin = home_para->h_mmenu;
								sportsbar_key_proc(msg,mmenu_attr->p_cmd_data);
								//sportsbar_key_proc(msg);
								break;
							case ID_SPORTS_HR:
								msg->h_deswin = home_para->h_mmenu;
								sportsHRbar_key_proc(msg,mmenu_attr->p_cmd_data);
								break;
							case ID_FACTORY:
								msg->h_deswin = home_para->h_mmenu;
								factorybar_key_proc(msg,mmenu_attr->p_cmd_data);
								break;
								

						}
			
		}

		if (msg->dwAddData1 == WRITE_BT_DATA) {
			readLen=read_BTData(ReadBTData);
			//__msg("huang###  readbuf = 0x%x \n",ReadBTData[0]);
			if (writeBTFlag) {
				if (currentMode != ID_MAIN) {
					curMode = 0x80;
					KEYAction = EPDK_TRUE;
				} else {
					curMode = 0x01;
					KEYAction = EPDK_FALSE;
				}
				//esKRNL_TimeDly(30);
				SendBTData[0]= BT_HEAD;
				SendBTData[1]= 0x02;
				SendBTData[2] = MODE_CMD;
				SendBTData[3] = curMode;
				SendBTData[4] = BT_END;
				bluetooth_writebuf(SendBTData,5);

				writeBTFlag = EPDK_FALSE;
			}
			if (readLen) {
				
				if (readFlag) {
					BTLen++;
					ReadBTbuf[BTLen] = ReadBTData[0];
				}

				if (ReadBTData[0] == BT_END) {
					//__msg("###########################################\n");
					//for (i=0; i<= BTLen; i++) {
					//__msg("huang###  readbuf = 0x%x \n",ReadBTbuf[i]);
					//}
					readFlag = EPDK_FALSE;
					BTLen = 0;
				}

				if (ReadBTData[0] == BT_HEAD) {
					ReadBTbuf[0] = BT_HEAD;
					readFlag = EPDK_TRUE;
				}
			//__msg("###########################################\n");
			//__msg("###########################################\n");
			//__msg("###########################################\n");
			//for (i=0; i< readLen; i++) {
				//__msg("huang###  readbuf = 0x%x \n",ReadBTbuf[i]);
			//}
			//__msg("###########################################\n");
			//__msg("###########################################\n");
		//	__msg("###########################################\n");
			}
			if (ReadBTbuf[0] == BT_HEAD && ReadBTbuf[ReadBTbuf[1]+2] == BT_END) {
				if (ReadBTbuf[3] == ERRSTATUS_CMD && ReadBTbuf[4] == 0x4F) {
					ERRStatusCount += 100;
					//__msg("huang####### ERRStatusCount = %d\n",ERRStatusCount);
						if (ERRStatusCount == 100000) {
							ERRStatusCount = 0;
						}
				}
				switch(ReadBTbuf[2]) {
					case CONNECT_CMD:
						SendBTData[0]= BT_HEAD;
						SendBTData[1]= 0x09;
						SendBTData[2] = CONNECT_CMD;
						SendBTData[3] = 0x19;
						SendBTData[4] = 0x01;
						SendBTData[5] = unitCode;
						#ifdef AC00280
						SendBTData[6] = 0x14;
						#else ifdef AC00285
						SendBTData[6] = 0x0D;
						#endif
						SendBTData[7] = 0x14;
						SendBTData[8] = 0x12;
						SendBTData[9] =0x12;
						SendBTData[10] =0x12;
						SendBTData[11] = BT_END;
						bluetooth_writebuf(SendBTData,12);
						writeBTFlag = EPDK_TRUE;
						BTConnectFlag = EPDK_TRUE;
						open_buffer(mmenu_attr->p_cmd_data,1, keyTonFlag);
						break;
					case MODE_CMD:
						if (ReadBTbuf[3] == 1) {
							
							//KEYAction = EPDK_FALSE;
						}
						break;
					case SET_CMD:
						BT_Weight = ReadBTbuf[5]*256+ReadBTbuf[6];
						BT_Age = ReadBTbuf[4];
						Write_ToBT_OK(SET_CMD);
						break;
					case MODESET_CMD:
						BTMode = ReadBTbuf[4];
						Write_ToBT_OK(MODESET_CMD);
						break;
					case TIME_CMD:
						BTTime = ReadBTbuf[3];
						Write_ToBT_OK(TIME_CMD);
						break;
					case MAXLEVEL_CMD:
						BTMAXLevel =  ReadBTbuf[3]+1;
						Write_ToBT_OK(MAXLEVEL_CMD);
						break;
					case TARGHR_CMD:
						BTTARGHR = ReadBTbuf[3];
						Write_ToBT_OK(TARGHR_CMD);
						break;
					case HRMODE_CMD:
						if (HRFlag) {
							if (HRW) {
								HRMode = 0x02;
							}else {
								HRMode = 0x01;
							}
						} else {
							HRMode = 0x00;
						}
						SendBTData[0] = BT_HEAD;
						SendBTData[1] = 0x02;
						SendBTData[2] = HRMODE_CMD;
						SendBTData[3] = HRMode;
						SendBTData[4] = BT_END;
						bluetooth_writebuf(SendBTData,5);
						break;
					case USER_CMD:
					{
						__u8 i;
						
						BTMode = ReadBTbuf[3];
						levelDotMax = ReadBTbuf[4];
						levelDotMin = ReadBTbuf[5];
						for(i=0 ; i<18; i++) {
							levelDot[i] = ReadBTbuf[6+i];
							__msg("huang##### levelDot[%d] = %d  \n",levelDot[i]);
						}
						Write_ToBT_OK(USER_CMD);
						break;
					}
					
					case WAIT_CMD:
						if (2 == ReadBTbuf[3]) {
							SendBTData[0]= BT_HEAD;
							SendBTData[1]= 0x02;
							SendBTData[2] = WAIT_CMD;
							SendBTData[3] = 0x02;
							SendBTData[4] = BT_END;
							bluetooth_writebuf(SendBTData,5);
							if (currentMode == ID_MAIN) {
							switch(BTMode) {
								case 1:
									msg->dwAddData1 = MANUAL_KEY;
									break;
								case 2:
									msg->dwAddData1 = HILL_KEY;
									break;
								case 3:
									msg->dwAddData1 = FATBURN_KEY;
									break;
								case 4:
									msg->dwAddData1 = CARDIO_KEY;
									break;
								case 5:
									msg->dwAddData1 = STRENGTH_KEY;
									break;
								case 6:
									msg->dwAddData1 = INTERUAL_KEY;
									break;
								case 7:
									msg->dwAddData1 = USER1_KEY;
									break;
								case 8:
									msg->dwAddData1 = USER2_KEY;
									break;
								case 9:
									msg->dwAddData1 = HR1_KEY;
									break;
								case 10:
									msg->dwAddData1 = HR2_KEY;
									break;
							}
							mainbar_key_proc(msg,mmenu_attr->p_cmd_data); 
							StartAction = EPDK_TRUE;
						}
						}
						break;
					case ERRSTATUS_CMD:
						
						break;
					
						
				}
				for (i=0 ; i<30; i++) {
					ReadBTbuf[i] = 0;
				}
			}
		

			if (BTConnectFlag) {
				errSendCount++;
				//__msg("huang#######  errSendCount = %d\n",errSendCount);
				if (errSendCount == 100000) {
						errSendCount = 0;
					}
				if (!(errSendCount %100)) {
					write_error_status();
					isConFlag ++;
				}
			//	__msg("huang#######  isConFlag = %d\n",isConFlag);
				if (isConFlag > 3) {
					if ((errSendCount - ERRStatusCount) > 300) {
						BTConnectFlag= EPDK_FALSE;
						open_buffer(mmenu_attr->p_cmd_data,1, keyTonFlag);
						msg->dwAddData1 = BT_DISCONNECT;
						msg->h_deswin = home_para->h_mmenu;
								
						switch(currentMode) {
							case ID_MANUAL:
								manuelbar_key_proc(msg);
								break;
							case ID_USER1:
							case ID_USER2:
								userbar_key_proc(msg);
								break;
							case ID_HR1:
							case ID_HR2:
								hrbar_key_proc(msg);
								break;
							case ID_HILL:
							case ID_FATBURN:
							case ID_CARDIO:
							case ID_STRENGTH:
							case ID_INTERVAL:
								presetbar_key_proc(msg);
								break;
							case ID_SPORTS:
								sportsbar_key_proc(msg,mmenu_attr->p_cmd_data);
								//sportsbar_key_proc(msg);
								break;
							case ID_SPORTS_HR:
								sportsHRbar_key_proc(msg,mmenu_attr->p_cmd_data);
								break;
		
						}
						errSendCount = 0;
						ERRStatusCount = 0;
						__msg("huang#########  bt   no  con  \n");
					} else {
						errSendCount = 0;
						ERRStatusCount = 0;
					}
					isConFlag = 0;
				}
			}

		}
		
		break;
	}
	case GUI_MSG_KEY:
	{
		__msg("******app_home_proc GUI_MSG_KEY*****\n");
		
	}
		break;
	case GUI_MSG_TOUCH:
		break;
	case DSK_MSG_FS_PART_PLUGIN:
	case DSK_MSG_FS_PART_PLUGOUT:
		{					
			
		}
		return EPDK_OK;
	case DSK_APP_RESTORE:
		{
			home_para_t *home_para;

			home_para = (home_para_t *)GUI_WinGetAddData(msg->h_deswin);

            com_set_palette_by_id(ID_HOME_PAL_BMP);

            __app_home_change_to_home_bg();
                       
			GUI_LyrWinSetSta(home_para->lyr_mmenu, GUI_LYRWIN_STA_ON);
			//GUI_LyrWinSetSta(home_para->lyr_smenu, GUI_LYRWIN_STA_ON);	
            //gscene_hbar_set_state(HBAR_ST_SHOW);
            gscene_bgd_set_bottom();
            //gscene_hbar_set_state(HBAR_ST_SHOW);
//            GUI_LyrWinSetSta(home_para->lyr_total_bg, GUI_LYRWIN_STA_ON);
//            GUI_LyrWinSetBottom(home_para->lyr_total_bg);

//            GUI_BMP_Draw(dsk_theme_hdl2buf(home_para->total_bg_hdl)
//               , main_uipara.lyr_total_bg.x, main_uipara.lyr_total_bg.y);

            GUI_WinSetFocusChild(home_para->h_mmenu);
		}
		return EPDK_OK;

	case DSK_APP_SUSPEND:
	{
		home_para_t *home_para;

		home_para = (home_para_t *)GUI_WinGetAddData(msg->h_deswin);
		
		GUI_LyrWinSetSta(home_para->lyr_mmenu, GUI_LYRWIN_STA_SUSPEND);
        main_menu_res_uninit(home_para->h_mmenu);

		eLIBs_printf(" -- %s, %d,  home suspend -- \n", __FILE__, __LINE__);
	}
		return EPDK_OK;
	
	case GUI_MSG_COMMAND:
		{
			home_para_t *home_para;

			home_para = (home_para_t *)GUI_WinGetAddData(msg->h_deswin);

			//__msg("huang##################  1 = %d  ,   2 =  %d  \n", LOWORD(msg->dwAddData1), HIWORD(msg->dwAddData1));
		
			switch(HIWORD(msg->dwAddData1))
			{
				case ID_MAIN:
					switch(currentMode) {
						case ID_MAIN:
							main_menu_win_delete(home_para->h_mmenu);
							main_menu_res_uninit(home_para->h_mmenu);
							break;
						case ID_FACTORY:
							factory_bar_win_delete(home_para->h_mmenu);
							factory_bar_res_uninit(home_para->h_mmenu);
							keyTonFlag = msg->dwAddData2;
							break;
						case ID_SPORTS:
							sports_bar_win_delete(home_para->h_mmenu);
							sports_bar_res_uninit(home_para->h_mmenu);
							
							break;
						case ID_SPORTS_HR:
							sports_hr_bar_win_delete(home_para->h_mmenu);
							sports_hr_bar_res_uninit(home_para->h_mmenu);
							break;
						case ID_MANUAL:
							manuel_bar_win_delete(home_para->h_mmenu);
							manuel_bar_res_uninit(home_para->h_mmenu);
							break;
						case ID_HILL:
						case ID_FATBURN:
						case ID_CARDIO:
						case ID_STRENGTH:
						case ID_INTERVAL:
							preset_bar_win_delete(home_para->h_mmenu);
							preset_bar_res_uninit(home_para->h_mmenu);
							break;
						case ID_USER1:
						case ID_USER2:
							//__msg("huang##################  USER  \n");
							user_bar_win_delete(home_para->h_mmenu);
							user_bar_res_uninit(home_para->h_mmenu);
							break;
						case ID_HR1:
						case ID_HR2:
							hr_bar_win_delete(home_para->h_mmenu);
							hr_bar_res_uninit(home_para->h_mmenu);
							break;
					}		
					home_para->focus_id = msg->dwAddData2;
					/*创建MAIN*/  
					main_menu_bar_creat(home_para);
					currentMode = ID_MAIN;
					//if (msg->dwAddData2) {
					//	dsk_reg_flush();
					//}
					if (BTConnectFlag) {
						Send_BT_status(1);
						KEYAction = EPDK_FALSE;
						StartAction = EPDK_FALSE;
					}
					break;

				case ID_MANUAL:
					currentMode = ID_MANUAL;
					home_para->focus_id = msg->dwAddData2;
					main_menu_win_delete(home_para->h_mmenu);
					main_menu_res_uninit(home_para->h_mmenu);
					/*创建Manuel*/  
					manuel_bar_creat(home_para);
					
					break;
				case ID_HILL:
					currentMode = ID_HILL;
					home_para->focus_id = msg->dwAddData2;
					main_menu_win_delete(home_para->h_mmenu);
					main_menu_res_uninit(home_para->h_mmenu);
					/*创建Manuel*/  
					preset_bar_creat(home_para, currentMode);
					
					break;
				case ID_FATBURN:
					currentMode = ID_FATBURN;
					home_para->focus_id = msg->dwAddData2;
					main_menu_win_delete(home_para->h_mmenu);
					main_menu_res_uninit(home_para->h_mmenu);
					/*创建Manuel*/  
					preset_bar_creat(home_para, currentMode);
					
					break;
				case ID_CARDIO:
					currentMode = ID_CARDIO;
					home_para->focus_id = msg->dwAddData2;
					main_menu_win_delete(home_para->h_mmenu);
					main_menu_res_uninit(home_para->h_mmenu);
					/*创建Manuel*/  
					preset_bar_creat(home_para, currentMode);
					
					break;
				case ID_STRENGTH:
					currentMode = ID_STRENGTH;
					home_para->focus_id = msg->dwAddData2;
					main_menu_win_delete(home_para->h_mmenu);
					main_menu_res_uninit(home_para->h_mmenu);
					/*创建Manuel*/  
					preset_bar_creat(home_para, currentMode);
					
					break;
				case ID_INTERVAL:
					currentMode = ID_INTERVAL;
					home_para->focus_id = msg->dwAddData2;
					main_menu_win_delete(home_para->h_mmenu);
					main_menu_res_uninit(home_para->h_mmenu);
					/*创建Manuel*/  
					preset_bar_creat(home_para, currentMode);
					
					break;

				case ID_HR1:
					currentMode = ID_HR1;
					home_para->focus_id = msg->dwAddData2;
					main_menu_win_delete(home_para->h_mmenu);
					main_menu_res_uninit(home_para->h_mmenu);
					/*创建Manuel*/  
					hr_bar_creat(home_para, currentMode);
					
					break;

				case ID_HR2:
					currentMode = ID_HR2;
					home_para->focus_id = msg->dwAddData2;
					main_menu_win_delete(home_para->h_mmenu);
					main_menu_res_uninit(home_para->h_mmenu);
					/*创建Manuel*/  
					hr_bar_creat(home_para, currentMode);
					
					break;	

				case ID_USER1:
					currentMode = ID_USER1;
					home_para->focus_id = msg->dwAddData2;
					main_menu_win_delete(home_para->h_mmenu);
					main_menu_res_uninit(home_para->h_mmenu);
					/*创建Manuel*/  
					user_bar_creat(home_para, currentMode);
					
					break;	

				case ID_USER2:
					currentMode = ID_USER2;
					home_para->focus_id = msg->dwAddData2;
					main_menu_win_delete(home_para->h_mmenu);
					main_menu_res_uninit(home_para->h_mmenu);
					/*创建Manuel*/  
					user_bar_creat(home_para, currentMode);
					
					break;	

				case ID_FACTORY:
					currentMode = ID_FACTORY;
					home_para->focus_id = msg->dwAddData2;
					main_menu_win_delete(home_para->h_mmenu);
					main_menu_res_uninit(home_para->h_mmenu);
					/*创建Manuel*/  
					factory_bar_creat(home_para);
					break;

				case ID_SPORTS:
					home_para->timeFlag = msg->dwAddData2;
					switch(currentMode) {
						case ID_MAIN:
							SP_Data[4] = ID_MANUAL;
							main_menu_win_delete(home_para->h_mmenu);
							main_menu_res_uninit(home_para->h_mmenu);
							break;
						case ID_MANUAL:
							SP_Data[4] = ID_MANUAL;
							manuel_bar_win_delete(home_para->h_mmenu);
							manuel_bar_res_uninit(home_para->h_mmenu);
							break;
						case ID_HILL:
						case ID_FATBURN:
						case ID_CARDIO:
						case ID_STRENGTH:
						case ID_INTERVAL:
							preset_bar_win_delete(home_para->h_mmenu);
							preset_bar_res_uninit(home_para->h_mmenu);
							break;
						case ID_USER1:
						case ID_USER2:
							user_bar_win_delete(home_para->h_mmenu);
							user_bar_res_uninit(home_para->h_mmenu);
							break;
					}
					currentMode = ID_SPORTS;
					
					/*创建Sports*/  
					sports_bar_creat(home_para);
					
					break;

				case ID_SPORTS_HR:
					home_para->timeFlag = msg->dwAddData2;
					hr_bar_win_delete(home_para->h_mmenu);
					hr_bar_res_uninit(home_para->h_mmenu);
					currentMode = ID_SPORTS_HR;
					
					/*创建HR Sports*/  
					sports_hr_bar_creat(home_para);
					
					break;
				
				default:
				break;
			}
	
		}
		return EPDK_OK;
	}
	
	return GUI_ManWinDefaultProc(msg);	
}

/*******************************************************************************************************
*********************************************************************************************************/
H_WIN app_home_create(root_para_t *para)
{
	__gui_manwincreate_para_t create_info;

	__inf("****************************************************************************************\n");
	__inf("********  enter home app  **************\n");
	__inf("****************************************************************************************\n");

	//__wait__;
	
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = APP_HOME;
	create_info.hParent         = para->h_parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)app_home_proc); 
	create_info.attr            = (void*)para;
	create_info.id				= APP_HOME_ID;
	create_info.hHosting        = NULL;

	return(GUI_ManWinCreate(&create_info));
}
