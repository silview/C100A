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
#include "user_bar.h"


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


static __bool PRESET_FLAG  = EPDK_FALSE;

static H_WIN h_deswin_old;

#define USER_SET		0x1003	//user 设置闪烁定时器

static H_LYR lry; //半透明图层

static __s32 focus = 1;

static __u8 stopFocus = 0;

static __u8  numKeyCount = 0; //按键按下次数

static __u16 numTmp = 0;	//按键数字保存

static __u8  curID ; // 当前模式

static __bool  userConfirm = EPDK_TRUE;

static __u8 nameCount = 0;
static __u8 curNameCount = 1;
static __u8 name[7] = {0};
static __u8 level[18] ;
static __u8 incline[18];

static __u8 fanFlag = 0;

static __bool isSaveFlag = EPDK_FALSE;//是否保存数据

static __u8 curSet = 1; //该段设置值
static __u8 curSetCount = 0; //当前 设置的段数

static __bool twinkleFlag = EPDK_TRUE;  //闪烁标志

static __u8  maxLevel = 0;;

static __bool unitFlag = EPDK_FALSE;

static __bool MormalFlag = EPDK_FALSE;

static __bool StartFlag = EPDK_FALSE;

static __u8 pageData[2];
static __u8 writeData[32];
static __u8 user1In_flag;
static __u8 user2In_flag;

static __bool BTSET_FLAG;
static __u16  BT_weight;
static __u16  BT_age;
static __u16  BT_time;
static __u8  *BT_level;

static __u16 NoKeyCount = 0;

static __u16 ageTmp;
static __u16 weightTmp;
static __u16 timeTmp;
static __u16 levelTmp;
static __u16 incTmp;
static __bool bufferOn = EPDK_TRUE;

static __bool KEYNUM_Flag = EPDK_FALSE;
static __bool keyTonFlag;
#ifdef AC00280

#else ifdef AC00285
static __u16 STRIDE_INC_TABLE[13] = {
	180,185,190,195,200,205,210,215,220,225,230,235,240,
};

#endif


static __u16 SP_Data[5];
/*
0---名字输入画面
1---常用设置画面
2---level设置画面
3---incline设置画面
4---run program画面
5---delete program 画面


*/
static __u8 curPicture = 0; //当前所处画面
#if 0
static __s32 setNUM_ID[] = {
	ID_HOME_TV_NUMBER_SETTING_0_BMP,
	ID_HOME_TV_NUMBER_SETTING_1_BMP,
	ID_HOME_TV_NUMBER_SETTING_2_BMP,
	ID_HOME_TV_NUMBER_SETTING_3_BMP,
	ID_HOME_TV_NUMBER_SETTING_4_BMP,
	ID_HOME_TV_NUMBER_SETTING_5_BMP,
	ID_HOME_TV_NUMBER_SETTING_6_BMP,
	ID_HOME_TV_NUMBER_SETTING_7_BMP,
	ID_HOME_TV_NUMBER_SETTING_8_BMP,
	ID_HOME_TV_NUMBER_SETTING_9_BMP,
	ID_HOME_TV_NUMBER_SETTING_POINT_BMP,
};
#endif
#if 0
static __s32 userName_ID[] = {
	ID_HOME_TV_NUMBER_SETTING_A_BMP,
	ID_HOME_TV_NUMBER_SETTING_B_BMP,
	ID_HOME_TV_NUMBER_SETTING_C_BMP,
	ID_HOME_TV_NUMBER_SETTING_D_BMP,
	ID_HOME_TV_NUMBER_SETTING_E_BMP,
	ID_HOME_TV_NUMBER_SETTING_F_BMP,
	ID_HOME_TV_NUMBER_SETTING_G_BMP,
	ID_HOME_TV_NUMBER_SETTING_H_BMP,
	ID_HOME_TV_NUMBER_SETTING_I_BMP,
	ID_HOME_TV_NUMBER_SETTING_J_BMP,
	ID_HOME_TV_NUMBER_SETTING_K_BMP,
	ID_HOME_TV_NUMBER_SETTING_L_BMP,
	ID_HOME_TV_NUMBER_SETTING_M_BMP,
	ID_HOME_TV_NUMBER_SETTING_N_BMP,
	ID_HOME_TV_NUMBER_SETTING_O_BMP,
	ID_HOME_TV_NUMBER_SETTING_P_BMP,
	ID_HOME_TV_NUMBER_SETTING_Q_BMP,
	ID_HOME_TV_NUMBER_SETTING_R_BMP,
	ID_HOME_TV_NUMBER_SETTING_S_BMP,
	ID_HOME_TV_NUMBER_SETTING_T_BMP,
	ID_HOME_TV_NUMBER_SETTING_U_BMP,
	ID_HOME_TV_NUMBER_SETTING_V_BMP,
	ID_HOME_TV_NUMBER_SETTING_W_BMP,
	ID_HOME_TV_NUMBER_SETTING_X_BMP,
	ID_HOME_TV_NUMBER_SETTING_Y_BMP,
	ID_HOME_TV_NUMBER_SETTING_Z_BMP,
	ID_HOME_TV_NUMBER_SETTING_SPACE_BMP,
};
#endif
static mm_res_id_t ui_res_user[] =
{
	{ID_HOME_BTN_SETTING_AGE_1_BMP,ID_HOME_BTN_SETTING_AGE_2_BMP},
	{ID_HOME_BTN_SETTING_WEIGHT_1_BMP,ID_HOME_BTN_SETTING_WEIGHT_2_BMP},
	{ID_HOME_BTN_SETTING_TIME_1_BMP,ID_HOME_BTN_SETTING_TIME_2_BMP},
	{ID_HOME_BTN_SETTING_LEVEL_1_BMP,ID_HOME_BTN_SETTING_LEVEL_2_BMP},
	#ifdef AC00280
	{ID_HOME_BTN_SETTING_INCLINE_1_BMP,ID_HOME_BTN_SETTING_INCLINE_2_BMP},
	#else ifdef AC00285
	{ID_HOME_BTN_SETTING_STRIDE_1_BMP,ID_HOME_BTN_SETTING_STRIDE_2_BMP},
	#endif
};
#if 0
 static __s32 sports_set_profile_ID[] = {
	ID_HOME_IMG_SETTING_PROFILE_00_BMP,
	ID_HOME_IMG_SETTING_PROFILE_01_BMP,
	ID_HOME_IMG_SETTING_PROFILE_02_BMP,
	ID_HOME_IMG_SETTING_PROFILE_03_BMP,
	ID_HOME_IMG_SETTING_PROFILE_04_BMP,
	ID_HOME_IMG_SETTING_PROFILE_05_BMP,
	ID_HOME_IMG_SETTING_PROFILE_06_BMP,
	ID_HOME_IMG_SETTING_PROFILE_07_BMP,
	ID_HOME_IMG_SETTING_PROFILE_08_BMP,
};

static __s32 runProgram_ID[]= {
	ID_HOME_BTN_SETTING_RUN_1_BMP,
	ID_HOME_BTN_SETTING_RUN_2_BMP,
};

static __s32 deleteProgram_ID[]= {
	ID_HOME_BTN_SETTING_DELETE_1_BMP,
	ID_HOME_BTN_SETTING_DELETE_2_BMP,
};
#endif

static mm_res_id_t* user_get_ui_res(__s32 rotate)
{
	return ui_res_user;	
}


/*
	计算四舍五入函数
	传入分子和分母
	返回计算结果
*/

static __u16 countNUM(__u16 numerator,__u16 denominator)
{
	__u16 result;
	__u16 ret;

	result = numerator*10/denominator;

	if (result > 99) {
		if (result%100%10 > 4) {
			ret = result/10+1;
		} else {
			ret = result/10;
		}
	} else if (result > 9 ) {
		if (result%10 > 4) {
			ret = result/10 + 1;
		} else {
			ret = result/10;
		}
	} else {
		if (result> 4) {
			ret = 1;
		} else {
			ret = 1;
		}
	}

	
	return ret;
}

/*无条件进位算法*/

static __u16 DMTONUM(__u16 numerator,__u16 denominator) 
{
	
	__u16 result;
	__u16 ret;
	
	result = numerator*100/denominator;

	if (result > 999) {
		if (result%100 > 0) {
			ret = result/100+1;
		} else {
			ret = result/100;
		}
	} else if (result > 99 ) {
		if (result%100 > 0) {
			ret = result/100 + 1;
		} else {
			ret = result/100;
		}
	} else {
		if (result> 0) {
			ret = 1;
		} else {
			ret = 1;
		}
	}


	return ret;

}

static __u8 ProfileCount(__u8 value) 
{
	__u8 ret;

	switch(value) {
		case 1:
		case 2:
			ret = 1;
			break;
		case 3:
		case 4:
		case 5:
			ret = 2;
			break;
		case 6:
		case 7:
			ret = 3;
			break;
		case 8:
		case 9:
		case 10:
			ret = 4;
			break;
		case 11:
		case 12:
			ret = 5;
			break;
		case 13:
		case 14:
		case 15:
			ret = 6;
			break;
		case 16:
		case 17:
			ret = 7;
			break;
		case 18:
		case 19:
		case 20:
			ret = 8;
			break;

	}

	return ret;

}


/*
	申请资源
*/
static __s32 init_user_res(mmenu_attr_t *mmenu_attr)
{
    __u32 	i;    
   // home_uipara_t* home_ui_para;
    mm_res_id_t* main_ui_res;

    if(EPDK_TRUE == mmenu_attr->res_init)
    {
        return EPDK_OK;
    }

    main_ui_res = user_get_ui_res(GUI_GetScnDir());
    if(!main_ui_res)
    {
        __err("main_ui_para is null...\n");
        return EPDK_FAIL;
    }

	//for(i = 0 ; i < 11; i++)
	//{
		mmenu_attr->settingnum_bmp[0] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_0_BMP);
		mmenu_attr->settingnum_bmp[1] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_1_BMP);
		mmenu_attr->settingnum_bmp[2] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_2_BMP);
		mmenu_attr->settingnum_bmp[3] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_3_BMP);
		mmenu_attr->settingnum_bmp[4] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_4_BMP);
		mmenu_attr->settingnum_bmp[5] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_5_BMP);
		mmenu_attr->settingnum_bmp[6] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_6_BMP);
		mmenu_attr->settingnum_bmp[7] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_7_BMP);
		mmenu_attr->settingnum_bmp[8] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_8_BMP);
		mmenu_attr->settingnum_bmp[9] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_9_BMP);
		mmenu_attr->settingnum_bmp[10] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_POINT_BMP);
	//}

	//for(i = 0 ; i < 27; i++)
	//{
		mmenu_attr->USER_NAME[0] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_A_BMP);
		mmenu_attr->USER_NAME[1] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_B_BMP);
		mmenu_attr->USER_NAME[2] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_C_BMP);
		mmenu_attr->USER_NAME[3] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_D_BMP);
		mmenu_attr->USER_NAME[4] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_E_BMP);
		mmenu_attr->USER_NAME[5] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_F_BMP);
		mmenu_attr->USER_NAME[6] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_G_BMP);
		mmenu_attr->USER_NAME[7] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_H_BMP);
		mmenu_attr->USER_NAME[8] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_I_BMP);
		mmenu_attr->USER_NAME[9] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_J_BMP);
		mmenu_attr->USER_NAME[10] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_K_BMP);
		mmenu_attr->USER_NAME[11] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_L_BMP);
		mmenu_attr->USER_NAME[12] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_M_BMP);
		mmenu_attr->USER_NAME[13] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_N_BMP);
		mmenu_attr->USER_NAME[14] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_O_BMP);
		mmenu_attr->USER_NAME[15] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_P_BMP);
		mmenu_attr->USER_NAME[16] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_Q_BMP);
		mmenu_attr->USER_NAME[17] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_R_BMP);
		mmenu_attr->USER_NAME[18] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_S_BMP);
		mmenu_attr->USER_NAME[19] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_T_BMP);
		mmenu_attr->USER_NAME[20] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_U_BMP);
		mmenu_attr->USER_NAME[21] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_V_BMP);
		mmenu_attr->USER_NAME[22] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_W_BMP);
		mmenu_attr->USER_NAME[23] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_X_BMP);
		mmenu_attr->USER_NAME[24] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_Y_BMP);
		mmenu_attr->USER_NAME[25] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_Z_BMP);
		mmenu_attr->USER_NAME[26] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_SPACE_BMP);
	//}
	
	//for(i = 0 ; i < 9; i++)
	//{
		mmenu_attr->sports_profile_bmp[0]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_00_BMP);
		mmenu_attr->sports_profile_bmp[1]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_01_BMP);
		mmenu_attr->sports_profile_bmp[2]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_02_BMP);
		mmenu_attr->sports_profile_bmp[3]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_03_BMP);
		mmenu_attr->sports_profile_bmp[4]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_04_BMP);
		mmenu_attr->sports_profile_bmp[5]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_05_BMP);
		mmenu_attr->sports_profile_bmp[6]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_06_BMP);
		mmenu_attr->sports_profile_bmp[7]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_07_BMP);
		mmenu_attr->sports_profile_bmp[8]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_08_BMP);
	//}

	//for (i = 0; i < 2; i++)
	//{
		mmenu_attr->user_delete_bmp[0] = dsk_theme_open(ID_HOME_BTN_SETTING_DELETE_1_BMP);
		mmenu_attr->user_delete_bmp[1] = dsk_theme_open(ID_HOME_BTN_SETTING_DELETE_2_BMP);
		mmenu_attr->user_run_bmp[0] = dsk_theme_open(ID_HOME_BTN_SETTING_RUN_1_BMP);
		mmenu_attr->user_run_bmp[1] = dsk_theme_open(ID_HOME_BTN_SETTING_RUN_2_BMP);
	//}
	
	mmenu_attr->item_nr = 5; 
	
	for(i = 0; i < mmenu_attr->item_nr; i++)
	{       				
		mmenu_attr->unfocus_bmp[i] = dsk_theme_open(main_ui_res[i].unfocus_bmp_id);				

		mmenu_attr->focus_bmp[i] = dsk_theme_open(main_ui_res[i].focus_bmp_id);	
	}    

	mmenu_attr->levelProfile_bmp = dsk_theme_open(ID_HOME_IMG_SETTING_PROFILE_LEVEL_BMP);
	#ifdef AC00280
	mmenu_attr->inclineProfile_bmp = dsk_theme_open(ID_HOME_IMG_SETTING_PROFILE_INCLINE_BMP);
	#else ifdef AC00285
	mmenu_attr->inclineProfile_bmp = dsk_theme_open(ID_HOME_IMG_SETTING_PROFILE_STRIDE_BMP);
	#endif
	mmenu_attr->curProfile_bmp = dsk_theme_open(ID_HOME_IMG_SETTING_PROFILE_CURRENT_BMP);
	mmenu_attr->EnterUsername_bmp = dsk_theme_open(ID_HOME_IMG_SETTING_NAME_BMP);
	mmenu_attr->message_bmp_1 = dsk_theme_open(ID_HOME_TV_MESSAGE_SETTING_01_BMP);
	mmenu_attr->message_bmp_2 = dsk_theme_open(ID_HOME_TV_MESSAGE_SETTING_02_BMP);
	mmenu_attr->message_user1_bmp = dsk_theme_open(ID_HOME_TV_MESSAGE_SETTING_USER_01_BMP);
	mmenu_attr->message_user2_bmp = dsk_theme_open(ID_HOME_TV_MESSAGE_SETTING_USER_02_BMP);
	mmenu_attr->welcome_bmp = dsk_theme_open(ID_HOME_IMG_SETTING_WELCOME_BMP);
	switch(curID) {
		case ID_USER1:
			mmenu_attr->title_bmp = dsk_theme_open(ID_HOME_TV_SETTING_USER1_BMP);
			break;
		case ID_USER2:
			mmenu_attr->title_bmp = dsk_theme_open(ID_HOME_TV_SETTING_USER2_BMP);
			break;
	}
	
    mmenu_attr->res_init = EPDK_TRUE;
	 
	return EPDK_OK;
}

/*
	释放资源
*/
static __s32 uninit_user_res(mmenu_attr_t *mmenu_attr)
{
	  __u32 	i; 

    if(EPDK_FALSE == mmenu_attr->res_init)
    {
        return EPDK_OK;
    }

	mmenu_attr->item_nr = 5;

	for (i=0; i<mmenu_attr->item_nr; i++)
	{
		__msg("i = %d\n", i);
		dsk_theme_close(mmenu_attr->unfocus_bmp[i]);
		dsk_theme_close(mmenu_attr->focus_bmp[i]);		
	}

	for (i = 0; i < 11; i++)
	{
		dsk_theme_close(mmenu_attr->settingnum_bmp[i]);
	}

	
	for (i = 0; i < 27; i++)
	{
		dsk_theme_close(mmenu_attr->USER_NAME[i]);
	}

	for (i = 0; i < 9; i++)
	{
		dsk_theme_close(mmenu_attr->sports_profile_bmp[i]);
	}

	for (i = 0; i < 2; i++)
	{
		dsk_theme_close(mmenu_attr->user_delete_bmp[i]);
		dsk_theme_close(mmenu_attr->user_run_bmp[i]);
	}
	
	dsk_theme_close(mmenu_attr->message_bmp_1);
	dsk_theme_close(mmenu_attr->message_bmp_2);
	dsk_theme_close(mmenu_attr->message_user1_bmp);
	dsk_theme_close(mmenu_attr->message_user2_bmp);

	dsk_theme_close(mmenu_attr->title_bmp);
	dsk_theme_close(mmenu_attr->EnterUsername_bmp);
	dsk_theme_close(mmenu_attr->levelProfile_bmp);
	dsk_theme_close(mmenu_attr->inclineProfile_bmp);
	dsk_theme_close(mmenu_attr->curProfile_bmp);
	dsk_theme_close(mmenu_attr->welcome_bmp);
    mmenu_attr->res_init = EPDK_FALSE;
    
	return EPDK_OK;
}

static __u8 NUM2Stride(__u16  num) 
{
	__u8 stride;

	if (num <= 182) {
		stride = 1;
	} else if(num>182 && num <=187 ){
		stride = 2;
	}else if(num>187 && num <=192 ){
		stride = 3;
	}else if(num>192 && num <=197 ){
		stride = 4;
	}else if(num>197 && num <=202 ){
		stride = 5;
	}else if(num>202 && num <=207 ){
		stride = 6;
	}else if(num>207 && num <=212 ){
		stride = 7;
	}else if(num>212 && num <=217 ){
		stride = 8;
	}else if(num>217 && num <=222 ){
		stride = 9;
	}else if(num>222 && num <=227 ){
		stride = 10;
	}else if(num>227 && num <=232 ){
		stride = 11;
	}else if(num>232 && num <=237 ){
		stride = 12;
	}else if(num>237){
		stride = 13;
	}

	return stride;
}

/*
	画年龄
*/

static void draw_user_age(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 269;
	__s32 setNUM_y = 312;
	void *pbmp;


	if (KEYNUM_Flag) {
		if (mmenu_attr->manuel_set_p[0].manuel_setNUM < AGE_MIN || mmenu_attr->manuel_set_p[0].manuel_setNUM > AGE_MAX) {
			mmenu_attr->manuel_set_p[0].manuel_setNUM = ageTmp;
		}
	}
	
	if (mmenu_attr->manuel_set_p[0].manuel_setNUM < AGE_MIN) {
		mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_MIN;
	} else if (mmenu_attr->manuel_set_p[0].manuel_setNUM > AGE_MAX) {
		mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_MAX;
	}


	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[0].manuel_setNUM/10]);
	GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[0].manuel_setNUM%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
	ageTmp = mmenu_attr->manuel_set_p[0].manuel_setNUM;
}

static void draw_KEYNUM_user_age(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 269;
	__s32 setNUM_y = 312;
	void *pbmp;

	
	if (mmenu_attr->manuel_set_p[0].manuel_setNUM < 10) {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*2, setNUM_y+125);
	} else  {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[0].manuel_setNUM/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[0].manuel_setNUM%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
}

/*
	画level
*/

static void draw_user_level(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 179;
	__s32 setNUM_y = 312;
	void *pbmp;

	if (KEYNUM_Flag) {
		if (curSet<LEVEL_MIN ||curSet > LEVEL_MAX) {
			curSet = levelTmp;
		}
	}

	if (curSet > LEVEL_MAX) {
		curSet = LEVEL_MAX;
	} else if (curSet < LEVEL_MIN) {
		curSet = LEVEL_MIN;
	}
	
	if (curSet < 10) {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37, setNUM_y+125);
	} else  {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[curSet/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[curSet%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
	levelTmp = curSet;
}

static void draw_KEYNUM_user_level(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 179;
	__s32 setNUM_y = 312;
	void *pbmp;

	if (curSet > 20) {
		curSet = curSet/10;
		return;
	}
	if (curSet < 10) {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*2, setNUM_y+125);
	} else  {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[curSet/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[curSet%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
}


/*
	画incline
*/

static void draw_user_incline(mmenu_attr_t *mmenu_attr)
{
#ifdef AC00280
	__s32 setNUM_x = 179;
	__s32 setNUM_y = 312;
	void *pbmp;

	
	if (KEYNUM_Flag) {
		if (curSet<INCLINE_MIN ||curSet > INCLINE_MAX) {
			curSet = incTmp;
		}
	}

	if (curSet > INCLINE_MAX) {
		curSet = INCLINE_MAX;
	} else if (curSet < INCLINE_MIN) {
		curSet = INCLINE_MIN;
	}
	
	if (curSet < 10) {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37, setNUM_y+125);
	} else  {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[curSet/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[curSet%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
	incTmp = curSet;
#else ifdef AC00285
	__s32 setNUM_x = 144;
	__s32 setNUM_y = 312;
	void *pbmp;

	if (KEYNUM_Flag) {
	
	}

	if (curSet > 13) {
		curSet = 13;
	} else if (curSet < 1) {
		curSet = 1;
	}
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[STRIDE_INC_TABLE[curSet-1]/100]);
	GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[STRIDE_INC_TABLE[curSet-1]%100/10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37*2,setNUM_y);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[STRIDE_INC_TABLE[curSet-1]%100%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37*3,setNUM_y);
	incTmp = STRIDE_INC_TABLE[curSet];
#endif
}

static void draw_KEYNUM_user_incline(mmenu_attr_t *mmenu_attr)
{
#ifdef AC00280

	__s32 setNUM_x = 179;
	__s32 setNUM_y = 312;
	void *pbmp;
	

	if (curSet > 20) {
		curSet = curSet/10;
		return;
	}
	if (curSet < 10) {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*2, setNUM_y+125);
	} else  {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[curSet/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[curSet%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
#else ifdef AC00285
	__s32 setNUM_x = 144;
	__s32 setNUM_y = 312;
	void *pbmp;

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[numTmp/100]);
	GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[numTmp%100/10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37*2,setNUM_y);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[numTmp%100%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37*3,setNUM_y);
#endif
}

/*
	画体重
*/
static void draw_user_weight(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 458;
	__s32 setNUM_y = 312;
	void *pbmp;

	if (KEYNUM_Flag) {
		if (!unitFlag) {
			if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_B || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_B) {
				mmenu_attr->manuel_set_p[1].manuel_setNUM = weightTmp;
			}
		} else {
			if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_M || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_M) {
				mmenu_attr->manuel_set_p[1].manuel_setNUM = weightTmp;
			}
		}		
	}

	if (!unitFlag) {
		if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_B) {
			mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_MIN_B;
		} else if (mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_B) {
			mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_MAX_B;
		}

	}else {
		if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_M) {
			mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_MIN_M;
		} else if (mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_M) {
			mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_MAX_M;
		}
	}
	GUI_SetBkColor(0x00000000);
	GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*3, setNUM_y+125);
	if(mmenu_attr->manuel_set_p[1].manuel_setNUM >= 100)
	{
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37*2,setNUM_y);
	} else {
		//GUI_SetBkColor(0x00000000);
		//GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37, setNUM_y+125);
		if (mmenu_attr->manuel_set_p[1].manuel_setNUM > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/10%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
		} else {
			//GUI_SetBkColor(0x00000000);
			//GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*2, setNUM_y+125);
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37*2,setNUM_y);
	}

	weightTmp = mmenu_attr->manuel_set_p[1].manuel_setNUM;
}

static void draw_KEYNUM_user_weight(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 458;
	__s32 setNUM_y = 312;
	void *pbmp;

	GUI_SetBkColor(0x00000000);
	GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*3, setNUM_y+125);
	if(mmenu_attr->manuel_set_p[1].manuel_setNUM >= 100)
	{
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37*2,setNUM_y);
	} else {
		//GUI_SetBkColor(0x00000000);
		//GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37, setNUM_y+125);
		if (mmenu_attr->manuel_set_p[1].manuel_setNUM > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/10%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
		} else {
			//GUI_SetBkColor(0x00000000);
			//GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*3, setNUM_y+125);
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37*2,setNUM_y);
	}
}


/*
	画时间
*/

static void draw_user_time(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 685;
	__s32 setNUM_y = 312;
	void *pbmp;

	if(KEYNUM_Flag) {
		if (mmenu_attr->manuel_set_p[2].manuel_setNUM > 0 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
			mmenu_attr->manuel_set_p[2].manuel_setNUM = timeTmp;
		}
	}

	if (mmenu_attr->manuel_set_p[2].manuel_setNUM < 10 && mmenu_attr->manuel_set_p[2].manuel_setNUM > 0) {
		mmenu_attr->manuel_set_p[2].manuel_setNUM = 0;
	}

	if (mmenu_attr->manuel_set_p[2].manuel_setNUM < TIME_MIN) {
		mmenu_attr->manuel_set_p[2].manuel_setNUM = TIME_MAX;
	} else if (mmenu_attr->manuel_set_p[2].manuel_setNUM > TIME_MAX) {
		mmenu_attr->manuel_set_p[2].manuel_setNUM = TIME_MIN;
	}
	
	if (mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37, setNUM_y+125);
	} else  {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[2].manuel_setNUM/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[2].manuel_setNUM%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);

	timeTmp = mmenu_attr->manuel_set_p[2].manuel_setNUM;
}

static void draw_KEYNUM_user_time(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 685;
	__s32 setNUM_y = 312;
	void *pbmp;

	if (mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*2, setNUM_y+125);
	} else  {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[2].manuel_setNUM/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[2].manuel_setNUM%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
}



/*
	画设置的数字
*/

static void draw_setNUM(mmenu_attr_t *mmenu_attr, int num)
{
	if (num == 0)
	{
		draw_user_age(mmenu_attr);
	}
	
	if (num == 1)
	{
		draw_user_weight(mmenu_attr);
	}

	if (num == 2)
	{
		draw_user_time(mmenu_attr);
	} 

	if (num == 3)
	{
		draw_user_age(mmenu_attr);
		draw_user_weight(mmenu_attr);
		draw_user_time(mmenu_attr);
	}

}

static void draw_KEYNUM_setNUM(mmenu_attr_t *mmenu_attr, int num)
{
	if (num == 0)
	{
		draw_KEYNUM_user_age(mmenu_attr);
	}
	
	if (num == 1)
	{
		draw_KEYNUM_user_weight(mmenu_attr);
	}

	if (num == 2)
	{
		draw_KEYNUM_user_time(mmenu_attr);
	} 

}
static void draw_user_set(mmenu_attr_t *mmenu_attr)
{
	__s32 pbmp_x = 216;
	__s32 pbmp_y = 237;
	void *pbmp;
	__s32 i;

	for (i=0; i<3; i++)
	{
		if (mmenu_attr->manuel_set_p[i].manuel_focus)
		{
			pbmp = dsk_theme_hdl2buf(mmenu_attr->focus_bmp[i]);
		} else {
	 		pbmp = dsk_theme_hdl2buf(mmenu_attr->unfocus_bmp[i]);
		}
	
		GUI_BMP_Draw(pbmp,pbmp_x,pbmp_y);

		pbmp_x += 208;
	 }
}

static void draw_message(mmenu_attr_t *mmenu_attr, int num)
{
	__s32 pbmp_x = 0;
	__s32 pbmp_y = 504;
	void *pbmp;

	if (num == 1) {
		PRESET_FLAG = EPDK_TRUE;
		pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_1);
		
	}else if(num == 2){
		PRESET_FLAG = EPDK_FALSE;
		pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_2);
	} else if (num == 3) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->message_user1_bmp);
	} else if (num == 4) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->message_user2_bmp);
	}

	GUI_BMP_Draw(pbmp,pbmp_x,pbmp_y);
}

/*
	user 已保存profile
*/

static void draw_user_in_profile(mmenu_attr_t *mmenu_attr)
{
	__s32 pbmp_x = 126;
	__s32 pbmp_y = 237;
	void *pbmp;
	__u8 i;
	__s32 profile_value;


	pbmp = dsk_theme_hdl2buf(mmenu_attr->curProfile_bmp);
	GUI_BMP_Draw(pbmp,pbmp_x,pbmp_y);

	for (i = 0; i < 18; i++) {
		if (maxLevel == 1) {
			profile_value = 1;
		} else {
			profile_value = DMTONUM(mmenu_attr->level_saved[i] * 8 , maxLevel);
			//profile_value = ProfileCount(mmenu_attr->level_saved[i]);
			
			if (profile_value < 1) {
				profile_value = 1;
			}
		}
		
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_profile_bmp[profile_value]);

		GUI_BMP_Draw(pbmp,166 + 24*i ,321);
	}
	
}

static void draw_user_bt_profile(mmenu_attr_t *mmenu_attr)
{
	__s32 pbmp_x = 126;
	__s32 pbmp_y = 237;
	void *pbmp;
	__u8 i;
	__s32 profile_value;
	reg_root_para_t *root_para;

	root_para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->curProfile_bmp);
	GUI_BMP_Draw(pbmp,pbmp_x,pbmp_y);

	for (i = 0; i < 18; i++) {
		profile_value = root_para->BT_level[i];
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_profile_bmp[profile_value]);

		GUI_BMP_Draw(pbmp,166 + 24*i ,321);
	}
	
}

/*
user 设置level profile
*/
static void draw_user_levelProfile(mmenu_attr_t *mmenu_attr)
{
	__s32 pbmp_x = 430;
	__s32 pbmp_y = 321;
	void *pbmp;
	__u8 i;
	__s32 profile_value;

	if (curSet <= LEVEL_MAX && curSet >= LEVEL_MIN) {
		//profile_value = countNUM(curSet* 8 , 20);
		profile_value = ProfileCount(curSet);
		if (profile_value < 1) {
			profile_value= 1;
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_profile_bmp[profile_value]);

		GUI_BMP_Draw(pbmp,pbmp_x + 24*curSetCount,321);
	}// else if(curSet < LEVEL_MIN) {
//		curSet = LEVEL_MIN;
	//} else if (curSet > LEVEL_MAX) {
//		curSet = LEVEL_MAX;
//	}

	
}

/*
user profile  闪烁
static __bool twinkle
*/
static void draw_user_twinkleProfile(mmenu_attr_t *mmenu_attr)
{
	__s32 pbmp_x = 430;
	__s32 pbmp_y = 321;
	void *pbmp;
		
	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_profile_bmp[0]);

	GUI_BMP_Draw(pbmp,pbmp_x + 24*curSetCount,321);
	
	
}


/*
user 设置level profile
*/
static void draw_user_inclineProfile(mmenu_attr_t *mmenu_attr)
{
	__s32 pbmp_x = 430;
	__s32 pbmp_y = 321;
	void *pbmp;
	__u8 i;
	__s32 profile_value;

#ifdef AC00280
	if (curSet <= INCLINE_MAX && curSet >= INCLINE_MIN) {
		//profile_value = countNUM(curSet* 8 , 20);
		profile_value = ProfileCount(curSet);
		if (profile_value < 1) {
			profile_value = 1;
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_profile_bmp[profile_value]);

		GUI_BMP_Draw(pbmp,pbmp_x + 24*curSetCount,321);
	} //else if(curSet < INCLINE_MIN) {
		//curSet = INCLINE_MIN;
	//} else if (curSet > LEVEL_MAX) {
		//curSet = INCLINE_MAX;
	//}
#else ifdef AC00285
	if (curSet <= 13 && curSet >= 1) {
		profile_value = (curSet+1)/ 2;
		if (profile_value < 1) {
			profile_value = 1;
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_profile_bmp[profile_value]);

		GUI_BMP_Draw(pbmp,pbmp_x + 24*curSetCount,321);
	} else if(curSet < 1) {
		curSet = 1;
	} else if (curSet > 13) {
		curSet = 13;
	}
#endif
	
}



static void draw_userRun_program(mmenu_attr_t *mmenu_attr)
{
	__s32 pbmp_x = 723;
	__s32 pbmp_y = 237;
	void *pbmp;

	if (userConfirm) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->user_run_bmp[0]);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->user_run_bmp[1]);
	}
	GUI_BMP_Draw(pbmp,pbmp_x,pbmp_y);
	
}

static void draw_userDelete_program(mmenu_attr_t *mmenu_attr)
{
	__s32 pbmp_x = 723;
	__s32 pbmp_y = 237;
	void *pbmp;

	if (userConfirm) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->user_delete_bmp[0]);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->user_delete_bmp[1]);
	}
	GUI_BMP_Draw(pbmp,pbmp_x,pbmp_y);
	
}


static void draw_user_name(mmenu_attr_t *mmenu_attr,__bool flag)
{
	__s32 pbmp_x = 135;
	__s32 pbmp_y = 237;
	void *pbmp;
	__u8 i;

	switch(nameCount){
		case 0:
		case 1:
			pbmp_x = 480;
			break;
		case 2:
		case 3:
			pbmp_x = 417;
			break;
		case 4:
		case 5:
			pbmp_x = 354;
			break;
		case 6:
		case 7:
			pbmp_x = 291;
			break;
			
	}
	if (flag) {
		GUI_SetBkColor(0);
		GUI_ClearRect(291, 237, 291+500, pbmp_y+125);
		for (i = 0; i< (nameCount+1); i++) {
			if (name[i] == 27) {
				continue;
			}
			pbmp = dsk_theme_hdl2buf(mmenu_attr->USER_NAME[name[i]-1]);
	
			GUI_BMP_Draw(pbmp,pbmp_x + i*63,pbmp_y);
			
		}

	}else {
		
		if (curNameCount == 22 || curNameCount == 24) {
			GUI_SetBkColor(0);
			GUI_ClearRect(pbmp_x+nameCount*63, pbmp_y, pbmp_x+nameCount*63+62, pbmp_y+125);
		}

		if (curNameCount == 9 ) {
			GUI_SetBkColor(0);
			GUI_ClearRect(pbmp_x+nameCount*63, pbmp_y, pbmp_x+nameCount*63+50, pbmp_y+125);
		}

		pbmp = dsk_theme_hdl2buf(mmenu_attr->USER_NAME[curNameCount-1]);
		
		GUI_BMP_Draw(pbmp,pbmp_x + nameCount*63,pbmp_y);
	
	}
	
}


static void draw_savedName(mmenu_attr_t *mmenu_attr)
{
	__s32 pbmp_x = 135;
	__s32 pbmp_y = 307;
	void *pbmp;
	__u8 i;
	__u8 SaveNameCount = 0;

	for (i=0;i<7;i++) {
		if (name[i] > 0) {
			SaveNameCount ++;
		}
	}
	switch(SaveNameCount){
		case 0:
		case 1:
			pbmp_x = 480;
			break;
		case 2:
		case 3:
			pbmp_x = 417;
			break;
		case 4:
		case 5:
			pbmp_x = 354;
			break;
		case 6:
		case 7:
			pbmp_x = 291;
			break;
			
	}

	for (i = 0; i< SaveNameCount; i++) {
			if (name[i] == 27) {
				continue;
			}
			pbmp = dsk_theme_hdl2buf(mmenu_attr->USER_NAME[name[i]-1]);
	
			GUI_BMP_Draw(pbmp,pbmp_x + i*63,pbmp_y);
			
		}

}

static void draw_normal_set(mmenu_attr_t *mmenu_attr)
{
	GUI_RECT gui_rect;
	reg_root_para_t *para;
	__u8 i;
	
	GUI_SetBkColor(0);
	gui_rect.x0 = 0;
    gui_rect.y0 = 225;  
    gui_rect.x1 = 1024;
    gui_rect.y1 = 600;
    GUI_ClearRectEx(&gui_rect);
	
	para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);
	//focus = 1;
	for (i=0;i<3;i++) {
		if (i== (focus-1)) {
			mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_TRUE;
		} else {
			mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_FALSE;
		}
	}
	/*
	mmenu_attr->manuel_set_p[0].manuel_focus = EPDK_TRUE;
	mmenu_attr->manuel_set_p[1].manuel_focus = EPDK_FALSE;
	mmenu_attr->manuel_set_p[2].manuel_focus = EPDK_FALSE;
	*/
	draw_user_set(mmenu_attr);
	
	draw_setNUM(mmenu_attr,3);
	
	draw_message(mmenu_attr,1);

	curPicture = 1;

	StartFlag = EPDK_TRUE;

	nameCount = 0;
	if (curID == ID_USER1) {
		para->age_1 = mmenu_attr->manuel_set_p[0].manuel_setNUM;
		para->weight_1= mmenu_attr->manuel_set_p[1].manuel_setNUM;
		para->time_1= mmenu_attr->manuel_set_p[2].manuel_setNUM;
		if (isSaveFlag) {
			user1In_flag = EPDK_TRUE;
		}
		for(i=0; i<18; i++) {
		para->level_1[i] = level[i];
		para->incline_1[i] = incline[i];
		}
		para->max_level_1 = LEVEL_MAX;
	} else {
		para->age_2 = mmenu_attr->manuel_set_p[0].manuel_setNUM;
		para->weight_2= mmenu_attr->manuel_set_p[1].manuel_setNUM;
		para->time_2= mmenu_attr->manuel_set_p[2].manuel_setNUM;
		if (isSaveFlag) {
			user2In_flag = EPDK_TRUE;
		}
		for(i=0; i<18; i++) {
		para->level_2[i] = level[i];
		para->incline_2[i] = incline[i];
		}
		para->max_level_2 = LEVEL_MAX;
	}
	
}

/*
	user level设置页面
*/
static void draw_user_level_set(mmenu_attr_t *mmenu_attr)
{
	GUI_RECT gui_rect;
	__s32 pbmp_x = 126;
	__s32 pbmp_y = 237;
	void *pbmp;
	__s32 i;
	__s32 profile_value;

	StartFlag = EPDK_FALSE;
	curSet = level[0];
	curSetCount = 0;
	GUI_SetBkColor(0);
	gui_rect.x0 = 0;
    gui_rect.y0 = 225;  
    gui_rect.x1 = 1024;
    gui_rect.y1 = 600;
    GUI_ClearRectEx(&gui_rect);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->focus_bmp[3]);
	
	GUI_BMP_Draw(pbmp,pbmp_x,pbmp_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->levelProfile_bmp);

	GUI_BMP_Draw(pbmp,392,237);
	
	draw_user_level(mmenu_attr);


	for(i=0; i<18; i++) {
		profile_value = ProfileCount(level[i]);
		if (profile_value < 1) {
			profile_value = 1;
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_profile_bmp[profile_value]);
		GUI_BMP_Draw(pbmp,430 + 24*i,321);//初始化profile
	}
	draw_message(mmenu_attr,1);

	GUI_SetTimer(h_deswin_old , USER_SET , 50, NULL);

	curPicture = 2;
	
}

/*
	user incline 设置页面
*/

static void draw_user_incline_set(mmenu_attr_t *mmenu_attr)
{
	GUI_RECT gui_rect;
	__s32 pbmp_x = 126;
	__s32 pbmp_y = 237;
	void *pbmp;
	__s32 i;
	__s32 profile_value;
	
	GUI_SetBkColor(0);
	gui_rect.x0 = 0;
    gui_rect.y0 = 225;  
    gui_rect.x1 = 1024;
    gui_rect.y1 = 600;
    GUI_ClearRectEx(&gui_rect);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->focus_bmp[4]);
	
	GUI_BMP_Draw(pbmp,pbmp_x,pbmp_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->inclineProfile_bmp);

	GUI_BMP_Draw(pbmp,392,237);
	
	draw_user_incline(mmenu_attr);

	//draw_user_levelProfile(mmenu_attr);

	for(i=0; i<18; i++) {
	#ifdef AC00280
		profile_value = ProfileCount(incline[i]);
	#else ifdef AC00285
		profile_value = (incline[i]+1)/2;
	#endif
		if (profile_value < 1) {
			profile_value = 1;
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_profile_bmp[profile_value]);
		GUI_BMP_Draw(pbmp,430 + 24*i,321);//初始化profile
	}
	draw_message(mmenu_attr,1);
	GUI_SetTimer(h_deswin_old , USER_SET , 50, NULL);
	
	curPicture = 3;
	
}


/*
	显示Manuel界面
*/

static void paint_user_all(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	GUI_RECT gui_rect;
	reg_root_para_t *para;
	RECT	rect;
	__u8 i;
	
	
	if(EPDK_FALSE == mmenu_attr->res_init)
    {
        init_user_res(mmenu_attr);
    }

	GUI_LyrWinSel(mmenu_attr->layer);
	GUI_SetBkColor(0);
    //GUI_SetBkColor(TB_BACKCOLOR);

    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
	gui_rect.x0 = 0;
    gui_rect.y0 = 70;  
    gui_rect.x1 = 1024;
    gui_rect.y1 = 600;
    GUI_ClearRectEx(&gui_rect);
	
	GUI_BMP_Draw(pbmp,0, 70);

	para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);
	
	switch(curID) {
		case ID_USER1:
			if (BTSET_FLAG) {
				draw_user_bt_profile(mmenu_attr);
				draw_message(mmenu_attr,4);
				draw_userRun_program(mmenu_attr);
				curPicture = 4;
				break;
			}
			if (user1In_flag) {
				for(i=0; i<18;i++) {
					mmenu_attr->level_saved[i] = para->level_1[i];
					mmenu_attr->incline_saved[i] = para->incline_1[i];
					if (maxLevel < mmenu_attr->level_saved[i]) {
						maxLevel = mmenu_attr->level_saved[i];
					}
				}
				pbmp = dsk_theme_hdl2buf(mmenu_attr->welcome_bmp);
				GUI_BMP_Draw(pbmp,126, 237);
				for(i=0;i<7;i++) {
					name[i] = para->username_1[i];
				}
				draw_message(mmenu_attr,4);
				draw_savedName(mmenu_attr);
				esKRNL_TimeDly(150);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 237, 1024, 600);
				//maxLevel = para->max_level_1;
				draw_user_in_profile(mmenu_attr);
				draw_message(mmenu_attr,4);
				draw_userRun_program(mmenu_attr);
				curPicture = 4;
				
			} else {
				pbmp = dsk_theme_hdl2buf(mmenu_attr->EnterUsername_bmp);
				GUI_BMP_Draw(pbmp,126, 237);
				draw_message(mmenu_attr,3);
				rect.x = 0;
				rect.y = 70;
				rect.width = 1024;
				rect.height = 530;
				lry = home_32bpp_layer_create(&rect, 2);
				
				if( !lry )
				{
					__err("new layer create error !\n");
				}
				GUI_LyrWinSetSta(lry, GUI_LYRWIN_STA_ON);
				GUI_LyrWinSel(lry);
				GUI_LyrWinSetTop(lry);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				name[0] = 1;
				draw_user_name(mmenu_attr,EPDK_FALSE);
				
				
				curPicture = 0;
			}
			break;
		case ID_USER2:
			if (BTSET_FLAG) {
				draw_user_bt_profile(mmenu_attr);
				draw_message(mmenu_attr,4);
				draw_userRun_program(mmenu_attr);
				curPicture = 4;
				break;
			}
			if (user2In_flag) {
				for (i=0; i<18; i++) {
					mmenu_attr->level_saved[i] = para->level_2[i];
					mmenu_attr->incline_saved[i] = para->incline_2[i];
					if (maxLevel < mmenu_attr->level_saved[i]) {
						maxLevel = mmenu_attr->level_saved[i];
					}
				}
				pbmp = dsk_theme_hdl2buf(mmenu_attr->welcome_bmp);
				GUI_BMP_Draw(pbmp,126, 237);
				for(i=0;i<7;i++) {
					name[i] = para->username_2[i];
				}
				draw_message(mmenu_attr,4);
				draw_savedName(mmenu_attr);
				
				esKRNL_TimeDly(150);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 237, 1024, 600);
				//maxLevel = para->max_level_2;
				draw_user_in_profile(mmenu_attr);
				draw_message(mmenu_attr,4);
				draw_userRun_program(mmenu_attr);
				curPicture = 4;
			} else {
				pbmp = dsk_theme_hdl2buf(mmenu_attr->EnterUsername_bmp);
				GUI_BMP_Draw(pbmp,126, 237);
				draw_message(mmenu_attr,3);
				rect.x = 0;
				rect.y = 70;
				rect.width = 1024;
				rect.height = 530;
				lry = home_32bpp_layer_create(&rect, 2);
				
				if( !lry )
				{
					__err("new layer create error !\n");
				}
				GUI_LyrWinSetSta(lry, GUI_LYRWIN_STA_ON);
				GUI_LyrWinSel(lry);
				GUI_LyrWinSetTop(lry);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				name[0] = 1;
				draw_user_name(mmenu_attr,EPDK_FALSE);
				
				
				curPicture = 0;
			}
			break;
			
	}
	 
	
}


/*
	主界面按键响应
*/
 __s32  userbar_key_proc(__gui_msg_t *msg)
{
	__u32 keycode;
	mmenu_attr_t *mmenu_attr;
	__s32 i;
	
	
	mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(h_deswin_old);
	if (!mmenu_attr)
	{
		__err("invalid para...\n");
		return EPDK_FAIL;
	}

	keycode = msg->dwAddData1;
	if (keycode != 4096) {
		NoKeyCount = 0;
	} else {
		NoKeyCount++;
		if (NoKeyCount > 6000) {
			if (lry != 0) {
				GUI_LyrWinSel(lry);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
			}
			if( GUI_IsTimerInstalled(msg->h_deswin, USER_SET)) {
					GUI_KillTimer(msg->h_deswin, USER_SET);
			}
			GUI_LyrWinSel(mmenu_attr->layer);
			GUI_SetBkColor(0x00000000);
			GUI_ClearRect(0, 0, 1024, 600);
			main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data); 
		}
	} 
	switch(keycode)
	{
		case FAN_KEY:
			fanFlag = !fanFlag;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			//__msg("huang#####  keyTonFlag = %d\n", keyTonFlag);
			SetFanOnOff(mmenu_attr->p_cmd_data,fanFlag);
			break;
		case START_KEY:
		{
			reg_root_para_t *para;
			//if (!StartFlag) {
				//return EPDK_TRUE;
			//}
			isSaveFlag = EPDK_TRUE;
			if(!curPicture) {
				return EPDK_TRUE;
			}
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if( GUI_IsTimerInstalled(msg->h_deswin, USER_SET))	{	
				GUI_KillTimer(msg->h_deswin, USER_SET);
			}
			para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);
			if (BTSET_FLAG) {
				Send_BT_status(4); 
				mmenu_attr->manuel_set_p[0].manuel_setNUM = BT_age;
				mmenu_attr->manuel_set_p[1].manuel_setNUM = BT_weight;
				mmenu_attr->manuel_set_p[2].manuel_setNUM = BT_time;
			} else {
				if (curPicture != 4) {
					if (curID == ID_USER1) {
						user1In_flag = 1;
						for(i=0; i<18; i++) {
							para->incline_1[i] = incline[i];
							para->level_1[i] = level[i];
						}
					} else {
						user2In_flag = 1;
						for(i=0; i<18; i++) {
							para->incline_2[i] = incline[i];
							para->level_2[i] = level[i];
						}
					}
				}
			}
			
			mmenu_attr->curID = curID;

			
			if (mmenu_attr->manuel_set_p[0].manuel_setNUM > 99 || mmenu_attr->manuel_set_p[0].manuel_setNUM < 10) {
				mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_INIT;
			} 

			if (!unitFlag) {
				if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_B || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_B) {
					mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_B;
					
				}
			} else {
				if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_M || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_M) {
					mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_M;
				
				}
			}	

			if (mmenu_attr->manuel_set_p[2].manuel_setNUM > 0 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
				mmenu_attr->manuel_set_p[2].manuel_setNUM = 0;
			}
			if (!unitFlag) {
				mmenu_attr->manuel_set_p[1].manuel_setNUM = mmenu_attr->manuel_set_p[1].manuel_setNUM * 5 / 11;
			}
			
			if (curPicture != 4 && curPicture != 5) {
				if(curID == ID_USER1) {
					para->age_1 = mmenu_attr->manuel_set_p[0].manuel_setNUM;
					para->weight_1 = mmenu_attr->manuel_set_p[1].manuel_setNUM;
					para->time_1 = mmenu_attr->manuel_set_p[2].manuel_setNUM;
				} else {
					para->age_2 = mmenu_attr->manuel_set_p[0].manuel_setNUM;
					para->weight_2 = mmenu_attr->manuel_set_p[1].manuel_setNUM;
					para->time_2 = mmenu_attr->manuel_set_p[2].manuel_setNUM;
				}
			}
			SP_Data[0] = mmenu_attr->manuel_set_p[0].manuel_setNUM;
			SP_Data[1] = mmenu_attr->manuel_set_p[1].manuel_setNUM;
			SP_Data[2] = mmenu_attr->manuel_set_p[2].manuel_setNUM;
			SP_Data[3] = mmenu_attr->manuel_set_p[3].manuel_setNUM;
			SP_Data[4] = mmenu_attr->curID;
			isSaveFlag = EPDK_TRUE;
			if (curID == ID_USER1) {
				user1In_flag = 1;
				writeData[7] = user1In_flag;
			} else {
				user2In_flag = 1;
				writeData[8] = user2In_flag;
			}
			if (!BTSET_FLAG) {
				pageData[0] = 0x60;
				writePage(writeData,pageData);
			}
			main_cmd2manuel(h_deswin_old,ID_SPORTS,0,SP_Data); //倒数
				
			break;
		}
		case STOP_KEY:
			if (BTSET_FLAG) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
				main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data); 
				break;
			}
			numKeyCount = 0;
			switch(curPicture) {
				case 0:
					nameCount++;
					
					if (nameCount > 6) {
						nameCount = 6;
						return EPDK_TRUE;
					}
					open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
					name[nameCount] = 1;
					curNameCount = 1;
					draw_user_name(mmenu_attr,EPDK_TRUE);
					break;
				case 1:
					if (mmenu_attr->manuel_set_p[0].manuel_focus) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
						
						main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data); //
						break;
					} else {
						switch(focus) {
								case 1:
									if (mmenu_attr->manuel_set_p[0].manuel_setNUM > 99 || mmenu_attr->manuel_set_p[0].manuel_setNUM < 10) {
										mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_INIT;
										draw_user_age(mmenu_attr);
										return EPDK_OK;
									} 
									break;
								case 2:
									if (!unitFlag) {
										if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_B || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_B) {
											mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_B;
											draw_user_weight(mmenu_attr);
											return EPDK_OK;
										}
									} else {
										if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_M || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_M) {
											mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_M;
											draw_user_weight(mmenu_attr);
											return EPDK_OK;
										}
									}	
									
									break;
								case 3:
									if (mmenu_attr->manuel_set_p[2].manuel_setNUM > 0 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
										mmenu_attr->manuel_set_p[2].manuel_setNUM = TIME_INIT;
										draw_user_time(mmenu_attr);
										return EPDK_OK;
									}
									break;
							
						}
						focus--;
						stopFocus--;
						open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
						for (i = 0; i < 3; i++)
						{
							if (i != stopFocus) 
							{
								mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_FALSE;
							} else {
								mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_TRUE;
							}	
						}	
						draw_user_set(mmenu_attr);
						draw_setNUM(mmenu_attr,3);
					}
						
					break;
					
				case 2:
				{
					open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
					if (curSet>20 || curSet<1) {
						if (curSetCount == 17) {
							curSet = 1;
						} else {
							curSet = level[curSetCount+1];
						}
						draw_KEYNUM_user_level(mmenu_attr);
						draw_user_levelProfile(mmenu_attr);
						return EPDK_OK;
					}
					if (curSetCount) {
						
						draw_user_levelProfile(mmenu_attr);
						curSetCount--;
						curSet = level[curSetCount];
						draw_user_level(mmenu_attr);
					} else {
						GUI_KillTimer(msg->h_deswin, USER_SET);
						focus = 3;
						draw_normal_set(mmenu_attr);
					}
					break;
				}
				case 3:
				{
					void *pbmp;
					open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
					if (curSet>20 || curSet<1) {
						if (curSetCount == 17) {
							curSet = 1;
						} else {
							curSet = incline[curSetCount+1];
						}
						draw_KEYNUM_user_incline(mmenu_attr);
						draw_user_inclineProfile(mmenu_attr);
						return EPDK_OK;
					}
					if (curSetCount == 18) {
						curSet = incline[0];
						curSetCount = 0;
						
						pbmp = dsk_theme_hdl2buf(mmenu_attr->focus_bmp[4]);
						GUI_BMP_Draw(pbmp,126,237);
						draw_user_incline(mmenu_attr);
						draw_message(mmenu_attr,1);
						StartFlag = EPDK_FALSE;
						GUI_SetTimer(h_deswin_old , USER_SET , 50, NULL);	
						break;
					}
					if (curSetCount) {
						draw_user_inclineProfile(mmenu_attr);
						curSetCount--;
						curSet = incline[curSetCount];
						draw_user_incline(mmenu_attr);
					} else {
						draw_user_level_set(mmenu_attr);
					}
				}
					break;
				case 4:
					
				case 5:
					open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
					main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data); 
					break;
			}

			break;
		
		case ENTER_KEY:
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			numKeyCount = 0;
			switch(curPicture) {
				case 0:
				{
					reg_root_para_t *para;
					
					para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);
					//__msg("huang########   name   = %d  \n",name[0]);
					if (curID == ID_USER1) {
						for(i=0; i<7; i++) {		
							para->username_1[i] = name[i];
						}
					} else {
						for(i=0; i<7; i++) {		
							para->username_2[i] = name[i];
						}
					}	
					GUI_SetBkColor(0x00000000); //删除上面图层
					GUI_ClearRect(0, 0, 1024, 600);
					GUI_LyrWinDelete(lry);
					lry = 0;
					GUI_LyrWinSel(mmenu_attr->layer);
					draw_normal_set(mmenu_attr);
					break;
				}
				case 1:
				{
					reg_root_para_t *para;
					switch(focus) {
						case 1:
							if (mmenu_attr->manuel_set_p[0].manuel_setNUM > 99 || mmenu_attr->manuel_set_p[0].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_INIT;
								draw_user_age(mmenu_attr);
								return EPDK_OK;
							} 
							break;
						case 2:
							if (!unitFlag) {
								if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_B || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_B) {
									mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_B;
									draw_user_weight(mmenu_attr);
									return EPDK_OK;
								}
							} else {
								if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_M || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_M) {
									mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_M;
									draw_user_weight(mmenu_attr);
									return EPDK_OK;
								}
							}	
							
							break;
						case 3:
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM > 0 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 0;
								draw_user_time(mmenu_attr);
								return EPDK_OK;
							}
							break;
					
				}					
					if (focus == 3) { //全部设置完再按ENTER进入level设置
						para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);
						if(curID == ID_USER1) {
							para->age_1 = mmenu_attr->manuel_set_p[0].manuel_setNUM;
							para->weight_1 = mmenu_attr->manuel_set_p[1].manuel_setNUM;
							para->time_1 = mmenu_attr->manuel_set_p[2].manuel_setNUM;
						} else {
							para->age_2 = mmenu_attr->manuel_set_p[0].manuel_setNUM;
							para->weight_2 = mmenu_attr->manuel_set_p[1].manuel_setNUM;
							para->time_2 = mmenu_attr->manuel_set_p[2].manuel_setNUM;
						}
						for(i=0; i<7;i++) {
							__msg("huang#####   name = %d \n",para->username_1[i]);
						}
						KEYNUM_Flag = EPDK_FALSE;
						draw_user_level_set(mmenu_attr);
						focus = 0;
						break;
					}
					for (i = 0; i < 3; i++)
					{
						if (i != focus) 
						{
							mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_FALSE;
						} else {
							mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_TRUE;
						}	
					}	
					draw_user_set(mmenu_attr);
					draw_setNUM(mmenu_attr,3);
				
					focus ++;
					stopFocus = focus-1;
					numKeyCount = 0;
					break;
				}
				case 2:
				{
					reg_root_para_t *para;
					if (curSet>20 || curSet<1) {
						curSet = 1;
						draw_KEYNUM_user_level(mmenu_attr);
						draw_user_levelProfile(mmenu_attr);
						return EPDK_OK;
					}
					if (curSetCount < 18) {
						draw_user_levelProfile(mmenu_attr);
						level[curSetCount] = curSet;
					}
					curSetCount++;
					
					//curSet = level[curSetCount];
					//__msg("huang###   incline[0] = %d \n",incline[0]);
					
					
					if (curSetCount == 18) {
						GUI_KillTimer(msg->h_deswin, USER_SET);
						para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);
						if (curID == ID_USER1) {
							for(i=0; i<18; i++) {
								para->level_1[i] = level[i];
							}
		
						} else {
							for(i=0; i<18; i++) {
								para->level_2[i] = level[i];
							}
			
						}
						curSetCount = 0;
						curSet = incline[0];
					//	__msg("huang    curSet = %d \n",curSet);
						KEYNUM_Flag = EPDK_FALSE;
						draw_user_incline_set(mmenu_attr);
	
						break;
					}
					draw_user_level(mmenu_attr);
					break;
				}
				case 3:
				{
					void *pbmp;
					reg_root_para_t *para;
					__u8 i;
					__u8 profile_value;
					if (curSet>20 || curSet<1) {
						curSet = 1;
						draw_KEYNUM_user_incline(mmenu_attr);
						draw_user_inclineProfile(mmenu_attr);
						return EPDK_OK;
					}
					if (curSetCount < 18) {
						draw_user_inclineProfile(mmenu_attr);
						incline[curSetCount] = curSet;
					}
					curSetCount++;
					
					//curSet = incline[curSetCount];
					
					if (curSetCount == 18) { //incline 设置完18段后，进入STANDBY
						GUI_KillTimer(msg->h_deswin, USER_SET);
						para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);
						if (curID == ID_USER1) {
							user1In_flag = 1;
							for(i=0; i<18; i++) {
								para->incline_1[i] = incline[i];
							}
							writeData[7] = user1In_flag;
						} else {
							user2In_flag = 1;
							for(i=0; i<18; i++) {
								para->incline_2[i] = incline[i];
							}
							writeData[8] = user2In_flag;
						}
						writeData[0] = unitFlag;
						pageData[0] = 0x60;
						writePage(writeData,pageData);
						pbmp = dsk_theme_hdl2buf(mmenu_attr->unfocus_bmp[3]);
						curSet = level[0];
						GUI_BMP_Draw(pbmp,126,237);
						//draw_user_level_set(mmenu_attr);
						pbmp = dsk_theme_hdl2buf(mmenu_attr->levelProfile_bmp);

						GUI_BMP_Draw(pbmp,392,237);
						
						draw_user_level(mmenu_attr);


						for(i=0; i<18; i++) {
							profile_value = ProfileCount(level[i]);
							if (profile_value < 1) {
								profile_value = 1;
							}
							pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_profile_bmp[profile_value]);
							GUI_BMP_Draw(pbmp,430 + 24*i,321);//初始化profile
						}
						draw_message(mmenu_attr,2);
						StartFlag = EPDK_TRUE;
						
						break;
					}

					if (curSetCount == 19) {
						/*
						pbmp = dsk_theme_hdl2buf(mmenu_attr->EnterUsername_bmp);
						GUI_BMP_Draw(pbmp,126, 237);
						draw_savedName(mmenu_attr);
						draw_message(mmenu_attr,3);
						curPicture = 0;
						StartFlag = EPDK_FALSE;
						*/
						isSaveFlag = EPDK_TRUE;
						focus = 1;
						draw_normal_set(mmenu_attr);
						
						break;
					}
				
					draw_user_incline(mmenu_attr);
					break;
				}
				case 4:
					if (userConfirm) {
						mmenu_attr->curID = curID;
						SP_Data[4] = mmenu_attr->curID ;
						main_cmd2manuel(h_deswin_old,ID_SPORTS,0,SP_Data); //倒数
					} else {
						userConfirm = EPDK_TRUE;
						draw_userDelete_program(mmenu_attr);
						curPicture = 5;
					}
					break;
				case 5:
				{
					reg_root_para_t *para;
					__s32 i;
					RECT	rect;
					GUI_RECT gui_rect;
					void *pbmp;
					
					if (userConfirm) {
						//draw_normal_set(mmenu_attr);
						GUI_SetBkColor(0);
						gui_rect.x0 = 0;
					    gui_rect.y0 = 225;  
					    gui_rect.x1 = 1024;
					    gui_rect.y1 = 600;
					    GUI_ClearRectEx(&gui_rect);
						pbmp = dsk_theme_hdl2buf(mmenu_attr->EnterUsername_bmp);
						GUI_BMP_Draw(pbmp,126, 237);
						rect.x = 0;
						rect.y = 70;
						rect.width = 1024;
						rect.height = 530;
						draw_message(mmenu_attr,3);
						lry = home_32bpp_layer_create(&rect, 2);
						
						if( !lry )
						{
							__err("new layer create error !\n");
						}
						GUI_LyrWinSetSta(lry, GUI_LYRWIN_STA_ON);
						GUI_LyrWinSel(lry);
						GUI_LyrWinSetTop(lry);
						GUI_SetBkColor(0x00000000);
						GUI_ClearRect(0, 0, 1024, 600);
						for(i=1;i<7;i++){
							name[i] = 0;
						}
						name[0] = 1;
						
						draw_user_name(mmenu_attr,EPDK_FALSE);
						
						curPicture = 0;
						para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);

						if(curID == ID_USER1) {
							user1In_flag = 0;
							for (i = 0 ; i < 18 ; i++) {
								para->level_1[i] = 1;
								para->incline_1[i] = 1;
							}
							writeData[7] = user1In_flag;
						} else {
							user2In_flag = 0;
							for (i = 0 ; i < 18 ; i++) {
								para->level_2[i] = 1;
								para->incline_2[i] = 1;
							}
							writeData[8] = user2In_flag;
						}
						
						
						writePage(writeData,pageData);
					} else {
						userConfirm = EPDK_TRUE;
						draw_userRun_program(mmenu_attr);
						curPicture = 4;
					}
					break;
				}	
			}
			
			
			break;
		case DOWN_LONG_KEY:
		case DOWN_QUICK_LONG_KEY:
		case INCDEC_LONG_KEY:
		case INCDEC_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case DOWN_KEY:
		case INCDEC_KEY:
			if (curSetCount == 18) {
				break;
			}
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
		
			numKeyCount = 0;
			switch(curPicture) {
				case 0:
					curNameCount--;
					if (curNameCount < 1) {
						curNameCount = 27;
					}
					name[nameCount] = curNameCount;
					draw_user_name(mmenu_attr,EPDK_FALSE);
					break;
				case 1:
				{
					__u8 i;

					if (!PRESET_FLAG) {
						return EPDK_TRUE;
					}
					KEYNUM_Flag = EPDK_FALSE;
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus == EPDK_TRUE )
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM--;
							
							
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM < 10 && mmenu_attr->manuel_set_p[2].manuel_setNUM > 0) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 0;
							}

							draw_setNUM(mmenu_attr,i);
						}
					}
					
					break;
				}
				case 2:
				{
					curSet--;
					draw_user_level(mmenu_attr);
					draw_user_levelProfile( mmenu_attr);
					level[curSetCount] = curSet;
					break;
				}	
				case 3:
				{
					if (curSetCount > 17) {
						break;
					}
					curSet--;
					draw_user_incline(mmenu_attr);
					draw_user_inclineProfile( mmenu_attr);
					incline[curSetCount] = curSet;
					break;
				}	
				case 4:
					userConfirm = !userConfirm;
					draw_userRun_program(mmenu_attr);
					break;
				case 5:
					userConfirm = !userConfirm;
					draw_userDelete_program(mmenu_attr);
					break;
			}
			
			break;
		
		case UP_LONG_KEY:
		case UP_QUICK_LONG_KEY:
		case INCPLUSE_LONG_KEY:
		case INCPLUSE_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case UP_KEY:
		case INCPLUSE_KEY:
			if (curSetCount == 18) {
				break;
			}
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
			numKeyCount = 0;
			switch(curPicture) {
				case 0:
					curNameCount++;
					if (curNameCount > 27) {
						curNameCount = 1;
					}
					name[nameCount] = curNameCount;
					draw_user_name(mmenu_attr,EPDK_FALSE);
					break;
				case 1:
					if (!PRESET_FLAG) {
						return EPDK_TRUE;
					}
					KEYNUM_Flag = EPDK_FALSE;
					for (i = 0; i < 4; i++)
					{
					
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM++;
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							draw_setNUM(mmenu_attr,i);
						}
						
					
					}
					break;
				case 2:
				{		
					curSet++;
					draw_user_level(mmenu_attr);
					draw_user_levelProfile( mmenu_attr);
					level[curSetCount] = curSet;
					break;
				}
				case 3:
					if (curSetCount > 17) {
						break;
					}
					curSet++;
					draw_user_incline(mmenu_attr);
					draw_user_inclineProfile( mmenu_attr);
					incline[curSetCount] = curSet;
					break;
				case 4:
					userConfirm = !userConfirm;
					draw_userRun_program(mmenu_attr);
					break;
				case 5:
					userConfirm = !userConfirm;
					draw_userDelete_program(mmenu_attr);
					break;
			}
			

			break;

		case FAST_LONG_KEY:
		case FAST_LONG_QUICK_KEY:
		case LEVELPLUSE_LONG_KEY:
		case LEVELPLUSE_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case FAST_KEY:
		case LEVELPLUSE_KEY:
			if (curSetCount == 18) {
				break;
			}
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
			numKeyCount = 0;
			switch(curPicture) {
				case 0:
					curNameCount++;
					if (curNameCount > 27) {
						curNameCount = 1;
					}
					name[nameCount] = curNameCount;
					draw_user_name(mmenu_attr,EPDK_FALSE);
					break;
				case 1:
					if (!PRESET_FLAG) {
						return EPDK_TRUE;
					}
					KEYNUM_Flag = EPDK_FALSE;
					for (i = 0; i < 4; i++)
					{
					
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM++;
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							draw_setNUM(mmenu_attr,i);
						}
						
					
					}
					break;
				case 2:
					curSet++;
					draw_user_level(mmenu_attr);
					draw_user_levelProfile( mmenu_attr);
					level[curSetCount] = curSet;
					break;
				case 3:
					if (curSetCount > 17) {
						break;
					}
					curSet++;
					draw_user_incline(mmenu_attr);
					draw_user_inclineProfile( mmenu_attr);
					incline[curSetCount] = curSet;
					break;
				case 4:
					userConfirm = !userConfirm;
					draw_userRun_program(mmenu_attr);
					break;
				case 5:
					userConfirm = !userConfirm;
					draw_userDelete_program(mmenu_attr);
					break;
			}

			break;

		
		case SLOW_LONG_KEY:
		case SLOW_LONG_QUICK_KEY:
		case LEVELDEC_LONG_KEY:
		case LEVELDEC_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case SLOW_KEY:
		case LEVELDEC_KEY:
			if (curSetCount == 18) {
				break;
			}
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
			numKeyCount = 0;
			switch(curPicture) {
				case 0:
					curNameCount--;
					if (curNameCount < 1) {
						curNameCount = 27;
					}
					name[nameCount] = curNameCount;
					draw_user_name(mmenu_attr,EPDK_FALSE);
					break;
				case 1:
				{
					__u8 i;
			
					if (!PRESET_FLAG) {
						return EPDK_TRUE;
					}
					KEYNUM_Flag = EPDK_FALSE;
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus == EPDK_TRUE )
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM--;
							
							
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM < 10 && mmenu_attr->manuel_set_p[2].manuel_setNUM > 0) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 0;
							}
							draw_setNUM(mmenu_attr,i);
						}
					}
					break;
				}
				case 2:
				{
					curSet--;
					draw_user_level(mmenu_attr);
					draw_user_levelProfile( mmenu_attr);
					level[curSetCount] = curSet;
					break;
				}
				case 3:
				{
					if (curSetCount > 17) {
						break;
					}
					curSet--;
					draw_user_incline(mmenu_attr);
					draw_user_inclineProfile( mmenu_attr);
					incline[curSetCount] = curSet;
					break;
				}	
				case 4:
					userConfirm = !userConfirm;
					draw_userRun_program(mmenu_attr);
					break;
				case 5:
					userConfirm = !userConfirm;
					draw_userDelete_program(mmenu_attr);
					break;
			}
			
			break;

		case MANUAL_KEY:
			if (!PRESET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 1;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+1;
				#ifdef AC00286
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
				#else
				if (!mmenu_attr->manuel_set_p[1].manuel_focus && curPicture != 3) {
					numKeyCount = 0;
				}
				#endif
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+1;
				numKeyCount = 0;
			}

			switch(curPicture) {
				case 0:
					break;
				case 1:
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
							/*
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							*/
							draw_KEYNUM_setNUM(mmenu_attr,i);
						}	
					}
					break;
				case 2:
					curSet = numTmp;
					draw_KEYNUM_user_level(mmenu_attr);
					//draw_user_levelProfile(mmenu_attr);
					level[curSetCount] = curSet;
					break;
				case 3:
					#ifdef AC00286
					curSet = numTmp;
					#else
					curSet = NUM2Stride(numTmp);
					#endif
					draw_KEYNUM_user_incline(mmenu_attr);
					//draw_user_inclineProfile(mmenu_attr);
					incline[curSetCount] = curSet;
					break;
			}	
			break;
		case HILL_KEY:
			if (!PRESET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);

			if (numKeyCount == 1) {
				numTmp = 2;
			} else if (numKeyCount == 2) {
			
				numTmp = numTmp*10+2;
				
				
				#ifdef AC00286
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
				#else
				if (!mmenu_attr->manuel_set_p[1].manuel_focus && curPicture != 3) {
					numKeyCount = 0;
				}
				#endif
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+2;
				numKeyCount = 0;
			}

			switch(curPicture) {
				case 0:
					break;
				case 1:
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
							/*
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							*/
							draw_KEYNUM_setNUM(mmenu_attr,i);
						}	
					}
					break;
				case 2:
					curSet = numTmp;
					draw_KEYNUM_user_level(mmenu_attr);
					//draw_user_levelProfile(mmenu_attr);
					level[curSetCount] = curSet;
					break;
				case 3:
					#ifdef AC00286
					curSet = numTmp;
					#else
					curSet = NUM2Stride(numTmp);
					#endif
					draw_KEYNUM_user_incline(mmenu_attr);
					//draw_user_inclineProfile(mmenu_attr);
					incline[curSetCount] = curSet;
					break;
			}
			break;
		case FATBURN_KEY:
			if (!PRESET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			numKeyCount ++;
			
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 3;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+3;
				#ifdef AC00286
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
				#else
				if (!mmenu_attr->manuel_set_p[1].manuel_focus && curPicture != 3) {
					numKeyCount = 0;
				}
				#endif
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+3;
				numKeyCount = 0;
			}

			switch(curPicture) {
				case 0:
					break;
				case 1:
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
							/*
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							*/
							draw_KEYNUM_setNUM(mmenu_attr,i);
						}	
					}
					break;
				case 2:
					curSet = numTmp;
					draw_KEYNUM_user_level(mmenu_attr);
					//draw_user_levelProfile(mmenu_attr);
					level[curSetCount] = curSet;
					break;
				case 3:
					#ifdef AC00286
					curSet = numTmp;
					#else
					curSet = NUM2Stride(numTmp);
					#endif
					draw_KEYNUM_user_incline(mmenu_attr);
					//draw_user_inclineProfile(mmenu_attr);
					incline[curSetCount] = curSet;
					break;
			}
			break;
		case CARDIO_KEY:
			if (!PRESET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);

			if (numKeyCount == 1) {
				numTmp = 4;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+4;
				#ifdef AC00286
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
				#else
				if (!mmenu_attr->manuel_set_p[1].manuel_focus && curPicture != 3) {
					numKeyCount = 0;
				}
				#endif
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+4;
				numKeyCount = 0;
			}

			switch(curPicture) {
				case 0:
					break;
				case 1:
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
							/*
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							*/
							draw_KEYNUM_setNUM(mmenu_attr,i);
						}	
					}
					break;
				case 2:
					curSet = numTmp;
					draw_KEYNUM_user_level(mmenu_attr);
					//draw_user_levelProfile(mmenu_attr);
					level[curSetCount] = curSet;
					break;
				case 3:
					#ifdef AC00286
					curSet = numTmp;
					#else
					curSet = NUM2Stride(numTmp);
					#endif
					draw_KEYNUM_user_incline(mmenu_attr);
				//	draw_user_inclineProfile(mmenu_attr);
					incline[curSetCount] = curSet;
					break;
			}
			break;
		case STRENGTH_KEY:
			if (!PRESET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);

			if (numKeyCount == 1) {
				numTmp = 5;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+5;
				#ifdef AC00286
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
				#else
				if (!mmenu_attr->manuel_set_p[1].manuel_focus && curPicture != 3) {
					numKeyCount = 0;
				}
				#endif
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+5;
				numKeyCount = 0;
			}

			switch(curPicture) {
				case 0:
					break;
				case 1:
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
							/*
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							*/
							draw_KEYNUM_setNUM(mmenu_attr,i);
						}	
					}
					break;
				case 2:
					curSet = numTmp;
					draw_KEYNUM_user_level(mmenu_attr);
				//	draw_user_levelProfile(mmenu_attr);
					level[curSetCount] = curSet;
					break;
				case 3:
					#ifdef AC00286
					curSet = numTmp;
					#else
					curSet = NUM2Stride(numTmp);
					#endif
					draw_KEYNUM_user_incline(mmenu_attr);
					//draw_user_inclineProfile(mmenu_attr);
					incline[curSetCount] = curSet;
					break;
			}
			break;
		case INTERUAL_KEY:
			if (!PRESET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);

			if (numKeyCount == 1) {
				numTmp = 6;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+6;
				#ifdef AC00286
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
				#else
				if (!mmenu_attr->manuel_set_p[1].manuel_focus && curPicture != 3) {
					numKeyCount = 0;
				}
				#endif
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+6;
				numKeyCount = 0;
			}

			switch(curPicture) {
				case 0:
					break;
				case 1:
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
							/*
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							*/
							draw_KEYNUM_setNUM(mmenu_attr,i);
						}	
					}
					break;
				case 2:
					curSet = numTmp;
					draw_KEYNUM_user_level(mmenu_attr);
					//draw_user_levelProfile(mmenu_attr);
					level[curSetCount] = curSet;
					break;
				case 3:
					#ifdef AC00286
					curSet = numTmp;
					#else
					curSet = NUM2Stride(numTmp);
					#endif
					draw_KEYNUM_user_incline(mmenu_attr);
					//draw_user_inclineProfile(mmenu_attr);
					incline[curSetCount] = curSet;
					break;
			}
			break;
		case USER1_KEY:
			if (!PRESET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);

			if (numKeyCount == 1) {
				numTmp = 7;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+7;
				#ifdef AC00286
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
				#else
				if (!mmenu_attr->manuel_set_p[1].manuel_focus && curPicture != 3) {
					numKeyCount = 0;
				}
				#endif
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+7;
				numKeyCount = 0;
			}

			switch(curPicture) {
				case 0:
					break;
				case 1:
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
							/*
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							*/
							draw_KEYNUM_setNUM(mmenu_attr,i);
						}	
					}
					break;
				case 2:
					curSet = numTmp;
					draw_KEYNUM_user_level(mmenu_attr);
					//draw_user_levelProfile(mmenu_attr);
					level[curSetCount] = curSet;
					break;
				case 3:
					#ifdef AC00286
					curSet = numTmp;
					#else
					curSet = NUM2Stride(numTmp);
					#endif
					draw_KEYNUM_user_incline(mmenu_attr);
				//	draw_user_inclineProfile(mmenu_attr);
					incline[curSetCount] = curSet;
					break;
			}
			break;
			
		case USER2_KEY:
			if (!PRESET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);

			if (numKeyCount == 1) {
				numTmp = 8;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+8;
				#ifdef AC00286
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
				#else
				if (!mmenu_attr->manuel_set_p[1].manuel_focus && curPicture != 3) {
					numKeyCount = 0;
				}
				#endif
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+8;
				numKeyCount = 0;
			}

			switch(curPicture) {
				case 0:
					break;
				case 1:
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
							/*
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							*/
							draw_KEYNUM_setNUM(mmenu_attr,i);
						}	
					}
					break;
				case 2:
					curSet = numTmp;
					draw_KEYNUM_user_level(mmenu_attr);
				//	draw_user_levelProfile(mmenu_attr);
					level[curSetCount] = curSet;
					break;
				case 3:
					#ifdef AC00286
					curSet = numTmp;
					#else
					curSet = NUM2Stride(numTmp);
					#endif
					draw_KEYNUM_user_incline(mmenu_attr);
					//draw_user_inclineProfile(mmenu_attr);
					incline[curSetCount] = curSet;
					break;
			}
			break;
		case HR1_KEY:
			if (!PRESET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);

			if (numKeyCount == 1) {
				numTmp = 9;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+9;
				#ifdef AC00286
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
				#else
				if (!mmenu_attr->manuel_set_p[1].manuel_focus && curPicture != 3) {
					numKeyCount = 0;
				}
				#endif
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+9;
				numKeyCount = 0;
			}

			switch(curPicture) {
				case 0:
					break;
				case 1:
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
							/*
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							*/
							draw_KEYNUM_setNUM(mmenu_attr,i);
						}	
					}
					break;
				case 2:
					curSet = numTmp;
					draw_KEYNUM_user_level(mmenu_attr);
					//draw_user_levelProfile(mmenu_attr);
					level[curSetCount] = curSet;
					break;
				case 3:
					#ifdef AC00286
					curSet = numTmp;
					#else
					curSet = NUM2Stride(numTmp);
					#endif
					draw_KEYNUM_user_incline(mmenu_attr);
					//draw_user_inclineProfile(mmenu_attr);
					incline[curSetCount] = curSet;
					break;
			}
			break;
		case HR2_KEY:
			if (!PRESET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);

			if (numKeyCount == 1) {
				numTmp = 0;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10;
				#ifdef AC00286
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
				#else
				if (!mmenu_attr->manuel_set_p[1].manuel_focus && curPicture != 3) {
					numKeyCount = 0;
				}
				#endif
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+0;
				numKeyCount = 0;
			}

			switch(curPicture) {
				case 0:
					break;
				case 1:
					for (i = 0; i < 3; i++)
					{
						if (mmenu_attr->manuel_set_p[i].manuel_focus)
						{
							mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
							/*
							if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
								mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
							}
							*/
							draw_KEYNUM_setNUM(mmenu_attr,i);
						}	
					}
					break;
				case 2:
					if (numKeyCount == 1) {
						numKeyCount = 0;
					}
					curSet = numTmp;
					draw_KEYNUM_user_level(mmenu_attr);
					//draw_user_levelProfile(mmenu_attr);
					level[curSetCount] = curSet;
					break;
				case 3:
					if (numKeyCount == 1) {
						numKeyCount = 0;
					}
					#ifdef AC00286
					curSet = numTmp;
					#else
					curSet = NUM2Stride(numTmp);
					#endif
					
					draw_KEYNUM_user_incline(mmenu_attr);
					//draw_user_inclineProfile(mmenu_attr);
					incline[curSetCount] = curSet;
					break;
			}
			break;
		case BT_DISCONNECT:
			main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data); 
			break;

	}


	return EPDK_OK;
}
	
/*
	
*/
static __s32 _user_bar_Proc(__gui_msg_t *msg)
{
  
	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
			return EPDK_OK;
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
			init_manuel_res(mmenu_attr);
			
			mmenu_attr->font = mmenu_para->mmenu_font;
			mmenu_attr->layer = mmenu_para->layer;
			mmenu_attr->item_w = home_ui_para->item_width;
			mmenu_attr->focus_item = mmenu_para->focus_id;
			mmenu_attr->first_item = 0;
			mmenu_attr->focus_txt_color = MAIN_32_YELLOW;//APP_COLOR_YELLOW;
			mmenu_attr->unfocus_txt_color =   MAIN_32_WHITE;//APP_COLOR_BLACK; //cai modify//APP_COLOR_WHITE;
            mmenu_attr->res_init = EPDK_FALSE;

			
			GUI_WinSetAddData(msg->h_deswin, (__s32)mmenu_attr);
            
		}
		return EPDK_OK;
		#endif
		
		
		}
	case GUI_MSG_CLOSE:
		{
			
			GUI_FrmWinDelete(msg->h_deswin);
		}
		return EPDK_OK;
	case GUI_MSG_DESTROY:
		{
			mmenu_para_t *mmenu_para;
			mmenu_attr_t *mmenu_attr;
	
			
			mmenu_para = (mmenu_para_t *)GUI_WinGetAttr(msg->h_deswin);
			mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			uninit_user_res(mmenu_attr);

			__msg("huang####  user GUI_MSG_DESTROY \n");
		
			esMEMS_Bfree(mmenu_attr, sizeof(mmenu_attr_t));
			esMEMS_Bfree(mmenu_para, sizeof(mmenu_para_t));//esMEMS_Bfree(mmenu_para, sizeof(mmenu_para));//内存释放问题

			if (isSaveFlag) {
				dsk_reg_flush();
			}
		}
		return EPDK_OK;
	case GUI_MSG_PAINT:
        {
			mmenu_attr_t *mmenu_attr;
			mmenu_para_t *mmenu_para;
			__s32 i;

			mmenu_para = (mmenu_para_t *)GUI_WinGetAttr(msg->h_deswin);
			
			mmenu_attr = (mmenu_attr_t *)esMEMS_Balloc(sizeof(mmenu_attr_t));
            if(!mmenu_attr)
            {
                return EPDK_FAIL;                
            }
			eLIBs_memset((void *)mmenu_attr, 0, sizeof(mmenu_attr));
			//init_manuel_res(mmenu_attr);
			StartFlag = EPDK_FALSE;
			MormalFlag = EPDK_FALSE;
			focus = 1;
			stopFocus = 0;
			nameCount = 0;
			curNameCount = 1;
			curSetCount = 0;
			isSaveFlag = EPDK_FALSE;
			KEYNUM_Flag = EPDK_FALSE;
			maxLevel = 0;
			NoKeyCount = 0;
			userConfirm = EPDK_TRUE;
			mmenu_attr->font = mmenu_para->mmenu_font;
			mmenu_attr->layer = mmenu_para->layer;

			mmenu_attr->focus_item = mmenu_para->focus_id;
			mmenu_attr->first_item = 0;
            mmenu_attr->res_init = EPDK_FALSE;
			mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_INIT;
			if (!unitFlag) {
				mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_B;
			} else {
				mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_M;
			}
			//mmenu_attr->manuel_set_p[2].manuel_setNUM = TIME_INIT;
			mmenu_attr->manuel_set_p[2].manuel_setNUM = 0;
			mmenu_attr->manuel_set_p[0].manuel_focus = EPDK_TRUE;
			for(i = 1 ; i < 4; i++)
			{
				mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_FALSE;
			}

			
			GUI_WinSetAddData(msg->h_deswin, (__s32)mmenu_attr);

            if (GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(GUI_WinGetLyrWin(msg->h_deswin)))
            {
                __msg("layer not on, not need paint...\n");
    			return EPDK_OK;
            }

			h_deswin_old = msg->h_deswin;
            mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);

            if (!mmenu_attr)
    		{
    			__err("invalid para...\n");
    			return EPDK_FAIL;
    		}
		
			__here__;
			for (i=0 ; i<18; i++) {
				level[i] = 1;
				incline[i] = 1;
			}
			for (i = 0; i < 7; i++) {
				name[i] = 0;
			}
            init_user_res(mmenu_attr);
    		
    		//paint_mmain_item_all(mmenu_attr);
    		paint_user_all(mmenu_attr);
    		return EPDK_OK;
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
		__u8 i;

		mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);
		if (!mmenu_attr)
		{
			__err("mmenu_attr is null...\n");
            return EPDK_OK;
		}

		if (msg->dwAddData1 == USER_SET) {
			twinkleFlag = !twinkleFlag;
			if (curPicture == 2) {	
				if(twinkleFlag) {
					draw_user_levelProfile(mmenu_attr);
				} else {
					draw_user_twinkleProfile(mmenu_attr);
				}
			} else if (curPicture == 3) {
				if(twinkleFlag) {
					draw_user_inclineProfile(mmenu_attr);
				} else {
					draw_user_twinkleProfile(mmenu_attr);
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

__s32 user_bar_win_create(H_WIN h_parent, mmenu_para_t *para, __u8 id)
{
	__gui_framewincreate_para_t framewin_para;
	mmenu_para_t *mmenu_para;
	FB fb;
	__u8 i;
	reg_root_para_t *root_para;
	
	GUI_LyrWinGetFB(para->layer, &fb);
    __here__;

	 curID = id;
	mmenu_para = (mmenu_para_t *)esMEMS_Balloc(sizeof(mmenu_para_t));
    if(!mmenu_para)
    {
    __here__;
        return EPDK_FAIL;
    }
	eLIBs_memset((void*)mmenu_para, 0, sizeof(mmenu_para_t));
	mmenu_para->layer = para->layer;
	mmenu_para->focus_id = para->focus_id;

	BTSET_FLAG = para->BTSET_Flag;
	if (BTSET_FLAG) {
		BT_weight = para->weight;
		BT_time = para->time;
		BT_age = para->age;
		BT_level = para->dot;
		root_para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);
		for(i=0;i<18;i++) {
			__msg("huang##########  level[%d] = %d \n",i,BT_level[i]);
			root_para->BT_level[i] = BT_level[i]+1;			
		}
	}
	
	pageData[0] = 0x60;
	readPage(writeData,pageData);
	unitFlag = writeData[0];
	keyTonFlag = writeData[3];
	user1In_flag = writeData[7];
	user2In_flag = writeData[8];
	

	
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	"user_bar_win",
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = USER_BAR_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_user_bar_Proc);
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

void user_bar_win_delete(H_WIN mm_win)
{
	GUI_FrmWinDelete(mm_win);
}

void user_bar_res_uninit(H_WIN hwin)
{    
    mmenu_attr_t * mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(hwin);

    if(mmenu_attr)
    {
        uninit_user_res(mmenu_attr);
    }
}

void userbar_res_init(H_WIN hwin)
{
    mmenu_attr_t * mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(hwin);

    if(mmenu_attr)
    {
        init_user_res(mmenu_attr);
    }
}

