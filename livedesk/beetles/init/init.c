/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    		: init.c
* By      		: Andy.zhang
* Func		: init thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "mod_init_i.h"
#include "ui\\init_server.h"
//#include "ui\\tp_adjust_scene\\scene_adjust.h"

#define XML_PATH	BEETLES_APP_ROOT"apps\\desktop"

static H_WIN init_mainwin;

/* DBMS module path */
#define	 DBMS_MOD_PATH	BEETLES_APP_ROOT"mod\\dbms.mod"
#if 0
#define DebugPrintf(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)		)
#else
#define DebugPrintf(...) 
#endif

#if BEETLES_BOOST_IC_CTRL
#define PIOC_MEM_BASE   (0xf1c20800)
#define PIO_REG_CFG(n, i)               ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x00))
#define PIO_REG_DLEVEL(n, i)            ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x14))
#define PIO_REG_PULL(n, i)              ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x1C))
#define PIO_REG_DATA(n) 	            ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + 0x10))
#endif


static int 		ModDBMSID;
static __mp    *ModDBMS;
extern GUI_FONT    *SWFFont;
__u8 bl_off_flag = 0;
__s32 show_charge_scene(void);

static __s32 _install_dbms_mod(void)
{
    /* install dbms module */
	ModDBMSID = esMODS_MInstall(DBMS_MOD_PATH, 0);
    if(ModDBMSID == 0)
    {
    	__wrn("Install dbms module failed");
    	return EPDK_FAIL;
    }
	__inf("Install dbms module succeeded\n");
	
    /* open dbms module */
    ModDBMS = esMODS_MOpen(ModDBMSID, 0);
    if(ModDBMS == (__mp *)0)
    {
        __wrn("Open dbms module failed");
    	return EPDK_FAIL;
    }
	__inf("Open dbms module succeeded\n");
	
	return EPDK_OK;
}

#if BEETLES_BOOST_IC_CTRL

void pull_up_gpio(__u32 port, __u32 port_num)
{
    volatile  __u32    *tmp_addr;
    __u32               tmp_data;
    __u32               tmp1;
    __u32               tmp2;
    

    //设置为输出
    tmp1 = (port_num - ((port_num>>3)<<3))<<2;
    tmp_addr = PIO_REG_CFG(port, (port_num>>3));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x07 << tmp1);
    tmp_data |=  (0x01 << tmp1);
    *tmp_addr = tmp_data;

    //设置为上拉
    tmp2 = (port_num - ((port_num>>4)<<4))<<1;
    tmp_addr = PIO_REG_PULL(port, (port_num>>4));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x03 << tmp2);
    tmp_data |=  (0x01 << tmp2);
    *tmp_addr = tmp_data;      
    
    //输出高电平
    tmp_addr = PIO_REG_DATA(port);
    tmp_data = *tmp_addr;
    tmp_data &= ~(1 << port_num);
    tmp_data |=  (1 << port_num);
    *tmp_addr = tmp_data;    
    
}

void pull_down_gpio(__u32 port, __u32 port_num)
{
    volatile  __u32    *tmp_addr;
    __u32               tmp_data;
    __u32               tmp1;
    __u32               tmp2;
    

    //设置为输出
    tmp1 = (port_num - ((port_num>>3)<<3))<<2;
    tmp_addr = PIO_REG_CFG(port, (port_num>>3));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x07 << tmp1);
    tmp_data |=  (0x01 << tmp1);
    *tmp_addr = tmp_data;

    //设置为下拉
    tmp2 = (port_num - ((port_num>>4)<<4))<<1;
    tmp_addr = PIO_REG_PULL(port, (port_num>>4));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x03 << tmp2);
    tmp_data |=  (0x02 << tmp2);
    *tmp_addr = tmp_data;      
    
    //输出低电平
    tmp_addr = PIO_REG_DATA(port);
    tmp_data = *tmp_addr;
    tmp_data &= ~(1 << port_num);
    //tmp_data |=  (1 << port_num);
    *tmp_addr = tmp_data;    
    
}
#endif


static void _uninstall_dbms_mod(void)
{
    /* uninstall dbms module    */
    if (ModDBMS)
    {
		esMODS_MClose(ModDBMS);
	}
	if (ModDBMSID)
	{
		esMODS_MUninstall(ModDBMSID);
	}
    ModDBMS   = NULL;
	ModDBMSID = 0;
}
static void _close_audio_dev(void)
{
	ES_FILE		 * p_dac;
				
	p_dac = eLIBs_fopen("b:\\AUDIO\\CTRL", "r+");
    if(!p_dac)
    {        	
    	__err("open dac device fail!\n");
    }
            
	eLIBs_fioctrl(p_dac, AUDIO_DEV_CMD_CLOSE_DEV, AUDIO_DEV_PLAY, 0);
	eLIBs_fclose(p_dac);
}

static void _close_lcd(void)
{
	ES_FILE *disp;
	__u32 outputtype;
	__u32 	arg[3];	
	
	disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");		
	if(!disp)
	{
		__wrn("open display fail\n");
		return ;
	}
	
	outputtype = eLIBs_fioctrl(disp,DISP_CMD_GET_OUTPUT_TYPE,0,(void *)arg);
	if(outputtype == DISP_OUTPUT_TYPE_LCD)
	{
		arg[0] = 0;
		arg[1] = 0;
		arg[2] = 0;
		eLIBs_fioctrl(disp,DISP_CMD_LCD_OFF,0,(void *)arg);
	}

	eLIBs_fclose(disp);
}

static void _framework_init(void)
{
	AppletInfoManager	*info_manager = NULL;
	ActivityManager 	*activity_manager = NULL;
__msg("czp is here......");	
	info_manager = applet_info_manager_create();		
	applet_info_manager_load_dir(info_manager, XML_PATH);		
	//applet_info_manager_printf(info_manager);		// 打印applet info 信息
	applet_info_manager_set_defualt(info_manager);	
__msg("czp is here......");	
	activity_manager = activity_manager_create();	
	activity_manager_set_default(activity_manager);		
}

static void _framework_exit(void)
{
	AppletInfoManager 	*info_manager = applet_info_manager_get_default();
	ActivityManager 	*activity_manager = activity_manager_get_default(); 
	
	activity_manager_destroy(activity_manager);
	applet_info_manager_destroy(info_manager);
	
	applet_info_manager_set_defualt(NULL);
	activity_manager_set_default(NULL);
}

__s32 key_hook_cb(__gui_msg_t *msg)
{
    if(init_mainwin)
    {
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(init_mainwin);
	
	init_lock(msg);	
	if( !init_ctr->key_msg_enble )				// 禁止按键
	{		
		init_unlock(msg);
		return -1;
	}
		
	if( init_ctr->screen_close_done)// 已经关屏
	{		
        init_unlock(msg);
		if(/*GUI_MSG_KEY_ENTER == msg->dwAddData1 && */KEY_UP_ACTION == msg->dwAddData2)		
		{			
			__here__;		
			NOTIFY_MSG(DSK_MSG_SCREEN_OPEN, NULL, msg->h_deswin, 0, 0);			
			if((0x0 != GUI_WinGetHandFromName("app_movie")) || (0x0 != GUI_WinGetHandFromName("app_photo")))
			{
				g_disable_close_scn();
			}
		}
        __here__;		
        return 0;        
	}
	
	if( init_ctr->screen_close_done )
	{		
		init_unlock(msg);
		return -1;		
	}
	
	init_unlock(msg);
	esPWM_UsrEventNotify();	
    __msg("msg->h_deswin=%x msg->id=%d msg->dwAddData1=%d msg->dwAddData2=%d\n"
        , msg->h_deswin, msg->id, msg->dwAddData1, msg->dwAddData2);   
    }
    
	GUI_PostMessage(msg);

    {//按键enter键down和up键同时到达，导致消息丢失，之间加一个延时
        if(GUI_MSG_KEY_ENTER == msg->dwAddData1
            && KEY_DOWN_ACTION == msg->dwAddData2)
        {
            esKRNL_TimeDly(10);
        }
    }      

	return 0;
}

/* tp msg hook */
__s32 tp_hook_cb(__gui_msg_t *msg)
{	
    if(init_mainwin)
    {
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(init_mainwin);
	
	init_lock(msg);	
	if( !init_ctr->tp_msg_enble )			// 禁止 touch
	{
	
		init_unlock(msg);
		return -1;	
	}	
	if( init_ctr->screen_close_done )
	{
	
		init_unlock(msg);
		return -1;		
	}
	
	init_unlock(msg);
	esPWM_UsrEventNotify();			
	//__msg("msg->h_deswin = %x, msg->id = %d\n", msg->h_deswin, msg->id);
	 __here__;
    GUI_PostMessage(msg);

	return 0;
}
}
/* desktop msg hook */
__s32 init_hook_cb(__gui_msg_t *msg)
{		
    if(init_mainwin)
    {
	__init_ctl_t * init_ctr = (__init_ctl_t *)GUI_WinGetAttr(init_mainwin);
	
	/* 请求开屏 */
	if( init_ctr->screen_close_done )
	{
		NOTIFY_MSG(DSK_MSG_SCREEN_OPEN, NULL, msg->h_deswin, 0, 0);
		if(0x0 != GUI_WinGetHandFromName("app_movie"))
		{
			g_disable_close_scn();
		}
	}		
    }
    __msg("msg->h_deswin=%x msg->id=%d msg->dwAddData1=%d msg->dwAddData2=%d\n"
        , msg->h_deswin, msg->id, msg->dwAddData1, msg->dwAddData2);   
    
	GUI_PostMessage(msg);		
	//GUI_SendNotifyMessage(msg);
	__here__;	
	return 0;
}

static void _process_init(void)
{	
	__memit_hook_t 	emit_hook;
	H_WIN			scene_adjust;
		
	/* orchid */
//	dsk_orchid_createDisk();	
//	dsk_orchid_check();  
	
	/* framework init */

#if BEETLES_BOOST_IC_CTRL
	pull_up_gpio(BOOST_IC_PORT, BOOST_IC_PORTNUM);
#endif

	_framework_init();

#if 0
	if(0)
	{
		
        reg_system_para_t* para;
        para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
		if(!para)
		{
			__msg("init get reg para fail............\n");
		}
		else
		{
			__msg("para->tp_adjust_finish_flag=%d\n",para->tp_adjust_finish_flag);
		    if(para->tp_adjust_finish_flag==0)
			{
				H_WIN	scene_adjust;
				__here__;
				scene_adjust = scene_adjust_create(init_mainwin);
				scene_adjust_destroy(scene_adjust);
				para->tp_adjust_finish_flag = 1;				
			}
		}
	}
#endif

	/* load main*/		
	activity_load_app("application://app_root");	

    //__wait__;
	/* messge server init*/		
	//msg_emit_init();	

	//emit_hook.key_hook 			= key_hook_cb;
	//emit_hook.tp_hook 	   		= tp_hook_cb;
	//emit_hook.init_hook 		= init_hook_cb;
		
	//msg_emit_register_hook(&emit_hook);		
}

static void _process_exit(void)
{	
    bookengine_close();
    _uninstall_dbms_mod();
    
	//卸载USB
//	App_RmmodUsbHostDriver(1);
//	App_RmmodUsbHostDriver(2);
	
	/* deinit message server */
	msg_emit_deinit();
	
	/* framework deinit */
	_framework_exit();
	
	/* close device */
	_close_audio_dev();
	_close_lcd();

#if BEETLES_BOOST_IC_CTRL
	pull_down_gpio(BOOST_IC_PORT, BOOST_IC_PORTNUM);
#endif
		
	/* plug out driver  */
//	esDEV_Plugout("\\drv\\mp.drv", 0);
//	esDEV_Plugout("\\drv\\touchpanel.drv", 0);	
//	esDEV_Plugout("\\drv\\orchid.drv",0); 
	esDEV_Plugout("\\drv\\audio.drv", 0 );	
//    esDEV_Plugout("\\drv\\display.drv", 0 );	
//	esDEV_Plugout("\\drv\\ir.drv", 0 );
	
//	App_WaitForUsbHostReomveComplete(1);	
//	App_WaitForUsbHostReomveComplete(2);
}


//充电场景
#if 0

#define CHARGE_BMP_NUM 3
typedef struct
{
    __u8 play_timer_id;
    __u8 bl_off_timer_id;
	
    H_LYR hlayer;
    __s32 bmp_id[CHARGE_BMP_NUM];
    HTHEME bmp_hdl[CHARGE_BMP_NUM];
}chagre_attr_t;

static H_LYR charge_scene_32bpp_layer_create( RECT* LayerRect )
{
	H_LYR layer = NULL;
	FB  fb =
	{
	    {0, 0},                                   						/* size      */
	    {0, 0, 0},                                      				/* buffer    */
	    {FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, (__rgb_seq_t)0}},    	/* fmt       */

	};

	__disp_layer_para_t lstlyr =
	{
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
        "charge scene",
      	NULL,
      	GUI_LYRWIN_STA_SLEEP,
      	GUI_LYRWIN_NORMAL
    };

    fb.size.width			= LayerRect->width;
    fb.size.height			= LayerRect->height;
    fb.fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888;

	lstlyr.src_win.x  		= 0;
	lstlyr.src_win.y  		= 0;
	lstlyr.src_win.width 	= LayerRect->width;
	lstlyr.src_win.height   = LayerRect->height;

	lstlyr.scn_win.x		= LayerRect->x;
	lstlyr.scn_win.y		= LayerRect->y;
	lstlyr.scn_win.width  	= LayerRect->width;
	lstlyr.scn_win.height 	= LayerRect->height;

	lstlyr.pipe 			= 1;
	lstlyr.fb 				= &fb;
	lyrcreate_info.lyrpara 	= &lstlyr;

	layer = GUI_LyrWinCreate(&lyrcreate_info);

	if( !layer )
	{
		__wrn("desktop scene layer create error !\n");
	}
	return layer;
}

static __s32 charge_paint(__gui_msg_t *msg)
{
    static __s32 index = 0;
    chagre_attr_t* pattr;
    void* pbmp;
    HBMP  hbmp;
    __s32 width;
    __s32 height;
    __s32 screen_width;
    __s32 screen_height;
    GUI_MEMDEV_Handle hmem_dev;

    //esMEMS_Info();

    pattr = (chagre_attr_t*)GUI_WinGetAttr(msg->h_deswin);
    if(NULL == pattr)
    {
        return EPDK_OK;
    }

    __here__;
    dsk_display_get_size(&screen_width, &screen_height);
    
    __wrn("index=%d, pattr->bmp_hdl[index]=%x\n", index, pattr->bmp_hdl[index]);
    pbmp = dsk_theme_hdl2buf(pattr->bmp_hdl[index]);    
    if(NULL == pbmp)
    {
        __wrn("pbmp is null...\n");
        return EPDK_OK;
    }
    __here__;

    hbmp = bmp_open(pbmp);
    if(NULL == hbmp)
    {
        return EPDK_OK;
    }
    __here__;

    width = bmp_get_width(hbmp);
    height = bmp_get_height(hbmp);
    __wrn("width=%d, height=%d\n", width, height);

    if(hbmp)
    {
        bmp_close(hbmp);
        hbmp = NULL;
    }
    __here__;

    GUI_LyrWinSetSta(pattr->hlayer, GUI_LYRWIN_STA_ON);
    GUI_LyrWinSel(pattr->hlayer);
    
	hmem_dev = GUI_MEMDEV_Create((screen_width-width)/2, (screen_height-height)/2, width, height);
	if (!hmem_dev)
	{
        GUI_MEMDEV_Select( NULL );			
	}
    else
    {
        GUI_MEMDEV_Select( hmem_dev );	
    }
    __here__;
    GUI_BMP_Draw(pbmp, (screen_width-width)/2, (screen_height-height)/2);
    __here__;
    if(hmem_dev)
    {
        GUI_MEMDEV_CopyToLCD(hmem_dev); 
    	GUI_MEMDEV_Select( NULL );			
    	GUI_MEMDEV_Delete(hmem_dev);
    	hmem_dev = NULL;
    }
    
    index++;
    if(index >= CHARGE_BMP_NUM)//循环切换
    {
        index = 0;
    }
    
    if(EPDK_TRUE == dsk_power_is_full())//如果充满了，就停留在最后一张
    {
        index = CHARGE_BMP_NUM-1;
    }

    __here__;

    {
        GUI_RECT rect;
        char str[64];
        __s32 w;

       dsk_langres_get_menu_text(STRING_CHARGE_MODE, str, sizeof(str));
        if(SWFFont)
        {
            GUI_SetFont(SWFFont);
        }

        GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
        GUI_CharSetToEncode(EPDK_CHARSET_ENM_UTF8);
        GUI_SetColor(GUI_RED);

        GUI_GetTextExtend(&rect, str, eLIBs_strlen(str));
        w = rect.x1 - rect.x0;
        GUI_DispStringAt(str, (screen_width-w)/2, screen_height-32);
    }
    
    return EPDK_OK;
}

static __s32 charge_mainwin_cb(__gui_msg_t *msg)
{
	switch( msg->id )
	{
		case GUI_MSG_CREATE:				
		{	
            __s32 i;
            chagre_attr_t* pattr;
            RECT rect;
            __s32 screen_width;
            __s32 screen_height;
			

            pattr = (chagre_attr_t*)GUI_WinGetAttr(msg->h_deswin);
            if(NULL == pattr)
            {
                return NULL;
            }

            for(i = 0 ; i < CHARGE_BMP_NUM ; i++)
            {
                __wrn("pattr->bmp_id[i]=%d\n", pattr->bmp_id[i]);
                
                if(pattr->bmp_id[i])
                {
                    pattr->bmp_hdl[i] = dsk_theme_open(pattr->bmp_id[i]);
                    __wrn("i=%d, pattr->bmp_hdl[i]=%x\n", i, pattr->bmp_hdl[i]);
                }
                else
                {
                    __wrn("dsk_theme_open fail,bmp_id=%d\n", pattr->bmp_id[i]);
                    pattr->bmp_hdl[i] = NULL;
                }
            }

            __here__;

            dsk_display_get_size(&screen_width, &screen_height);

            rect.x = 0;
            rect.y = 0;
            rect.width = screen_width;
            rect.height = screen_height;

            __wrn("screen_width=%d, screen_height=%d\n", screen_width, screen_height);
            pattr->hlayer = charge_scene_32bpp_layer_create(&rect);
            if(NULL == pattr->hlayer)
            {
                return EPDK_OK;
            }

            __here__;

            GUI_LyrWinSel(pattr->hlayer);
            GUI_SetBkColor(0x0);
            GUI_Clear();
            					
			GUI_SetTimer(msg->h_deswin, pattr->play_timer_id, 100, NULL);
			
			
			GUI_SetTimer(msg->h_deswin, pattr->bl_off_timer_id, 100*10, NULL);									

            GUI_LyrWinCacheOff();

            charge_paint(msg);
			
			return EPDK_OK;
		}
		
		case GUI_MSG_DESTROY:
		{			
            __s32 i;
            chagre_attr_t* pattr;

            DebugPrintf("charge_mainwin_cb, GUI_MSG_DESTROY begin...\n");
            pattr = (chagre_attr_t*)GUI_WinGetAttr(msg->h_deswin);
            if(NULL == pattr)
            {
                __wrn("show charge scene alreadly been destroy...\n");
                return NULL;
            }
            
			if( GUI_IsTimerInstalled(msg->h_deswin, pattr->play_timer_id))		
			{
				GUI_KillTimer(msg->h_deswin, pattr->play_timer_id);
			}
			
			if( GUI_IsTimerInstalled(msg->h_deswin, pattr->bl_off_timer_id))		
			{
				GUI_KillTimer(msg->h_deswin, pattr->bl_off_timer_id);
			}

          //  gscene_bgd_set_state(BGD_STATUS_SHOW);
			

            __here__;
            
            if(pattr->hlayer)
            {
                GUI_LyrWinDelete(pattr->hlayer);
                pattr->hlayer = NULL;
            }

            __here__;
            
            for(i = 0 ; i < CHARGE_BMP_NUM ; i++)
            {
                DebugPrintf("dsk_theme_close, pattr->bmp_hdl[i]=0x%x\n", pattr->bmp_hdl[i]);
                if(pattr->bmp_hdl[i])
                {
                    dsk_theme_close(pattr->bmp_hdl[i]);
                    pattr->bmp_hdl[i] = NULL;
                }
            }        

            __here__;

            eLIBs_memset(pattr, 0, sizeof(chagre_attr_t));
            
			esMEMS_Bfree(pattr, sizeof(chagre_attr_t));

            GUI_WinSetAttr(msg->h_deswin, NULL);
            
            DebugPrintf("charge_mainwin_cb, GUI_MSG_DESTROY end...\n");
			return EPDK_OK;
		}
		case GUI_MSG_CLOSE:		
			__here__;
            DebugPrintf("GUI_MSG_CLOSE\n");
			GUI_ManWinDelete(msg->h_deswin);
			__msg("bl_off_flag=%d\n");
			if(bl_off_flag ==1)
			{
				dsk_display_lcd_on();	
			}
			
			return EPDK_OK;	

		case GUI_MSG_TIMER:
		{
            chagre_attr_t* pattr;

            pattr = (chagre_attr_t*)GUI_WinGetAttr(msg->h_deswin);
            if(NULL == pattr)
            {
                return EPDK_OK;
            }

            __here__;
			if( msg->dwAddData1 == pattr->play_timer_id )
        	{
                __here__;
                charge_paint(msg);
                __here__;
                
                if(EPDK_FALSE == dsk_get_charge_state())
                {
                    __gui_msg_t msgex;

                    DebugPrintf("EPDK_FALSE == dsk_get_charge_state()\n");

                    eLIBs_memset(&msgex, 0, sizeof(__gui_msg_t));

                    msgex.id = GUI_MSG_CLOSE;
                    msgex.h_deswin = msg->h_deswin;
                    GUI_SendNotifyMessage(&msgex); 
			return EPDK_OK;
                }
        	}
            
			if( msg->dwAddData1 == pattr->bl_off_timer_id )
        	{
                
				dsk_display_lcd_off();	
				bl_off_flag = 1;
                
        	}
			return EPDK_OK;
		}	
		#if 1
		case GUI_MSG_KEY:
		{	

			
			DebugPrintf("GUI_MSG_KEY_PLAY_PAUSE\n");
            if(KEY_DOWN_ACTION == msg->dwAddData2)
            {
            #if 0
                if(GUI_MSG_KEY_PLAY_PAUSE == msg->dwAddData1)
                {
                if (bl_off_flag == 0)
                  {
				   	__gui_msg_t msgex;
                    DebugPrintf("GUI_MSG_KEY_PLAY_PAUSE\n");

                    eLIBs_memset(&msgex, 0, sizeof(__gui_msg_t));

                    msgex.id = GUI_MSG_CLOSE;
                    msgex.h_deswin = msg->h_deswin;
                    GUI_SendNotifyMessage(&msgex);
                  }
				else
					{
						dsk_display_lcd_on();	
						bl_off_flag = 0;
					}
                }
				#endif
                if((GUI_MSG_KEY_LEFT== msg->dwAddData1)
					||(GUI_MSG_KEY_RIGHT== msg->dwAddData1)
					//||(GUI_MSG_KEY_VOLADD== msg->dwAddData1)
					//||(GUI_MSG_KEY_VOLDEC== msg->dwAddData1)
					//||(GUI_MSG_KEY_TVOUT== msg->dwAddData1)
					||(GUI_MSG_KEY_MENU== msg->dwAddData1)
					||(GUI_MSG_KEY_MODE== msg->dwAddData1)
					||(GUI_MSG_KEY_MOVIE== msg->dwAddData1)
					||(GUI_MSG_KEY_MUSIC== msg->dwAddData1)
					||(GUI_MSG_KEY_MUSIC== msg->dwAddData1)
					||(GUI_MSG_KEY_MOVIE== msg->dwAddData1)
					||(GUI_MSG_KEY_ENTER== msg->dwAddData1)
					||(GUI_MSG_KEY_NUM0== msg->dwAddData1)
					||(GUI_MSG_KEY_NUM1== msg->dwAddData1)
					||(GUI_MSG_KEY_NUM2== msg->dwAddData1)
					||(GUI_MSG_KEY_NUM3== msg->dwAddData1)
					||(GUI_MSG_KEY_NUM4== msg->dwAddData1)
					||(GUI_MSG_KEY_NUM5== msg->dwAddData1)
					||(GUI_MSG_KEY_NUM6== msg->dwAddData1)
					||(GUI_MSG_KEY_NUM7== msg->dwAddData1)
					||(GUI_MSG_KEY_NUM8== msg->dwAddData1)
					||(GUI_MSG_KEY_NUM9== msg->dwAddData1)
					//||(GUI_MSG_KEY_AUX== msg->dwAddData1)
					//||(GUI_MSG_KEY_EBOOK== msg->dwAddData1)
					||(GUI_MSG_KEY_RECORD== msg->dwAddData1)
					||(GUI_MSG_KEY_FM== msg->dwAddData1)
					||(GUI_MSG_KEY_PHOTO== msg->dwAddData1)
					//||(GUI_MSG_KEY_EQ== msg->dwAddData1)
					)
          /*      {
					dsk_display_lcd_on();	
					bl_off_flag = 0;
                }*/
					{
                if (bl_off_flag == 0)
                  {
				   	__gui_msg_t msgex;
                    DebugPrintf("GUI_MSG_KEY_PLAY_PAUSE\n");

                    eLIBs_memset(&msgex, 0, sizeof(__gui_msg_t));

                    msgex.id = GUI_MSG_CLOSE;
                    msgex.h_deswin = msg->h_deswin;
					
                    GUI_SendNotifyMessage(&msgex);
                  }
				else
					{
						dsk_display_lcd_on();	
						bl_off_flag = 0;
					}
                }
            }
            
			return EPDK_OK;		
		}	
		#endif
		case DSK_MSG_FS_PART_PLUGIN:
		{
			__gui_msg_t msgex;
                    	__msg("DSK_MSG_FS_PART_PLUGIN\n");

                    	eLIBs_memset(&msgex, 0, sizeof(__gui_msg_t));

                    	msgex.id = GUI_MSG_CLOSE;
                    	msgex.h_deswin = msg->h_deswin;
					
                    	GUI_SendNotifyMessage(&msgex);
			return EPDK_OK;
		}
		case GUI_MSG_TOUCH:			
		{
			return EPDK_OK;		
		}		
		default:			
	    {
			break;
		}
	}

	return GUI_ManWinDefaultProc(msg);
}

static H_WIN show_charege_mainwin_create(void)
{
	H_WIN  						hManWin;	
	__gui_manwincreate_para_t  	create_info;	
    chagre_attr_t               *pattr;
			
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));

    pattr = (chagre_attr_t*)esMEMS_Balloc(sizeof(chagre_attr_t));
    if(NULL == pattr)
    {
        return NULL;
    }
    eLIBs_memset(pattr, 0, sizeof(chagre_attr_t));
    pattr->play_timer_id = 0x43;
    pattr->bmp_id[0] = ID_INIT_CHARGE_BMP0_BMP;
    pattr->bmp_id[1] = ID_INIT_CHARGE_BMP1_BMP;
    pattr->bmp_id[2] = ID_INIT_CHARGE_BMP2_BMP;
    
	create_info.attr            = pattr; 
	create_info.hParent			= NULL;
	create_info.hHosting 		= NULL;   
    create_info.ManWindowProc   = charge_mainwin_cb;
    create_info.name            = "charge"; 	
	hManWin 					= GUI_ManWinCreate(&create_info);
	
	if(hManWin == NULL)
    {        
        __wrn("show_charege_mainwin_create failed!\n");      
        return NULL;
    }	
		
	return hManWin;
}


__s32 show_charge_scene(void)
{
    __s32 ret;
    H_WIN hManWin;
    __gui_msg_t 	msg;	    
    
    if(EPDK_FALSE == dsk_get_charge_state())
    {
        __wrn("not in charge state...\n");
        return EPDK_OK;
    }

    DebugPrintf("in charge state...\n");

    hManWin = show_charege_mainwin_create();
    if(NULL == hManWin)
    {
        DebugPrintf("show_charege_mainwin_create fail...\n");
        return EPDK_OK;
    }
        
    GUI_SetActiveManWin(hManWin);	
	__wrn("hManWin = %x\n", hManWin);	
    
	/* set message loop win */
	message_loop_win_set_default(hManWin);    

    while( GUI_GetMessageEx(&msg, hManWin) )	// 消息循环
	{
#ifdef __MSG
		if (msg.id != GUI_MSG_TIMER)
		{
			__msg("msg.h_deswin=%x, msg.id=%d, msg.dwAddData1=%d,msg.dwAddData2=%d\n"
				, msg.h_deswin, msg.id, msg.dwAddData1, msg.dwAddData2);
		}
#endif
		ret = GUI_DispatchMessage(&msg);		// 分发消息到回调		
		if( msg.p_arg )							// 同步消息回应
        {             	
        	GUI_SetSyncMsgRetVal(&msg, ret);
        	GUI_PostSyncSem(&msg);      
        }
       // __here__;
	}		
	GUI_WinThreadCleanup(hManWin);		
	
	message_loop_win_set_default(NULL);

    GUI_SetActiveManWin(NULL);//置空，防止消息队列无对应窗口
    
    return EPDK_OK;
}
#endif

void application_init_process(void *arg)
{			
	__gui_msg_t 	msg;
	
	__memit_hook_t 	emit_hook;
	__s32			ret = EPDK_FAIL;
		
	eLIBs_printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
	eLIBs_printf("&           application_init_process enter!          		       &\n");	
	eLIBs_printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");

    //__wait__;
    
    dsk_amplifier_onoff(0);	//wqx 20130406
    

	dsk_power_onoff_ctl(1);	//wqx 20130408
	
    /* drvier plugin */
//	App_InsmodUsbHostDriver(1);
//	App_InsmodUsbHostDriver(2);
//	esDEV_Plugin("\\drv\\ir.drv", 0, 0, 1);
    //DebugPrintf("before audio plugin\n");
	esDEV_Plugin("\\drv\\fm.drv", 0, 0, 1);
	esDEV_Plugin("\\drv\\audio.drv", 0, 0, 1);
//	esDEV_Plugin("\\drv\\key.drv", 0, 0, 1);
//	esDEV_Plugin("\\drv\\rocker.drv", 0, 0, 1);
	__msg("after audio plugin\n");
	msg_emit_init();	

	emit_hook.key_hook 			= key_hook_cb;
	emit_hook.tp_hook 	   		= tp_hook_cb;
	emit_hook.init_hook 		= init_hook_cb;
		
	msg_emit_register_hook(&emit_hook);      

    live_init();

    //backgrd	
  {
        SIZE screen_size;
        __s32 w, h;
        
        dsk_display_get_size(&w, &h);
        
    	screen_size.width	 = w;
    	screen_size.height	 = h;		
    	gscene_bgd_init(&screen_size, BGD_STATUS_HIDE, FB_TYPE_YUV);	
    }

    //esKSRV_SysInfo();


	//充电模式
    //show_charge_scene();

	DebugPrintf("**************************\n");
    //esKSRV_SysInfo();
	/* create main win*/	

    //__wait__;
	init_mainwin = init_mainwin_create();	
	GUI_SetActiveManWin(init_mainwin);	
	__msg("init_mainwin = %x\n", init_mainwin);	
	/* set message loop win */
	message_loop_win_set_default(init_mainwin);
		
	/* drv or mod plugin */
	__here__;
    _process_init();	

    __here__;
    //dsk_reg_flush();

    /*enable device monitor */
    if(1){
        ES_FILE     *pHwsc;

        pHwsc = eLIBs_fopen("b:\\HWSC\\hwsc", "rb+");
        if(pHwsc)
        {
            eLIBs_fioctrl(pHwsc, DEV_IOC_USR_HWSC_ENABLE_MONITOR, 0, NULL);
            eLIBs_fclose(pHwsc);
            __inf(".start device monitor .......................................\n");
        }
        else
        {
            __err("try to open b:\\HWSC\\hwsc failed!\n");
        }
    }
    
	__msg("113\n");
	/* message loop*/
	while( GUI_GetMessageEx(&msg, init_mainwin) )	// 消息循环
	{
//        __msg("msg.h_deswin=%x, msg.id=%d, msg.dwAddData1=%d,msg.dwAddData2=%d\n"
//            , msg.h_deswin, msg.id, msg.dwAddData1, msg.dwAddData2);
		ret = GUI_DispatchMessage(&msg);		// 分发消息到回调		
		if( msg.p_arg )							// 同步消息回应
        {             	
        	GUI_SetSyncMsgRetVal(&msg, ret);
        	GUI_PostSyncSem(&msg);      
        }
       // __here__;
	}		
    __here__;
	GUI_WinThreadCleanup(init_mainwin);		
    __here__;
	
	message_loop_win_set_default(NULL);
    __here__;
	
	/* unstall mod and driver */
	_process_exit();
    
	
	__inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
	__inf("&           application_init_process exit!                          &\n");
	__inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
    #if  1
    {//remote_power ctrl
        ES_FILE* fp;

        fp = eLIBs_fopen("b:\\INPUT\\IR_KEY", "rb");
        if(fp)
        {
            __wrn("b:\\INPUT\\IR_KEY open success...\n");
  //          eLIBs_fioctrl(fp, DRV_IR_SET_POWER_OFF_STATUS, 0, 0);
            eLIBs_fclose(fp);
            fp = NULL;
        }
        else
        {
            __wrn("b:\\INPUT\\IR_KEY open fail...\n");
        }
    }
	#endif
 dsk_amplifier_onoff(0);	//wqx 20130406
	/* 关闭电源 */
	esKSRV_PowerOff();	
	dsk_power_onoff_ctl(0);	//wqx 20130408
    __msg("end of application_init_process\n");
}


