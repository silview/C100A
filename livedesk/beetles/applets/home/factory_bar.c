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
#include "factory_bar.h"


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

#define POP_INIT_ID   0x1006
#define FAC3_TEST_ID   0x1007
#define COUNTER_ID		0x1111

static __u16 loopCount = 1;

static H_LYR lyr; //半透明图层
static H_LYR threeLyr;

static __u8 ProgramRDData[32]={0};
static __u8 ProgramWRData[32]={0};
static __u8 TotalData[32] = {0};
static __u8 pageData[2];

static H_WIN h_deswin_old;

static __u8 curFactory = 0;

static __u8 focus = 0;

static __bool EMFlag = EPDK_FALSE;

static __bool yesNoFlag = EPDK_TRUE;

static __bool judeFlag = EPDK_FALSE;

static __u8 maxINC = 0;

static __u8 errCode = 0;

static __bool ERRFlag = EPDK_FALSE;

static __u32 TotalTime = 0;
static __u32 TotalDist = 0;

static __u8  factory2Count = 0 ; //factory II 按键计数

static __s8 fac3Count = 0; //factory III按键计数  默认焦点在key test
static __s8 fac3Old = 0;   

static __bool disModeFlag = EPDK_FALSE;  //false 为 ON  true为 OFF

static __bool pauModeFlag = EPDK_FALSE;	//false 为 ON  true为 OFF

static __bool keyToneFlag = EPDK_FALSE;	//false 为 ON  true为 OFF

static __bool ChildLockFlag = EPDK_FALSE;

static __bool enterFlag = EPDK_FALSE;

static __bool keyTestFlag = EPDK_FALSE;

static __bool motorTestFlag = EPDK_FALSE;

static __bool manualTestFlag = EPDK_FALSE;

static __bool facExitFlag = EPDK_TRUE;

static __bool backHomeJudeFlag = EPDK_TRUE;

static __u8 keyTestValue = 0;

static __u8 keyTestCount[19] = {0} ;

static __u8 manTestLevel = 0;

static __u16 SP_Data[5];



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

static __s32 FactoryBlueNum_ID[] = {
	ID_HOME_TV_FACTORY_0_2_BMP,
	ID_HOME_TV_FACTORY_1_2_BMP,
	ID_HOME_TV_FACTORY_2_2_BMP,
	ID_HOME_TV_FACTORY_3_2_BMP,
	ID_HOME_TV_FACTORY_4_2_BMP,
	ID_HOME_TV_FACTORY_5_0_BMP,
	ID_HOME_TV_FACTORY_6_2_BMP,
	ID_HOME_TV_FACTORY_7_2_BMP,
	ID_HOME_TV_FACTORY_8_2_BMP,
	ID_HOME_TV_FACTORY_9_2_BMP,
};

static __s32 FacCounterNum_ID[] = {
	ID_HOME_TV_POP_FACTORY_TEST_0_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_1_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_2_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_3_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_4_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_5_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_6_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_7_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_8_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_9_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_COUNTER_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L_BMP,	
};

static __s32 FactoryInit_ID[] = {
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

static __s32 factory_test_ID[]= {
	ID_HOME_TV_POP_FACTORY_TEST_L01_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L02_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L03_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L04_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L05_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L06_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L07_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L08_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L09_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L10_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L11_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L12_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L13_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L14_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L15_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L16_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L17_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L18_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L19_BMP,
	ID_HOME_TV_POP_FACTORY_TEST_L20_BMP,
};

static mm_res_id_t modeTest_ID[] = {
	{ID_HOME_BTN_FACTORY_KEY_TEST_1_BMP,ID_HOME_BTN_FACTORY_KEY_TEST_2_BMP},
	{ID_HOME_BTN_FACTORY_MOTOR_TEST_1_BMP,ID_HOME_BTN_FACTORY_MOTOR_TEST_2_BMP},
	{ID_HOME_BTN_FACTORY_MANUAL_TEST_1_BMP,ID_HOME_BTN_FACTORY_MANUAL_TEST_2_BMP},
	{ID_HOME_BTN_FACTORY_DISPLAY_MODE_1_BMP,ID_HOME_BTN_FACTORY_DISPLAY_MODE_2_BMP},
	{ID_HOME_BTN_FACTORY_PAUSE_MODE_1_BMP,ID_HOME_BTN_FACTORY_PAUSE_MODE_2_BMP},
	{ID_HOME_BTN_FACTORY_KEY_TONE_1_BMP,ID_HOME_BTN_FACTORY_KEY_TONE_2_BMP},
	{ID_HOME_BTN_FACTORY_CHILD_LOCK_1_BMP,ID_HOME_BTN_FACTORY_CHILD_LOCK_2_BMP},
};



/*
	申请资源
*/
static __s32 init_factory_res(mmenu_attr_t *mmenu_attr)
{
    __u32 	i;    
   // home_uipara_t* home_ui_para;
    mm_res_id_t* main_ui_res;

    if(EPDK_TRUE == mmenu_attr->res_init)
    {
        return EPDK_OK;
    }

	for(i = 0 ; i < 7; i++)
	{
		mmenu_attr->factoryModeTest_focus[i] = dsk_theme_open(modeTest_ID[i].focus_bmp_id);
		mmenu_attr->factoryModeTest_unfocus[i] = dsk_theme_open(modeTest_ID[i].unfocus_bmp_id);
	}

	for(i = 0 ; i < 13; i++)
	{
		mmenu_attr->factoryNum_bmp[i] = dsk_theme_open(FactoryNUM_ID[i]);
	}

	for(i = 0 ; i < 20; i++)
	{
		mmenu_attr->factoryTest_bmp[i] = dsk_theme_open(factory_test_ID[i]);
	}

	for(i = 0 ; i < 25; i++)
	{
		mmenu_attr->factoryInitPop_bmp[i] = dsk_theme_open(FactoryInit_ID[i]);
	}
	
	mmenu_attr->factoryCal_bmp = dsk_theme_open(ID_HOME_TV_FACTORY_CALIBRATION_BMP);
	mmenu_attr->factoryUnit[0] = dsk_theme_open(ID_HOME_BTN_FACTORY_UNIT_ENGLISH_BMP);
	mmenu_attr->factoryUnit[1] = dsk_theme_open(ID_HOME_BTN_FACTORY_UNIT_METRIC_BMP);
	mmenu_attr->factoryUnit_bmp[0] =  dsk_theme_open(ID_HOME_BTN_FACTORY_UNIT_1_BMP);
	mmenu_attr->factoryUnit_bmp[1] =  dsk_theme_open(ID_HOME_BTN_FACTORY_UNIT_2_BMP);
	#ifdef AC00280
	mmenu_attr->factoryMaxInc[0] = dsk_theme_open(ID_HOME_BTN_FACTORY_MAX_INCLINE_1_BMP);
	mmenu_attr->factoryMaxinc_bmp = dsk_theme_open(ID_HOME_TV_FACTORY_MAX_INCLINE_BMP);
	mmenu_attr->factoryMininc_bmp = dsk_theme_open(ID_HOME_TV_FACTORY_MIN_INCLINE_BMP);
	#else
	mmenu_attr->factoryMaxInc[0] = dsk_theme_open(ID_HOME_BTN_FACTORY_MAX_STRIDE_1_BMP);
	mmenu_attr->factoryMaxinc_bmp = dsk_theme_open(ID_HOME_TV_FACTORY_MAX_STRIDE_BMP);
	mmenu_attr->factoryMininc_bmp = dsk_theme_open(ID_HOME_TV_FACTORY_MIN_STRIDE_BMP);
	#endif
	mmenu_attr->factoryMaxInc[1] = dsk_theme_open(ID_HOME_BTN_FACTORY_MAX_INCLINE_2_BMP);
	
	mmenu_attr->factoryLine_bmp = dsk_theme_open(ID_HOME_IMG_FACTORY_LINE_BMP);
	mmenu_attr->factory_ERR_bmp = dsk_theme_open(ID_HOME_IMG_POP_ERROR_3_BMP);
	mmenu_attr->factoryTotalDistance_bmp = dsk_theme_open(ID_HOME_TV_FACTORY_TOTAL_DISTANCE_BMP);
	mmenu_attr->factoryTotalTime_bmp= dsk_theme_open(ID_HOME_TV_FACTORY_TOTAL_TIME_BMP);
	mmenu_attr->factorySoftVer_bmp= dsk_theme_open(ID_HOME_TV_FACTORY_SOFTWARE_VER_BMP);
	mmenu_attr->factoryJudg_bmp[0] = dsk_theme_open(ID_HOME_BTN_POP_FACTORY_YES_BMP);
	mmenu_attr->factoryJudg_bmp[1] = dsk_theme_open(ID_HOME_BTN_POP_FACTORY_NO_BMP);
	mmenu_attr->factoryOnOff_bmp[0] = dsk_theme_open(ID_HOME_BTN_FACTORY_OFF_BMP);
	mmenu_attr->factoryOnOff_bmp[1] = dsk_theme_open(ID_HOME_BTN_FACTORY_ON_BMP);
	mmenu_attr->factoryOk_bmp[0] = dsk_theme_open(ID_HOME_BTN_FACTORY_OK_1_BMP);
	mmenu_attr->factoryOk_bmp[1] = dsk_theme_open(ID_HOME_BTN_FACTORY_OK_2_BMP);
	mmenu_attr->facMotorTest_bmp = dsk_theme_open(ID_HOME_IMG_POP_FACTORY_MOTOR_TEST_BMP);
	mmenu_attr->facManualTest_bmp = dsk_theme_open(ID_HOME_IMG_POP_FACTORY_MANUAL_TEST_BMP);
	mmenu_attr->facExit_bmp = dsk_theme_open(ID_HOME_IMG_POP_FACTORY_EXIT_BMP);

	switch(curFactory) {
		case 1:
			mmenu_attr->title_bmp = dsk_theme_open(ID_HOME_TV_FACTORY_TITLE_1_BMP);
			mmenu_attr->message_bmp_1 = dsk_theme_open(ID_HOME_TV_FACTORY_MESSAGE_1_BMP);
			mmenu_attr->message_bmp_2 = dsk_theme_open(ID_HOME_TV_MESSAGE_ERROR_BMP);
			for (i = 0 ; i < 10; i++) {
				mmenu_attr->factoryBlueNum_bmp[i] = dsk_theme_open(FactoryBlueNum_ID[i]);
			}
			break;
		case 2:
			mmenu_attr->title_bmp = dsk_theme_open(ID_HOME_TV_FACTORY_TITLE_2_BMP);
			mmenu_attr->message_bmp_1 = dsk_theme_open(ID_HOME_TV_FACTORY_MESSAGE_2_BMP);
			mmenu_attr->message_bmp_2 = dsk_theme_open(ID_HOME_IMG_POP_FACTORY_RESET_BMP);
			break;
		case 3:
			mmenu_attr->title_bmp = dsk_theme_open(ID_HOME_TV_FACTORY_TITLE_3_BMP);
			mmenu_attr->message_bmp_1 = dsk_theme_open(ID_HOME_TV_FACTORY_MESSAGE_3_BMP);
			mmenu_attr->message_bmp_2 = dsk_theme_open(ID_HOME_TV_FACTORY_MESSAGE_4_BMP);
			for (i = 0 ; i < 12 ; i++) {
				mmenu_attr->factoryCounterNum_bmp[i] = dsk_theme_open(FacCounterNum_ID[i]);
			}
			break;
		case 4:
			mmenu_attr->title_bmp = dsk_theme_open(ID_HOME_TV_FACTORY_TITLE_4_BMP);
			mmenu_attr->message_bmp_1 = dsk_theme_open(ID_HOME_TV_FACTORY_MESSAGE_5_BMP);
			break;
	}
	
    mmenu_attr->res_init = EPDK_TRUE;
	 
	return EPDK_OK;
}

/*
	释放资源
*/
static __s32 uninit_factory_res(mmenu_attr_t *mmenu_attr)
{
	  __u32 	i; 

    if(EPDK_FALSE == mmenu_attr->res_init)
    {
        return EPDK_OK;
    }
  

	for (i = 0; i < 13; i++)
	{
		dsk_theme_close(mmenu_attr->factoryNum_bmp[i]);
	}
	for (i=0; i<25; i++) {
		dsk_theme_close(mmenu_attr->factoryInitPop_bmp[i]);
	}

	for (i=0; i<20; i++) {
		dsk_theme_close(mmenu_attr->factoryTest_bmp[i]);
	}

	for (i=0; i<7; i++) {
		dsk_theme_close(mmenu_attr->factoryModeTest_focus[i]);
		dsk_theme_close(mmenu_attr->factoryModeTest_unfocus[i]);
	}
	
	dsk_theme_close(mmenu_attr->factoryCal_bmp);
	dsk_theme_close(mmenu_attr->factoryMaxinc_bmp);
	dsk_theme_close(mmenu_attr->factoryMininc_bmp);
	dsk_theme_close(mmenu_attr->factoryLine_bmp);
	dsk_theme_close(mmenu_attr->factoryUnit[0]);
	dsk_theme_close(mmenu_attr->factoryUnit[1]);
	dsk_theme_close(mmenu_attr->factoryUnit_bmp[0]);
	dsk_theme_close(mmenu_attr->factoryUnit_bmp[1]);
	dsk_theme_close(mmenu_attr->factoryMaxInc[0]);
	dsk_theme_close(mmenu_attr->factoryMaxInc[1]);
	dsk_theme_close(mmenu_attr->message_bmp_1);
	dsk_theme_close(mmenu_attr->message_bmp_2);
	dsk_theme_close(mmenu_attr->factory_ERR_bmp);
	dsk_theme_close(mmenu_attr->factorySoftVer_bmp);
	dsk_theme_close(mmenu_attr->factoryTotalDistance_bmp);
	dsk_theme_close(mmenu_attr->factoryTotalTime_bmp);
	dsk_theme_close(mmenu_attr->title_bmp);
	dsk_theme_close(mmenu_attr->factoryJudg_bmp[0]);
	dsk_theme_close(mmenu_attr->factoryJudg_bmp[1]);
	dsk_theme_close(mmenu_attr->factoryOnOff_bmp[0]);
	dsk_theme_close(mmenu_attr->factoryOnOff_bmp[1]);
	dsk_theme_close(mmenu_attr->factoryOk_bmp[0]);
	dsk_theme_close(mmenu_attr->factoryOk_bmp[1]);
	dsk_theme_close(mmenu_attr->facManualTest_bmp);
	dsk_theme_close(mmenu_attr->facMotorTest_bmp);
	dsk_theme_close(mmenu_attr->facExit_bmp);
	if (curFactory == 1) {
		for(i = 0; i < 10; i++) {
			dsk_theme_close(mmenu_attr->factoryBlueNum_bmp[i]);
		}
	}

	if (curFactory == 3) {
		for(i = 0; i < 12; i++) {
			dsk_theme_close(mmenu_attr->factoryCounterNum_bmp[i]);
		}
	}
	
    mmenu_attr->res_init = EPDK_FALSE;
    
	return EPDK_OK;
}

static void init_data_flag()
{
	enterFlag = EPDK_FALSE;
	keyTestFlag = EPDK_FALSE;
	motorTestFlag = EPDK_FALSE;
	manualTestFlag = EPDK_FALSE;
	//EMFlag = EPDK_FALSE;
	yesNoFlag = EPDK_TRUE;
	judeFlag = EPDK_FALSE;
	facExitFlag = EPDK_FALSE;
	backHomeJudeFlag = EPDK_TRUE;
	keyTestValue = 0;
	factory2Count = 0;
	fac3Count = 0;
	fac3Old = 0;
}


static void draw_unitEM(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;

	GUI_LyrWinSel(threeLyr);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryUnit[1]);
		GUI_BMP_Draw(pbmp,526,140);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryUnit[0]);
		GUI_BMP_Draw(pbmp,526,140);
	}
}

static void  draw_unit(mmenu_attr_t *mmenu_attr, __bool flag) 
{
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryUnit_bmp[1]);
		GUI_BMP_Draw(pbmp,124,140);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryUnit_bmp[0]);
		GUI_BMP_Draw(pbmp,124,140);
	}

	draw_unitEM(mmenu_attr,EMFlag);
	
}

static void draw_maxINCData(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;

	GUI_LyrWinSel(threeLyr);
	#ifdef AC00280
	if (maxINC > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[maxINC/10]);
		GUI_BMP_Draw(pbmp,526,72);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[maxINC%10]);
		GUI_BMP_Draw(pbmp,542,72);
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(526, 72, 526+16, 72+61);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[maxINC%10]);
		GUI_BMP_Draw(pbmp,542,72);
	}
	#else
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[2]);
	GUI_BMP_Draw(pbmp,526,72);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[4]);
	GUI_BMP_Draw(pbmp,542,72);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[11]);
	GUI_BMP_Draw(pbmp,558,72);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[0]);
	GUI_BMP_Draw(pbmp,567,72);
	#endif
	
}

static void draw_cal_adc(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	__u8 adc;

	adc = mmenu_attr->p_cmd_data->curInc_ADC;
	
	if (adc > 99) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryBlueNum_bmp[adc/100]);
		GUI_BMP_Draw(pbmp,526,72);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryBlueNum_bmp[adc%100/10]);
		GUI_BMP_Draw(pbmp,542,72);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryBlueNum_bmp[adc%100%10]);
		GUI_BMP_Draw(pbmp,558,72);
	} else if (adc > 9){
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(526, 72, 526+16, 72+61);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryBlueNum_bmp[adc/10]);
		GUI_BMP_Draw(pbmp,542,72);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryBlueNum_bmp[adc%10]);
		GUI_BMP_Draw(pbmp,558,72);
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(526, 72, 526+16*2, 72+61);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryBlueNum_bmp[adc]);
		GUI_BMP_Draw(pbmp,558,72);
	}
	
}


static void draw_maxINC(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryMaxInc[1]);
		GUI_BMP_Draw(pbmp,124,72);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryMaxInc[0]);
		GUI_BMP_Draw(pbmp,124,72);
	}

	draw_maxINCData(mmenu_attr);
	
}

static void draw_calMaxINC(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;

	#ifdef AC00280

	if (maxINC > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[maxINC/10]);
		GUI_BMP_Draw(pbmp,526,209);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[maxINC%10]);
		GUI_BMP_Draw(pbmp,542,209);
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(526, 209, 526+16, 209+61);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[maxINC%10]);
		GUI_BMP_Draw(pbmp,542,209);
	}
	#else
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[2]);
	GUI_BMP_Draw(pbmp,526,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[4]);
	GUI_BMP_Draw(pbmp,542,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[11]);
	GUI_BMP_Draw(pbmp,558,209);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[0]);
	GUI_BMP_Draw(pbmp,567,209);
	#endif
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

static void draw_jude(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;
	
	GUI_LyrWinSel(threeLyr);
	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryJudg_bmp[0]);
		GUI_BMP_Draw(pbmp,234,346);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryJudg_bmp[1]);
		GUI_BMP_Draw(pbmp,234,346);
	}
	judeFlag = EPDK_TRUE;
}

static void draw_pop_jude(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	RECT	rect;

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
	GUI_SetBkColor(0xFE000000);
	
	GUI_ClearRect(0, 0, 1024, 600);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_2);
	GUI_BMP_Draw(pbmp,123,150);

	rect.x = 0;
	rect.y = 0;
	rect.width = 1024;
	rect.height = 600;
	threeLyr = home_32bpp_layer_create(&rect, 1);
	
	if( !threeLyr )
	{
		__err("new layer create error !\n");
	}
	GUI_LyrWinSetSta(threeLyr, GUI_LYRWIN_STA_ON);
	
	GUI_LyrWinSetTop(threeLyr);
	GUI_SetBkColor(0x00000000);
	
	GUI_ClearRect(0, 0, 1024, 600);

	draw_jude(mmenu_attr, EPDK_TRUE);
	
}
static void draw_calbration(mmenu_attr_t *mmenu_attr) 
{
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);
	GUI_SetBkColor(0);
	GUI_ClearRect(0, 69, 1024, 481);
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
	GUI_BMP_Draw(pbmp,0,0);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryCal_bmp);
	GUI_BMP_Draw(pbmp,124,72);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryLine_bmp);
	GUI_BMP_Draw(pbmp,124,137);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryMininc_bmp);
	GUI_BMP_Draw(pbmp,124,140);
	#ifdef AC00280
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[1]);
	GUI_BMP_Draw(pbmp,542,141);
	#else
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[1]);
	GUI_BMP_Draw(pbmp,526,141);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[8]);
	GUI_BMP_Draw(pbmp,542,141);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[11]);
	GUI_BMP_Draw(pbmp,558,141);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[0]);
	GUI_BMP_Draw(pbmp,567,141);
	#endif

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryLine_bmp);
	GUI_BMP_Draw(pbmp,124,203);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryMaxinc_bmp);
	GUI_BMP_Draw(pbmp,124,208);

	draw_calMaxINC(mmenu_attr);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryLine_bmp);
	GUI_BMP_Draw(pbmp,124,268);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryInitPop_bmp[0]);
	GUI_BMP_Draw(pbmp,454,328);
	esKRNL_TimeDly(10);
	GUI_SetTimer(h_deswin_old, POP_INIT_ID, 20, NULL);
	
	
}

static void draw_loop_Init(mmenu_attr_t *mmenu_attr) 
{
	void *pbmp;

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryInitPop_bmp[loopCount]);
	GUI_BMP_Draw(pbmp,454,328);
	
}

static void draw_ERR_3(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	RECT	rect;

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

	pbmp = dsk_theme_hdl2buf(mmenu_attr->factory_ERR_bmp);
	GUI_BMP_Draw(pbmp,123,150);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_2);
	GUI_BMP_Draw(pbmp,0,504);

	ERRFlag = EPDK_TRUE;
	
}

static void draw_ok(mmenu_attr_t *mmenu_attr, __bool flag, __u16 x, __u16 y)
{
	void *pbmp;

	GUI_LyrWinSel(threeLyr);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryOk_bmp[1]);
		GUI_BMP_Draw(pbmp,x,y);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryOk_bmp[0]);
		GUI_BMP_Draw(pbmp,x,y);
	}
}

static void draw_OnOff(mmenu_attr_t *mmenu_attr, __bool flag, __u16 x, __u16 y)
{
	void *pbmp;

	GUI_LyrWinSel(threeLyr);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryOnOff_bmp[0]);
		GUI_BMP_Draw(pbmp,x,y);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryOnOff_bmp[1]);
		GUI_BMP_Draw(pbmp,x,y);
	}
}

static void draw_disp_onoff(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;

	GUI_LyrWinSel(threeLyr);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryOnOff_bmp[0]);
		GUI_BMP_Draw(pbmp,526,72);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryOnOff_bmp[1]);
		GUI_BMP_Draw(pbmp,526,72);
	}
	
}


static void draw_child_OnOff(mmenu_attr_t *mmenu_attr, __bool flag, __u16 x, __u16 y)
{
	void *pbmp;

	GUI_LyrWinSel(threeLyr);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryOnOff_bmp[1]);
		GUI_BMP_Draw(pbmp,x,y);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryOnOff_bmp[0]);
		GUI_BMP_Draw(pbmp,x,y);
	}
}



static void draw_keyTest(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_focus[0]);
		GUI_BMP_Draw(pbmp,124,72);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_unfocus[0]);
		GUI_BMP_Draw(pbmp,124,72);
	}

	draw_ok(mmenu_attr,keyTestFlag,705,72);
	
}

static void draw_MotorTest(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_focus[1]);
		GUI_BMP_Draw(pbmp,124,140);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_unfocus[1]);
		GUI_BMP_Draw(pbmp,124,140);
	}

	draw_ok(mmenu_attr,motorTestFlag,705,140);
	
}

static draw_L_count(mmenu_attr_t *mmenu_attr,__u8 level)
{
	void *pbmp;

	GUI_LyrWinSel(lyr);
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryTest_bmp[level]);
	GUI_BMP_Draw(pbmp,211,136);

	SetRunLevel(mmenu_attr->p_cmd_data,(__u16) (level+1));
	
}

static draw_counter_val(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	__u16 counter;

	if (manTestLevel > 13) {
		if (counter = mmenu_attr->p_cmd_data->curCounter < 200) {
			counter = mmenu_attr->p_cmd_data->curCounter + 255;
		}
	} else {
		counter = mmenu_attr->p_cmd_data->curCounter;
	}
	
	GUI_LyrWinSel(lyr);
	if (counter > 99) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryCounterNum_bmp[counter/100]);
		GUI_BMP_Draw(pbmp,496,136);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryCounterNum_bmp[counter%100/10]);
		GUI_BMP_Draw(pbmp,520,136);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryCounterNum_bmp[counter%100%10]);
		GUI_BMP_Draw(pbmp,544,136);
	} else if (counter > 9){
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(496, 136, 496+24, 136+26);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryCounterNum_bmp[counter/10]);
		GUI_BMP_Draw(pbmp,520,136);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryCounterNum_bmp[counter%10]);
		GUI_BMP_Draw(pbmp,544,136);
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(496, 136, 496+24*2, 136+26);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryCounterNum_bmp[counter]);
		GUI_BMP_Draw(pbmp,544,136);
	}
	
}



static void draw_pop_manual_test(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	RECT	rect;

	motorTestFlag = EPDK_TRUE;
	GUI_LyrWinSel(threeLyr);
	GUI_SetBkColor(0xEE000000);
	GUI_ClearRect(0, 0, 1024, 600);


	pbmp = dsk_theme_hdl2buf(mmenu_attr->facManualTest_bmp);
	GUI_BMP_Draw(pbmp,123,150);
	
	rect.x = 123;
	rect.y = 150;
	rect.width = 778;
	rect.height = 301;
	lyr= home_32bpp_layer_create(&rect, 1); //创建按键键值图层	
	if( !lyr)
	{
		__err("new layer create error !\n");
	}
	GUI_LyrWinSetSta(lyr, GUI_LYRWIN_STA_ON);
	
	GUI_LyrWinSetTop(lyr);
	GUI_SetBkColor(0x00000000);
	GUI_ClearRect(0, 0, 778, 301);

	draw_L_count(mmenu_attr,0);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryCounterNum_bmp[10]);
	GUI_BMP_Draw(pbmp,319,136);

	GUI_SetTimer(h_deswin_old, COUNTER_ID, 100, NULL);
	//draw_counter_val(mmenu_attr);
	
}

static void draw_Exit_yes_no(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryJudg_bmp[0]);
		GUI_BMP_Draw(pbmp,111,196);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryJudg_bmp[1]);
		GUI_BMP_Draw(pbmp,111,196);
	}
}


static void draw_fac_exit(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	RECT	rect;

	GUI_LyrWinSel(threeLyr);
	GUI_SetBkColor(0xEE000000);
	GUI_ClearRect(0, 0, 1024, 600);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->facExit_bmp);
	GUI_BMP_Draw(pbmp,123,150);
	
	rect.x = 123;
	rect.y = 150;
	rect.width = 778;
	rect.height = 301;
	lyr= home_32bpp_layer_create(&rect, 1); //创建按键键值图层	
	if( !lyr)
	{
		__err("new layer create error !\n");
	}
	GUI_LyrWinSetSta(lyr, GUI_LYRWIN_STA_ON);
	
	GUI_LyrWinSetTop(lyr);
	GUI_SetBkColor(0x00000000);
	GUI_ClearRect(0, 0, 778, 301);
	facExitFlag = EPDK_TRUE;
	draw_Exit_yes_no(mmenu_attr,backHomeJudeFlag);

}


static void draw_ManualTest(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_focus[2]);
		GUI_BMP_Draw(pbmp,124,208);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_unfocus[2]);
		GUI_BMP_Draw(pbmp,124,208);
	}

	draw_ok(mmenu_attr,manualTestFlag,705,208);
	
}

static void draw_displayMode(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_focus[3]);
		GUI_BMP_Draw(pbmp,124,276);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_unfocus[3]);
		GUI_BMP_Draw(pbmp,124,276);
	}

	draw_OnOff(mmenu_attr,disModeFlag,526,276);
	
}

static void draw_PauseMode(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_focus[4]);
		GUI_BMP_Draw(pbmp,124,344);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_unfocus[4]);
		GUI_BMP_Draw(pbmp,124,344);
	}

	draw_OnOff(mmenu_attr,pauModeFlag,526,344);
	
}

static void draw_keyTone(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_focus[5]);
		GUI_BMP_Draw(pbmp,124,412);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_unfocus[5]);
		GUI_BMP_Draw(pbmp,124,412);
	}

	draw_OnOff(mmenu_attr,keyToneFlag,526,412);
	
}

static void draw_ChildLock(mmenu_attr_t *mmenu_attr, __bool flag)
{
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);

	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_focus[6]);
		GUI_BMP_Draw(pbmp,124,480);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_unfocus[6]);
		GUI_BMP_Draw(pbmp,124,480);
	}

	draw_child_OnOff(mmenu_attr,ChildLockFlag,526,480);
	
}

static void draw_mode(mmenu_attr_t *mmenu_attr, __bool flag,__u8 num)
{

	switch(num) {
		case 0:
			draw_keyTest(mmenu_attr, flag);
			break;
		case 1:
			draw_unit(mmenu_attr, flag);
			break;
		case 2:
			draw_ManualTest(mmenu_attr, flag);
			break;
		case 3:
			draw_displayMode(mmenu_attr, flag);
			break;
		case 4:
			draw_PauseMode(mmenu_attr, flag);
			break;
		case 5:
			draw_keyTone(mmenu_attr, flag);
			break;
		case 6:
			draw_ChildLock(mmenu_attr, flag);
			break;
		case 7:
			draw_keyTest(mmenu_attr, EPDK_TRUE);
			draw_unit(mmenu_attr, EPDK_FALSE);
			draw_ManualTest(mmenu_attr, EPDK_FALSE);
			draw_displayMode(mmenu_attr, EPDK_FALSE);
			draw_PauseMode(mmenu_attr, EPDK_FALSE);
			draw_keyTone(mmenu_attr, EPDK_FALSE);
			draw_ChildLock(mmenu_attr, EPDK_FALSE);
			break;
	}
	
}


static void draw_testKey_value(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;

	GUI_LyrWinSel(threeLyr);

	if (keyTestValue > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[keyTestValue/10]);
		GUI_BMP_Draw(pbmp,526,72);
	} else {
		GUI_SetBkColor(0);
		GUI_ClearRect(526, 72, 562+16, 72+61);	
	}
	pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryNum_bmp[keyTestValue%10]);
	GUI_BMP_Draw(pbmp,526+16,72);
}


/*
	显示Factory界面
*/

static void paint_factory_all(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	GUI_RECT gui_rect;
	
	
	if(EPDK_FALSE == mmenu_attr->res_init)
    {
        init_factory_res(mmenu_attr);
    }

	GUI_LyrWinSel(mmenu_attr->layer);
	GUI_SetBkColor(0);

    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	gui_rect.x0 = 0;
    gui_rect.y0 = 0;  
    gui_rect.x1 = 1024;
    gui_rect.y1 = 600;
    GUI_ClearRectEx(&gui_rect);

	switch(curFactory) {
		case 1: //factory 1
		{
			RECT	rect;
			rect.x = 0;
			rect.y = 0;
			rect.width = 1024;
			rect.height = 600;
			threeLyr= home_32bpp_layer_create(&rect, 2); //创建按键键值图层	
			if( !threeLyr)
			{
				__err("new layer create error !\n");
			}
			GUI_LyrWinSetSta(threeLyr, GUI_LYRWIN_STA_ON);
			//GUI_LyrWinSel(threeLyr);
			GUI_LyrWinSetTop(threeLyr);
			GUI_SetBkColor(0x00000000);
			GUI_ClearRect(0, 0, 1024, 600);
			
			pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
			GUI_BMP_Draw(pbmp,0,0);
			draw_maxINC(mmenu_attr,EPDK_FALSE);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_1);
			GUI_BMP_Draw(pbmp,0,550);
			break;
		}
		case 2: //factory 2
			pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
			GUI_BMP_Draw(pbmp,0,0);

			pbmp= dsk_theme_hdl2buf(mmenu_attr->factoryLine_bmp);
			GUI_BMP_Draw(pbmp,124,70);

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

			pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_1);
			GUI_BMP_Draw(pbmp,0,550);
			pageData[0] = 0x40;
			readPage(ProgramRDData,pageData);
			TotalTime = ProgramRDData[0]*65536+ProgramRDData[1]*256+ProgramRDData[2];
			TotalDist = ProgramRDData[3]*256+ProgramRDData[4];
			draw_totalTimeData(mmenu_attr);

			draw_totalDistanceData(mmenu_attr);

			draw_softVerData(mmenu_attr);
			break;
		case 3: //factory 3
		{
			RECT	rect;
			//__u16 sendBuf[4] = {0x5b,0x1,0xf0,0x5d};
			rect.x = 0;
			rect.y = 0;
			rect.width = 1024;
			rect.height = 600;
			threeLyr= home_32bpp_layer_create(&rect, 2); //创建按键键值图层	
			if( !threeLyr)
			{
				__err("new layer create error !\n");
			}
			GUI_LyrWinSetSta(threeLyr, GUI_LYRWIN_STA_ON);
			//GUI_LyrWinSel(threeLyr);
			GUI_LyrWinSetTop(threeLyr);
			GUI_SetBkColor(0x00000000);
			GUI_ClearRect(0, 0, 1024, 600);
			SetRunLevel (mmenu_attr->p_cmd_data,1);//回到初始状态1；
			//bluetooth_writebuf(sendBuf);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
			GUI_BMP_Draw(pbmp,0,0);

			draw_mode(mmenu_attr,EPDK_FALSE,7);
			GUI_LyrWinSel(mmenu_attr->layer);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_1);
			GUI_BMP_Draw(pbmp,0,550);
			
			break;
		}

		case 4:
		{
			RECT	rect;

			pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
			GUI_BMP_Draw(pbmp,0,0);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->factoryModeTest_focus[3]);
			GUI_BMP_Draw(pbmp,124,72);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_1);
			GUI_BMP_Draw(pbmp,0,550);
			rect.x = 0;
			rect.y = 0;
			rect.width = 1024;
			rect.height = 600;
			threeLyr= home_32bpp_layer_create(&rect, 2); //创建按键键值图层	
			if( !threeLyr)
			{
				__err("new layer create error !\n");
			}
			GUI_LyrWinSetSta(threeLyr, GUI_LYRWIN_STA_ON);
			//GUI_LyrWinSel(threeLyr);
			GUI_LyrWinSetTop(threeLyr);
			GUI_SetBkColor(0x00000000);
			GUI_ClearRect(0, 0, 1024, 600);
			
			draw_disp_onoff(mmenu_attr,disModeFlag);
			
			
			break;
		}
	}
	
	//pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
	
	

		
}


/*
	主界面按键响应
*/
 __s32  factorybar_key_proc(__gui_msg_t *msg, com_data_para_t *com_data)
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
	errCode = com_data->error_code;
	keycode = msg->dwAddData1;
	switch(keycode)
	{
		case ENTER_KEY:
		{
			
			switch(curFactory) {
				case 1:
				{
				#if 0
					focus++;
					if (focus == 1) {
						draw_unit(mmenu_attr,EPDK_TRUE);
						draw_maxINC(mmenu_attr,EPDK_FALSE);
					}
					if (focus == 2){
						para->UnitFlag = EMFlag;
					}
					if (focus == 3) {
						draw_unit(mmenu_attr,EPDK_FALSE);
						draw_maxINC(mmenu_attr,EPDK_TRUE);
						
					}

					if (focus > 3) {
						focus = 0;
					}
				#endif
					break;
				}
				case 2:
				{
					__u8 i;
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					if (!judeFlag) {
						if (factory2Count == 4) {
							draw_pop_jude(mmenu_attr);
							factory2Count = 0;
						}
					} else {
						if (yesNoFlag) {
							TotalDist = 0;
							TotalTime = 0;
							ProgramWRData[0] = 0;
							ProgramWRData[1] = 0;
							ProgramWRData[2] = 0;
							ProgramWRData[3] = 0;
							ProgramWRData[4] = 0;
							pageData[0] = 0x40;
							writePage(ProgramWRData,pageData);
							esKRNL_TimeDly(3);
							pageData[0] = 0x20;
							TotalData[0] = AGE_INIT;
							TotalData[1] = TIME_INIT;
							TotalData[2] = WEIGHT_INIT_M;
							TotalData[3] = (WEIGHT_INIT_B>>8)&0xFF;
							TotalData[4] = WEIGHT_INIT_B&0xFF;
						
							writePage(TotalData,pageData);
							esKRNL_TimeDly(3);
							pageData[0] = 0x60;
							readPage(ProgramWRData,pageData);
							esKRNL_TimeDly(3);
							ProgramWRData[0] = 0; //公英制 默认英制
							ProgramWRData[1] = 0; //display MODE 
							ProgramWRData[2] = 0; //PAUSE MODE
							ProgramWRData[3] = 0; //KEY TONE
							ProgramWRData[4] = 0; //CHILD LOCK
							ProgramWRData[7] = 0; // user1 flag
							ProgramWRData[8] = 0; //user2 flag
							
							writePage(ProgramWRData,pageData);
						} 
						GUI_SetBkColor(0x00000000);
						GUI_ClearRect(0, 0, 1024, 600);
						GUI_LyrWinDelete(threeLyr);
						GUI_SetBkColor(0x00000000);
						GUI_ClearRect(0, 0, 1024, 600);
						GUI_LyrWinDelete(lyr);
						GUI_LyrWinSel(mmenu_attr->layer);
						draw_totalDistanceData(mmenu_attr);
						draw_totalTimeData(mmenu_attr);
						judeFlag = EPDK_FALSE;
					}
					break;
				}
				case 3:
				{
					void *pbmp;

					if (motorTestFlag) {
						break;
					}
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					//__msg("huang#########  fac3Count = %d \n", fac3Count);
					if (keyTestFlag) {
						keyTestValue = 16;
						draw_testKey_value(mmenu_attr);
						keyTestCount[15] = 1;
						break;
					}
					if (facExitFlag) {
						GUI_LyrWinSel(lyr);
						GUI_SetBkColor(0x00000000);
						GUI_ClearRect(0, 0, 1024, 600);
						GUI_LyrWinDelete(lyr);
						if (backHomeJudeFlag) {
							GUI_LyrWinSel(threeLyr);
							GUI_SetBkColor(0x00000000);
							GUI_ClearRect(0, 0, 1024, 600);
							GUI_LyrWinSel(mmenu_attr->layer);
							GUI_SetBkColor(0x00000000);
							GUI_ClearRect(0, 0, 1024, 600);
							main_cmd2manuel(h_deswin_old,ID_MAIN,keyToneFlag,SP_Data);
						} else {
							facExitFlag = EPDK_FALSE;
							GUI_LyrWinSel(threeLyr);
							GUI_SetBkColor(0x00000000);
							//GUI_ClearRect(123, 150, 123+778, 150+301);
							GUI_ClearRect(0, 0, 1024, 600);
							pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
							GUI_BMP_Draw(pbmp,0,0);
							draw_keyTest(mmenu_attr, EPDK_FALSE);
							draw_unit(mmenu_attr, EPDK_FALSE);
							draw_ManualTest(mmenu_attr, EPDK_FALSE);
							draw_displayMode(mmenu_attr, EPDK_FALSE);
							draw_PauseMode(mmenu_attr, EPDK_FALSE);
							draw_keyTone(mmenu_attr, EPDK_FALSE);
							draw_ChildLock(mmenu_attr, EPDK_FALSE);
							draw_mode(mmenu_attr,EPDK_TRUE,fac3Count);
							GUI_LyrWinSel(mmenu_attr->layer);
							pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_1);
							GUI_BMP_Draw(pbmp,0,550);
							
						}
						break;
					}
					switch(fac3Count) {
						case 0:
						{
							void *pbmp;
			
							draw_ok(mmenu_attr,EPDK_FALSE,705,72);
							GUI_LyrWinSel(mmenu_attr->layer);
							pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_2);
							GUI_BMP_Draw(pbmp,0,550);
							keyTestFlag = EPDK_TRUE;
							
							GUI_SetTimer(h_deswin_old, FAC3_TEST_ID, 100, NULL);
							break;
						}
							
						case 1:
							enterFlag = !enterFlag;
							break;
						case 2:
							draw_pop_manual_test(mmenu_attr);
							break;
						case 3:
						case 4:	
						case 5:
						case 6:
							enterFlag = !enterFlag;
							
							break;			
					}
					
					break;
				}
			case 4:
				#if 0
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinSel(mmenu_attr->layer);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				pageData[0] = 0x60;//保存数据
				writePage(ProgramWRData,pageData);
				main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data);
				#endif
				break;
			}
			break;
		}
		case DOWN_KEY:
		case DOWN_LONG_KEY:
		case DOWN_QUICK_LONG_KEY:
		{
		
			switch(curFactory) {
				case 1:
				#if 0
					if (focus == 1) {
						EMFlag = !EMFlag;
						draw_unitEM(mmenu_attr,EMFlag);
					}

					if (focus == 3){
						maxINC --;
			
						if (maxINC < 10) {
							maxINC = 10;
						}
						draw_maxINCData(mmenu_attr);
						para->maxINC = maxINC;
					}
				#endif
					break;
				case 2:
					#if 0
					if (!judeFlag) {
						if (factory2Count == 1 ) {
							factory2Count = 2;
							break;
						}
						if (factory2Count == 3) {
							factory2Count = 4;
							break;
						}
						factory2Count = 0;
					} else {
						yesNoFlag = !yesNoFlag;
						draw_jude(mmenu_attr,yesNoFlag);
					}
					#endif
					break;
				case 3:
					//if (fac3Count!=5) {
					//	
					//}
					if (keyTestFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						keyTestValue = 14;
						draw_testKey_value(mmenu_attr);
						keyTestCount[13] = 1;
						break;
					}

					if (facExitFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						backHomeJudeFlag = !backHomeJudeFlag;
						draw_Exit_yes_no(mmenu_attr,backHomeJudeFlag);
						break;
					}
					if (enterFlag) {
						
						switch(fac3Count) {
							case 1:
								EMFlag = !EMFlag;
								draw_unitEM(mmenu_attr,EMFlag);
								ProgramWRData[0] = EMFlag;
								break;
							case 3:
								disModeFlag = !disModeFlag;
								draw_OnOff(mmenu_attr,disModeFlag,526,276);
								ProgramWRData[1] = disModeFlag;
								break;
							case 4:
								pauModeFlag = !pauModeFlag;
								draw_OnOff(mmenu_attr,pauModeFlag,526,344);
								ProgramWRData[2] = pauModeFlag;
								break;
							case 5:
								keyToneFlag = !keyToneFlag;
								draw_OnOff(mmenu_attr,keyToneFlag,526,412);
								ProgramWRData[3] = keyToneFlag;
								//open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
								break;
							case 6:
								ChildLockFlag = !ChildLockFlag;
								draw_child_OnOff(mmenu_attr,ChildLockFlag,526,480);
								ProgramWRData[4] = ChildLockFlag;
								break;
						}
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						break;
					}
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					if (motorTestFlag) {
						
						
						if (manTestLevel) {
							manTestLevel--;
						}
						draw_L_count(mmenu_attr,manTestLevel);
						break;
					}
					
					fac3Count++;
					if (fac3Count == 7) {
						fac3Old = 6;
						fac3Count = 0;
					} else {
						fac3Old = fac3Count - 1;
					}
					draw_mode(mmenu_attr,EPDK_FALSE,fac3Old);
					draw_mode(mmenu_attr,EPDK_TRUE,fac3Count);
					
					break;
				case 4:
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					disModeFlag = !disModeFlag;
					draw_disp_onoff(mmenu_attr,disModeFlag);
					ProgramWRData[1] = disModeFlag;
					break;
			}
		
			break;
		}
		case UP_KEY:
		case UP_LONG_KEY:
		case UP_QUICK_LONG_KEY:
		{
			
			switch (curFactory) {
				case 1:
				{
				#if 0
					if (focus == 1) {
						EMFlag = !EMFlag;
						draw_unitEM(mmenu_attr,EMFlag);
					}

					if (focus == 3){
						maxINC ++;
						if (maxINC > 20) {
							maxINC = 20;
						} 
						draw_maxINCData(mmenu_attr);
						para->maxINC = maxINC;
					}
					#endif
					break;
				}
				case 2:
					#if 0
					if (!judeFlag) {
						if (!factory2Count ) {
							factory2Count = 1;
							break;
						}

						if (factory2Count == 2) {
							factory2Count = 3;
							break;
						}
						factory2Count = 0;
					} else {
						yesNoFlag = !yesNoFlag;
						draw_jude(mmenu_attr,yesNoFlag);
					}
					#endif
					break;

				case 3:
					//if (fac3Count!=5) {
					//	open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					//}
					if (keyTestFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						keyTestValue = 13;
						draw_testKey_value(mmenu_attr);
						keyTestCount[12] = 1;
						break;
					}

					if (facExitFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						backHomeJudeFlag = !backHomeJudeFlag;
						draw_Exit_yes_no(mmenu_attr,backHomeJudeFlag);
						break;
					}
					
					if (enterFlag) {
						switch(fac3Count) {
							case 1:
								EMFlag = !EMFlag;
								draw_unitEM(mmenu_attr,EMFlag);
								ProgramWRData[0] = EMFlag;
								break;
							case 3:
								disModeFlag = !disModeFlag;
								draw_OnOff(mmenu_attr,disModeFlag,526,276);
								ProgramWRData[1] = disModeFlag;
								break;
							case 4:
								pauModeFlag = !pauModeFlag;
								draw_OnOff(mmenu_attr,pauModeFlag,526,344);
								ProgramWRData[2] = pauModeFlag;
								break;
							case 5:
								keyToneFlag = !keyToneFlag;
								draw_OnOff(mmenu_attr,keyToneFlag,526,412);
								ProgramWRData[3] = keyToneFlag;
								//open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
								break;
							case 6:
								ChildLockFlag = !ChildLockFlag;
								draw_child_OnOff(mmenu_attr,ChildLockFlag,526,480);
								ProgramWRData[4] = ChildLockFlag;
								break;
						}
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						break;
					}
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					if (motorTestFlag) {
						manTestLevel++;
						if (manTestLevel > 19) {
							manTestLevel = 19;
						}
						draw_L_count(mmenu_attr,manTestLevel);
						break;
					}
					fac3Count--;
						
					if (fac3Count < 0) {
						fac3Old = 0;
						fac3Count = 6;
					} else {
						fac3Old = fac3Count +1;
					}
					
					
					draw_mode(mmenu_attr,EPDK_FALSE,fac3Old);
					draw_mode(mmenu_attr,EPDK_TRUE,fac3Count);
					
					break;
				case 4:
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					disModeFlag = !disModeFlag;
					draw_disp_onoff(mmenu_attr,disModeFlag);
					ProgramWRData[1] = disModeFlag;
					break;
			}
			

			break;
		}
		case FAST_KEY:
		case FAST_LONG_KEY:
		case FAST_LONG_QUICK_KEY:
		{
		
			switch (curFactory) {
				case 1:
				{	
				#if 0
					if (focus == 1) {
						EMFlag = !EMFlag;
						draw_unitEM(mmenu_attr,EMFlag);
					}

					if (focus == 3){
						maxINC ++;
						if (maxINC > 20) {
							maxINC = 20;
						} 
						draw_maxINCData(mmenu_attr);
						para->maxINC = maxINC;
					}
				#endif
					break;
				}

				case 2:
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					if (!judeFlag) {
						if (!factory2Count ) {
							factory2Count = 1;
							break;
						}

						if (factory2Count == 2) {
							factory2Count = 3;
							break;
						}
						factory2Count = 0;
					} else {
						yesNoFlag = !yesNoFlag;
						draw_jude(mmenu_attr,yesNoFlag);
					}
					break;
				case 3:
					//if (fac3Count!=5) {
						//open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				//	}
					if (keyTestFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						keyTestValue = 19;
						draw_testKey_value(mmenu_attr);
						keyTestCount[18] = 1;
						break;
					}

					if (facExitFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						backHomeJudeFlag = !backHomeJudeFlag;
						draw_Exit_yes_no(mmenu_attr,backHomeJudeFlag);
						break;
					}
					if (enterFlag) {
						switch(fac3Count) {
							case 1:
								EMFlag = !EMFlag;
								draw_unitEM(mmenu_attr,EMFlag);
								ProgramWRData[0] = EMFlag;
								break;
							case 3:
								disModeFlag = !disModeFlag;
								draw_OnOff(mmenu_attr,disModeFlag,526,276);
								ProgramWRData[1] = disModeFlag;
								break;
							case 4:
								pauModeFlag = !pauModeFlag;
								draw_OnOff(mmenu_attr,pauModeFlag,526,344);
								ProgramWRData[2] = pauModeFlag;
								break;
							case 5:
								keyToneFlag = !keyToneFlag;
								draw_OnOff(mmenu_attr,keyToneFlag,526,412);
								ProgramWRData[3] = keyToneFlag;
								//open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
								break;
							case 6:
								ChildLockFlag = !ChildLockFlag;
								draw_child_OnOff(mmenu_attr,ChildLockFlag,526,480);
								ProgramWRData[4] = ChildLockFlag;
								break;
						}
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						break;
					}
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					if (motorTestFlag) {
						manTestLevel++;
						if (manTestLevel > 19) {
							manTestLevel = 19;
						}
						draw_L_count(mmenu_attr,manTestLevel);
						break;
					}
					
					__msg("huang#######  fac3Count = %d \n",fac3Count);	
				
					fac3Count--;
						
					if (fac3Count < 0) {
						fac3Old = 0;
						fac3Count = 6;
					} else {
						fac3Old = fac3Count +1;
					}
					draw_mode(mmenu_attr,EPDK_FALSE,fac3Old);
					draw_mode(mmenu_attr,EPDK_TRUE,fac3Count);
					break;
				case 4:
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					disModeFlag = !disModeFlag;
					draw_disp_onoff(mmenu_attr,disModeFlag);
					ProgramWRData[1] = disModeFlag;
					break;
					
			}
			
			
			break;
		}
		case SLOW_KEY:
		case SLOW_LONG_KEY:
		case SLOW_LONG_QUICK_KEY:
		{
			reg_root_para_t *para;
			para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);
			
			switch (curFactory) {
				case 1:
				{
				#if 0
					if (focus == 1) {
						EMFlag = !EMFlag;
						draw_unitEM(mmenu_attr,EMFlag);
					}

					if (focus == 3){
						maxINC --;
						if (maxINC < 10) {
							maxINC = 10;
						}
						draw_maxINCData(mmenu_attr);
						para->maxINC = maxINC;
					}
				#endif
					break;
				}
				case 2:
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					if (!judeFlag) {
						if (factory2Count == 1 ) {
							factory2Count = 2;
							break;
						}
						if (factory2Count == 3) {
							factory2Count = 4;
							break;
						}
						factory2Count = 0;
					} else {
						yesNoFlag = !yesNoFlag;
						draw_jude(mmenu_attr,yesNoFlag);
					}
					break;
				case 3:
					//if (fac3Count!=5) {
						//open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					//}
					if (keyTestFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						keyTestValue = 18;
						draw_testKey_value(mmenu_attr);
						keyTestCount[17] = 1;
						break;
					}

					if (facExitFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						backHomeJudeFlag = !backHomeJudeFlag;
						draw_Exit_yes_no(mmenu_attr,backHomeJudeFlag);
						break;
					}
					if (enterFlag) {
						switch(fac3Count) {
							case 1:
								EMFlag = !EMFlag;
								draw_unitEM(mmenu_attr,EMFlag);
								ProgramWRData[0] = EMFlag;
								break;
							case 3:
								disModeFlag = !disModeFlag;
								draw_OnOff(mmenu_attr,disModeFlag,526,276);
								ProgramWRData[1] = disModeFlag;
								break;
							case 4:
								pauModeFlag = !pauModeFlag;
								draw_OnOff(mmenu_attr,pauModeFlag,526,344);
								ProgramWRData[2] = pauModeFlag;
								break;
							case 5:
								keyToneFlag = !keyToneFlag;
								draw_OnOff(mmenu_attr,keyToneFlag,526,412);
								ProgramWRData[3] = keyToneFlag;
								//open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
								break;
							case 6:
								ChildLockFlag = !ChildLockFlag;
								draw_child_OnOff(mmenu_attr,ChildLockFlag,526,480);
								ProgramWRData[4] = ChildLockFlag;
								break;
						}
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						break;
					}
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					if (motorTestFlag) {
						
						if (manTestLevel) {
							manTestLevel--;
							
						}
						draw_L_count(mmenu_attr,manTestLevel);
						break;
					}

					fac3Count++;
					__msg("huang#######  fac3Count = %d \n",fac3Count);	
					if (fac3Count == 7) {
						fac3Old = 6;
						fac3Count = 0;
					} else {
						fac3Old = fac3Count - 1;
					}
					
					draw_mode(mmenu_attr,EPDK_FALSE,fac3Old);
					draw_mode(mmenu_attr,EPDK_TRUE,fac3Count);
					break;
				case 4:
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					disModeFlag = !disModeFlag;
					draw_disp_onoff(mmenu_attr,disModeFlag);
					ProgramWRData[1] = disModeFlag;
					break;
				
			}
			
			break;
		}	
		case START_KEY:
		{
			
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 15;
				draw_testKey_value(mmenu_attr);
				keyTestCount[14] = 1;
				break;
			}
			switch(curFactory) {
				case 1:
					if(GUI_IsTimerInstalled(msg->h_deswin,POP_INIT_ID)) {
						break;
					}
					open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
					GUI_SetBkColor(0);
					GUI_ClearRect(0, 0, 1024, 600);
					calbrationINC(mmenu_attr->p_cmd_data);
					draw_calbration(mmenu_attr);
					break;
				case 2:
					break;
				case 3:
					break;
			}	
			break;
		}
		case STOP_KEY:
		{
			
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 17;
				draw_testKey_value(mmenu_attr);
				keyTestCount[16] = 1;
				break;
			}
			switch(curFactory) {
				case 1:
					if (ERRFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						GUI_SetBkColor(0x00000000);
						GUI_ClearRect(0, 0, 1024, 600);
						GUI_LyrWinDelete(lyr);	
						main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data);
					}
					break;
				case 2:
					if (!judeFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data);
					}
					break;
				case 3:
				{
					void *pbmp;
					if (motorTestFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						GUI_KillTimer(h_deswin_old, COUNTER_ID);
						motorTestFlag = EPDK_FALSE;
						GUI_SetBkColor(0x00000000);
						GUI_ClearRect(0, 0, 778, 301);
						GUI_LyrWinDelete(lyr);
						
						GUI_LyrWinSel(threeLyr);
						GUI_SetBkColor(0x00000000);
						//GUI_ClearRect(123, 150, 123+778, 150+301);
						GUI_ClearRect(0, 0, 1024, 600);
						pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
						GUI_BMP_Draw(pbmp,0,0);
						draw_keyTest(mmenu_attr, EPDK_FALSE);
						draw_unit(mmenu_attr, EPDK_FALSE);
						draw_ManualTest(mmenu_attr, EPDK_FALSE);
						draw_displayMode(mmenu_attr, EPDK_FALSE);
						draw_PauseMode(mmenu_attr, EPDK_FALSE);
						draw_keyTone(mmenu_attr, EPDK_FALSE);
						draw_ChildLock(mmenu_attr, EPDK_FALSE);
						draw_mode(mmenu_attr,EPDK_TRUE,fac3Count);
						GUI_LyrWinSel(mmenu_attr->layer);
						pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_1);
						GUI_BMP_Draw(pbmp,0,550);
						
						SetRunLevel(mmenu_attr->p_cmd_data,1);
						
						break;
					}
					if (!facExitFlag) {
						open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
						draw_fac_exit(mmenu_attr);
						pageData[0] = 0x60;//保存数据
						writePage(ProgramWRData,pageData);
					}
					break;
				}
			case 4:
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinSel(mmenu_attr->layer);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				pageData[0] = 0x60;//保存数据
				writePage(ProgramWRData,pageData);
				main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data);
				break;
			}
			break;
		}

		case MANUAL_KEY :
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 1;
				draw_testKey_value(mmenu_attr);
				keyTestCount[0] = 1;
				break;
			}
			break;
		case HILL_KEY:
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 2;
				draw_testKey_value(mmenu_attr);
				keyTestCount[1] = 1;
				break;
			}
			break;
		case FATBURN_KEY:
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 3;
				draw_testKey_value(mmenu_attr);
				keyTestCount[2] = 1;
				break;
			}
			break;
		case CARDIO_KEY:
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 4;
				draw_testKey_value(mmenu_attr);
				keyTestCount[3] = 1;
				break;
			}
			break;
		case STRENGTH_KEY:
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 5;
				draw_testKey_value(mmenu_attr);
				keyTestCount[4] = 1;
				break;
			}
			break;
		case INTERUAL_KEY:
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 6;
				draw_testKey_value(mmenu_attr);
				keyTestCount[5] = 1;
				break;
			}
			break;
		case USER1_KEY:
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 7;
				draw_testKey_value(mmenu_attr);
				keyTestCount[6] = 1;
				break;
			}
			break;
		case USER2_KEY:
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 8;
				draw_testKey_value(mmenu_attr);
				keyTestCount[7] = 1;
				break;
			}
			break;
		case HR1_KEY:
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 9;
				draw_testKey_value(mmenu_attr);
				keyTestCount[8] = 1;
				break;
			}
			break;
		case HR2_KEY:
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 10;
				draw_testKey_value(mmenu_attr);
				keyTestCount[9] = 1;
				break;
			}
			break;
		case DISPLAY_KEY:
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 11;
				draw_testKey_value(mmenu_attr);
				keyTestCount[10] = 1;
				break;
			}
			break;
		case FAN_KEY:
			if (keyTestFlag) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyToneFlag);
				keyTestValue = 12;
				draw_testKey_value(mmenu_attr);
				keyTestCount[11] = 1;
				break;
			}
			break;
		
	}


	return EPDK_OK;
}
	
/*
	
*/
static __s32 _factory_bar_Proc(__gui_msg_t *msg)
{
  //  home_uipara_t* home_ui_para;
   // __here__;
    
   
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
		
	
			break;
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
			uninit_factory_res(mmenu_attr);
			if (curFactory != 2) {
				GUI_LyrWinDelete(threeLyr);//删除按键数字图层
			}
			//if (!curFactory) {
				//SetRunMAXINC(mmenu_attr->p_cmd_data,maxINC);
			//}
			esMEMS_Bfree(mmenu_attr, sizeof(mmenu_attr_t));
			esMEMS_Bfree(mmenu_para, sizeof(mmenu_para_t));//esMEMS_Bfree(mmenu_para, sizeof(mmenu_para));//内存释放问题
	
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
			init_factory_res(mmenu_attr);
			gscene_bgd_set_state(BGD_STATUS_HIDE);
			mmenu_attr->font = mmenu_para->mmenu_font;
			mmenu_attr->layer = mmenu_para->layer;
			//mmenu_attr->item_w = home_ui_para->item_width;
			mmenu_attr->focus_item = mmenu_para->focus_id;
			mmenu_attr->first_item = 0;
			pageData[0] = 0x60;
			readPage(ProgramRDData,pageData);
			for(i=0 ;i<32; i++) {
				ProgramWRData[i] = ProgramRDData[i];
			}
			maxINC = 20;
		
			EMFlag = ProgramRDData[0];
			disModeFlag = ProgramRDData[1];
			pauModeFlag = ProgramRDData[2];
			keyToneFlag = ProgramRDData[3];
			ChildLockFlag = ProgramRDData[4];
			
			__msg("huang EMFlag = %d \n",EMFlag);
			__msg("huang disModeFlag = %d \n",disModeFlag);
			__msg("huang pauModeFlag = %d \n",pauModeFlag);
			__msg("huang keyToneFlag = %d \n",keyToneFlag);
			__msg("huang ChildLockFlag = %d \n",ChildLockFlag);
			
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
			init_data_flag();
    		paint_factory_all(mmenu_attr);
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
		__u8 i = 0;
		 __u8 keyTestTotal = 0;
		 void *pbmp;

		mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);
		if (!mmenu_attr)
		{
			__err("mmenu_attr is null...\n");
            return EPDK_OK;
		}

		if (msg->dwAddData1 == FAC3_TEST_ID) {
			switch(fac3Count) {
				case 0:
				{
					for (i = 0; i< 19; i++) {
						if (keyTestCount[i] == 1) {
							keyTestTotal ++;
						}
					}

					if (keyTestTotal == 19) {
						keyTestFlag = EPDK_FALSE;
						for (i=0 ; i<19; i++) {
							keyTestCount[i] = 0;
						}
						GUI_SetBkColor(0x00000000);
						GUI_ClearRect(526, 72, 562+32, 72+61);	
						//GUI_LyrWinDelete(threeLyr);//删除按键数字图层
						GUI_LyrWinSel(mmenu_attr->layer);
						pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_1);
						GUI_BMP_Draw(pbmp,0,550);
						GUI_KillTimer(msg->h_deswin, FAC3_TEST_ID); //杀掉检测是否按键全部按到定时器
						draw_ok(mmenu_attr,EPDK_TRUE,705,72);
					}
					break;
				}

			case 1:
			{
				break;
			}
			}
			
		}
		
        if (msg->dwAddData1 == POP_INIT_ID) {
			loopCount ++;
			if (loopCount == 24) {
				loopCount = 1;
			}
			draw_loop_Init(mmenu_attr);
			if (loopCount%5 == 0) {
				GetRunIncAdc(mmenu_attr->p_cmd_data);
				draw_cal_adc(mmenu_attr);
			}
			
			GetINCstatus(mmenu_attr->p_cmd_data);
			//esKRNL_TimeDly(10);
			if (mmenu_attr->p_cmd_data->INCstatus == 0) {
				GUI_KillTimer(msg->h_deswin, POP_INIT_ID);
				GetRunIncMAXAdc(mmenu_attr->p_cmd_data);
				GetRunIncMINAdc(mmenu_attr->p_cmd_data);
				pageData[0] = 0x60;
				ProgramWRData[5] = mmenu_attr->p_cmd_data->maxInc_ADC;; // MAX ADC
				ProgramWRData[6] = mmenu_attr->p_cmd_data->minInc_ADC; //MIN ADC
				writePage(ProgramWRData,pageData);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);		
				main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data);
			}

			if (mmenu_attr->p_cmd_data->INCstatus == 1) {
				GUI_KillTimer(msg->h_deswin, POP_INIT_ID);
				draw_ERR_3(mmenu_attr);
			}
		}

		if (msg->dwAddData1 == COUNTER_ID) {
			GetCounter(mmenu_attr->p_cmd_data);
			draw_counter_val(mmenu_attr);
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

__s32 factory_bar_win_create(H_WIN h_parent, mmenu_para_t *para)
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
	eLIBs_memset((void*)mmenu_para, 0, sizeof(mmenu_para_t));
	mmenu_para->layer = para->layer;
	curFactory = para->focus_id;
	
	

	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	"factory_bar_win",
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = FACTORY_BAR_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_factory_bar_Proc);
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

void factory_bar_win_delete(H_WIN mm_win)
{
	GUI_FrmWinDelete(mm_win);
}

void factory_bar_res_uninit(H_WIN hwin)
{    
    mmenu_attr_t * mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(hwin);

    if(mmenu_attr)
    {
        uninit_factory_res(mmenu_attr);
    }
}

void factory_bar_res_init(H_WIN hwin)
{
    mmenu_attr_t * mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(hwin);

    if(mmenu_attr)
    {
        init_factory_res(mmenu_attr);
    }
}

