/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    		: app_root_win_scene.c
* By      		: Bayden
* Func		: app_root applets plugin interface define
* Version	: v1.0
* ============================================================================================================
* 2010-9-28 17:11:20  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "beetles_app.h"
#include "app_root.h"
#include "app_root_scene.h"
#include "app_root_init.h"
//#include "home/app_home.h"
//#include "home/main_menu.h"
//#include "home/manuel_bar.h"

//#include "explorer/explorer.h"
//#include "movie/app_movie.h"
//#include "music/music.h"
//#include "photo/app_photo.h"
//#include "setting/app_setting.h"
//#include "fm/app_fm.h"
//#include "ebook/app_ebook.h"
//#include "record/app_record.h"


#define REG_ROOT_PARA_FILE_PATH 			"e:\\setting.bin"

#if 1
    #define __here__            eLIBs_printf("@L%d(%s)\n", __LINE__, __FILE__);
	#define __msg(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
	#define __wrn(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
#else
    #define __msg(...)
#undef __wrn(...)
#define __wrn(...)
#endif



#define AMPLIFER_CHECK_TIMMER_ID 0x39


#define AUX_CHECK_TIMER_ID		0x38
#define AUX_CHECK_TIME			100
#define EX_MIC_DET_SUPPORT		1			//

#if BEETLES_MODIFY_TOOL
#define NUM_KEY_COUNT_TID		0x41
#define NUM_KEY_COUNT_TOUT		300
#endif

static __s32 num_key_count = 0;

//ai ++
enum
{
	no_music_file=1,
	no_movie_file,	
	no_photo_file,
	no_ebook_file,
};

static __s32 channel_mode_str[] = 
{
	STRING_MUSIC_STEREO,
	STRING_MUSIC_LEFT,
	STRING_MUSIC_RIGHT
};

static __s32 the_media_file=0; 
static char last_str[32] = 0;

static __s32 g_is_mute = 0;//1代表mute，0代表没mute
static __u8 g_movie_to_music=0;

static __u8	g_aux_is_in = 0;	//AUX插入: 0:否， 1:是
static __s32 is_mic_in = 0;

static __u8 key_msg_change_mask = 0;

typedef struct tag_root_ctrl
{
	H_WIN	h_app_home;		
	H_WIN	h_app_explorer;
	H_WIN	h_app_fm;	
	H_WIN	h_app_movie;
	H_WIN	h_app_music;
	H_WIN	h_app_photo;
	H_WIN	h_app_ebook;
	H_WIN	h_app_setting;		
	H_WIN	h_app_calendar;	
    H_WIN	h_app_record;
	H_WIN	h_app_linin;
	root_para_t *root_para;
}root_ctrl_t;
__u8 g_movie_to_movie=0;

#if ECHO_CTRL_SUPPORT

typedef struct 
{
	__s32	num;
	__s32	time;
}echo_level_param;

echo_level_param echo_level[8] = {{0, 0},
								  {1, 100},
								  {2, 100},
								  {2, 200},
								  {3, 200},
								  {3, 300},
								  {4, 300},
								  {5, 300}};
#endif

/**********************************************************************************************************************/

typedef enum
{
    APP_ROOT_MSG_DO_ASYNC=GUI_MSG_+1,
}app_root_msg_id_t;

typedef __s32 (*p_app_root_fun)(void* arg);

/******************************************************************************************************************************/
//cai++
typedef enum
{
	KEY_TO_FM = 0,
	KEY_TO_RECORD,	
	KEY_TO_MOVIE,
	KEY_TO_PHOTO,
	KEY_TO_MUSIC,
	KEY_TO_CALENDAR,
	KEY_TO_SETTING,
	KEY_TO_LINEIN,
	KEY_TO_EBOOK, 
}key_to_app_t;	

typedef enum app_root_type
{
	APP_ROOT_TYPE_MUSIC = 1,
	APP_ROOT_TYPE_MOVIE,
	APP_ROOT_TYPE_PHOTO,

	APP_ROOT_TYPE_MAX
}app_root_type_e;


#define APP_COM_DLG_ID 	(APP_ROOT_ID+1)
/*******************************************************************************************************************************/

static __krnl_event_t* g_root_scene_sem = NULL;
__u32 CurrDevType;
static __s32 app_root_show_dlg(H_WIN hwin, __s32 id, __s32 time)
{
	H_WIN dlg;
	__s32 lang_id[]={STRING_SET_CUE,STRING_SET_CUE};	
	__s32 null_disk;

	lang_id[1] = id;
	
	default_dialog(dlg, hwin, APP_COM_DLG_ID, ADLG_OK, lang_id);
    GUI_LyrWinCacheOff(); 
    if(0 != time)
    {
	    esKRNL_TimeDly(time);
    }
    else
    {
        esKRNL_TimeDly(150);
    }   
	app_dialog_destroy(dlg);
}
	
 __u32 SetCurrentDevTypeEx(__u32 curdev)
{
	CurrDevType=curdev;
	return EPDK_OK;
}
__u32 GetCurrentDevTypeEx(void)
{
	return CurrDevType;
}

//检查是否存在U盘或SD卡
//若两个都存在，则高8位为SD, 低8位为USB
__u32 root_check_disk(void)    //cai++ for kaiji_music
{
    __s32 i;
    __s32 ret;
    __s32 cnt;
    __u32 root_type;
    char diskname[RAT_MAX_PARTITION][4];

__msg("**********check_disk**********\n");
    cnt = 0;
    root_type = RAT_UNKNOWN;    
	
    ret = rat_get_partition_name(RAT_USB_DISK, diskname,1);
    if(EPDK_OK == ret)
    {
    	i = 0;    
	__msg("get USB fs part\n");	
    	{
    		if (diskname[i][0])
    		{
    			__msg("USB : diskname[%d]=%s\n", i, diskname[i]);
                root_type += RAT_USB;
                cnt++;
    		}						
    		i++;
    	}
    }
/*
    ret = rat_get_partition_name(RAT_SD_CARD, diskname,1);
    if(EPDK_OK == ret)
    {    
	__msg("get SD fs part\n");		
    	i = 0;    

    	{
    		if (diskname[i][0])
    		{
			__msg("SD : diskname[%d]=%s\n", i, diskname[i]);
          root_type += RAT_TF<<8;
          cnt++;
    		}						
    		i++;
    	}
    }
*/
__msg("===========================check root type is %d\n", root_type);	
	
	return root_type;   
}

//记录上一次播放的应用;
static __s32 app_root_set_last_root_type(app_root_type_e type)
{
	reg_root_para_t* para;

	para = (reg_root_para_t*)dsk_reg_get_para_by_app(REG_APP_ROOT);
	if(para == NULL)
	{
		return EPDK_FAIL;
	}
	para->last_app_root_type = type;
	dsk_reg_flush();
	
	return EPDK_OK;

}
static app_root_type_e app_root_get_last_root_type()
{
	reg_root_para_t* para;
	app_root_type_e type;
	
	para = (reg_root_para_t*)dsk_reg_get_para_by_app(REG_APP_ROOT);
	if(para == NULL)
	{
		return EPDK_FAIL;
	}
	type = para->last_app_root_type;

	return type;
}

//检查对应的盘符是否存在指定的媒体文件
__s32 root_check_mediafile_exist(__s32 root_type, rat_media_type_t type)
{
	__s32 ret, total;
	HRAT  h_rat = 0;
	char path[RAT_MAX_FULL_PATH_LEN];

	eLIBs_memset(path, 0, sizeof(path));
	ret = root_rat_get_search_path(root_type, path);	
	if(ret == EPDK_FAIL)
	{
		return EPDK_FAIL;
	}
	if(eLIBs_strlen(path) == 0)
	{
		return EPDK_FAIL;
	}
	
	h_rat  = rat_open(path, type, 0);	//在此处查盘，在此处检验是否需要多次搜索盘里面的对应媒体文件!
	
	if(h_rat == NULL)
	{
		__wrn("!!!!!!!!!!!!!open rat fail!!!!!!!!!!!!!!!!!\n");
		return EPDK_FAIL;
	}	
	//rat_move_cursor_to_first(list_para->rat.handle);
	
	total = rat_get_cur_scan_cnt(h_rat);
	
	if(total)
	{
		return EPDK_OK;	
	}
	return EPDK_FAIL;
}
//根据root_type获得全盘搜索路径
__s32  root_rat_get_search_path(__s32 root_type, char* path)
{
	__s32 ret = 0;
	char s_root_type[50];
	char disk_name[RAT_MAX_PARTITION][4];
	
	eLIBs_memset(s_root_type, 0, sizeof(s_root_type));
	eLIBs_memset(disk_name, 0, sizeof(disk_name));	

	switch(root_type)
	{
		case RAT_LOCAL:
			eLIBs_strcpy(s_root_type,RAT_LOCAL_DISK);				
			break;
		case RAT_TF:
			eLIBs_strcpy(s_root_type,RAT_SD_CARD);				
			break;
		case RAT_USB:
			eLIBs_strcpy(s_root_type,RAT_USB_DISK);				
			break;
		default:
			eLIBs_strcpy(s_root_type,RAT_SD_CARD);
			break;
	}
	ret = rat_get_partition_name(s_root_type, disk_name,1);
	eLIBs_strcpy(path,disk_name[0]);	
	
    __msg("huang ######disk_name[0]=%s\n", disk_name[0]);
    //eLIBs_strcpy(list_ctrl->search_path,"f:");//112350
    
	return ret;
}

__s32 __root_scene_sem_init(void)
{
    if (0 == g_root_scene_sem)
	{
		g_root_scene_sem = esKRNL_SemCreate(1);
	}

    if(g_root_scene_sem)
    {
        return EPDK_OK;
    }
    else
    {
        return EPDK_FAIL;
    }
}

__s32 __root_scene_sem_deinit(void)
{
    if (g_root_scene_sem)
	{		
        __u8 err;
        
		esKRNL_SemDel(g_root_scene_sem, OS_DEL_ALWAYS, &err);		
		g_root_scene_sem = NULL;
	}
    
    return EPDK_OK;
}

__s32 __root_scene_sem_pend(void)
{
    __u8 err;

    if(g_root_scene_sem)
    {
        __msg("before esKRNL_SemPend(g_root_scene_sem\n");
	    esKRNL_SemPend(g_root_scene_sem, 0, &err);
        __msg("after esKRNL_SemPend(g_root_scene_sem\n");
    }

    return 0;
}

__s32 __root_scene_sem_accept(void)
{
    __u16 sem_nr;

    sem_nr = 0;

    if(g_root_scene_sem)
    {
	    sem_nr = esKRNL_SemAccept(g_root_scene_sem);
    }
    
    return sem_nr;
}

__s32 __root_scene_sem_post(void)
{
    if(g_root_scene_sem)
    {
        esKRNL_SemPost(g_root_scene_sem);
    }
    
    return 0;
}


//向hwin发送异步消息，执行pfun函数
static __s32 __app_do_async(H_WIN hwin, __u32 msg_id, p_app_root_fun pfun, void* arg)
{
    if(0 == hwin)
    {
        return EPDK_FAIL;
    }

    {		
		__gui_msg_t setmsg;
		setmsg.id			=  msg_id;		
		setmsg.h_srcwin		=  NULL;
		setmsg.h_deswin		=  hwin;
		setmsg.dwAddData1	=  (__u32)pfun;
		setmsg.dwAddData2	=  (__u32)arg;
		setmsg.p_arg		=  NULL;		

        __msg("setmsg.id=%x, setmsg.h_deswin=%x, setmsg.dwAddData1=%x\n, setmsg.dwAddData2=%x\n",
            setmsg.id, setmsg.h_deswin, setmsg.dwAddData1, setmsg.dwAddData2);
		GUI_SendNotifyMessage(&setmsg);	
	}   
    
    return EPDK_OK;
}

//向hwin发送setfocus消息，syn 为1代表需要直接调用GUI_WinSetFocusChild。
static void __app_set_focus_child(H_WIN hwin, __s32 syn)
{

	__msg("huang ##############   __app_set_focus_child \n");
	if (0 == hwin)
	{
		return;
	}

	if (1 == syn)
	{
		GUI_WinSetFocusChild(hwin);
	}
	else
	{		
		__gui_msg_t setmsg;
		setmsg.id			=  GUI_MSG_SET_FOCUS;		
		setmsg.h_srcwin		=  NULL;
		setmsg.h_deswin		=  hwin;
		setmsg.dwAddData1	=  NULL;
		setmsg.dwAddData2	=  NULL;
		setmsg.p_arg		=  NULL;			
		GUI_SendNotifyMessage(&setmsg);	
	}
	
}

__s32 delete_all_app(root_ctrl_t   *root_ctrl)  //cai++ for kaiji_music
{
	reg_root_para_t* root_para;
	//H_LYR layer;

	if(root_ctrl == NULL)
	{
		return EPDK_FAIL;
	}
	__msg("................delete_all_app...........................\n");
	root_para = (reg_root_para_t*)dsk_reg_get_para_by_app(REG_APP_ROOT);
   
	if(root_ctrl->h_app_home != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_home);
		root_ctrl->h_app_home = 0;
	//	root_para ->last_app_id = APP_HOME_ID;
	//	root_para->last_app_root_type = root_ctrl->root_para->root_type;
	}


	if(root_ctrl->h_app_music!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_music);	
		root_ctrl->h_app_music = 0; 
	//	root_para->last_app_id = APP_MUSIC_ID;
	//	root_para->last_app_root_type = root_ctrl->root_para->root_type;		
	}
		__here__;
	if(root_ctrl->h_app_movie!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_movie);
		root_ctrl->h_app_movie = 0; 
	//	root_para->last_app_id = APP_MOVIE_ID;
	//	root_para->last_app_root_type = root_ctrl->root_para->root_type;		
	}
		__here__;
	if(root_ctrl->h_app_photo!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_photo);
		root_ctrl->h_app_photo = 0; 
	//	root_para->last_app_id = APP_PHOTO_ID;
	//	root_para->last_app_root_type = root_ctrl->root_para->root_type;		
	}
		__here__;
	if(root_ctrl->h_app_explorer!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_explorer);
		root_ctrl->h_app_explorer = 0; 
	//	root_para->last_app_id = APP_EXPLORER_ID;
	//	root_para->last_app_root_type = root_ctrl->root_para->root_type;		
	}
	if(root_ctrl->h_app_fm!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_fm);	
		root_ctrl->h_app_fm = 0; 
	//	root_para->last_app_id = APP_MUSIC_ID;
	//	root_para->last_app_root_type = root_ctrl->root_para->root_type;		
	}
	if(root_ctrl->h_app_record!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_record);	
		root_ctrl->h_app_record = 0; 
	//	root_para->last_app_id = APP_MUSIC_ID;
	//	root_para->last_app_root_type = root_ctrl->root_para->root_type;		
	}
	if(root_ctrl->h_app_linin != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_linin);
		root_ctrl->h_app_linin = 0;
	}
		__here__;
	if(root_ctrl->h_app_setting != NULL)//yoursmile 2011_11_25
	{
		GUI_ManWinDelete(root_ctrl->h_app_setting);
		root_ctrl->h_app_setting =0;
	//	root_para->last_app_id = APP_SETTING_ID;
	//	root_para->last_app_root_type = root_ctrl->root_para->root_type;
	}
	if(root_ctrl->h_app_calendar != NULL)//yoursmile 2011_11_25
	{
		GUI_ManWinDelete(root_ctrl->h_app_calendar);
		root_ctrl->h_app_calendar =0;
	//	root_para->last_app_id = APP_SETTING_ID;
	//	root_para->last_app_root_type = root_ctrl->root_para->root_type;
	}
	if(root_ctrl->h_app_ebook != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_ebook);
		root_ctrl->h_app_ebook = 0;
	}

	//g_enable_close_scn();
    	__here__;
    return EPDK_OK;
	
}
__s32 delete_all_app_except_music(root_ctrl_t   *root_ctrl)  //cai++ for kaiji_music
{
	reg_root_para_t* root_para;

	if(root_ctrl == NULL)
	{
		return EPDK_FAIL;
	}
	__msg("................delete_all_app_except_music...........................\n");
	root_para = (reg_root_para_t*)dsk_reg_get_para_by_app(REG_APP_ROOT);
   
	if(root_ctrl->h_app_home != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_home);
		root_ctrl->h_app_home = 0;
	}
		__here__;
	if(root_ctrl->h_app_movie!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_movie);
		root_ctrl->h_app_movie = 0; 	
	}
		__here__;
	if(root_ctrl->h_app_photo!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_photo);
		root_ctrl->h_app_photo = 0; 	
	}
		__here__;
	if(root_ctrl->h_app_explorer!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_explorer);
		root_ctrl->h_app_explorer = 0; 	
	}
	if(root_ctrl->h_app_fm!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_fm);	
		root_ctrl->h_app_fm = 0; 	
	}
	if(root_ctrl->h_app_record!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_record);	
		root_ctrl->h_app_record = 0; 	
	}
	if(root_ctrl->h_app_linin != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_linin);
		root_ctrl->h_app_linin = 0;
	}
	if(root_ctrl->h_app_setting != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_setting);
		root_ctrl->h_app_setting =0;
	}
	if(root_ctrl->h_app_calendar != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_calendar);
		root_ctrl->h_app_calendar =0;
	}
	if(root_ctrl->h_app_ebook != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_ebook);
		root_ctrl->h_app_ebook = 0;
	}
	//g_enable_close_scn();

		__here__;
    return EPDK_OK;
	
}

__s32 delete_all_app_except_home(root_ctrl_t   *root_ctrl)  //cai++ for ir power
{
	reg_root_para_t* root_para;
	//H_LYR layer;

	if(root_ctrl == NULL)
	{
		return EPDK_FAIL;
	}
	__msg("................delete_all_app...........................\n");
	root_para = (reg_root_para_t*)dsk_reg_get_para_by_app(REG_APP_ROOT);
   
	if(root_ctrl->h_app_music!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_music);	
		root_ctrl->h_app_music = 0; 
	}
		__here__;
	if(root_ctrl->h_app_movie!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_movie);
		root_ctrl->h_app_movie = 0; 	
	}
		__here__;
	if(root_ctrl->h_app_photo!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_photo);
		root_ctrl->h_app_photo = 0; 	
	}
		__here__;
	if(root_ctrl->h_app_explorer!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_explorer);
		root_ctrl->h_app_explorer = 0; 	
	}
	if(root_ctrl->h_app_fm!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_fm);	
		root_ctrl->h_app_fm = 0; 
	}
	if(root_ctrl->h_app_record!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_record);	
		root_ctrl->h_app_record = 0; 		
	}
	if(root_ctrl->h_app_linin != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_linin);
		root_ctrl->h_app_linin = 0;
	}
		__here__;
	if(root_ctrl->h_app_setting != NULL)//yoursmile 2011_11_25
	{
		GUI_ManWinDelete(root_ctrl->h_app_setting);
		root_ctrl->h_app_setting =0;
	}
	if(root_ctrl->h_app_calendar != NULL)//yoursmile 2011_11_25
	{
		GUI_ManWinDelete(root_ctrl->h_app_calendar);
		root_ctrl->h_app_calendar =0;
	}
	if(root_ctrl->h_app_ebook != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_ebook);
		root_ctrl->h_app_ebook = 0;
	}
	//g_enable_close_scn();
    	__here__;
    return EPDK_OK;
	
}

__s32 delete_all_app_suspend_home(root_ctrl_t   *root_ctrl)  //cai++ for ir power
{
	reg_root_para_t* root_para;
	//H_LYR layer;
						__gui_msg_t mymsg;

	if(root_ctrl == NULL)
	{
		return EPDK_FAIL;
	}
   
	
	if(root_ctrl->h_app_home)
	{
		__gui_msg_t mymsg;
		eLIBs_printf("-- %s, %d, Send msg to suspend home -- \n", __FILE__, __LINE__);
		mymsg.h_deswin = root_ctrl->h_app_home;
		mymsg.id = DSK_APP_SUSPEND;
		mymsg.dwAddData1 =0;
		mymsg.dwAddData2 = 0;
		GUI_SendNotifyMessage(&mymsg);
	}
	delete_all_app_except_home(root_ctrl);
	
    return EPDK_OK;
	
}

#if 0
__s32 delete_all_app_except_music_suspend_home(root_ctrl_t   *root_ctrl)  //cai++ for kaiji_music
{
	reg_root_para_t* root_para;

	if(root_ctrl == NULL)
	{
		return EPDK_FAIL;
	}

	if(root_ctrl->h_app_home)
	{
		__gui_msg_t mymsg;
		eLIBs_printf("-- %s, %d, Send msg to suspend home -- \n", __FILE__, __LINE__);
		mymsg.h_deswin = root_ctrl->h_app_home;
		mymsg.id = DSK_APP_SUSPEND;
		mymsg.dwAddData1 =0;
		mymsg.dwAddData2 = 0;
		GUI_SendNotifyMessage(&mymsg);
	}
	
	__msg("................delete_all_app_except_music...........................\n");
	root_para = (reg_root_para_t*)dsk_reg_get_para_by_app(REG_APP_ROOT);
   
	if(root_ctrl->h_app_movie!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_movie);
		root_ctrl->h_app_movie = 0; 	
	}
		__here__;
	if(root_ctrl->h_app_photo!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_photo);
		root_ctrl->h_app_photo = 0; 	
	}
		__here__;
	if(root_ctrl->h_app_explorer!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_explorer);
		root_ctrl->h_app_explorer = 0; 	
	}
	if(root_ctrl->h_app_fm!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_fm);	
		root_ctrl->h_app_fm = 0; 	
	}
	if(root_ctrl->h_app_record!= NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_record);	
		root_ctrl->h_app_record = 0; 	
	}
	if(root_ctrl->h_app_linin != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_linin);
		root_ctrl->h_app_linin = 0;
	}
	if(root_ctrl->h_app_setting != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_setting);
		root_ctrl->h_app_setting =0;
	}
	if(root_ctrl->h_app_calendar != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_calendar);
		root_ctrl->h_app_calendar =0;
	}
	if(root_ctrl->h_app_ebook != NULL)
	{
		GUI_ManWinDelete(root_ctrl->h_app_ebook);
		root_ctrl->h_app_ebook = 0;
	}
	//g_enable_close_scn();

		__here__;
    return EPDK_OK;
	
}
#endif

static void __app_home_restore(__gui_msg_t* msg)
{
    root_ctrl_t   *root_ctrl;
    
    root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	if (root_ctrl->h_app_home)
	{
		__gui_msg_t mymsg;
		
		mymsg.h_deswin = root_ctrl->h_app_home;
		mymsg.id = DSK_APP_RESTORE;
		mymsg.dwAddData1 =0;
		mymsg.dwAddData2 = 0;
		GUI_SendNotifyMessage(&mymsg);
						
		mymsg.h_deswin = root_ctrl->h_app_home;
		mymsg.id = DSK_MSG_FS_PART_PLUGOUT;
		mymsg.dwAddData1 = msg->dwAddData1;
		mymsg.dwAddData2 = msg->dwAddData2;
		GUI_SendNotifyMessage(&mymsg);
__msg("come to on lyr to home\n");
		__app_set_focus_child(root_ctrl->h_app_home, 0);
	}
	else
	{
__msg("come to create to home\n");
		root_ctrl->h_app_home = app_home_create(root_ctrl->root_para);
		__app_set_focus_child(root_ctrl->h_app_home, 0);
	}

	//g_disable_close_scn();
}

void app_root_volume_on(void)
{
	dsk_volume_on();
         g_is_mute = 0;	//wqx 20130406
}

__s32 app_root_mute_key_proc(__gui_msg_t *msg)
{
	static H_WIN dlg;
	root_ctrl_t   	*root_ctrl;
	root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	
	
	if(root_ctrl == NULL)
	{
		return EPDK_FAIL;
	}
	__msg("............mute.........................\n");
	if(KEY_UP_ACTION == msg->dwAddData2)
	{
	switch(msg->dwAddData1)
			{				
				case GUI_MSG_KEY_IR_VADD:
				case GUI_MSG_KEY_LONGIR_VADD:	
				{
					if(msg->dwAddData2== KEY_UP_ACTION)
					{
					   if( dsk_volume_is_on() == EPDK_FALSE)//sun++消除静音
						{
						__msg(".............on.........................\n");
						if (dlg!=NULL)
					{
						__msg("....444444................\n");
						app_dialog_destroy(dlg);
						dlg = NULL;
					}
						dsk_volume_on();
	                    	g_is_mute = 0;	//wqx 20130406
						}
						//dsk_volume_inc();
					}
					if(root_ctrl->h_app_home || root_ctrl->h_app_photo || root_ctrl->h_app_setting 
					   || root_ctrl->h_app_fm || root_ctrl->h_app_explorer || root_ctrl->h_app_calendar
					   || root_ctrl->h_app_record)
					{
						//dsk_volume_inc();
					}
					break;
				}	
				case GUI_MSG_KEY_IR_VDEC:	
				case GUI_MSG_KEY_LONGIR_VDEC:
				{
					if(msg->dwAddData2 == KEY_UP_ACTION)
					{
					   if( dsk_volume_is_on() == EPDK_FALSE)//sun++消除静音
						{
							__msg(".............on.........................\n");
							if (dlg!=NULL)
							{
								__msg("....444444................\n");
								app_dialog_destroy(dlg);
								dlg = NULL;
							}
							dsk_volume_on();
	                    	g_is_mute = 0;	//wqx 20130406
							
						}
						//dsk_volume_dec();
					if(root_ctrl->h_app_home || root_ctrl->h_app_photo || root_ctrl->h_app_setting 
					   || root_ctrl->h_app_fm || root_ctrl->h_app_explorer || root_ctrl->h_app_calendar
					   || root_ctrl->h_app_record)
					{
						//dsk_volume_dec();
					}
					}
					break;
				}
				//case GUI_MSG_KEY_LONGENTER:
				case GUI_MSG_KEY_IR_MUTE:
				case GUI_MSG_KEY_LONGIR_MUTE:
				{
				if( dsk_volume_is_on() == EPDK_TRUE)
					{
#if 0
						__s32 _bmp[] = {ID_MUSIC_VOLUME_MUTE_BMP};
						GUI_RECT gui_rect;
						__msg("volume mute bmp id = %d\n", ID_INIT_VOLUME_MUTE_BMP);
						gui_rect.x0 = 450;
						gui_rect.y0 = 4;//252;
						gui_rect.x1 = 465;
						gui_rect.y1 = 17;//268;
					__msg("..............off.......................\n");		
	                  	dlg= app_dialog_create_ex(msg->h_deswin, APP_COM_DLG_ID,(ADLG_TA_CENTER | ADLG_DS_VISIBLE | ADLG_MAKE_WND_STYLE(ADLG_ICONINFORMATION, ADLG_MUTE)), _bmp, NULL, 0, &gui_rect);				  
						if (dlg==NULL)                                                                        
						{
							__msg("....444444................\n");
						}
#endif						
						dsk_volume_off();
                    	g_is_mute = 1;	//wqx 20130406
						
						return EPDK_OK;  //cai modify
					}
					else	
					{
#if 0
						__msg(".............on.........................\n");
						if (dlg==NULL)
						{
							__msg("....3333................\n");
						}
						
						if (dlg!=NULL)
						{
							__msg("....444444................\n");
							app_dialog_destroy(dlg);
							dlg = NULL;
						}
#endif						
						dsk_volume_on();
                    	g_is_mute = 0;	//wqx 20130406
						return EPDK_OK;  //cai modify
					}
					__msg("-----------get a mute key action, addData2 = %d\n",msg->dwAddData2);
					break;
				}
				default:
					break;
			}		
		
	}
	else if(KEY_REPEAT_ACTION == msg->dwAddData2)
	{
	switch(msg->dwAddData1)
			{				
				//case GUI_MSG_KEY_IR_VADD:
				case GUI_MSG_KEY_LONGVADD:	
				{
					//if(msg->dwAddData2== KEY_UP_ACTION)
					{
					   if( dsk_volume_is_on() == EPDK_FALSE)//sun++消除静音
						{
						__msg(".............on.........................\n");
						if (dlg!=NULL)
					{
						__msg("....444444................\n");
						app_dialog_destroy(dlg);
						dlg = NULL;
					}
						dsk_volume_on();
						}
						//dsk_volume_inc();
					}
					if(root_ctrl->h_app_home || root_ctrl->h_app_photo || root_ctrl->h_app_setting 
					   || root_ctrl->h_app_fm || root_ctrl->h_app_explorer || root_ctrl->h_app_calendar
					   || root_ctrl->h_app_record)
					{
						//dsk_volume_inc();
					}
					break;
				}	
				//case GUI_MSG_KEY_IR_VDEC:	
				case GUI_MSG_KEY_LONGVDEC:
				{
					//if(msg->dwAddData2 == KEY_UP_ACTION)
					{
					   if( dsk_volume_is_on() == EPDK_FALSE)//sun++消除静音
						{
							__msg(".............on.........................\n");
							if (dlg!=NULL)
							{
								__msg("....444444................\n");
								app_dialog_destroy(dlg);
								dlg = NULL;
							}
							dsk_volume_on();
						}
						//dsk_volume_dec();
					if(root_ctrl->h_app_home || root_ctrl->h_app_photo || root_ctrl->h_app_setting 
					   || root_ctrl->h_app_fm || root_ctrl->h_app_explorer || root_ctrl->h_app_calendar
					   || root_ctrl->h_app_record)
					{
						//dsk_volume_dec();
					}
					}
					break;
				}
				//case GUI_MSG_KEY_LONGENTER:
				
				default:
					break;
			}		
		
	}
	__msg("exit app_root_mute_key_proc............\n");
	return EPDK_FAIL;
}


#if 0
static __s32 app_root_check_short_key(__gui_msg_t *msg)
{
}

#else

static __s32  __app_root_get_search_path(rat_root_t rat_root_type, char* search_path)
{
	__s32 ret;
	char str_type[50];
	char disk_name[RAT_MAX_PARTITION][4];
	
	eLIBs_memset(str_type, 0, sizeof(str_type));
	eLIBs_memset(disk_name, 0, sizeof(disk_name));
	eLIBs_memset(search_path, 0, sizeof(search_path));
		
	switch(rat_root_type)
	{
		case RAT_LOCAL:
			eLIBs_strcpy(str_type,RAT_LOCAL_DISK);				
			break;
		case RAT_TF:
			eLIBs_strcpy(str_type,RAT_SD_CARD);				
			break;
		case RAT_USB:
			eLIBs_strcpy(str_type,RAT_USB_DISK);				
			break;
		default:
            return EPDK_FAIL;			
	}
    __wrn("===================str_type=%s\n", str_type);
	//ret = rat_get_partition_name(str_type, disk_name, 0);    
	ret = rat_get_partition_name(str_type, disk_name,1);   
	eLIBs_strcpy(search_path,disk_name[0]);	
    __wrn("===================rat root type=%d\n", rat_root_type);
    __wrn("===================search_path=%s\n", search_path);
    if(0 == search_path[0])
    {
        return EPDK_FAIL;
    }
    
    return ret;
}
#endif


#if 0
//return EPDK_OK, 表示成功,
//return EPDK_FAIL,表示盘符中没有视频\音频文件,则返回到浏览器,告知客户没有文件
static __s32 __app_root_set_playfile(rat_root_t rat_root_type, rat_media_type_t rat_media_type)
{
	char file_path[RAT_MAX_FULL_PATH_LEN];
	HRAT hrat = NULL;
	__u32 total = 0;
	__u32 last_file_index = 0;
	char *last_file_path = NULL;
	reg_root_para_t* para;
	int ret;
	rat_entry_t ItemInfo;
		
	if(RAT_UNKNOWN== rat_root_type || RAT_MEDIA_TYPE_UNKNOWN == rat_media_type)
	{
		return EPDK_FAIL;
	}
	
	eLIBs_memset(file_path, 0, sizeof(file_path));
	
	ret = __app_root_get_search_path(rat_root_type, file_path);
	if(EPDK_FAIL == ret || eLIBs_strlen(file_path) == 0)	
	{
		return EPDK_FAIL;
	}
	
	hrat = rat_open(file_path, rat_media_type, 0);	
   // hrat = rat_open_ex(rat_root_type, rat_media_type, 0);	
	if(hrat == NULL)
	{
		return EPDK_FAIL;
	}
	
	total = rat_get_cur_scan_cnt(hrat);
	if(total == 0)
	{
		return EPDK_FAIL;
	}
	
	para = (reg_root_para_t*)dsk_reg_get_para_by_app(REG_APP_ROOT);
	if(para == NULL)
	{
		return EPDK_FAIL;
	}

    if(RAT_MEDIA_TYPE_VIDEO == rat_media_type)
    {
    	if(rat_root_type == RAT_TF)
    	{			
    		last_file_index = para->last_movie_index_sd;
    		last_file_path = para->last_movie_path_sd;
    	}
    	else
    	{
    		last_file_index = para->last_movie_index_ud;
    		last_file_path = para->last_movie_path_ud;	
    	}
    }
    else if(RAT_MEDIA_TYPE_AUDIO == rat_media_type)
    {
        if(rat_root_type == RAT_TF)
    	{			
    		last_file_index = para->last_music_index_sd;
    		last_file_path = para->last_music_path_sd;
    	}
    	else
    	{
    		last_file_index = para->last_music_index_ud;
    		last_file_path = para->last_music_path_ud;	
    	}
    }
    else if(RAT_MEDIA_TYPE_EBOOK == rat_media_type)
    {
        if(rat_root_type == RAT_TF)
    	{			
    		last_file_index = para->last_ebook_index_sd;
    		last_file_path = para->last_ebook_path_sd;
    	}
    	else
    	{
    		last_file_index = para->last_ebook_index_ud;
    		last_file_path = para->last_ebook_path_ud;	
    	}
    }
    else if(RAT_MEDIA_TYPE_PIC == rat_media_type)
    {
        if(rat_root_type == RAT_TF)
    	{			
    		last_file_index = para->last_photo_index_sd;
    		last_file_path = para->last_photo_path_sd;
    	}
    	else
    	{
    		last_file_index = para->last_photo_index_ud;
    		last_file_path = para->last_photo_path_ud;	
    	}
    }
	
	eLIBs_memset(&ItemInfo,0,sizeof(rat_entry_t));				
	ret = rat_get_item_info_by_index(hrat, last_file_index, &ItemInfo);	
	
	if(ret == EPDK_OK)
	{
			eLIBs_memset(file_path, 0, sizeof(file_path));
			eLIBs_strcpy(file_path, ItemInfo.Path);
			ret = eLIBs_strcmp(file_path, last_file_path);
			if(ret == 0)		//相等,则播放上一次的文件
			{
				rat_set_file_for_play(hrat,last_file_path);
				return EPDK_OK;
			}
	}		
	
	ret = rat_get_item_info_by_index(hrat, 0, &ItemInfo);	//直接播放第一首
	if(ret == EPDK_OK)
	{
		eLIBs_memset(file_path, 0, sizeof(file_path));
		eLIBs_strcpy(file_path, ItemInfo.Path);
		rat_set_file_for_play(hrat,file_path);
		return EPDK_OK;
	}	
	
	return EPDK_FAIL;		
}


void key2app(__gui_msg_t *msg, key_to_app_t app_flag)
{
	__u32 root_type = 0, temp_root=0, temp2;
	__u8 sd_root=0, usb_root = 0, device_cnt = 0;//device_cnt, 当前设备的个数	
	root_ctrl_t   *root_ctrl;
	root_para_t* root_para = NULL;
       char str[256]={0};
        H_WIN hmusic;
	   
	root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;
	
__msg("app_flag = %d\n", app_flag);
__msg("KEY_TO_SETTING = %d\n",KEY_TO_SETTING);
__msg("KEY_TO_CALENDAR = %d\n", KEY_TO_CALENDAR);
__msg("KEY_TO_FM = %d\n", KEY_TO_FM);

	if(app_flag !=KEY_TO_FM 
	   && app_flag !=KEY_TO_CALENDAR 
	   && app_flag !=KEY_TO_LINEIN
	   && app_flag !=KEY_TO_SETTING)
	{
		root_type = root_check_disk();	
		 __msg(".......11..............plugin******\n");
		__msg("-----22------root_type=0x%x----\n",root_type);
		sd_root = (root_type >> 8)&0x00ff;
		usb_root = (root_type)&0x00ff; 

	    __msg("---------------------------------------------------sdmmc or usb plugin\n");
		__msg("......33..........sd root is %d, usb_root is %d\n", sd_root, usb_root);			
				
		if(sd_root)
		{
			if(usb_root)	                    //两个都在,比较得知新插入的设备
			{
			__msg("......33.....root_ctrl->root_para->root_type is %d\n", root_ctrl->root_para->root_type);	
				if(root_ctrl->root_para->root_type == sd_root)//
				{			
					//temp_root = RAT_USB;        //新插入的设备为USB
					temp_root = RAT_TF;			//新插入的设备为SD			
					
				}
				else
				{
					//temp_root = RAT_TF;			//新插入的设备为SD			
					temp_root = RAT_USB;        //新插入的设备为USB
				}
	            device_cnt = 2;
			}	
			else
			{
				temp_root = RAT_TF;
	            device_cnt = 1;
			}				
		}			
		else if(usb_root)	                    //插入的是USB设备,当前只有USB设备		
		{
			temp_root = RAT_USB;
	        device_cnt = 1;
		}		
	    else
	    {
	    		app_root_show_dlg(root_ctrl->root_para->h_parent, STRING_HOME_NULL_DISK, 0);
	        	return;
	    }
	}
__here__;
		__msg("-- key2app  temp_root = %d -- \n", temp_root);
		switch(app_flag)
		{
			case KEY_TO_MOVIE:
			{
				
				//if(root_check_mediafile_exist(temp_root, RAT_MEDIA_TYPE_VIDEO) || root_ctrl->h_app_movie != NULL)  //检查对应盘是否存在对应的音频文件
				if(__app_root_set_playfile(temp_root,RAT_MEDIA_TYPE_VIDEO))
				{
					the_media_file=no_movie_file;
					__here__;
					app_root_show_dlg(root_ctrl->root_para->h_parent, STRING_HOME_NO_FILE, 0);
					
					return; //可以在此处创建无对应音频文件对话框
	                        	
				}
				else
				{
					the_media_file=0;
					__here__;
					//delete_all_app(root_ctrl);
					delete_all_app_suspend_home(root_ctrl);
				}		
                         __msg("-----44------temp_root=%d----\n",temp_root);

				app_root_set_last_root_type(APP_ROOT_TYPE_MOVIE);

				//gscene_hbar_set_state(HBAR_ST_HIDE);
#ifdef KEY_VOICE
				app_root_play_app_sounds(SHORT_KEY_WAVE_FILE_MOVIE);
#endif				
        			root_ctrl->root_para->root_type = temp_root; 
        			root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;  
				root_ctrl->root_para->data = KEY_SW_TO_MOVIE;
        			root_ctrl->root_para->h_parent =  msg->h_deswin;               
        			root_ctrl->h_app_movie = app_movie_create(root_ctrl->root_para);					

        			GUI_WinSetFocusChild(root_ctrl->h_app_movie);

				break;	
			}			
			case KEY_TO_MUSIC:
			{
				  //检查盘是否存在对应文件，这个查过一次后，会记录下来，第二次就不会再重新查盘，这个需要检验下看
				if(root_check_mediafile_exist(temp_root, RAT_MEDIA_TYPE_AUDIO))  //检查对应盘是否存在对应的音频文件
				{
					the_media_file=no_music_file;
					__here__;
					app_root_show_dlg(root_ctrl->root_para->h_parent, STRING_HOME_NO_FILE, 0);
					return; //可以在此处创建无对应音频文件对话框
	                        	
				}
				else
				{
					the_media_file=0;
					//if(root_ctrl->h_app_music == NULL)		
					//delete_all_app_except_music(root_ctrl);
					delete_all_app_except_music_suspend_home(root_ctrl);
				}		
                         __msg("-----44------temp_root=%d----\n",temp_root);
				app_root_set_last_root_type(APP_ROOT_TYPE_MUSIC);

				gscene_hbar_set_state(HBAR_ST_HIDE);
#ifdef KEY_VOICE
				app_root_play_app_sounds(SHORT_KEY_WAVE_FILE_MUSIC);
#endif				
        			root_ctrl->root_para->root_type = temp_root; 
        			root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;  
				root_ctrl->root_para->data = MUSICPLAYER_NORMAL;
        			root_ctrl->root_para->h_parent =  msg->h_deswin;
				//if(root_ctrl->h_app_music != NULL)
				if(__app_root_is_bg_music_exist(msg,&hmusic))
					app_music_restore(root_ctrl->h_app_music, root_ctrl->root_para);
				else
        				root_ctrl->h_app_music = app_music_create(root_ctrl->root_para);					

        			GUI_WinSetFocusChild(root_ctrl->h_app_music);

				break;
			}
			case KEY_TO_FM:
			{
				if(root_ctrl->h_app_linin)
				{
		    		app_root_show_dlg(root_ctrl->root_para->h_parent, STRING_HOME_CLOSE_LINE, 0);
				}
				else
				{
					//delete_all_app(root_ctrl);
					delete_all_app_suspend_home(root_ctrl);

					gscene_hbar_set_state(HBAR_ST_HIDE);
		
					root_ctrl->root_para->h_parent= msg->h_deswin;
					root_ctrl->h_app_fm =  app_fm_create(root_para);
					GUI_WinSetFocusChild(root_ctrl->h_app_fm);
				}
				break;
			}
			case KEY_TO_LINEIN:
			{
                         __msg("-----KEY_TO_LINEIN----\n");
				//delete_all_app(root_ctrl);
				delete_all_app_suspend_home(root_ctrl);

				gscene_hbar_set_state(HBAR_ST_HIDE);
				root_ctrl->root_para->h_parent= msg->h_deswin;
				root_ctrl->root_para->data=LINEIN_MODE_NORMAL;
				root_ctrl->h_app_linin=  app_linein_create(root_para);
				GUI_WinSetFocusChild(root_ctrl->h_app_linin);

				break;
			}
			case KEY_TO_RECORD:
			{
				//delete_all_app(root_ctrl);
				delete_all_app_suspend_home(root_ctrl);

				gscene_hbar_set_state(HBAR_ST_HIDE);
#ifdef KEY_VOICE
	 			app_root_play_app_sounds(SHORT_KEY_WAVE_FILE_RECORD);
#endif	

				dsk_set_record_is_recording(EPDK_TRUE);
				if(EPDK_TRUE == dsk_amplifier_is_on())
					dsk_amplifier_onoff(0);
				esKRNL_TimeDly(10);
				
                root_ctrl->root_para->root_type = temp_root;//RAT_USB或者RAT_TF
                root_ctrl->root_para->record_opt = 0;//正常录音
				root_ctrl->root_para->h_parent= msg->h_deswin;
				root_ctrl->h_app_record=  app_record_create(root_para);
				GUI_WinSetFocusChild(root_ctrl->h_app_record);	

				break;
			}
			case KEY_TO_PHOTO:
			{
				if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_PIC))  //检查对应盘是否存在对应的音频文件
				{
					the_media_file=no_photo_file;
					__here__;
					app_root_show_dlg(root_ctrl->root_para->h_parent, STRING_HOME_NO_FILE, 0);
					return; //可以在此处创建无对应对话框
	                        	
				}
				else
				{	
					the_media_file=0;
					delete_all_app_suspend_home(root_ctrl);

					esKRNL_TimeDly(100);
				}
				app_root_set_last_root_type(APP_ROOT_TYPE_PHOTO);

				gscene_hbar_set_state(HBAR_ST_HIDE);
#ifdef KEY_VOICE
				app_root_play_app_sounds(SHORT_KEY_WAVE_FILE_PHOTO);
#endif				
                            root_para->root_type = temp_root;
                            root_para->explr_root = root_ctrl->root_para->root_type;
                            root_para->data = KEY_SW_TO_PHOTO;
                            root_ctrl->root_para->h_parent= msg->h_deswin;
                            root_ctrl->h_app_photo =  app_photo_create(root_para);
                            GUI_WinSetFocusChild(root_ctrl->h_app_photo);	

				break;
			}
			case KEY_TO_EBOOK:
			{
				if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_EBOOK))  //检查对应盘是否存在对应的音频文件
				{
					the_media_file=no_ebook_file;
					__here__;
					return; //可以在此处创建无对应对话框
	                        	
				}
				else
				{	
						the_media_file=0;
						//delete_all_app(root_ctrl);
						delete_all_app_suspend_home(root_ctrl);
				}

				gscene_hbar_set_state(HBAR_ST_HIDE);
#ifdef KEY_VOICE
	 			app_root_play_app_sounds(SHORT_KEY_WAVE_FILE_EBOOK);
#endif				
                            root_para->root_type = temp_root;
                            root_para->explr_root = root_ctrl->root_para->root_type;
                            root_para->data = KEY_SW_TO_EBOOK;
                            root_ctrl->root_para->h_parent= msg->h_deswin;
                            root_ctrl->h_app_ebook=  app_ebook_create(root_para);
                            GUI_WinSetFocusChild(root_ctrl->h_app_ebook);	

				break;
			}
	#if BEETLES_RTC_EXIST
			case KEY_TO_CALENDAR:
			{
				delete_all_app(root_ctrl);

				gscene_hbar_set_state(HBAR_ST_HIDE);

				root_ctrl->root_para->h_parent= msg->h_deswin;
				root_ctrl->h_app_calendar=  app_calendar_create(root_para);
				GUI_WinSetFocusChild(root_ctrl->h_app_calendar);   	

				break;
			}
	#endif	
			case KEY_TO_SETTING:
			{
				delete_all_app(root_ctrl);

				 gscene_hbar_set_state(HBAR_ST_HIDE);
#ifdef KEY_VOICE
	 			app_root_play_app_sounds(SHORT_KEY_WAVE_FILE_SETTING);
#endif				
                             root_ctrl->root_para->setting_opt = 0;//通用设置
				root_ctrl->root_para->h_parent= msg->h_deswin;
				root_ctrl->h_app_setting=  app_setting_create(root_para);
				GUI_WinSetFocusChild(root_ctrl->h_app_setting);	

				break;
			}
			default:
				break;
		}		
					                                      

    return;
}

__s32 cmd_to_app_key_proc(__gui_msg_t *msg)
{
	__u32 root_type = 0, temp_root=0, temp2;
	__u8 sd_root=0, usb_root = 0, device_cnt = 0;//device_cnt, 当前设备的个数	
	root_ctrl_t   	*root_ctrl;
	root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);	
__here__;
	if(root_ctrl == NULL)
	{
		return EPDK_FAIL;
	}
__here__;

	if((msg->dwAddData1<GUI_MSG_KEY_PHOTO)||(msg->dwAddData1>GUI_MSG_KEY_MODE))
		return EPDK_FAIL;
	//if(root_ctrl->h_app_fm  && GUI_MSG_KEY_FM== msg->dwAddData1)
	{
		//return EPDK_OK;
	}
	if(root_ctrl->h_app_music  && GUI_MSG_KEY_MUSIC== msg->dwAddData1)
	{
		return EPDK_OK;
	}
	if(root_ctrl->h_app_movie  && GUI_MSG_KEY_MOVIE== msg->dwAddData1)
	{
		return EPDK_OK;
	}
	if(root_ctrl->h_app_record && GUI_MSG_KEY_RECORD== msg->dwAddData1)
	{
		return EPDK_OK;
	}
	if(root_ctrl->h_app_photo && GUI_MSG_KEY_PHOTO== msg->dwAddData1)
	{
		return EPDK_OK;
	}


	if(msg->dwAddData1 !=GUI_MSG_KEY_FM 
	   && msg->dwAddData1 !=GUI_MSG_KEY_CALENDAR
	   && msg->dwAddData1 !=GUI_MSG_KEY_SETTING
	   && msg->dwAddData1 !=GUI_MSG_KEY_LINEIN_PLUGIN)
	
	{
		root_type = root_check_disk();	
		sd_root = (root_type >> 8)&0x00ff;
		usb_root = (root_type)&0x00ff; 
				
		if(sd_root)
		{
			if(usb_root)	                    //两个都在,比较得知新插入的设备
			{
				if(root_ctrl->root_para->root_type == sd_root)//
				{			
					temp_root = RAT_USB;        //新插入的设备为USB
					
				}
				else
				{
					temp_root = RAT_TF;			//新插入的设备为SD			
				}
	            device_cnt = 2;
			}	
			else
			{
				temp_root = RAT_TF;
	            device_cnt = 1;
			}				
		}			
		else if(usb_root)	                    //插入的是USB设备,当前只有USB设备		
		{
			temp_root = RAT_USB;
	        device_cnt = 1;
		}		
	    else
	    {
			if(KEY_UP_ACTION == msg->dwAddData2)
			{
				app_root_show_dlg(root_ctrl->root_para->h_parent, STRING_HOME_NULL_DISK, 0);
	        	return EPDK_OK;

			}
	    }
	}

		switch(msg->dwAddData1)
		{
			case GUI_MSG_KEY_MOVIE:
			{
				if(root_ctrl->h_app_movie == NULL && KEY_UP_ACTION == msg->dwAddData2)
				{
					key2app(msg,KEY_TO_MOVIE);
				}				
				return EPDK_OK;
			}
			case GUI_MSG_KEY_MUSIC:
			{
				if(KEY_UP_ACTION == msg->dwAddData2)
				{
					key2app(msg,KEY_TO_MUSIC);
				}
				return EPDK_OK;
			}
			case GUI_MSG_KEY_FM:
			{
				if(root_ctrl->h_app_fm == NULL && KEY_UP_ACTION == msg->dwAddData2)
				{
				__here__;
					key2app(msg, KEY_TO_FM);
				}
				return EPDK_OK;
			}
			case GUI_MSG_KEY_RECORD:
			{
				if(root_ctrl->h_app_record == NULL && KEY_UP_ACTION == msg->dwAddData2)
				{
				__here__;
					key2app(msg, KEY_TO_RECORD);
				}
				return EPDK_OK;
			}
			case GUI_MSG_KEY_PHOTO:
			{
				if(root_ctrl->h_app_photo == NULL && KEY_UP_ACTION == msg->dwAddData2)
				{
	#if 1			
					key2app(msg, KEY_TO_PHOTO);
	#else
					if(!key_msg_change_mask)
					{
						__gui_msg_t mymsg;
						eLIBs_printf("-- %s, %d, Send msg to suspend home -- \n", __FILE__, __LINE__);
						mymsg.h_deswin = root_ctrl->h_app_home;
						mymsg.id = msg->id;
						mymsg.dwAddData1 =msg->dwAddData1;
						mymsg.dwAddData2 = msg->dwAddData2;
						GUI_SendNotifyMessage(&mymsg);
						key_msg_change_mask = 1;
					}
					else
					{
						key2app(msg, KEY_TO_PHOTO);
						key_msg_change_mask = 0;
					}
					
	#endif
				}
				return EPDK_OK;
			}
			case GUI_MSG_KEY_CALENDAR:
			{
				if(root_ctrl->h_app_calendar == NULL && KEY_UP_ACTION ==msg->dwAddData2)
				{
					key2app(msg,KEY_TO_CALENDAR);
				}
				return EPDK_OK;
			}
			case GUI_MSG_KEY_SETTING:
			{
				if(root_ctrl->h_app_setting == NULL && KEY_UP_ACTION ==msg->dwAddData2)
				{
					key2app(msg,KEY_TO_SETTING);
				}
				return EPDK_OK;
			}
			//case GUI_MSG_KEY_LONGFM:	//wqx 20130415
			case GUI_MSG_KEY_LINEIN_PLUGIN:
			{
				__here__;
				if(root_ctrl->h_app_linin== NULL && KEY_UP_ACTION ==msg->dwAddData2)
				{
					key2app(msg,KEY_TO_LINEIN);
				}
				__here__;
				return EPDK_OK;
			}
			case GUI_MSG_KEY_MODE:
			{
				__here__;
				if( KEY_UP_ACTION ==msg->dwAddData2)
				{
					if(root_ctrl->h_app_music)
					{
						if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_VIDEO)==EPDK_OK )
							key2app(msg,KEY_TO_MOVIE);
						else if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_PIC) ==EPDK_OK)
							key2app(msg,KEY_TO_PHOTO);
						//else if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_EBOOK) ==EPDK_OK)
						//	key2app(msg,KEY_TO_EBOOK);
					}
					else if(root_ctrl->h_app_movie)
					{
						if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_PIC) ==EPDK_OK)
							key2app(msg,KEY_TO_PHOTO);
						//else if(root_check_mediafile_exist(temp_root, RAT_MEDIA_TYPE_EBOOK) ==EPDK_OK)
						//	key2app(msg,KEY_TO_EBOOK);
						else if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_AUDIO) ==EPDK_OK)
							key2app(msg,KEY_TO_MUSIC);
					}
					else if(root_ctrl->h_app_photo)
					{
							key2app(msg,KEY_TO_FM);
					}					
					else if(root_ctrl->h_app_fm)
					{
						//if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_EBOOK) ==EPDK_OK)
						//	key2app(msg,KEY_TO_EBOOK);
						//else
						if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_AUDIO) ==EPDK_OK)
							key2app(msg,KEY_TO_MUSIC);
						else if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_VIDEO) ==EPDK_OK)
							key2app(msg,KEY_TO_MOVIE);
					}
					else //if(root_ctrl->h_app_ebook)
					{
						if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_AUDIO)==EPDK_OK )
							key2app(msg,KEY_TO_MUSIC);
						else if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_VIDEO)==EPDK_OK )
							key2app(msg,KEY_TO_MOVIE);
						//else if(__app_root_set_playfile(temp_root, RAT_MEDIA_TYPE_EBOOK) ==EPDK_OK)
						//	key2app(msg,KEY_TO_EBOOK);
					}
				}
				__here__;
				return EPDK_OK;
			}
			case GUI_MSG_KEY_IR_KOK:
			case GUI_MSG_KEY_IR_LONGKOK:
			{    
				if(KEY_UP_ACTION ==msg->dwAddData2)
				{
					__s32 channel;

					channel = robin_get_channel();
					channel = (channel+1)%3;
					robin_set_channel(channel);
					app_root_show_dlg(root_ctrl->root_para->h_parent, channel_mode_str[channel], 0);
					
				}
			    break;
			} 
		}


	return EPDK_FAIL;
	
}
#endif



void app_root_fs_device_plugin(__gui_msg_t *msg)   //cai++ for kaiji_music
{
	__u32 root_type = 0, temp_root=0, temp2,flag=0;
	__u8 sd_root=0, usb_root = 0, device_cnt = 0;//device_cnt, 当前设备的个数	
	root_ctrl_t   *root_ctrl;
	root_para_t* root_para = NULL;
	//char str[256]={0};
	char str[32] = 0;

#if BEETLES_C100_AUTO_UPDATE
	char update_path[32] = {0};	  
	int ret = EPDK_FAIL;	
	ES_FILE* fp = NULL;
#endif	

	   
	root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;
    
	root_type = root_check_disk();	
	sd_root = (root_type >> 8)&0x00ff;
	usb_root = (root_type)&0x00ff; 

	__msg(" --  root_type = 0x%x -- \n", root_type);
			
	if(sd_root)
	{
		if(usb_root)	                    //两个都在,比较得知新插入的设备
		{
			if(root_ctrl->root_para->root_type == sd_root)//
			{			
				temp_root = RAT_USB;        //新插入的设备为USB
				root_ctrl->root_para->late_root_type = 0;  //cai++ for main_menu_change
				
			}
			else
			{
				temp_root = RAT_TF;			//新插入的设备为SD	
				root_ctrl->root_para->late_root_type = 1;  //cai++ for main_menu_change
			}
            device_cnt = 2;
		}	
		else
		{
			temp_root = RAT_TF;
			root_ctrl->root_para->late_root_type = 1;  //cai__ for main_menu_change
            device_cnt = 1;
		}				
	}			
	else if(usb_root)	                    //插入的是USB设备,当前只有USB设备		
	{
		temp_root = RAT_USB;
		root_ctrl->root_para->late_root_type = 0;  //cai++ for main_menu_change
        device_cnt = 1;
	}		
    else
    {
    		root_ctrl->root_para->late_root_type = 3;  //UNKOWN_DISK_TYPE  //cai++ for main_menu_change
        return;
    }

	if(temp_root==2)
	    	dsk_langres_get_menu_text(STRING_HOME_HBAR_SD, str, sizeof(str)); 
		
	else if(temp_root==3)
		dsk_langres_get_menu_text(STRING_HOME_HBAR_USB, str, sizeof(str)); 

	gscene_hbar_set_title(str,sizeof(str));

	eLIBs_strcpy(last_str, str);


	root_ctrl->root_para->root_type = temp_root;
	__msg(" --  temp_root = %d -- \n", temp_root);
	
#if BEETLES_C100_AUTO_UPDATE
		if(temp_root >= RAT_TF)
		{
			eLIBs_remove(REG_ROOT_PARA_FILE_PATH);
			ret = __app_root_get_search_path(temp_root, update_path);
			__msg("huang####  ret = %d  update path = %s \n",ret, update_path);
			if(EPDK_OK == ret)	
			{
				eLIBs_strcat(update_path, "\\AC00280.img");

				fp = eLIBs_fopen(update_path, "r");
				if(fp)
				{
					eLIBs_fclose(fp);
					fp = NULL;

					esKRNL_TimeDly(50);
					__msg("huang####   updating  path = %s \n",update_path);
					dsk_start_fw_update(update_path);
					__msg("huang####   updating  end \n");
					return;
				}
			}
		}
#endif

	#if 0
	root_type = app_root_get_last_root_type();	

	if(root_type == APP_ROOT_TYPE_MUSIC)
	{
		//root_ctrl->root_para->root_type = temp_root; 
		//root_ctrl->root_para->root_type_cnt = device_cnt;

		if(root_check_mediafile_exist(temp_root, RAT_MEDIA_TYPE_AUDIO))  //检查对应盘是否存在对应的音频文件
		{
			app_root_show_dlg(root_ctrl->root_para->h_parent, STRING_HOME_NO_FILE, 0);
			return; //可以在此处创建无对应音频文件对话框
		}
		else
		{
			app_root_set_last_root_type(APP_ROOT_TYPE_MUSIC);

		//delete_all_app(root_ctrl);
		delete_all_app_suspend_home(root_ctrl);

		flag = 2;
		gscene_hbar_set_state(HBAR_ST_HIDE);
		root_ctrl->root_para->root_type = temp_root; 
		root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;  
		root_ctrl->root_para->data = MUSICPLAYER_NORMAL;
		root_ctrl->root_para->h_parent =  msg->h_deswin;               
		root_ctrl->h_app_music = app_music_create(root_ctrl->root_para);					

			GUI_WinSetFocusChild(root_ctrl->h_app_music);
		}

		return;
	}
	else
	{
			if(__app_root_set_playfile(temp_root,RAT_MEDIA_TYPE_VIDEO))
			{
				if(root_check_mediafile_exist(temp_root, RAT_MEDIA_TYPE_AUDIO))  //检查对应盘是否存在对应的音频文件
				{
					app_root_show_dlg(root_ctrl->root_para->h_parent, STRING_HOME_NO_FILE, 0);
					return; //可以在此处创建无对应音频文件对话框
				}
				else
				{
					flag = 2;
					//root_ctrl->root_para->root_type = temp_root; 
					//root_ctrl->root_para->root_type_cnt = device_cnt;
#if 0					
					app_root_set_last_root_type(APP_ROOT_TYPE_MUSIC);
				
					//delete_all_app(root_ctrl);
					delete_all_app_suspend_home(root_ctrl);

					gscene_hbar_set_state(HBAR_ST_HIDE);
					root_ctrl->root_para->root_type = temp_root; 
					root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;  
					root_ctrl->root_para->data = MUSICPLAYER_NORMAL;
					root_ctrl->root_para->h_parent =  msg->h_deswin;               
					root_ctrl->h_app_music = app_music_create(root_ctrl->root_para);					

					GUI_WinSetFocusChild(root_ctrl->h_app_music);
#else

					if(root_ctrl->h_app_home)
					{
						__gui_msg_t mymsg;
						eLIBs_printf("-- %s, %d, Send key msg to home -- \n", __FILE__, __LINE__);
						mymsg.h_deswin = root_ctrl->h_app_home;
						mymsg.id = GUI_MSG_KEY;
						mymsg.dwAddData1 =GUI_MSG_KEY_MUSIC;
						mymsg.dwAddData2 = KEY_UP_ACTION;
						GUI_SendNotifyMessage(&mymsg);
					}	
					else
					{
						eLIBs_printf("-- %s, %d, Home is not exist -- \n", __FILE__, __LINE__);
					}
#endif
					return;

				}
                        	
			}
			//else
			{
				__here__;
				flag = 1;
				//delete_all_app(root_ctrl);
				delete_all_app_suspend_home(root_ctrl);
			}
			
			app_root_set_last_root_type(APP_ROOT_TYPE_MOVIE);

#if 0			              
			//gscene_hbar_set_state(HBAR_ST_HIDE);
			root_ctrl->root_para->root_type = temp_root; 
			root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;  
			root_ctrl->root_para->data = KEY_SW_TO_MOVIE;
			root_ctrl->root_para->h_parent =  msg->h_deswin;               
			root_ctrl->h_app_movie = app_movie_create(root_ctrl->root_para);					

			GUI_WinSetFocusChild(root_ctrl->h_app_movie);
#else
			//root_ctrl->root_para->root_type = temp_root; 
			//root_ctrl->root_para->root_type_cnt = device_cnt;
			if(root_ctrl->h_app_home)
			{
				__gui_msg_t mymsg;
				eLIBs_printf("-- %s, %d, Send key msg to home -- \n", __FILE__, __LINE__);
				mymsg.h_deswin = root_ctrl->h_app_home;
				mymsg.id = GUI_MSG_KEY;
				mymsg.dwAddData1 =GUI_MSG_KEY_MOVIE;
				mymsg.dwAddData2 = KEY_UP_ACTION;
				GUI_SendNotifyMessage(&mymsg);
			}	
			else
			{
				eLIBs_printf("-- %s, %d, Home is not exist -- \n", __FILE__, __LINE__);
			}
#endif
	}
	
#endif
    return;
}                


void app_root_fs_device_plugout(__gui_msg_t * msg)   //cai++ for kaiji_music
{
	__u32 root_type = 0, temp_root=0, temp2;
	__u8 sd_root=0, usb_root = 0;	
	root_ctrl_t   *root_ctrl;
	     //char str[256]={0};
	     char str[32] = {0};
	root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	
	root_type = root_check_disk();	
	sd_root = (root_type >> 8)&0x00ff;
	usb_root = (root_type)&0x00ff; 
	
	if (sd_root == 2)
		root_type=2;//仅有sd时root_type=512
	
	if(root_type == 0)			//没有设备	
	{				
		gscene_hbar_set_title(str,sizeof(str));  //cai++ for 无卡不显示
		eLIBs_strcpy(last_str, str);
		
		if(root_ctrl->h_app_setting != NULL || root_ctrl->h_app_calendar != NULL || root_ctrl->h_app_fm != NULL|| root_ctrl->h_app_movie!= NULL)
			return;
		else
			__app_home_restore(msg);
		return;
	}	
	if(root_ctrl->h_app_setting != NULL || root_ctrl->h_app_calendar != NULL || root_ctrl->h_app_fm != NULL|| root_ctrl->h_app_linin!= NULL)
		return;
	
	if(root_ctrl->root_para->root_type != root_type)			//只播放当前存在的设备
	{                                               //当前播放设备被拔出了,root_type 为剩下的设备
		__msg("usb or sd plugout....root_type = %d\n", root_type);
		if(sd_root)
		{
			if(root_ctrl->root_para->root_type == RAT_TF)
			{
				return;
			}
			temp_root = RAT_TF;
		}
		else if(usb_root)							//拔出的是SD卡
		{
			temp_root = RAT_USB;
		}
		else
		{
			root_ctrl->root_para->root_type = 0;
			return;
		}

		#if 1
		if(temp_root==2)
		{
				dsk_langres_get_menu_text(STRING_HOME_HBAR_SD, str, sizeof(str)); 
				root_ctrl->root_para->root_type = temp_root;
		}
		else
		{
			dsk_langres_get_menu_text(STRING_HOME_HBAR_USB, str, sizeof(str)); 
			root_ctrl->root_para->root_type = temp_root;
		}
		
				gscene_hbar_set_title(str,sizeof(str));
		#endif
		if(root_ctrl->h_app_record!= NULL)
			return;

#if 0		
		//只剩下一个盘符		
		if(root_check_mediafile_exist(temp_root, RAT_MEDIA_TYPE_VIDEO))
		{
			if(root_check_mediafile_exist(temp_root, RAT_MEDIA_TYPE_AUDIO))  //检查对应盘是否存在对应的音频文件
			{
				return; //可以在此处创建无对应音频文件对话框
			}
			else
			{
				if(root_ctrl->h_app_movie == NULL)
				{
					delete_all_app(root_ctrl);

					gscene_hbar_set_state(HBAR_ST_HIDE);  //cai++
					root_ctrl->root_para->root_type = temp_root; 
					root_ctrl->root_para->explr_root = root_ctrl->root_para->root_type;   
					root_ctrl->root_para->data = MUSICPLAYER_NORMAL;
					root_ctrl->root_para->h_parent =  msg->h_deswin;               
					root_ctrl->h_app_music = app_music_create(root_ctrl->root_para);					

					GUI_WinSetFocusChild(root_ctrl->h_app_music);
				}
				else
				{
					root_ctrl->root_para->root_type = temp_root; 
					root_ctrl->root_para->explr_root = temp_root;   

					key2app(msg,KEY_TO_MUSIC);
				}
				return;
			}
		}
		
		if(root_ctrl->h_app_movie== NULL)
		{
			delete_all_app(root_ctrl);

			//gscene_hbar_set_state(HBAR_ST_HIDE);
			root_ctrl->root_para->root_type = temp_root; 
			root_ctrl->root_para->explr_root = temp_root;//root_ctrl->root_para->root_type;  
			root_ctrl->root_para->data = KEY_SW_TO_MOVIE;
			root_ctrl->root_para->h_parent =  msg->h_deswin;               
			root_ctrl->h_app_movie = app_movie_create(root_ctrl->root_para);					

			GUI_WinSetFocusChild(root_ctrl->h_app_movie);
		}
		else
		{
			root_ctrl->root_para->root_type = temp_root; 
			root_ctrl->root_para->explr_root = temp_root;   
			
			key2app(msg,KEY_TO_MOVIE);
		}
#endif		
		return;
	}				
	else
  	{
		root_ctrl->root_para->explr_root = root_type;   
  	}
	return;
}


void root_check_fs_device(__gui_msg_t *msg)   //cai++ for kaiji_music
{

	
	switch(msg->id)
	{
		case DSK_MSG_FS_PART_PLUGIN:
		    app_root_fs_device_plugin(msg);
			break;				
		case DSK_MSG_FS_PART_PLUGOUT:
            app_root_fs_device_plugout(msg);		
			break;			
	}
	return;
}
#if 0
static __s32 __app_root_is_bg_music_exist(__gui_msg_t *msg, H_WIN* hMusic)
{
    H_WIN root, child;
	char  winname[256];  
    __bool exist;
   
    root  = GUI_WinGetHandFromName(APP_ROOT);	
	child = GUI_WinGetFirstChild(root);

    exist = 0;
    *hMusic = 0;
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 256);
		GUI_WinGetName(child, winname);
				
		__msg("winname = %s \n", winname);
		
		if( 0 == eLIBs_strcmp( APP_MUSIC, winname))		
		{
            *hMusic = child;
			exist = 1;
            break;
		}
		
		child = GUI_WinGetNextBro(child);
	}

    return exist;
}
#endif

#if 0
static void __app_root_delete_bg_music(__gui_msg_t *msg)
{
    H_WIN root, child, next_child;
	char  winname[256]; 
    root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

    root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;
		
	root  = GUI_WinGetHandFromName(APP_ROOT);	
    if(!root)
    {
        __err("GUI_WinGetHandFromName fail...\n");
        return ;
    }
	child = GUI_WinGetFirstChild(root);
	
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 256);
		GUI_WinGetName(child, winname);

        next_child = GUI_WinGetNextBro(child);
				
		__msg("winname = %s \n", winname);
		
		if( 0 == eLIBs_strcmp( APP_MUSIC, winname))		
		{
			GUI_ManWinDelete(child);
            root_ctrl->h_app_music = NULL;
            break;
		}
		
		child = next_child;
	} 	
}
#endif

#if 0
static void __app_root_create_bg_music(__gui_msg_t *msg)
{
    H_WIN root, child;
	char  winname[256];  
    __bool exist;
    root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

    root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

    root  = GUI_WinGetHandFromName(APP_ROOT);	
	child = GUI_WinGetFirstChild(root);

    exist = 0;
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 256);
		GUI_WinGetName(child, winname);
				
		__msg("winname = %s \n", winname);
		
		if( 0 == eLIBs_strcmp( APP_MUSIC, winname))		
		{
			exist = 1;
            break;
		}
		
		child = GUI_WinGetNextBro(child);
	}

    if(0 == exist)
    {        				
		root_para->data = MUSICPLAYER_BACKGROUND;
		root_ctrl->root_para->h_parent= GUI_WinGetHandFromName(APP_ROOT);//GUI_WinGetParent(root_ctrl->h_app_home); // cai modify for photo_bg_music_error                                              
		root_ctrl->h_app_music =  app_music_create(root_para);		
    }
}


//进入应用之前把背景设置为默认背景
static void __app_root_change_to_default_bg(void)
{    
    reg_init_para_t* para;
    para = dsk_reg_get_para_by_app(REG_APP_INIT);
    if(para)
    {
        para->bgd_flag_in_main = gscene_bgd_get_flag();
        if(EPDK_TRUE == para->bgd_flag_in_main)//当前是默认背景，则不做任何事情
        {                                       
        }
        else//当前不是默认背景，修改为默认背景
        {
            gscene_bgd_set_flag(EPDK_TRUE);
            gscene_bgd_refresh();
        }                                    
    }
    else
    {
        __msg("dsk_reg_get_para_by_app fail...\n");
    }
}
#endif

#if 0
static __s32 app_root_command_proc(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;
	__gui_msg_t mymsg;
	__s32       null_dis=0;


	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

    __msg("*****************app_root_command_proc**************\n");

	switch (LOWORD(msg->dwAddData1))
	{
		case APP_LINEIN_ID:		//cai++
		{
			switch(HIWORD(msg->dwAddData1))
			{
		                case SWITCH_TO_MMENU:
		                {
		                    __s32 ret;
		                    __s32 dwReserved;

		                    dwReserved = msg->dwReserved;//0代表全部删除，1代表进入linin后台模式
		                    
		                    __wrn("root_ctrl->h_app_linin=%x\n", root_ctrl->h_app_linin);
		                    if(0 == dwReserved)
		                    {                        
		                        if(root_ctrl->h_app_linin)
		                        {
		                            __wrn("before GUI_ManWinDelete(root_ctrl->h_app_linin)\n");
		                            GUI_ManWinDelete(root_ctrl->h_app_linin);
		                            __wrn("after GUI_ManWinDelete(root_ctrl->h_app_linin)\n");
		                    		root_ctrl->h_app_linin = 0;
		                        }         
		                    }
		                    else
		                    {
		                        if(root_ctrl->h_app_linin)
		                        {
		                            app_linein_set_mode(root_ctrl->h_app_linin, LINEIN_MODE_BACKGROUND);
		                        }
		                    }
		                    
		//                    ret = __app_root_has_forground_apps(root_ctrl);
		                    __wrn("ret=%d\n", ret);
		                    
		//                    if(EPDK_FALSE == ret)
		 //                   {        			    
		                	    __app_home_restore(msg);
		//                    }            

		                    break;
		                }                

		                case SWITCH_TO_LINE_IN:
		                {                    
		//                    __app_root_to_linein(root_ctrl);			//暂时不要
		                  
		                    break;
		                }                
		                default:
		                {
		                    break;
		                }
			}
			break;
		}
		case APP_HOME_ID:
		{
			__here__;
			switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_OTHER_APP:
				{
					switch(msg->dwAddData2)
					{						
						case ID_HOME_FM:
						{                            
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                            //__app_root_change_to_default_bg();
                            
                            //删除后台音乐
                            __app_root_delete_bg_music(msg);
                            
							__msg("**********enter fm**********\n");							
							root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
							root_ctrl->h_app_fm=  app_fm_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_fm);
							//g_enable_close_scn();
							break;                          
						}
						break;
						case ID_HOME_MOVIE:
						{   
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                           // __app_root_change_to_default_bg();
                            
							__msg("**********enter movie explorer**********\n");							                           

							__msg("root type=%d\n", msg->dwReserved);
                            root_para->root_type = msg->dwReserved;//RAT_TF;
                            root_para->explr_root = msg->dwReserved;
                            root_para->data = ID_EXPLORER_MOVIE;
                            root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                            root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                            __msg("root_ctrl->h_app_explorer = %x\n", root_ctrl->h_app_explorer);
                            GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
							//g_enable_close_scn();
							break;							
						}	
						break;
						case ID_HOME_PHOTO:
						{               
                            //gscene_hbar_set_state(HBAR_ST_HIDE);
                            //__app_root_change_to_default_bg();
                            
							__msg("**********enter photo explorer**********\n");														
							
							__msg("root type=%d\n", msg->dwReserved);
                            root_para->root_type = msg->dwReserved;
                            root_para->explr_root = msg->dwReserved;
                            root_para->data = ID_EXPLORER_PHOTO;
                            root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                            root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                            __msg("root_ctrl->h_app_explorer = %x\n", root_ctrl->h_app_explorer);
                            GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
							//g_enable_close_scn();
							break;
						}
						break;
						case ID_HOME_MUSIC:
						{       
                            __s32 ret;
                            __s32 exist;
                            H_WIN hmusic;
                            
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                           // __app_root_change_to_default_bg();

                            root_para->explorer_play_music = 0;
                            root_para->root_type = msg->dwReserved;                     
                            root_para->data = MUSICPLAYER_NORMAL;
                            root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);                                                      

                            exist = __app_root_is_bg_music_exist(msg, &hmusic);
                            if(1 == exist)//重建后台音乐
                            {
                                //ret = app_music_restore(hmusic, root_para->root_type); 
                                ret = app_music_restore(hmusic, root_para); 
                                if(EPDK_OK == ret)
                                {
                                    if(root_ctrl->h_app_music != hmusic)
                                    {
                                        __msg("*******root_ctrl->h_app_music != hmusic********\n");
                                    }
                                    root_ctrl->h_app_music = hmusic;
                                }
                                else
                                {
                                    __msg("*****app_music_restore fail*******\n");
                                    __app_root_delete_bg_music(msg);
                                    __here__;
                                    root_ctrl->h_app_music =  app_music_create(root_para);
                                    __here__;
                                }
                            }
                            else
                            {
                                root_ctrl->h_app_music =  app_music_create(root_para);							    
                            }
                            GUI_WinSetFocusChild(root_ctrl->h_app_music);                                                       
						 //g_enable_close_scn();                           
                           // gscene_bgd_set_state(BGD_STATUS_HIDE);
							
						}
						break;
						case ID_HOME_EBOOK:
						{                 
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                            //__app_root_change_to_default_bg();
                            
							root_para->root_type = msg->dwReserved;
							root_para->explr_root = msg->dwReserved;
							root_para->data = ID_EXPLORER_EBOOK;
							root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
							root_ctrl->h_app_explorer=  app_explorer_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
							//g_enable_close_scn();
						}
						break;
                        case ID_HOME_RECORD:
                        {
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                          //  __app_root_change_to_default_bg();

                            //删除后台音乐
                            __app_root_delete_bg_music(msg);

							dsk_set_record_is_recording(EPDK_TRUE);
							if(EPDK_TRUE == dsk_amplifier_is_on())
								dsk_amplifier_onoff(0);
							esKRNL_TimeDly(10);
				
                            __msg("**********ID_HOME_RECORD**********\n");	
                            root_ctrl->root_para->root_type = msg->dwReserved;//RAT_USB或者RAT_TF
                            root_ctrl->root_para->record_opt = 0;//正常录音
							root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
							root_ctrl->h_app_record=  app_record_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_record);
							//g_enable_close_scn();
							break;
                        }
                        case ID_HOME_LINEIN:
                        {
                            __bool is_linein_insert;

                            //删除后台音乐
                            __app_root_delete_bg_music(msg);

                            if(root_ctrl->h_app_linin)
                            {
                            	return;
                            }
                            else
                            {
                                root_para->data = LINEIN_MODE_NORMAL;//linein_mode_e
                                root_ctrl->h_app_linin = app_linein_create(root_para);
    	                        GUI_WinSetFocusChild(root_ctrl->h_app_linin);
                            }
							//g_enable_close_scn();
                            break;
                        }
			
						case ID_HOME_OTHERS:
						{                                                                          
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                            //删除后台音乐
                            //__app_root_delete_bg_music(msg);
                            
							__msg("**********enter other apps**********\n");
							if (0 == msg->dwReserved)
							{
                               //__app_root_change_to_default_bg();
                                
								__msg("**********enter setting**********\n");	
                                root_ctrl->root_para->setting_opt = 0;//通用设置
								root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
								root_ctrl->h_app_setting=  app_setting_create(root_para);
								GUI_WinSetFocusChild(root_ctrl->h_app_setting);
								//g_enable_close_scn();
								break;
							}
							else if (1 == msg->dwReserved)
							{
                               // __app_root_change_to_default_bg();
                                
								__msg("**********enter setting**********\n");							
                                root_ctrl->root_para->setting_opt = 1;//电源管理
								root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
								root_ctrl->h_app_setting=  app_setting_create(root_para);
								GUI_WinSetFocusChild(root_ctrl->h_app_setting);
								//g_enable_close_scn();
								break;
							}							
							else if (2 == msg->dwReserved)
							{                                                               								
#if BEETLES_RTC_EXIST
                                __msg("**********enter calendar**********\n");  

                                //__app_root_change_to_default_bg();
								root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
								root_ctrl->h_app_calendar=  app_calendar_create(root_para);
								GUI_WinSetFocusChild(root_ctrl->h_app_calendar);                                

#else

                                __msg("**********enter explorer manage**********\n");							
								gscene_hbar_set_state(HBAR_ST_HIDE);
                                //__app_root_change_to_default_bg();
                                
    							__msg("**********enter movie explorer**********\n");							                           

    							__msg("root type=%d\n", msg->dwReserved);
                                root_para->root_type = RAT_TF;//msg->dwReserved;//RAT_TF;
                                root_para->explr_root = RAT_TF;//msg->dwReserved;
                                root_para->data = ID_EXPLORER_ALL;
                                root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                                root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                                __msg("root_ctrl->h_app_explorer = %x\n", root_ctrl->h_app_explorer);
                                GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
								//g_enable_close_scn();
#endif
								break;
							}
                            else if (3 == msg->dwReserved)
							{      
#if BEETLES_RTC_EXIST                                
								__msg("**********enter explorer manage**********\n");							
								gscene_hbar_set_state(HBAR_ST_HIDE);
                                //__app_root_change_to_default_bg();
                                
    							__msg("**********enter movie explorer**********\n");							                           

    							__msg("root type=%d\n", msg->dwReserved);
                                root_para->root_type = RAT_TF;//msg->dwReserved;//RAT_TF;
                                root_para->explr_root = RAT_TF;//msg->dwReserved;
                                root_para->data = ID_EXPLORER_ALL;
                                root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                                root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                                __msg("root_ctrl->h_app_explorer = %x\n", root_ctrl->h_app_explorer);
                                GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
              					//g_enable_close_scn();                  
#else

                                __err("sub menu focus item invalid...\n");

#endif
								break;
							}
							else
							{
								__err("sub menu focus item invalid...\n");
								break;
							}
						}
						break;
						default:
							break;
					}
				}
					break;
				default:
					break;
			}
		}
		break;
		case APP_EXPLORER_ID:
		{
			switch(HIWORD(msg->dwAddData1))
			{
			case SWITCH_TO_OTHER_APP:
				{
					switch( msg->dwAddData2 )
					{
					case EXPLR_SW_TO_MAIN:
						{                            
                            if(root_ctrl->h_app_explorer)
                            {
                                GUI_ManWinDelete(root_ctrl->h_app_explorer);
			                    root_ctrl->h_app_explorer = 0;
                            }
                            __here__;

                            __app_home_restore(msg);
						}
						break;
					case EXPLR_SW_TO_MOVIE:
						{                            
							app_root_set_last_root_type(APP_ROOT_TYPE_MOVIE);

							//删除后台音乐
                            __app_root_delete_bg_music(msg);
                            
							__msg("**********explorer to movie**********\n");
							
							//root_para->root_type = msg->dwReserved;
							root_para->data = 0;
							root_para->root_type = root_para->explr_root  ;
							root_ctrl->h_app_movie = app_movie_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_movie);							
						}
						break;
					case EXPLR_SW_TO_PHOTO:
						{
							app_root_set_last_root_type(APP_ROOT_TYPE_PHOTO);

                            //删除后台音乐
                            //__app_root_delete_bg_music(msg);
                            
							__msg("**********explorer to photo**********\n");
							
							//root_para->root_type = msg->dwReserved;
							root_para->data = 0;
							root_para->root_type = root_para->explr_root  ;
							root_ctrl->h_app_photo = app_photo_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_photo);
							
						}
						break;
					case EXPLR_SW_TO_MUSIC:
						{       
	                            __s32 ret;
	                            __s32 exist;
	                            H_WIN hmusic;
	                            
	                            gscene_hbar_set_state(HBAR_ST_HIDE);
	                           // __app_root_change_to_default_bg();
					//eLIBs_strncpy( root_para->music_file_name , (const char *) msg->dwReserved, sizeof(root_para->music_file_name) ) ;
					//__msg( "root_para->music_file_name =%s\n",root_para->music_file_name );

					root_para->explorer_play_music = 1;
	                            root_para->root_type = root_para->explr_root  ;     

					//root_para->root_type = root_para->explr_root;
					__msg("app_root_scene, root_para->root_type = %d\n", root_para->root_type);								
	                            root_para->data = MUSICPLAYER_NORMAL;
	                            root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);                                                      

	                            exist = __app_root_is_bg_music_exist(msg, &hmusic);
	                            if(1 == exist)//重建后台音乐
	                            {
	                                //ret = app_music_restore(hmusic, root_para->root_type); 
	                                ret = app_music_restore(hmusic, root_para); 
	                                if(EPDK_OK == ret)
	                                {
	                                    if(root_ctrl->h_app_music != hmusic)
	                                    {
	                                        __msg("*******root_ctrl->h_app_music != hmusic********\n");
	                                    }
	                                    root_ctrl->h_app_music = hmusic;
	                                }
	                                else
	                                {
	                                    __msg("*****app_music_restore fail*******\n");
	                                    __app_root_delete_bg_music(msg);
	                                    __here__;
	                                    root_ctrl->h_app_music =  app_music_create(root_para);
	                                    __here__;
	                                }
	                            }
	                            else
	                            {
	                                root_ctrl->h_app_music =  app_music_create(root_para);							    
	                            }
	                            GUI_WinSetFocusChild(root_ctrl->h_app_music);                                                       
	                            
	                           // gscene_bgd_set_state(BGD_STATUS_HIDE);
								
							}
							break ;
					case EXPLR_SW_TO_EBOOK:
						{                            
                            //删除后台音乐
                            //__app_root_delete_bg_music(msg);
							__msg("**********explorer to ebook**********\n");
							
							//root_para->root_type = msg->dwReserved;
							root_para->data = 0;
							root_para->root_type = root_para->explr_root  ;   

							root_ctrl->h_app_ebook= app_ebook_create(root_para);
							__here__;
							GUI_WinSetFocusChild(root_ctrl->h_app_ebook);
							__here__;
							
						}						
						break;
					default:
                        {
                            __err("media type err...\n");
					    }
						break;
					}
					root_ctrl->h_app_explorer = NULL;//浏览器已自己删除
				}
				break;
			default:
				break;
			}
		}
		break;
		case APP_MOVIE_ID:
		{
            __msg("**********movie to explorer**********\n");
            __msg("msg->dwAddData1=%d\n", msg->dwAddData1);
            __msg("msg->dwAddData2=%d\n", msg->dwAddData2);
            __msg("msg->dwReserved=%d\n", msg->dwReserved);
			switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_EXPLORER:
				{
                    GUI_ManWinDelete(root_ctrl->h_app_movie);
        			root_ctrl->h_app_movie = 0;
                    __here__;
                    root_para->root_type = root_para->explr_root;
                    root_para->data = ID_EXPLORER_MOVIE;
		if(NULL!=root_ctrl->h_app_home)
                   root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                    root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                    GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
                    break;
				}
                case SWITCH_TO_MMENU:
				{
				__msg("经过这里\n");
                    GUI_ManWinDelete(root_ctrl->h_app_movie);
        			root_ctrl->h_app_movie = 0;
                    __here__;
                    __app_home_restore(msg);
                    
                    break;
                }
                default:
                {
                    break;
                }
			}
			            			            	                    
            break;
            
		}		
		case APP_MUSIC_ID:
		{
			__msg("**********receive msg from music,msg->dwAddData2=%d**********\n", msg->dwAddData2);

            switch (msg->dwAddData2)
			{
				case SWITCH_TO_MMENU:
				{
                    if(MUSICPLAYER_BACKGROUND == msg->dwReserved)//音乐后台，不删除音乐
                    {
                        __msg("music background...\n");
                    }
                    else
                    {
                        __msg("delete music...\n");
                        GUI_ManWinDelete(root_ctrl->h_app_music);
			            root_ctrl->h_app_music = 0;                                  
                    }   
                    
                    __app_home_restore(msg);
                    
                    break;
				}
                case CLOSE_BG_MUSIC:
                {
                    __msg("delete background music...\n");
                    if(root_ctrl->h_app_music)
                    {
                        GUI_ManWinDelete(root_ctrl->h_app_music);
    		            root_ctrl->h_app_music = 0;  
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }            									
		}
		break;
		case APP_PHOTO_ID:
		{
            switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_EXPLORER:
				{
                    //删除后台音乐
                     __app_root_delete_bg_music(msg);
                    
                    __msg("**********photo to explorer**********\n");
			
        			GUI_ManWinDelete(root_ctrl->h_app_photo);
        			root_ctrl->h_app_photo = 0;
                    
                    gscene_bgd_set_flag(EPDK_TRUE);
                    //gscene_bgd_refresh();
                    
        			__here__;
                    root_para->root_type = root_para->explr_root;
                    root_para->data = ID_EXPLORER_PHOTO;
                    root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                    root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                    GUI_WinSetFocusChild(root_ctrl->h_app_explorer);  
                    break;
				}
                case OPEN_BG_MUSIC:
                {
                    __app_root_create_bg_music(msg);
                    break;
                }
                case CLOSE_BG_MUSIC:
                {
                    __app_root_delete_bg_music(msg);
                    break;
                }
                case SWITCH_TO_MMENU:
				{
				H_WIN hmusic;
					
                    GUI_ManWinDelete(root_ctrl->h_app_photo);
        			root_ctrl->h_app_photo= 0;
                    __here__;
                    __app_home_restore(msg);
		__msg("开始指定焦点窗口\n");			
                   // if(__app_root_is_bg_music_exist(msg,&hmusic))
                    	//{
                    		__msg("指定主界面窗口为焦点窗口\n");
				GUI_WinSetFocusChild(root_ctrl->h_app_home);  
                    	//}
                    break;
                }
                default:
                {
                    break;
                }
            }		
						
			break;
		}
		case APP_EBOOK_ID:
		{
            switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_EXPLORER:
				{
                    //删除后台音乐
                    __app_root_delete_bg_music(msg);
                    
                    __msg("**********ebook to explorer**********\n");
			
        			GUI_ManWinDelete(root_ctrl->h_app_ebook);
        			root_ctrl->h_app_ebook = 0;

                    //gscene_bgd_set_flag(EPDK_TRUE);
                    //gscene_bgd_refresh();
        			
        			root_para->root_type = root_para->explr_root;
        			root_para->data = ID_EXPLORER_EBOOK;
        			root_ctrl->h_app_explorer =  app_explorer_create(root_para);
        			GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
                    break;
				}
                case OPEN_BG_MUSIC:
                {
                    __app_root_create_bg_music(msg);
                    break;
                }
                case CLOSE_BG_MUSIC:
                {
                    __app_root_delete_bg_music(msg);
                    break;
                }
                case SWITCH_TO_MMENU:
				{
                    GUI_ManWinDelete(root_ctrl->h_app_ebook);
        			root_ctrl->h_app_ebook = 0;
                    __here__;
                    __app_home_restore(msg);
                    
                    break;
                }
                default:
                {
                    break;
                }
            }							
			
			break;
		}
				
		case APP_FM_ID:
		{
            switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_MMENU:
				{
                    __msg("**********fm to home**********\n");
					
        			GUI_ManWinDelete(root_ctrl->h_app_fm);
        			root_ctrl->h_app_fm = 0;
                    
        			__app_home_restore(msg);

                    {//重新初始化按键音
                        __set_keytone_t state;

                        dsk_keytone_get_state(&state);
                        dsk_keytone_exit();
                        dsk_keytone_init(BEETLES_APP_ROOT"res\\sounds\\chord.wav");                    
                        dsk_keytone_set_state(state);
                    }
                    break;
				}
                case SWITCH_TO_OTHER_APP:
				{
					switch(LOWORD(msg->dwAddData2))
					{
					    case FM_SW_TO_RECORD:
						{
                            __here__;                            

                            GUI_ManWinDelete(root_ctrl->h_app_fm);
        			        root_ctrl->h_app_fm = 0;

                            //__app_root_change_to_default_bg();//这里不是通过主界面进入的，不记录进入ap之前背景的状态。
                                
							__msg("**********FM_SW_TO_RECORD**********\n");	
                            root_ctrl->root_para->root_type = HIWORD(msg->dwAddData2);//RAT_USB或者RAT_TF
                            root_ctrl->root_para->record_opt = 1;//fm录音
							root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
							root_ctrl->h_app_record=  app_record_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_record);

                            {//重新初始化按键音
                                __set_keytone_t state;

                                dsk_keytone_get_state(&state);
                                dsk_keytone_exit();
                                dsk_keytone_init(BEETLES_APP_ROOT"res\\sounds\\chord.wav");                    
                                dsk_keytone_set_state(state);
                            }
                            break;
					    }
                        default:
                        {
                            break;
                        }
					}
                }
                default:
                {
                    break;
                }
            }										
					
			break;
		}
        case APP_RECORD_ID:
		{
            switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_MMENU:
				{
                    __msg("**********record to home**********\n");
					
        			GUI_ManWinDelete(root_ctrl->h_app_record);
        			root_ctrl->h_app_record = 0;

        			__app_home_restore(msg);
                    
                    break;
				}
                case SWITCH_TO_OTHER_APP:
				{
					switch(LOWORD(msg->dwAddData2))
					{
                        case RECORD_SW_TO_FM:
                        {
                            __here__;
                            //__app_root_change_to_default_bg();//这里不是通过主界面进入的，不记录进入ap之前背景的状态。

                            GUI_ManWinDelete(root_ctrl->h_app_record);
        			        root_ctrl->h_app_record = 0;
                                
							__msg("**********FM_SW_TO_RECORD**********\n");	                                                        
							root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
							root_ctrl->h_app_fm = app_fm_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_fm);
                            break;
                        }
                        default:
                        {
                            break;
                        }
					}                    
                }
                default:
                {
                    break;
                }
            }										
					
			break;
		}
        case APP_CALENDAR_ID:
		{
            switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_MMENU:
				{
                    __msg("**********calendar to home**********\n");
					
        			GUI_ManWinDelete(root_ctrl->h_app_calendar);
        			root_ctrl->h_app_calendar = 0;

        			__app_home_restore(msg);
                    
                    break;
				}
                default:
                {
                    break;
                }
            }										
					
			break;
		}

		case APP_SETTING_ID:
		{
			switch(HIWORD(msg->dwAddData1))
			{
			case SWITCH_TO_OTHER_APP:
				{
					switch(msg->dwAddData2)
					{
					case SETTING_SW_TO_MAIN:
						{
							__msg("**********setting to home**********\n");
							GUI_ManWinDelete(root_ctrl->h_app_setting);
        			        root_ctrl->h_app_setting= 0;
							__app_home_restore(msg);
							
						}
						break;
					case SETTING_SW_TO_MULTIMEDIA:
					{
						
					}
						break;
					case SETTING_SW_TO_PROMPT_UPGRADE:
					{

					}
					break;
					case SETTING_SW_TO_PROMPT_DEFAULT:
					{
					}						
					break;
					case EXPLR_SW_TO_UPGRADE:
					{
	
					}						
					break;
					default:
						break;
					}
					//root_ctrl->h_app_setting = NULL;
				}
					break;
				default:
					break;
			}	
			break;
		}
		default:
			break;
	}

	return EPDK_OK;
}

#endif
static __s32 __app_root_broadcast_msg(__gui_msg_t *msg)
{
    H_WIN root, child;
	char  winname[256];  
    __gui_msg_t set_msg;

    eLIBs_memcpy(&set_msg, msg, sizeof(__gui_msg_t));
        
	root  = msg->h_deswin;	
	child = GUI_WinGetFirstChild(root);
	
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 256);
		GUI_WinGetName(child, winname);
				
		__msg("app_root_broadcast_msg msg->id=%d,winname = %s \n", 
            msg->id, winname);

        set_msg.h_deswin = child;
		GUI_SendMessage(&set_msg);
        // GUI_SendNotifyMessage(&set_msg);
        __here__;
		
		child = GUI_WinGetNextBro(child);
	} 	
    
    return EPDK_OK;
}

static __s32  __app_root_send_msg2focus_child(__gui_msg_t *msg)
{
    H_WIN root, child;	
    __gui_msg_t set_msg;

    eLIBs_memcpy(&set_msg, msg, sizeof(__gui_msg_t));
		
	root  = msg->h_deswin;	
	child = GUI_WinGetFocusChild(root);
	if(child)
	{	
    	GUI_SendMessage(&set_msg);
	}
    
    return EPDK_OK;
}

static __hdle           g_pe0 = 0;
static __hdle           g_pe1 = 0;
static __hdle           g_pe2 = 0;
static __hdle           g_pe3 = 0;

static __s32 __app_request_pins(void)
{
    __s32            ret;
    user_gpio_set_t  gpio_set[1];  
    
	/* 申请input_row1 */
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "input_row1", (int *)gpio_set, sizeof(user_gpio_set_t)/4);

    __msg("ret= %d\n", ret);

    {        
        __u32 *pTPCtrl = NULL;
        __u32 data = 0;
        pTPCtrl = (__u32 *)0xf1c24800; //必须打开AD模块，PE2口才会有效
        data = *pTPCtrl; 
        __msg("tp ctrl registor is %d\n", data); 
        data |= 0x07;
        *pTPCtrl = data; 
        __msg("tp ctrl registor is %d\n", data);
    }

    if(!g_pe0)
    {
        /* 申请tv_en */
        eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
        ret = esCFG_GetKeyValue("test_peport", "test_pea", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
        if (!ret)
        {
            g_pe0 = esPINS_PinGrpReq(gpio_set, 1);
            if (!g_pe0)
            {
                __msg("request output_col pin failed\n");
                return EPDK_FAIL;
            }
        }
        else
        {
            __msg("fetch para from script failed\n");
            return EPDK_FAIL;
        }
    }

    if(!g_pe1)
    {
        /* 申请tv_en */
        eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
        ret = esCFG_GetKeyValue("test_peport", "test_peb", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
        if (!ret)
        {
            g_pe1 = esPINS_PinGrpReq(gpio_set, 1);
            if (!g_pe1)
            {
                __msg("request output_col pin failed\n");
                return EPDK_FAIL;
            }
        }
        else
        {
            __msg("fetch para from script failed\n");
            return EPDK_FAIL;
        }
    }

    if(!g_pe2)
    {
        /* 申请tv_en */
        eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
        ret = esCFG_GetKeyValue("test_peport", "test_pec", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
        if (!ret)
        {
            g_pe2 = esPINS_PinGrpReq(gpio_set, 1);
            if (!g_pe2)
            {
                __msg("request output_col pin failed\n");
                return EPDK_FAIL;
            }
        }
        else
        {
            __msg("fetch para from script failed\n");
            return EPDK_FAIL;
        }
    }

    if(!g_pe3)
    {
        /* 申请tv_en */
        eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
        ret = esCFG_GetKeyValue("test_peport", "test_ped", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
        if (!ret)
        {
            g_pe3 = esPINS_PinGrpReq(gpio_set, 1);
            if (!g_pe3)
            {
                __msg("request output_col pin failed\n");
                return EPDK_FAIL;
            }
        }
        else
        {
            __msg("fetch para from script failed\n");
            return EPDK_FAIL;
        }
    }

    return EPDK_OK;
}

static __s32 __app_pullup_pe(void)
{  
    __s32            ret;
    user_gpio_set_t  gpio_set[16];  

    if(!g_pe0 ||!g_pe1 ||!g_pe2 ||!g_pe3 )
    {
        __msg("__app_pullup_pe fail...\n");
        return EPDK_FAIL;
    }

    __msg("__app_pullup_pe...\n");
    esPINS_WritePinData(g_pe0, 1, 0);
    esPINS_WritePinData(g_pe1, 1, 0);
    esPINS_WritePinData(g_pe2, 1, 0);
    esPINS_WritePinData(g_pe3, 1, 0);
		
	return EPDK_OK;
}

static __s32 __app_pulldown_pe(void)
{  
     __s32            ret;
    user_gpio_set_t  gpio_set[16];  

    if(!g_pe0 ||!g_pe1 ||!g_pe2 ||!g_pe3 )
    {
        __msg("__app_pullup_pe fail...\n");
        return EPDK_FAIL;
    }

    __msg("__app_pulldown_pe...\n");
    esPINS_WritePinData(g_pe0, 0, 0);
    esPINS_WritePinData(g_pe1, 0, 0);
    esPINS_WritePinData(g_pe2, 0, 0);
    esPINS_WritePinData(g_pe3, 0, 0);
		
	return EPDK_OK;
}

static __s32 mic_status=0; 
__s32 app_root_get_mic_state(void)
{
    return mic_status;
}



__s32 app_root_win_proc(__gui_msg_t *msg)
{
    __msg("app_root_win_proc msg->id=%d, msg->h_deswin=%x, msg->dwAddData1=%d, msg->dwAddData2=%d\n"
        ,msg->id, msg->h_deswin, msg->dwAddData1, msg->dwAddData2);
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
 		{			
			root_para_t *root_para;
			root_ctrl_t   *root_ctrl;				
			
			root_para = esMEMS_Balloc(sizeof(root_para_t));
            if(!root_para)
            {
                return EPDK_FAIL;
            }
			eLIBs_memset(root_para, 0, sizeof(root_para_t));

			root_ctrl = esMEMS_Balloc(sizeof(root_ctrl_t));
            if(!root_ctrl)
            {
                esMEMS_Bfree(root_para, sizeof(root_para_t));
                return EPDK_FAIL;
            }
			eLIBs_memset(root_ctrl, 0, sizeof(root_ctrl_t));           
            
			root_para->h_parent		= msg->h_deswin;
			root_para->font			= SWFFont;
			root_para->root_type	= 0;
			root_ctrl->h_app_home	= app_home_create(root_para);
            __msg("root_ctrl->h_app_home = %x\n", root_ctrl->h_app_home);
            
			GUI_WinSetFocusChild(root_ctrl->h_app_home);
			
			root_ctrl->root_para = root_para;
			GUI_WinSetAddData(msg->h_deswin, (__u32)root_ctrl);                       

            __root_scene_sem_init();           
                                  

		   
 			root_ctrl->root_para->amplify_open_delay_cnt = 0;

			//g_disable_close_scn();
		}
		return EPDK_OK;
		
#if 1//wqx 20130406		
        case GUI_MSG_TIMER:
        {
			#if 0
            root_ctrl_t   *root_ctrl;
			root_para_t *root_para;
		
			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			root_para = root_ctrl->root_para;

			if (!root_ctrl)
			{
				__err("root_ctrl is null...\n");
                return EPDK_OK;
			}
			#endif

            break;
        }
#endif
		case GUI_MSG_CLOSE:
		{
			GUI_ManWinDelete(msg->h_deswin);
		}
		return EPDK_OK;
		case GUI_MSG_DESTROY:
 		{
			root_para_t *root_para;
			root_ctrl_t   *root_ctrl;
			
			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			if (!root_ctrl)
			{
				__msg("******err****\n");
                return EPDK_OK;
			}
			
			root_para = (root_para_t *)root_ctrl->root_para;
			if (!root_para)
			{
				__msg("******err****\n");
                return EPDK_OK;
			}

            dsk_amplifier_onoff(0);
            if(GUI_IsTimerInstalled(msg->h_deswin, AMPLIFER_CHECK_TIMMER_ID))
            {
                GUI_KillTimer(msg->h_deswin, AMPLIFER_CHECK_TIMMER_ID);
            }
#if BEETLES_LINE_IN_CTRL
            if(GUI_IsTimerInstalled(msg->h_deswin, AUX_CHECK_TIMER_ID))
            {
                GUI_KillTimer(msg->h_deswin, AUX_CHECK_TIMER_ID);
            }  
#endif	

#if BEETLES_MODIFY_TOOL
            if(GUI_IsTimerInstalled(msg->h_deswin, NUM_KEY_COUNT_TID))
            {
                GUI_KillTimer(msg->h_deswin, NUM_KEY_COUNT_TID);
            } 
#endif

			esMEMS_Bfree(root_para, sizeof(root_para_t));
			esMEMS_Bfree(root_ctrl, sizeof(root_ctrl_t));	

            __root_scene_sem_deinit();
 		}
		return EPDK_OK;
		case GUI_MSG_COMMAND:
 		{           
 			//app_root_command_proc(msg);
            
			return EPDK_OK;
 		}					
		case GUI_MSG_KEY:
		{
			
            __bool audio_engine_is_open;            
			root_ctrl_t   *root_ctrl;
			break;
			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
            
			__msg("**********************before app_root_win_proc GUI_MSG_KEY**************************\n");                      
            
            audio_engine_is_open = 0;

            if(!audio_engine_is_open)
            {
                __here__;
                if(EPDK_TRUE == dsk_radio_rcv_is_open())
                {
                    __here__;
                    audio_engine_is_open = 1;    
                }
            }


            __here__;

            if(!audio_engine_is_open)
            {
                __here__;
                if(EPDK_TRUE == robin_is_open())
                {
                    //__cedar_status_t status;

                    //__here__;
                    //status = robin_get_fsm_status();
                    //__msg("status = %d\n", status);
                    
                    //if(CEDAR_STAT_PAUSE != status && CEDAR_STAT_STOP != status)
                    {
                        __here__;
                        audio_engine_is_open = 1;
                    }
                }
            }

            __here__;

#if BEETLES_ENABLE_WKM
			if(!audio_engine_is_open)
            {
                if(EPDK_TRUE == dsk_wkm_is_open())
                {
                    //__cedar_status_t status;

                    //status = dsk_wkm_get_fsm_state();
                    //if(CEDAR_STAT_PAUSE != status && CEDAR_STAT_STOP != status)
                    {
                        audio_engine_is_open = 1;
                    }               
                }
            }
#endif
            __here__;
			if(audio_engine_is_open)
			{
		                __u32 key_type;
		                key_type = dsk_keytone_get_key_type();
		                if(KEY_DOWN_ACTION == msg->dwAddData2)
		                {
		                    __here__;
		                    if(KEYTONE_KEY_TYPE_DOWN & key_type)
		                    {
		                        __here__;
					app_root_play_keytone();
		                        __here__;
		                    }            
		                }
		                else if(KEY_UP_ACTION == msg->dwAddData2)
		                {
		                    __here__;
		                    if(KEYTONE_KEY_TYPE_UP & key_type)
		                    {
		                        __here__;
					app_root_play_keytone();
		                        __here__;
		                    }   
		                }
		            __here__;
			}
			else
            if(//(!audio_engine_is_open)&&
				(!root_ctrl->h_app_record))
            {
                __u32 key_type;

                __here__;
                key_type = dsk_keytone_get_key_type();
                if(KEY_DOWN_ACTION == msg->dwAddData2)
                {
                    __here__;
                    if(KEYTONE_KEY_TYPE_DOWN & key_type)
                    {
                        __here__;
                        dsk_keytone_on();
                        __here__;
                    }            
                }
                else if(KEY_UP_ACTION == msg->dwAddData2)
                {
                    __here__;
                    if(KEYTONE_KEY_TYPE_UP & key_type)
                    {
                        __here__;
                        dsk_keytone_on();
                        __here__;
                    }   
                }
                else if(KEY_REPEAT_ACTION == msg->dwAddData2)
                {
                    __here__;
                    if(KEYTONE_KEY_TYPE_REPEAT& key_type)
                    {
                        __here__;
			if((GUI_MSG_KEY_LONGVADD != msg->dwAddData1)&&(GUI_MSG_KEY_LONGVDEC!= msg->dwAddData1))
                        dsk_keytone_on();
                        __here__;
                    }   
                }               
            }
#if 1	//wqx 20131031
			if(msg->dwAddData1==GUI_MSG_KEY_SCR_ONOFF)
			{
				//if(root_ctrl->h_app_movie)
				//return EPDK_OK;
				if(msg->dwAddData2 == KEY_UP_ACTION)
				{
					if(g_screen_is_on())
					{
						if(root_ctrl->h_app_movie || root_ctrl->h_app_photo)
							g_enable_close_scn();
						g_close_screen();
					}
					else
					{
						//if(root_ctrl->h_app_movie || root_ctrl->h_app_photo)
						//	g_disable_close_scn();
						//g_open_screen();
					}
				}
				return EPDK_OK;
			}
#endif
#if 0
__here__;
			if(cmd_to_app_key_proc(msg) == EPDK_OK)    //应用程序快捷键响应
			{
				return EPDK_OK;				//响应app_key 则不再向下发送消息
			}
#endif
			if((msg->dwAddData1 == GUI_MSG_KEY_IR_POWER || msg->dwAddData1 == GUI_MSG_KEY_LONGIR_POWER)
				&& msg->dwAddData2 == KEY_UP_ACTION)  //在此处添加遥控的power键处理//进入主界面后在进入待机模式
			{
				root_ctrl_t   *root_ctrl;
				
				root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
				
				delete_all_app_except_home(root_ctrl);
				if(root_ctrl->h_app_home == NULL)
					__app_home_restore(msg);
				
				g_close_screen();

				return EPDK_OK;
			}

//#if BEETLES_MODIFY_TOOL
#if 0
			//启动屏参调节
			if((msg->dwAddData1 == GUI_MSG_KEY_NUM8) && (msg->dwAddData2 == KEY_UP_ACTION))
			{
				eLIBs_printf("-- %s, %d, Root Key_Num8, setting_opt = %d -- \n", __FILE__, __LINE__, root_ctrl->root_para->setting_opt);
				if(root_ctrl->h_app_setting && (root_ctrl->root_para->setting_opt == 0))
				{
					num_key_count ++;
					eLIBs_printf("-- %s, %d, Root Key_Num8 , num_key_count = %d-- \n", __FILE__, __LINE__, num_key_count);

					if(num_key_count < 4)
					{
			            if(GUI_IsTimerInstalled(msg->h_deswin, NUM_KEY_COUNT_TID))
			            {
				            GUI_ResetTimer(msg->h_deswin, NUM_KEY_COUNT_TID, NUM_KEY_COUNT_TOUT, 0);
			            } 
						else
						{
				             GUI_SetTimer(msg->h_deswin, NUM_KEY_COUNT_TID, NUM_KEY_COUNT_TOUT, NULL);                        
						}
					}
					else
					{
						eLIBs_printf("-- %s, %d, Root Key_Num8 -- \n", __FILE__, __LINE__);
					
			            if(GUI_IsTimerInstalled(msg->h_deswin, NUM_KEY_COUNT_TID))
			            {
			                GUI_KillTimer(msg->h_deswin, NUM_KEY_COUNT_TID);
			            }  
						num_key_count = 0;
						
						//重新启动设置
						{
							root_ctrl_t   *root_ctrl;
							root_para_t* root_para = NULL;

							root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
							root_para = root_ctrl->root_para;
						
							delete_all_app(root_ctrl);

							 gscene_hbar_set_state(HBAR_ST_HIDE);
#ifdef KEY_VOICE
				 			app_root_play_app_sounds(SHORT_KEY_WAVE_FILE_SETTING);
#endif				
                            root_ctrl->root_para->setting_opt = 1;//电源设置
							root_ctrl->root_para->h_parent= msg->h_deswin;
							root_ctrl->h_app_setting=  app_setting_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_setting);	
						}
					}

				}
			}
#endif			
/*
			switch(msg->dwAddData1)//在此处添加红外遥控音量键处理
			{
				case GUI_MSG_KEY_VADD:
				case GUI_MSG_KEY_LONGVADD:
				{
					if(msg->dwAddData2 != KEY_UP_ACTION)
					{
						dsk_volume_inc();
					}
					return EPDK_OK;
				}
				case GUI_MSG_KEY_VDEC:
				case GUI_MSG_KEY_LONGVDEC:
				{
					if(msg->dwAddData2 != KEY_UP_ACTION)
					{
						dsk_volume_dec();
					}
					return EPDK_OK;
				}
				default:
					break;
			}	
*/			

#if 1	//Modify By Ethan; 
		if(app_root_mute_key_proc(msg) == EPDK_OK)
		{
			return EPDK_OK;
		}
#endif
			
            __msg("after app_root_win_proc GUI_MSG_KEY\n");
			break;
		}
		case GUI_MSG_TOUCH:			
		{
			__msg("app_root_win_proc GUI_MSG_TOUCH\n");
			break;
		}		
		case DSK_MSG_FS_PART_PLUGOUT:
 		{
 			root_ctrl_t   *root_ctrl;
 			H_WIN root, child, next_child;
 			char  winname[256];          

            __msg("**************app_root_win_proc, DSK_MSG_FS_PART_PLUGOUT**************\n");
            if(('D' == msg->dwAddData2)&&('E' == msg->dwAddData2)&&('Z' == msg->dwAddData2))
            {
                __msg("internal disk, not need kill apps, msg->dwAddData1=%d\n", msg->dwAddData2);
                __root_scene_sem_post();
                return EPDK_OK;
            }           

            __msg("app_root_broadcast_msg, msg->dwAddData2=%d\n", msg->dwAddData2);
            __app_root_broadcast_msg(msg);	                      

            __msg("********before __app_do_async***********\n");
            __msg("rat_clear_all_list_by_root=%x,msg->dwAddData2=%x\n", (__u32)rat_clear_all_list_by_root, msg->dwAddData2);
            __app_do_async(msg->h_deswin, APP_ROOT_MSG_DO_ASYNC, rat_clear_all_list_by_root, (void*)(msg->dwAddData2));
          //  rat_clear_all_list();
            __msg("********after __app_do_async***********\n");

		root_check_fs_device(msg);   //cai++
			
            return EPDK_OK;
 		}		
		case DSK_MSG_FS_PART_PLUGIN:
		{
 			root_ctrl_t   *root_ctrl;
 			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

            __msg("app_root_win_proc, DSK_MSG_FS_PART_PLUGIN\n");
            if(('D' == msg->dwAddData1)&&('E' == msg->dwAddData1)&&('Z' == msg->dwAddData1))
            {
                __msg("internal disk, not need notify home app, msg->dwAddData1=%d\n", msg->dwAddData1);
                return EPDK_OK;
            }

            __msg("app_root_broadcast_msg, msg->dwAddData1=%d\n", msg->dwAddData1);
            __app_root_broadcast_msg(msg);

		 root_check_fs_device(msg); 			//add by libaiao,2011.7.13   //cai++
            return EPDK_OK;
		}               		        
        case DSK_MSG_VOLTAGE_CHANGE:        
        {
            __msg("app_root_broadcast_msg, msg->dwAddData1=%d\n", msg->dwAddData1);
            __app_root_broadcast_msg(msg);
            return EPDK_OK;
        }		
        case DSK_MSG_TVDAC_PLUGOUT:
        case DSK_MSG_HDMI_PLUGOUT:
        {
            __msg("__app_root_broadcast_msg, msg->dwAddData1=%d\n", msg->dwAddData1);
            __app_root_broadcast_msg(msg);
            return EPDK_OK;
        }
        case APP_ROOT_MSG_DO_ASYNC:
        {            
            p_app_root_fun pfun;

            __msg("msg->dwAddData1=%x, msg->dwAddData2=%x\n",
                msg->dwAddData1, msg->dwAddData2);
            
            if(!msg->dwAddData1)
            {
                return EPDK_OK;
            }
            
            pfun = (p_app_root_fun)(msg->dwAddData1);
            __msg("pfun=%x, msg->dwAddData2=%x\n", pfun, msg->dwAddData2);
            __here__;
            pfun((void*)(msg->dwAddData2));
            __here__;
            return EPDK_OK;
        }
		case DSK_MSG_HBAR_DISK_PAINT:
			 gscene_hbar_set_title(last_str,sizeof(last_str));
			 return EPDK_OK;

		case DSK_MSG_MIC_IN_TIP:
		{
            root_ctrl_t   *root_ctrl;
			
			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			
    		app_root_show_dlg(root_ctrl->root_para->h_parent, STRING_MIC_DETECT_TIP, 0);
		}
		return EPDK_OK;
		
		case DSK_MSG_LOW_POWER_TIP:
		{
			root_ctrl_t   *root_ctrl;

			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

			app_root_show_dlg(root_ctrl->root_para->h_parent, STRING_HOME_LOW_POWER, 0);
		}
		break;			 
	}
	
	return GUI_ManWinDefaultProc(msg); 
}

__s32 app_root_win_proc_test(__gui_msg_t *msg)
{
    __msg("app_root_win_proc_test msg->id=%d, msg->h_deswin=%x, msg->dwAddData1=%d, msg->dwAddData2=%d\n"
        ,msg->id, msg->h_deswin, msg->dwAddData1, msg->dwAddData2);
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
 		{			
			__msg("app_root_win_proc_test GUI_MSG_CREATE\n");
 		}
		return EPDK_OK;
		case GUI_MSG_CLOSE:
		{
			GUI_ManWinDelete(msg->h_deswin);
		}
		return EPDK_OK;
		case GUI_MSG_DESTROY:
 		{
			__msg("app_root_win_proc_test GUI_MSG_DESTROY\n");			
 		}
		return EPDK_OK;
		case GUI_MSG_COMMAND:
 		{
 			__msg("app_root_win_proc_test GUI_MSG_COMMAND\n");			

			return EPDK_OK;
 		}					
		case GUI_MSG_KEY:
		{
			__msg("app_root_win_proc_test GUI_MSG_KEY\n");		
			return EPDK_OK;
		}
	}
	
	return GUI_ManWinDefaultProc(msg); 
}


/* 创建根窗口 */
H_WIN app_root_wincreate(Activity *activity)
{
	H_WIN  						hManWin;
	
	__gui_manwincreate_para_t  	create_info;	
	
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.attr            = activity; 
	create_info.hParent			= activity_get_root_win(activity) ;
	create_info.hHosting 		= NULL;   
    create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)app_root_win_proc);
    create_info.name            = APP_ROOT; 	
	hManWin 					= GUI_ManWinCreate(&create_info);

    __msg("root win = %x\n", hManWin);
	
	if(hManWin == NULL)
    {        
        __wrn("home: create movie windows failed!\n");      
        return NULL;
    }
	
	return hManWin;
}
