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
#include "sports_hr_bar.h"
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

#define SPORTS_HR_TIME_ID		0x1002		//运行定时器
#define SPORTS_HR_PROFILE_ID  0x1005	//心形切换定时器
#define HR_BACK_HOME_ID		0x2000   //返回主界面定时器
#define HR_PAU_BACK_ID			0x2001 	//暂停模式返回主界面定时器

static __u16 RPMTme = 0;
static __bool RPM_Flag = EPDK_FALSE;
static H_WIN h_deswin_old; //保存创建的窗口
static __bool keyTonFlag;
static __bool level_incline_flag = EPDK_TRUE; //判断level和incline标志

static __u8 timeFlag = 0; //判断时间正记还是倒记的标志
static __u8 timeTmp = 0; //设置过来的时间保存
static __u16 RunTime = 0; //已运行的时间。。用来计算每段的时间
static __u32 sportsAllTime = 0;//运动过的所有时间
static __u16 Age;         
static __u16 weight;
static __u8 curLevel;
static __u8 fanFlag = 0;

static __u8  TargHR = 0;  //设置过来的目标心跳
static __bool HRSummaryInit = EPDK_FALSE;
static __u8  RunedMaxLevel;
static __u16 RunedMaxWatt;

static __u64 SumIncline;
static __u32 SumLevel;
static __u32 SumPace;
static __u32 SumPulse;
static __u32 SumRpm;
static __u32 SumWatt;
static __u32 SumSpd;
static __u16 SumCount;
static __u16 SP_Data[6] = {0};

static __bool BTSET_FLAG ;

static __u8 pageData[2];
static __u8 writeData[32];
static __u8 TotalData[32];
static __bool unitFlag;
static __bool pauseModeFlag;
static __u16 TotalTime ;
static __u16 TotalDist;

static __u8 incErrorCount;


static H_LYR lry; //半透明图层
static H_LYR lyrThree;
static H_LYR E2_Lyr;


static __u16  HrFlag = 0;   //心跳信号
static __u16  errCode; //出错信息

static __bool RpmFlag = EPDK_FALSE; //检查是否有RPM输入过

static __u8 MaxLevel = 17;

static __bool display_falg = EPDK_TRUE; //显示或者隐藏display标志位

static __u8 dispalyCount = 0; //display显示时间

static __u8 HRTooHighCount = 0;

static __u8 stopCount = 0;//stop键按下次数

static __u8 HRbackHomeCount = 0;

static __u8 SendBTData[30];

static __u8 BT_MIN;
static __u8 BT_SEC;
static __u16 BT_DIST;
static __u8 BT_CAL;
static __u8 BT_PLUSE;
static __u8 BT_RPM;
static __u16 BT_SPEED;
static __u16 BT_tmpSpeed;
static __u16 BT_WATT;
static __u8 BT_HR;
static __u8 BT_FOCUS;

static __u16 BT_TOTALTIME;
static __u16 BT_SUMDIST;
static __u16 BT_SUMCAL;
static __u16 BT_AVGSPEED;
static __u8 BT_AVGPULSE;
static __u8 BT_AVGINC;
static __u8 BT_AVGLEVEL;
static __u8 BT_AVGRPM;
static __u16 BT_AVGWATT;

static __bool bufferOn = EPDK_TRUE;

static __u8 curID;				//当前运行模式

static __bool reaStandby = EPDK_FALSE;
static __bool reaching = EPDK_FALSE;

static __bool E2ErrorFlag = EPDK_FALSE;
static __bool LevelError = EPDK_FALSE;

static __u16 oldDist = 0;
static __u16 runnedDist;

static __u8 HR_pulse;


static __bool mainTain = EPDK_FALSE;

static __bool reachTime60Flag = EPDK_FALSE;

static __bool reachTime75Flag = EPDK_FALSE;

static __bool reachLoop = EPDK_FALSE;

//static __bool BackHomeFlag = EPDK_FALSE;

static __bool HeartFlag = EPDK_FALSE;

static __bool hwWarring = EPDK_FALSE;

static __bool HRTooHigh = EPDK_FALSE;

static __bool HRReduce = EPDK_FALSE;

static __bool unitFlag = EPDK_FALSE;

static __bool FirstHRFlag = EPDK_TRUE;

static __bool YellowTarHR = EPDK_FALSE;
static __u8 YellowTarHRCount = 0;

static __u8 hrSigTime = 0;

//static __u16 BackHomeTime = 0;

static __u32 reduceTime = 60;

static __u32  reachTime = 0;

static __u32  reachTime_1 = 0;

static __u16 mainTainTime = 0;

static __s8 HRProfileTime = 0;


static __u16 HR0 = 0;

static __u16 HR60 = 0;

static __u16 HR75 = 0;

static __s16 Goal = 0;

static __s16 ROR1 = 0;

static __s16 ROR2 = 0;

#ifdef AC00280

#else ifdef AC00285
static __u16 STRIDE_INC_TABLE[13] = {
	180,185,190,195,200,205,210,215,220,225,230,235,240,
};

#endif



#ifdef AC00280
/*watt 对照表*/
static __u16 wattTable[20][22] = {
	{3,4,6,9,10,13,16,19,23,27,29,32,35,40,42,47,50,54,57,61,64,68},
	{3,5,7,10,11,14,17,21,25,30,32,35,38,43,46,51,53,57,62,63,68,73},
	{3,5,7,10,12,15,18,23,28,32,36,39,43,48,51,57,59,63,69,70,76,77},
	{3,6,8,11,13,16,21,26,31,36,40,44,48,53,57,63,66,72,77,79,85,90},
	{4,6,9,12,15,20,23,29,34,40,44,49,53,60,64,71,75,82,86,91,96,101},
	{4,6,10,14,17,22,27,32,39,45,50,56,61,67,72,80,84,92,97,103,108,114},
	{4,7,11,15,19,24,30,37,44,51,57,64,69,76,82,90,95,103,109,115,122,129},
	{5,8,12,17,21,27,34,42,50,58,65,73,78,87,93,103,107,114,123,130,137,144},
	{6,8,14,19,25,32,40,48,57,68,75,83,90,102,108,120,125,134,143,150,159,165},
	{6,10,15,23,29,38,46,55,66,79,88,98,106,116,126,141,144,156,165,173,183,189},
	{7,11,19,27,34,44,55,65,77,92,103,113,123,135,146,161,166,179,189,199,210,217},
	{8,14,22,31,40,51,64,76,91,107,120,133,142,157,170,188,193,208,220,232,243,252},
	{10,15,26,37,47,61,75,89,107,125,141,156,167,183,198,220,226,243,254,266,281,293},
	{11,20,30,44,55,72,89,105,126,148,166,184,195,215,232,258,263,282,296,306,326,337},
	{13,22,36,53,66,85,106,124,149,177,196,217,231,254,274,301,309,332,347,356,381,395},
	{16,27,43,61,78,100,124,146,175,206,228,250,268,295,317,344,357,381,398,413,437,457},
	{19,32,50,72,91,116,145,169,203,236,261,288,307,336,361,394,410,438,459,477,502,526},
	{22,38,58,83,106,135,168,198,236,273,302,328,351,387,415,450,470,502,524,544,574,598},
	{25,43,66,93,121,153,192,224,267,312,344,375,400,421,468,507,531,567,593,610,649,675},
	{27,47,70,99,129,164,203,248,293,330,363,398,430,465,502,537,570,604,637,666,698,730},

};

#else ifdef AC00285
static __u16 wattTable[20][23] = {
	{3,7,10,15,20,26,31,38,45,54,62,70,77,85,93,101,109,131,152,163,173,184,194},
	{3,8,12,18,24,31,37,46,54,64,74,83,92,102,112,123,134,156,177,190,202,214,226},
	{4,9,13,20,26,34,41,50,59,71,82,92,102,114,125,137,149,174,199,213,227,241,254},
	{4,9,14,22,29,38,47,57,67,80,92,104,116,129,141,155,168,193,218,233,248,263,278},
	{4,10,16,24,32,42,52,64,76,90,104,118,131,145,158,173,187,214,241,258,275,292,308},
	{5,11,17,27,36,47,57,71,85,100,114,128,142,158,174,191,208,236,263,282,300,318,336},
	{5,13,20,30,40,53,66,81,95,113,130,146,162,180,198,218,238,268,298,319,340,361,381},
	{6,14,22,34,46,61,75,92,109,128,146,165,184,204,223,244,265,299,333,357,380,403,426},
	{6,16,25,39,52,69,86,105,124,146,168,190,212,234,256,281,305,344,382,409,435,461,487},
	{7,18,28,43,58,77,96,118,140,166,192,217,241,266,290,318,346,385,424,454,483,512,541},
	{8,20,31,49,66,88,110,135,159,188,216,243,270,298,326,358,390,421,452,484,515,546,577},
	{9,22,35,55,75,100,125,152,178,210,242,273,303,335,366,400,434,469,503,538,573,608,642},
	{10,25,40,62,83,111,139,170,200,235,269,304,338,375,411,448,485,524,563,603,642,681,719},
	{11,28,44,69,94,125,155,189,223,261,298,336,374,414,453,494,535,578,621,665,708,751,793},
	{13,31,49,77,105,140,174,211,248,289,330,372,413,457,500,545,590,637,684,732,780,827,873},
	{14,35,56,87,118,156,193,234,275,321,366,412,457,505,553,603,652,704,756,809,862,914,965},
	{16,41,66,102,138,180,222,271,320,366,412,464,515,569,623,679,735,794,852,912,971,1029,1087},
	{18,46,73,112,150,197,244,291,337,398,458,515,572,632,692,754,816,882,947,1014,1080,1145,1210},
	{20,49,78,120,162,211,260,307,353,417,480,540,600,663,726,792,857,926,994,1064,1133,1201,1269},
	{22,53,84,129,174,222,270,320,370,437,503,566,629,695,761,830,898,970,1042,1115,1188,1259,1330},

};

#endif

#if 0

static __s32 sports_yellow_ID[] = {
	ID_HOME_TV_NUMBER_SPORTMODE_0_3_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_1_3_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_2_3_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_3_3_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_4_3_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_5_3_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_6_3_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_7_3_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_8_3_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_9_3_BMP,
};

static __s32 sports_time_ID[] = {
	ID_HOME_TV_NUMBER_SPORTMODE_TIME_0_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_TIME_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_TIME_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_TIME_3_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_TIME_4_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_TIME_5_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_TIME_6_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_TIME_7_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_TIME_8_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_TIME_9_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_TIME_COLON_BMP,
};

static __s32 sports_white_ID[] = {
	ID_HOME_TV_NUMBER_SPORTMODE_0_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_1_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_2_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_3_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_4_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_5_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_6_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_7_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_8_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_9_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_COLON_1_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_POINT_1_BMP,
};

static __s32 sports_blue_ID[] = {
	ID_HOME_TV_NUMBER_SPORTMODE_0_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_1_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_2_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_3_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_4_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_5_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_6_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_7_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_8_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_9_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_COLON_2_BMP,
	ID_HOME_TV_NUMBER_SPORTMODE_POINT_2_BMP,
};

static __s32 sports_hrprofile_ID[] = {
	ID_HOME_IMG_SPORTMODE_HRPROFILE_01_BMP,
	ID_HOME_IMG_SPORTMODE_HRPROFILE_02_BMP,
	ID_HOME_IMG_SPORTMODE_HRPROFILE_03_BMP,
	ID_HOME_IMG_SPORTMODE_HRPROFILE_04_BMP,
	ID_HOME_IMG_SPORTMODE_HRPROFILE_05_BMP,
	ID_HOME_IMG_SPORTMODE_HRPROFILE_06_BMP,
	ID_HOME_IMG_SPORTMODE_HRPROFILE_07_BMP,
	ID_HOME_IMG_SPORTMODE_HRPROFILE_08_BMP,
	ID_HOME_IMG_SPORTMODE_HRPROFILE_09_BMP,
	ID_HOME_IMG_SPORTMODE_HRPROFILE_10_BMP,
};

static __s32 HRProfile_ID[] = {
	ID_HOME_TV_SPORTMODE_HR_1_BMP,
	ID_HOME_TV_SPORTMODE_HR_2_BMP,
	ID_HOME_TV_SPORTMODE_HR_3_BMP,
	ID_HOME_TV_SPORTMODE_HR_4_BMP,
	ID_HOME_TV_SPORTMODE_HR_CHECK_PULSE_BMP,
};

static __s32 displayNum_ID[] = {
	ID_HOME_TV_NUMBER_DISPLAY_0_BMP,
	ID_HOME_TV_NUMBER_DISPLAY_1_BMP,
	ID_HOME_TV_NUMBER_DISPLAY_2_BMP,
	ID_HOME_TV_NUMBER_DISPLAY_3_BMP,
	ID_HOME_TV_NUMBER_DISPLAY_4_BMP,
	ID_HOME_TV_NUMBER_DISPLAY_5_BMP,
	ID_HOME_TV_NUMBER_DISPLAY_6_BMP,
	ID_HOME_TV_NUMBER_DISPLAY_7_BMP,
	ID_HOME_TV_NUMBER_DISPLAY_8_BMP,
	ID_HOME_TV_NUMBER_DISPLAY_9_BMP,
	ID_HOME_TV_NUMBER_DISPLAY_COLON_BMP,
	ID_HOME_TV_NUMBER_DISPLAY_POINT_BMP,
};
 
static __s32  summaryNum_ID [] = {
	ID_HOME_TV_NUMBER_SUMMARY_0_BMP,
	ID_HOME_TV_NUMBER_SUMMARY_1_BMP,
	ID_HOME_TV_NUMBER_SUMMARY_2_BMP,
	ID_HOME_TV_NUMBER_SUMMARY_3_BMP,
	ID_HOME_TV_NUMBER_SUMMARY_4_BMP,
	ID_HOME_TV_NUMBER_SUMMARY_5_BMP,
	ID_HOME_TV_NUMBER_SUMMARY_6_BMP,
	ID_HOME_TV_NUMBER_SUMMARY_7_BMP,
	ID_HOME_TV_NUMBER_SUMMARY_8_BMP,
	ID_HOME_TV_NUMBER_SUMMARY_9_BMP,
	ID_HOME_TV_NUMBER_SUMMARY_COLON_BMP,
	ID_HOME_TV_NUMBER_SUMMARY_POINT_BMP,		
};

static __s32 summaryUnit_ID [] = {
	ID_HOME_TV_UNIT_SUMMARY_BPM_BMP,
	ID_HOME_TV_UNIT_SUMMARY_FT_BMP,
	ID_HOME_TV_UNIT_SUMMARY_KCAL_BMP,
	ID_HOME_TV_UNIT_SUMMARY_KMPH_BMP,
	ID_HOME_TV_UNIT_SUMMARY_KM_BMP,
	ID_HOME_TV_UNIT_SUMMARY_MILE_BMP,
	ID_HOME_TV_UNIT_SUMMARY_MIN_BMP,
	ID_HOME_TV_UNIT_SUMMARY_MPH_BMP,
	ID_HOME_TV_UNIT_SUMMARY_M_BMP,
};

#endif

/*
	申请资源
*/
static __s32 init_sports_hr_res(mmenu_attr_t *mmenu_attr)
{
    __u32 	i;    
  
    if(EPDK_TRUE == mmenu_attr->res_init)
    {
        return EPDK_OK;
    }
  
	//for(i = 0 ; i < 12; i++)
	//{
		mmenu_attr->displayNum_bmp[0] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_0_BMP);
		mmenu_attr->displayNum_bmp[1] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_1_BMP);
		mmenu_attr->displayNum_bmp[2] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_2_BMP);
		mmenu_attr->displayNum_bmp[3] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_3_BMP);
		mmenu_attr->displayNum_bmp[4] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_4_BMP);
		mmenu_attr->displayNum_bmp[5] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_5_BMP);
		mmenu_attr->displayNum_bmp[6] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_6_BMP);
		mmenu_attr->displayNum_bmp[7] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_7_BMP);
		mmenu_attr->displayNum_bmp[8] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_8_BMP);
		mmenu_attr->displayNum_bmp[9] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_9_BMP);
		mmenu_attr->displayNum_bmp[10] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_COLON_BMP);
		mmenu_attr->displayNum_bmp[11] = dsk_theme_open(ID_HOME_TV_NUMBER_DISPLAY_POINT_BMP);
	//}

	
	//for(i = 0 ; i < 10; i++)
	//	{
		mmenu_attr->sports_yellow_bmp[0] = dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_0_3_BMP);
		mmenu_attr->sports_yellow_bmp[1] = dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_1_3_BMP);
		mmenu_attr->sports_yellow_bmp[2] = dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_2_3_BMP);
		mmenu_attr->sports_yellow_bmp[3] = dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_3_3_BMP);
		mmenu_attr->sports_yellow_bmp[4] = dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_4_3_BMP);
		mmenu_attr->sports_yellow_bmp[5] = dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_5_3_BMP);
		mmenu_attr->sports_yellow_bmp[6] = dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_6_3_BMP);
		mmenu_attr->sports_yellow_bmp[7] = dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_7_3_BMP);
		mmenu_attr->sports_yellow_bmp[8] = dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_8_3_BMP);
		mmenu_attr->sports_yellow_bmp[9] = dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_9_3_BMP);
		
	//	}

	//for(i = 0 ; i < 12; i++)
	//{
		
	//}

	//for(i = 0; i < 9; i++) {
		
	//}
	
	//for(i = 0 ; i < 11; i++)
	//{
		mmenu_attr->sports_time_bmp[0]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_TIME_0_BMP);
		mmenu_attr->sports_time_bmp[1]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_TIME_1_BMP);
		mmenu_attr->sports_time_bmp[2]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_TIME_2_BMP);
		mmenu_attr->sports_time_bmp[3]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_TIME_3_BMP);
		mmenu_attr->sports_time_bmp[4]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_TIME_4_BMP);
		mmenu_attr->sports_time_bmp[5]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_TIME_5_BMP);
		mmenu_attr->sports_time_bmp[6]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_TIME_6_BMP);
		mmenu_attr->sports_time_bmp[7]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_TIME_7_BMP);
		mmenu_attr->sports_time_bmp[8]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_TIME_8_BMP);
		mmenu_attr->sports_time_bmp[9]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_TIME_9_BMP);
		mmenu_attr->sports_time_bmp[10]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_TIME_COLON_BMP);
	//}

	//for(i = 0 ; i < 12; i++)
	//{
		mmenu_attr->sports_white_bmp[0]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_0_1_BMP);
		mmenu_attr->sports_white_bmp[1]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_1_1_BMP);
		mmenu_attr->sports_white_bmp[2]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_2_1_BMP);
		mmenu_attr->sports_white_bmp[3]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_3_1_BMP);
		mmenu_attr->sports_white_bmp[4]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_4_1_BMP);
		mmenu_attr->sports_white_bmp[5]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_5_1_BMP);
		mmenu_attr->sports_white_bmp[6]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_6_1_BMP);
		mmenu_attr->sports_white_bmp[7]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_7_1_BMP);
		mmenu_attr->sports_white_bmp[8]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_8_1_BMP);
		mmenu_attr->sports_white_bmp[9]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_9_1_BMP);
		mmenu_attr->sports_white_bmp[10]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_COLON_1_BMP);
		mmenu_attr->sports_white_bmp[11]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_POINT_1_BMP);
	//}

	//for(i = 0 ; i < 12; i++)
	//{
		mmenu_attr->sports_blue_bmp[0]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_0_2_BMP);
		mmenu_attr->sports_blue_bmp[1]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_1_2_BMP);
		mmenu_attr->sports_blue_bmp[2]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_2_2_BMP);
		mmenu_attr->sports_blue_bmp[3]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_3_2_BMP);
		mmenu_attr->sports_blue_bmp[4]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_4_2_BMP);
		mmenu_attr->sports_blue_bmp[5]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_5_2_BMP);
		mmenu_attr->sports_blue_bmp[6]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_6_2_BMP);
		mmenu_attr->sports_blue_bmp[7]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_7_2_BMP);
		mmenu_attr->sports_blue_bmp[8]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_8_2_BMP);
		mmenu_attr->sports_blue_bmp[9]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_9_2_BMP);
		mmenu_attr->sports_blue_bmp[10]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_COLON_2_BMP);
		mmenu_attr->sports_blue_bmp[11]= dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_POINT_2_BMP);
	//}

	//for(i = 0 ; i < 10; i++)
	//{
		mmenu_attr->sports_hrprofile_bmp[0]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_HRPROFILE_01_BMP);
		mmenu_attr->sports_hrprofile_bmp[1]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_HRPROFILE_02_BMP);
		mmenu_attr->sports_hrprofile_bmp[2]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_HRPROFILE_03_BMP);
		mmenu_attr->sports_hrprofile_bmp[3]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_HRPROFILE_04_BMP);
		mmenu_attr->sports_hrprofile_bmp[4]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_HRPROFILE_05_BMP);
		mmenu_attr->sports_hrprofile_bmp[5]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_HRPROFILE_06_BMP);
		mmenu_attr->sports_hrprofile_bmp[6]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_HRPROFILE_07_BMP);
		mmenu_attr->sports_hrprofile_bmp[7]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_HRPROFILE_08_BMP);
		mmenu_attr->sports_hrprofile_bmp[8]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_HRPROFILE_09_BMP);
		mmenu_attr->sports_hrprofile_bmp[9]= dsk_theme_open(ID_HOME_IMG_SPORTMODE_HRPROFILE_10_BMP);
	//}
	
	//for(i = 0 ; i < 5; i++)
	//{
		mmenu_attr->HRProfile_bmp[0]= dsk_theme_open(ID_HOME_TV_SPORTMODE_HR_1_BMP);
		mmenu_attr->HRProfile_bmp[1]= dsk_theme_open(ID_HOME_TV_SPORTMODE_HR_2_BMP);
		mmenu_attr->HRProfile_bmp[2]= dsk_theme_open(ID_HOME_TV_SPORTMODE_HR_3_BMP);
		mmenu_attr->HRProfile_bmp[3]= dsk_theme_open(ID_HOME_TV_SPORTMODE_HR_4_BMP);
		mmenu_attr->HRProfile_bmp[4]= dsk_theme_open(ID_HOME_TV_SPORTMODE_HR_CHECK_PULSE_BMP);
	//}
	
	mmenu_attr->bt_bmp = dsk_theme_open(ID_HOME_IMG_BT_BMP);
	mmenu_attr->sports_hrWar_bmp = dsk_theme_open(ID_HOME_IMG_POP_HR_WARNNING_BMP);
	mmenu_attr->targTime_bmp = dsk_theme_open(ID_HOME_IMG_POP_TARGET_TIME_BMP);
	mmenu_attr->inc_error_bmp = dsk_theme_open(ID_HOME_TV_NUMBER_SPORTMODE_ERROR_BMP);
#ifdef AC00280
	mmenu_attr->sports_incline_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_INCLINE_BMP);
#else ifdef AC00285
	mmenu_attr->sports_incline_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_STRIDE_BMP);
#endif
	
	mmenu_attr->stop_bmp = dsk_theme_open(ID_HOME_IMG_POP_STOP_BMP);
	mmenu_attr->pause_bmp = dsk_theme_open(ID_HOME_IMG_POP_PAUSE_BMP);
	mmenu_attr->display_bmp = dsk_theme_open(ID_HOME_IMG_POP_DISPLAY_3_BMP);
	mmenu_attr->sports_rpm_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_RPM_BMP);
	mmenu_attr->sports_targHR_bmp[0] = dsk_theme_open(ID_HOME_IMG_SPORTMODE_TARGETHR_1_BMP);
	mmenu_attr->sports_targHR_bmp[1] = dsk_theme_open(ID_HOME_IMG_SPORTMODE_TARGETHR_2_BMP);
	mmenu_attr->sports_distance_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_DISTANCE_BMP);
	mmenu_attr->sports_pace_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_PACE_BMP);
	mmenu_attr->sports_level_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_LEVEL_BMP);
	//mmenu_attr->sports_incline_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_INCLINE_BMP);
	mmenu_attr->sports_pulse_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_PULSE_BMP);
	mmenu_attr->sports_hr_bmp= dsk_theme_open(ID_HOME_IMG_SPORTMODE_HR_BMP);
	mmenu_attr->sports_calories_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_CALORIES_BMP);
	mmenu_attr->sports_watts_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_WATTS_BMP);
	
	mmenu_attr->title_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_BMP);
	mmenu_attr->message_bmp_2 = dsk_theme_open(ID_HOME_TV_MESSAGE_PAUSE_BMP);
	mmenu_attr->E2_ERROR_bmp = dsk_theme_open(ID_HOME_IMG_POP_ERROR_2_BMP);
	mmenu_attr->error_message_bmp = dsk_theme_open(ID_HOME_TV_MESSAGE_ERROR_BMP);

	switch(curID) {
		case ID_HR1:
			mmenu_attr->message_bmp_1 = dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROGRAM_HR1_BMP);
			mmenu_attr->sports_hrProfile_1_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_HR1_1_BMP);
			mmenu_attr->sports_hrProfile_2_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_HR1_2_BMP);
			break;
		case ID_HR2:
			mmenu_attr->message_bmp_1 = dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROGRAM_HR2_BMP);
			mmenu_attr->sports_hrProfile_1_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_HR2_1_BMP);
			mmenu_attr->sports_hrProfile_2_bmp = dsk_theme_open(ID_HOME_IMG_SPORTMODE_PROFILE_HR2_2_BMP);
			break;
	}
	
    mmenu_attr->res_init = EPDK_TRUE;
	 
	return EPDK_OK;
}

static __s32 init_hr_summary_res(mmenu_attr_t *mmenu_attr)
{
	 if(EPDK_TRUE == HRSummaryInit)
    {
        return EPDK_OK;
    }
	mmenu_attr->summary_num_bmp[0] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_0_BMP);
	mmenu_attr->summary_num_bmp[1] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_1_BMP);
	mmenu_attr->summary_num_bmp[2] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_2_BMP);
	mmenu_attr->summary_num_bmp[3] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_3_BMP);
	mmenu_attr->summary_num_bmp[4] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_4_BMP);
	mmenu_attr->summary_num_bmp[5] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_5_BMP);
	mmenu_attr->summary_num_bmp[6] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_6_BMP);
	mmenu_attr->summary_num_bmp[7] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_7_BMP);
	mmenu_attr->summary_num_bmp[8] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_8_BMP);
	mmenu_attr->summary_num_bmp[9] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_9_BMP);
	mmenu_attr->summary_num_bmp[10] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_COLON_BMP);
	mmenu_attr->summary_num_bmp[11] = dsk_theme_open(ID_HOME_TV_NUMBER_SUMMARY_POINT_BMP);

	mmenu_attr->summaryUnit_bmp[0] = dsk_theme_open(ID_HOME_TV_UNIT_SUMMARY_BPM_BMP);
	mmenu_attr->summaryUnit_bmp[1] = dsk_theme_open(ID_HOME_TV_UNIT_SUMMARY_FT_BMP);
	mmenu_attr->summaryUnit_bmp[2] = dsk_theme_open(ID_HOME_TV_UNIT_SUMMARY_KCAL_BMP);
	mmenu_attr->summaryUnit_bmp[3] = dsk_theme_open(ID_HOME_TV_UNIT_SUMMARY_KMPH_BMP);
	mmenu_attr->summaryUnit_bmp[4] = dsk_theme_open(ID_HOME_TV_UNIT_SUMMARY_KM_BMP);
	mmenu_attr->summaryUnit_bmp[5] = dsk_theme_open(ID_HOME_TV_UNIT_SUMMARY_MILE_BMP);
	mmenu_attr->summaryUnit_bmp[6] = dsk_theme_open(ID_HOME_TV_UNIT_SUMMARY_MIN_BMP);
	mmenu_attr->summaryUnit_bmp[7] = dsk_theme_open(ID_HOME_TV_UNIT_SUMMARY_MPH_BMP);
	mmenu_attr->summaryUnit_bmp[8] = dsk_theme_open(ID_HOME_TV_UNIT_SUMMARY_M_BMP);

	mmenu_attr->welcome_bmp= dsk_theme_open(ID_HOME_IMG_POP_SAVING_BMP);//save
	mmenu_attr->summary_message_bmp = dsk_theme_open(ID_HOME_TV_MESSAGE_SUMMARY_2_BMP);
	#ifdef AC00280
	mmenu_attr->summary_bmp = dsk_theme_open(ID_HOME_IMG_SUMMARY_BMP);
	#else ifdef AC00285
	mmenu_attr->summary_bmp = dsk_theme_open(ID_HOME_IMG_SUMMARY_285_BMP);
	#endif

	HRSummaryInit = EPDK_TRUE;

	return EPDK_OK;
}

static __s32 uninit_HR_summary_res(mmenu_attr_t *mmenu_attr)
{
	__u8 i;

	 if(EPDK_FALSE == HRSummaryInit)
    {
        return EPDK_OK;
    }
	for (i = 0; i < 12; i++) 
	{
		dsk_theme_close(mmenu_attr->summary_num_bmp[i]); 
	}

	for (i = 0; i < 9; i++) 
	{
		dsk_theme_close(mmenu_attr->summaryUnit_bmp[i]); 
	}

	dsk_theme_close(mmenu_attr->summary_bmp);
	dsk_theme_close(mmenu_attr->summary_message_bmp);
	dsk_theme_close(mmenu_attr->welcome_bmp);
	HRSummaryInit = EPDK_FALSE;
	return EPDK_OK;
}

/*
	释放资源
*/
static __s32 uninit_sports_hr_res(mmenu_attr_t *mmenu_attr)
{
	  __u32 	i; 

    if(EPDK_FALSE == mmenu_attr->res_init)
    {
        return EPDK_OK;
    }

	for(i = 0 ; i < 11; i++)
	{
		dsk_theme_close(mmenu_attr->sports_time_bmp[i]);
	}

	for(i = 0 ; i < 12; i++)
	{
		dsk_theme_close(mmenu_attr->sports_white_bmp[i]);
	}

	for(i = 0 ; i < 12; i++)
	{
		dsk_theme_close(mmenu_attr->sports_blue_bmp[i]);
	}

	for(i = 0 ; i < 10; i++)
	{
		dsk_theme_close(mmenu_attr->sports_hrprofile_bmp[i]);
	}

	for(i = 0 ; i < 5; i++)
	{
		dsk_theme_close(mmenu_attr->HRProfile_bmp[i]);
	}

	

	for (i = 0; i < 12; i++) 
	{
		dsk_theme_close(mmenu_attr->displayNum_bmp[i]); 
	}

	for (i = 0; i < 10; i++) 
	{
		dsk_theme_close(mmenu_attr->sports_yellow_bmp[i]); 
	}

	
	dsk_theme_close(mmenu_attr->bt_bmp);
	
	dsk_theme_close(mmenu_attr->sports_rpm_bmp);
	dsk_theme_close(mmenu_attr->sports_distance_bmp);
	dsk_theme_close(mmenu_attr->sports_pace_bmp);
	dsk_theme_close(mmenu_attr->sports_incline_bmp); 
	dsk_theme_close(mmenu_attr->sports_level_bmp);
	dsk_theme_close(mmenu_attr->sports_pulse_bmp);
	dsk_theme_close(mmenu_attr->sports_hr_bmp);
	dsk_theme_close(mmenu_attr->sports_calories_bmp);
	dsk_theme_close(mmenu_attr->sports_watts_bmp); 
	dsk_theme_close(mmenu_attr->message_bmp_1); 
	dsk_theme_close(mmenu_attr->message_bmp_2);
	dsk_theme_close(mmenu_attr->title_bmp);
	dsk_theme_close(mmenu_attr->stop_bmp);
	dsk_theme_close(mmenu_attr->display_bmp);
	dsk_theme_close(mmenu_attr->pause_bmp);
	dsk_theme_close(mmenu_attr->targTime_bmp);
	dsk_theme_close(mmenu_attr->sports_targHR_bmp[0]);
	dsk_theme_close(mmenu_attr->sports_targHR_bmp[1]);
	dsk_theme_close(mmenu_attr->sports_hrProfile_1_bmp);
	dsk_theme_close(mmenu_attr->sports_hrProfile_2_bmp);
	dsk_theme_close(mmenu_attr->sports_hrWar_bmp);
	dsk_theme_close(mmenu_attr->inc_error_bmp);
	dsk_theme_close(mmenu_attr->E2_ERROR_bmp);
	dsk_theme_close(mmenu_attr->error_message_bmp);
	//dsk_theme_close(mmenu_attr->curProfile_bmp);
	
    mmenu_attr->res_init = EPDK_FALSE;
    
	return EPDK_OK;
}

static void draw_save(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);
	GUI_SetBkColor(0x00000000);
	GUI_ClearRect(0, 70, 1024, 600);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->welcome_bmp);
	GUI_BMP_Draw(pbmp,123,150);

	//pbmp = dsk_theme_hdl2buf(mmenu_attr->curProfile_bmp);
	//GUI_BMP_Draw(pbmp,454,220);
}


static void draw_summary_time(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 191;
	__s32 setNUM_y = 44;
	void *pbmp;
	__s32 min;
	__s32 sec;

	
	min = sportsAllTime / 60;
	sec = sportsAllTime % 60;

	if (sportsAllTime < 6000) {
		if (min > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[min/10]);
			GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		
		} else {
			GUI_SetBkColor(0x00000000);

			GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19, setNUM_y+62);
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[min%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19+19,setNUM_y);
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[sec/10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19*2+13,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[sec%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19*3+13,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->summaryUnit_bmp[6]);
		GUI_BMP_Draw(pbmp,303,setNUM_y);
	}else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[min/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[min%100/10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[min%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19*2,setNUM_y);
	}
	BT_TOTALTIME = sportsAllTime;
}


static void draw_summary_avgPace(mmenu_attr_t *mmenu_attr)
{
	//__s32 setNUM_x = 555;
	//__s32 setNUM_y = 294;
	__s32 setNUM_x = 191;
	__s32 setNUM_y = 294;
	void *pbmp;
	__s32 min;
	__s32 sec;
	__s32 avgPace;

	if (sportsAllTime != 0) {
		avgPace = SumPace / sportsAllTime;
	} else {
		avgPace = 0;
	}
	min = avgPace / 60;
	sec = avgPace % 60;

	if (min > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[min/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

	
	} else {
		GUI_SetBkColor(0x00000000);

		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19, setNUM_y+62);
	}
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[min%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[10]);
	GUI_BMP_Draw(pbmp,setNUM_x+19+19,setNUM_y);
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[sec/10]);
	GUI_BMP_Draw(pbmp,setNUM_x+19*2+13,setNUM_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[sec%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+19*3+13,setNUM_y);
}

static void draw_summary_avgSpeed(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 555;
	__s32 setNUM_y = 44;
	void *pbmp;
	__u16 avgSpd;
	
 
	if (sportsAllTime != 0) {
		avgSpd = SumSpd/sportsAllTime;
	} else {
		avgSpd = 0;
		}

	if (avgSpd > 99) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgSpd/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		
	} else {
		GUI_SetBkColor(0x00000000);

		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19, setNUM_y+62);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgSpd%100/10]);
	GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[11]);
	GUI_BMP_Draw(pbmp,setNUM_x+19+19,setNUM_y);

	/*小数部分*/
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgSpd%100%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+19*2+19,setNUM_y);

	if (!unitFlag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summaryUnit_bmp[7]);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summaryUnit_bmp[3]);
	}
	GUI_BMP_Draw(pbmp,648,setNUM_y);

	BT_AVGSPEED =  avgSpd;

}


static void draw_summary_distance(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 191;
	__s32 setNUM_y = 127;
	void *pbmp;
	__u16 distance_decimal; //小数
	__u16 distance_int;		//整数


	if (!unitFlag) {
		distance_decimal = mmenu_attr->sports_data[1] * 5 / 80000 ;
	} else {
		distance_decimal = mmenu_attr->sports_data[1] / 10000 ;
	}

	if (distance_decimal > 9 && distance_decimal <100) {
		distance_decimal = distance_decimal%10;
	} else if (distance_decimal >= 100) {
		distance_decimal = distance_decimal % 100 % 10;
	} else if (distance_decimal > 999) {
		mmenu_attr->sports_data[1] = 0;
	}

	if (!unitFlag) {
		distance_int  = mmenu_attr->sports_data[1] * 5 / 800000;
	} else {
		distance_int  = mmenu_attr->sports_data[1] / 100000;
	}
	
	/*整数部分*/
	if (distance_int > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[distance_int/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		
	} else {
		GUI_SetBkColor(0x00000000);

		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19, setNUM_y+62);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[distance_int%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[11]);
	GUI_BMP_Draw(pbmp,setNUM_x+19+19,setNUM_y);

	/*小数部分*/
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[distance_decimal]);
	GUI_BMP_Draw(pbmp,setNUM_x+19*2+13,setNUM_y);

	if(!unitFlag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summaryUnit_bmp[5]);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summaryUnit_bmp[4]);
	}
	BT_SUMDIST = distance_int*100 + distance_decimal*10;
	GUI_BMP_Draw(pbmp,303,setNUM_y);
}

static void draw_summary_avgIncline(mmenu_attr_t *mmenu_attr)
{
	#ifdef AC00280
	__s32 setNUM_x = 555;
	__s32 setNUM_y = 127;
	void *pbmp;
	__u8 avgIncline;

	if (SumCount != 0) {
		avgIncline = SumIncline / SumCount;
	} else {
		avgIncline = 1;
	}
	if (avgIncline > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgIncline/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);

		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19, setNUM_y+62);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgIncline%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
#else ifdef AC00285
	__s32 setNUM_x = 555;
	__s32 setNUM_y = 127;
	void *pbmp;
	__u8 avgIncline;

	if (sportsAllTime != 0) {
		avgIncline = SumIncline / sportsAllTime;
	} else {
		avgIncline = 0;
	}
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgIncline/100]);
	GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgIncline%100/10]);
	GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[11]);
	GUI_BMP_Draw(pbmp,setNUM_x+19*2,setNUM_y);
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgIncline%100%10]);
	GUI_BMP_Draw(pbmp,606,setNUM_y);
#endif
BT_AVGINC = avgIncline;

}


static void draw_summary_calories(mmenu_attr_t *mmenu_attr)
{
	
	__s32 setNUM_x = 191;
	__s32 setNUM_y = 210;
	void *pbmp;
	__s32 curCORL;
	

	curCORL = mmenu_attr->sports_data[8] / 100000;
	
	if (curCORL > 999) {
		curCORL = 0;
		mmenu_attr->sports_data[8] = 0;
	}
	if(curCORL >= 100)
	{
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[curCORL/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[curCORL/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[curCORL%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19*2,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19, setNUM_y+62);
		if (curCORL > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[curCORL/10]);
			GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
		} else {
			GUI_SetBkColor(0x00000000);

			GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19*2, setNUM_y+62);
		}
		
	}
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[curCORL%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+19*2,setNUM_y);
	BT_SUMCAL = curCORL;
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summaryUnit_bmp[2]);
	GUI_BMP_Draw(pbmp,303,setNUM_y);
}

static void draw_summary_avgLevel(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 555;
	__s32 setNUM_y = 210;
	void *pbmp;
	__u8 avgLevel;

	if (SumCount != 0) {
		avgLevel = SumLevel/ SumCount;
	} else {
		avgLevel = 1;
	}
	
	if (avgLevel > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgLevel/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);

		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19, setNUM_y+62);
	}
	BT_AVGLEVEL = avgLevel;
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgLevel%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
}


static void draw_summary_avgWatt(mmenu_attr_t *mmenu_attr)
{
	//__s32 setNUM_x = 191;
	//__s32 setNUM_y = 294;
	__s32 setNUM_x = 555;
	__s32 setNUM_y = 294;
	void *pbmp;
	__u32 avgWatt;

	
	if (sportsAllTime != 0) {
		avgWatt= SumWatt/ sportsAllTime;
	} else {
		avgWatt= 0;
	}
	
	if(avgWatt >= 100)
	{
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgWatt/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgWatt/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgWatt%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19*2,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19, setNUM_y+62);
		if (avgWatt > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgWatt/10]);
			GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
		} else {
			GUI_SetBkColor(0x00000000);

			GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19*2, setNUM_y+62);
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgWatt%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19*2,setNUM_y);
	}
	BT_AVGWATT = avgWatt;
}

static void draw_summary_avgRpm(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 191;
	__s32 setNUM_y = 377;
	void *pbmp;
	__u8 avgRpm;

	
	if (sportsAllTime != 0) {
		avgRpm= SumRpm/ sportsAllTime;
	} else {
		avgRpm= 0;
	}
	if(avgRpm >= 100)
	{
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgRpm/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgRpm/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgRpm%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19*2,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19, setNUM_y+62);
		if (avgRpm > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgRpm/10]);
			GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
		} else {
			GUI_SetBkColor(0x00000000);

			GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19*2, setNUM_y+62);
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgRpm%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19*2,setNUM_y);
	}
	BT_AVGRPM = avgRpm;

}

static void draw_summary_avgPulse(mmenu_attr_t *mmenu_attr)
{	
	__s32 setNUM_x = 555;
	__s32 setNUM_y = 377;
	void *pbmp;
	__u8 avgPluse;

	
	if (sportsAllTime != 0) {
		avgPluse = SumPulse/ sportsAllTime;
	} else {
		avgPluse = 0;
	}
	if(avgPluse >= 100)
	{
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgPluse/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgPluse/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgPluse%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19*2,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19, setNUM_y+62);
		if (avgPluse > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgPluse/10]);
			GUI_BMP_Draw(pbmp,setNUM_x+19,setNUM_y);
		} else {
			GUI_SetBkColor(0x00000000);

			GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+19*2, setNUM_y+62);
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_num_bmp[avgPluse%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+19*2,setNUM_y);
		
	}
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summaryUnit_bmp[0]);
	GUI_BMP_Draw(pbmp,648,setNUM_y);
	BT_AVGPULSE = avgPluse;
}

static void SendBTSummary()
{
	__u8 i;
	
	SendBTData[0] = BT_HEAD;
	SendBTData[1] = 0x11;
	SendBTData[2] = SUMMAY_CMD;
	SendBTData[3] = (BT_TOTALTIME>>8)&0xFF;
	SendBTData[4] = BT_TOTALTIME&0xFF;
	SendBTData[5] = (BT_SUMDIST>>8)&0xFF;
	SendBTData[6] = BT_SUMDIST&0xFF;
	SendBTData[7] = (BT_SUMCAL>>8)&0xFF;
	SendBTData[8] = BT_SUMCAL&0xFF;
	SendBTData[9] = (BT_AVGSPEED>>8)&0xFF;
	SendBTData[10] = BT_AVGSPEED&0xFF;
	SendBTData[11] = BT_AVGPULSE;
	SendBTData[12] = BT_AVGINC;
	SendBTData[13] = BT_AVGLEVEL;
	SendBTData[14] = BT_AVGRPM;
	SendBTData[15] =  (BT_AVGWATT>>8)&0xFF;
	SendBTData[16] = BT_AVGWATT&0xFF;;
	SendBTData[17] = 0x00;
	SendBTData[18] = 0x00;
	SendBTData[19] = BT_END;

	//for(i=0;i<20;i++) {
		//__msg("huang####   Summary[%d] = 0x%x   \n",i,SendBTData[i]);
	//}

	bluetooth_writebuf(SendBTData,20);

}


static void draw_summary(mmenu_attr_t *mmenu_attr) 
{
	
	RECT	rect;
	
	rect.x = 142;
	rect.y = 47;
	rect.width = 741;
	rect.height = 447;
	lyrThree = home_32bpp_layer_create(&rect, 1);
	GUI_LyrWinSetSta(lyrThree, GUI_LYRWIN_STA_ON);
	GUI_LyrWinSel(lyrThree);
	GUI_LyrWinSetTop(lyrThree);

	GUI_SetBkColor(0x00000000);	
	GUI_ClearRect(142, 47, 883, 494);

	draw_summary_time(mmenu_attr);
	draw_summary_avgSpeed(mmenu_attr);
	draw_summary_distance(mmenu_attr);
	draw_summary_avgIncline(mmenu_attr);
	draw_summary_calories(mmenu_attr);
	draw_summary_avgLevel(mmenu_attr);
	draw_summary_avgWatt(mmenu_attr);
	draw_summary_avgPace(mmenu_attr);
	draw_summary_avgRpm(mmenu_attr);
	draw_summary_avgPulse(mmenu_attr);

	if (BTSET_FLAG) {
		Send_BT_status(7);
		esKRNL_TimeDly(10);
		SendBTSummary();
	}

}

static void count_first_watt_data(mmenu_attr_t *mmenu_attr)
{
	mmenu_attr->sports_data[9] = 1667 *weight *((5*TargHR) - (mmenu_attr->sports_data[0]*10))*5/9000000;

	if (mmenu_attr->sports_data[9]  < 0) {
		mmenu_attr->sports_data[9] = wattTable[0][0];
	}
		BT_WATT = mmenu_attr->sports_data[9];
}

static void save_total(__u8 time, __u8 dist) 
{
	pageData[0] = 0x40;
	readPage(TotalData,pageData);
	TotalTime = TotalData[0]*65536+TotalData[1]*256+TotalData[2] + time;
	TotalDist = TotalData[3]*256+TotalData[4] + dist;
	TotalData[0] = (TotalTime>>16)&0xFF;
	TotalData[1] = (TotalTime>>8)&0xFF;
	TotalData[2] = TotalTime&0xFF;
	TotalData[3] = (TotalDist>>8)&0xFF;
	TotalData[4] = TotalDist&0xFF;
	writePage(TotalData,pageData);
					
}

static void count_watt_data(mmenu_attr_t *mmenu_attr)
{
	__u8 value_h;
	__u8 value_l;
	__u8 levelIndex;
	
	if (mmenu_attr->sports_data[0] < 15) {
		mmenu_attr->sports_data[9] = 0;
	}/* else if (mmenu_attr->sports_data[0] >= 15 && mmenu_attr->sports_data[0] < 20) {
		mmenu_attr->sports_data[9] = (mmenu_attr->sports_data[0] - 15)*wattTable[mmenu_attr->sports_data[4]-1][1]/5;
	
	}*/else if (mmenu_attr->sports_data[0] >= 15 && mmenu_attr->sports_data[0] < 100){
		value_h = mmenu_attr->sports_data[0] / 10;
		value_l = mmenu_attr->sports_data[0] % 10;
		if (value_l < 5) {
		levelIndex = (value_h*10+5 - 15)/5 ;
		mmenu_attr->sports_data[9] = wattTable[mmenu_attr->sports_data[4]-1][levelIndex-1] + 
			( wattTable[mmenu_attr->sports_data[4]-1][levelIndex] - wattTable[mmenu_attr->sports_data[4]-1][levelIndex-1])/5*value_l;
		} else {
			levelIndex = ((value_h+1)*10 - 15)/5 ;
			mmenu_attr->sports_data[9] = wattTable[mmenu_attr->sports_data[4]-1][levelIndex-1] + 
				( wattTable[mmenu_attr->sports_data[4]-1][levelIndex] - wattTable[mmenu_attr->sports_data[4]-1][levelIndex-1])/5*(value_l-5);
		}
		
	} else {
		value_h = mmenu_attr->sports_data[0] / 10;
		value_l = mmenu_attr->sports_data[0] % 100 % 10;
		if (value_l < 5) {
		levelIndex = (value_h*10+5 - 15)/5 ;
		mmenu_attr->sports_data[9] = wattTable[mmenu_attr->sports_data[4]-1][levelIndex-1] + 
			( wattTable[mmenu_attr->sports_data[4]-1][levelIndex] - wattTable[mmenu_attr->sports_data[4]-1][levelIndex-1])/5*value_l;
		} else {
			levelIndex = ((value_h+1)*10 - 15)/5 ;
			mmenu_attr->sports_data[9] = wattTable[mmenu_attr->sports_data[4]-1][levelIndex-1] + 
				( wattTable[mmenu_attr->sports_data[4]-1][levelIndex] - wattTable[mmenu_attr->sports_data[4]-1][levelIndex-1])/5*(value_l-5);
		}
	}
	BT_WATT = mmenu_attr->sports_data[9];
	
}



static void count_sports_data(mmenu_attr_t *mmenu_attr)
{
	__u32 curSpeed = 0; 
	__u8 decTmp;
	__u32 speed_M = 0;
	
	speed_M =  114*mmenu_attr->sports_data[0]*96/10000; //扩大了10倍
	curSpeed = 114*mmenu_attr->sports_data[0]*60/10000; //扩大10
	sportsAllTime++;
	
	mmenu_attr->sports_data[1] +=  speed_M*100/36;//扩大了100000倍；

	if (speed_M != 0) {
		if (unitFlag) {
			decTmp = (360000/speed_M)%1000%100%10;
			SumSpd += speed_M;
			if (decTmp>4) {
				mmenu_attr->sports_data[3] =  36000/speed_M + 1;
			} else {
				mmenu_attr->sports_data[3] =  36000/speed_M;
			}
		} else {
			//curSpeed = 114*mmenu_attr->sports_data[0]*60/10000; //扩大10
			SumSpd += curSpeed;
			decTmp = (360000/curSpeed)%1000%100%10;
			if (decTmp>4) {
				mmenu_attr->sports_data[3] =  36000/curSpeed + 1;
			} else {
				mmenu_attr->sports_data[3] =  36000/curSpeed;
			}
		}
	
	} else {
		mmenu_attr->sports_data[3] = 0;
	}
	
	if (curSpeed != 0) {
		mmenu_attr->sports_data[8] += ((weight+1)*10000+(curSpeed*12256)+(3500*curSpeed*mmenu_attr->sports_data[4]))*5/(2*360);//扩大了100000
	}

	
}

static void Send_BT_inc_level(__u8 cmd,__u8 data)
{
	SendBTData[0] = BT_HEAD;
	SendBTData[1] = 0x02;
	SendBTData[2] = cmd;
	SendBTData[3] = data-1;
	SendBTData[4] = BT_END;

	bluetooth_writebuf(SendBTData,5);

}

static void Send_BT_targetHR(__u8 data)
{
	SendBTData[0] = BT_HEAD;
	SendBTData[1] = 0x02;
	SendBTData[2] = TARGHR_CMD;
	SendBTData[3] = data;
	SendBTData[4] = BT_END;

	bluetooth_writebuf(SendBTData,5);


}
static void Send_BT_DATA()
{
	__u8 i;
	if (!unitFlag) {
		BT_tmpSpeed = 114*BT_RPM*60; //扩大了100000倍
	} else {
		BT_tmpSpeed = 114*BT_RPM*12 * 8;
	}

	BT_SPEED = BT_tmpSpeed / 100;
	
	SendBTData[0] = BT_HEAD;
	SendBTData[1] = 0x13;
	SendBTData[2] = DATASEND_CMD;
	SendBTData[3] = BT_MIN;
	SendBTData[4] = BT_SEC;
	SendBTData[5] = (BT_DIST>>8)&0xFF;
	SendBTData[6] = BT_DIST&0xFF;
	SendBTData[7] = (BT_CAL>>8)&0xFF;
	SendBTData[8] = BT_CAL&0xFF;
	SendBTData[9] = BT_PLUSE;
	SendBTData[10] = BT_RPM;
	SendBTData[11] = (BT_SPEED>>8)&0xFF;
	SendBTData[12] = BT_SPEED&0xFF;
	SendBTData[13] = (BT_WATT>>8)&0xFF;
	SendBTData[14] = BT_WATT & 0xFF;
	SendBTData[15] = 0x00;
	SendBTData[16] = 0x00;
	SendBTData[17] = BT_HR;
	SendBTData[18] = 0x00;
	SendBTData[19] = 0x00;
	SendBTData[20] = BT_FOCUS;
	SendBTData[21] = BT_END;
	//for (i=0; i<21; i++) {
		//__msg("huang######### SendBTdata[%d] = %d \n",i,SendBTData[i]);
//	}
	bluetooth_writebuf(SendBTData,22);
	
}

static void draw_LAPS(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 210;
	__s32 setNUM_y = 30;
	void *pbmp;
	__u16 laps;

	laps = mmenu_attr->sports_data[1]  / 40000;

	if (laps >= 100) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[laps/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[laps/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);

		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[laps%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28*2,setNUM_y);
	} else {
		GUI_SetBkColor(0x0);	
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+28, setNUM_y+87);
		if (laps > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[laps/10%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);
		} else {
			GUI_SetBkColor(0x0);	
			GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+28*2, setNUM_y+87);
		}

		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[laps%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28*2,setNUM_y);
	}

	

}

static void draw_SegmentTime(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 604;
	__s32 setNUM_y = 74;
	void *pbmp;
	__u16 SegTime;
	__u16 min;
	__u16 sec;
	
	if (timeFlag) {
		SegTime = 60;
	}	else {
		SegTime = mmenu_attr->sports_profile[0].time_unit;
	}

	min = SegTime / 60;
	sec = SegTime % 60;

	if (min > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[min/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[min%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);		
	} else {
		GUI_SetBkColor(0x0);	
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+28, setNUM_y+87);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[min%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[10]);
	GUI_BMP_Draw(pbmp,setNUM_x+28*2,setNUM_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[sec/10]);
	GUI_BMP_Draw(pbmp,setNUM_x+28*2+19,setNUM_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[sec%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+28*3+19,setNUM_y);	
	
}

static void draw_dis_level(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 604;
	__s32 setNUM_y = 74;
	void *pbmp;

	if (mmenu_attr->sports_data[4] > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[4]/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[4]%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);
	} else {
		GUI_SetBkColor(0x0);	
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+28, setNUM_y+87);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[4]%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);
	}
}


/*
	画DISPLAY RPM
*/

static void draw_rpm_data(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 210;
	__s32 setNUM_y = 338;
	void *pbmp;

	
	if (mmenu_attr->sports_data[0] < 15){
		mmenu_attr->sports_data[0] = 0;

		GUI_SetBkColor(0x00000000);
	
		//GUI_ClearRect(114, 248, 31*2, 109);
		GUI_ClearRect(210, 338, 210+28*2, 338+87);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[0]%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28*2,setNUM_y);
	} else if(mmenu_attr->sports_data[0] >= 100)
	{	
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[0]/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[0]/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[0]%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28*2,setNUM_y);
	} else {

		GUI_SetBkColor(0x00000000);
	
		GUI_ClearRect(210, 338, 210+28*2, 338+87);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[0]/10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[0]%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28*2,setNUM_y);
	}
	BT_RPM = (__u8) mmenu_attr->sports_data[0];
}


/*
	画display WAttS
*/

static void draw_dis_watts_data(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 604;
	__s32 setNUM_y  =300;
	void *pbmp;

	
	if(mmenu_attr->sports_data[9] >= 100)
	{
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[9]/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[9]/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[9]%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28*2,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+28, setNUM_y+87);
		if (mmenu_attr->sports_data[9] > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[9]/10%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);
		}else {
			GUI_SetBkColor(0x00000000);
			GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+28*2, setNUM_y+87);
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[mmenu_attr->sports_data[9]%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28*2,setNUM_y);
	}
	
}


static void draw_speed(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 210;
	__s32 setNUM_y = 184;
	void *pbmp;
	__u32 speed;

	if (!unitFlag) {
		speed = 114*mmenu_attr->sports_data[0]*60; //扩大了100000倍
	} else {
		speed = 114*mmenu_attr->sports_data[0]*12 * 8;
	}

	speed = speed / 10000;

	if (speed > 999) {
		speed = 999;
	}

	if (speed > 99) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[speed/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	} else {
		GUI_SetBkColor(0x0);	
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+28, setNUM_y+87);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[speed%100/10]);
	GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[11]);
	GUI_BMP_Draw(pbmp,setNUM_x+28*2,setNUM_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[speed%100%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+28*2+19,setNUM_y);
	BT_SPEED = speed*100;
}

static void draw_targetHR(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 604;
	__s32 setNUM_y = 184;
	void *pbmp;

	if (TargHR >= 100) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[TargHR/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[TargHR/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);

		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[TargHR%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28*2,setNUM_y);
	} else {
		GUI_SetBkColor(0x0);	
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+28, setNUM_y+87);
	
		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[TargHR/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->displayNum_bmp[TargHR%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+28*2,setNUM_y);
	}
}



/*
	画display悬浮窗
*/

static void draw_pop_display(mmenu_attr_t *mmenu_attr)
{
		__s32 setNUM_x = 123;
		__s32 setNUM_y = 72;
		void *pbmp;
	
		RECT	rect;
	
		rect.x = 123;
		rect.y = 72;
		rect.width = 778;
		rect.height = 456;
		lyrThree = home_32bpp_layer_create(&rect, 1);
		
		if( !lyrThree )
		{
			__err("new layer create error !\n");
		}
	
	
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->display_bmp);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	
		GUI_LyrWinSetSta(lyrThree, GUI_LYRWIN_STA_ON);
		GUI_LyrWinSel(lyrThree);
		GUI_LyrWinSetTop(lyrThree);
		GUI_SetBkColor(0x00000000);
		
		GUI_ClearRect(123, 72, 901, 528);
		draw_dis_level(mmenu_attr);
		draw_LAPS(mmenu_attr);
		draw_speed(mmenu_attr);
		draw_rpm_data(mmenu_attr);
		draw_dis_watts_data(mmenu_attr);

}


/*
	画stop悬浮窗
*/

static void draw_pop_stop(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 123;
	__s32 setNUM_y = 150;
	void *pbmp;

	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->stop_bmp);
	GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	uninit_sports_hr_res(mmenu_attr);
	esKRNL_TimeDly(10);
	init_hr_summary_res(mmenu_attr);
	esKRNL_TimeDly(30);
	GUI_SetBkColor(0xEE000000);	
	GUI_ClearRect(0, 0, 1024, 600);
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_bmp);
	GUI_BMP_Draw(pbmp,142,47);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_message_bmp);
	GUI_BMP_Draw(pbmp,0,504);

	draw_summary(mmenu_attr);
}


/*
	画pause悬浮窗
*/

static void draw_pop_pause(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 123;
	__s32 setNUM_y = 150;
	void *pbmp;

	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->pause_bmp);
	GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_2);
	GUI_BMP_Draw(pbmp,0,504);

	if (BTSET_FLAG) {
		Send_BT_status(6);
	}
}


/*
	画无心跳警告画面
*/

static void draw_pop_hrwarring(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 123;
	__s32 setNUM_y = 150;
	void *pbmp;

	RECT	rect;

	rect.x = 0;
	rect.y = 0;
	rect.width = 1024;
	rect.height = 600;
	lry = home_32bpp_layer_create(&rect, 2);
	
	if( !lry )
	{
		__err("new layer create error !\n");
	}
	GUI_LyrWinSetSta(lry, GUI_LYRWIN_STA_ON);
	GUI_LyrWinSel(lry);
	GUI_LyrWinSetTop(lry);
	GUI_SetBkColor(0xEE000000);
	
	GUI_ClearRect(0, 0, 1024, 600);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_hrWar_bmp);
	GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	
}




/*
	画time target悬浮窗
*/

static void draw_pop_TargetTime(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 123;
	__s32 setNUM_y = 150;
	void *pbmp;
	RECT	rect;

	rect.x = 0;
	rect.y = 0;
	rect.width = 1024;
	rect.height = 600;
	lry = home_32bpp_layer_create(&rect, 2);
	
	if( !lry )
	{
		__err("new layer create error !\n");
	}
	GUI_LyrWinSetSta(lry, GUI_LYRWIN_STA_ON);
	GUI_LyrWinSel(lry);
	GUI_LyrWinSetTop(lry);
	GUI_SetBkColor(0xEE000000);
	
	GUI_ClearRect(0, 0, 1024, 600);
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->targTime_bmp);
	GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

	uninit_sports_hr_res(mmenu_attr);
	esKRNL_TimeDly(180);
	init_hr_summary_res(mmenu_attr);
	
	GUI_SetBkColor(0xEE000000);
	
	GUI_ClearRect(0, 0, 1024, 600);
	stopCount = 2;

	pbmp = dsk_theme_hdl2buf(mmenu_attr->summary_bmp);
	GUI_BMP_Draw(pbmp,142,47);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_2);
	GUI_BMP_Draw(pbmp,0,504);

	draw_summary(mmenu_attr);
	//pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_2);
	//GUI_BMP_Draw(pbmp,0,504);
	
}


/*
	画RPM
*/

static void draw_rpmHR_data(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 114;
	__s32 setNUM_y = 248;
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);
	
	if (mmenu_attr->sports_data[0] < 15){
		mmenu_attr->sports_data[0] = 0;

		GUI_SetBkColor(0x00000000);
	
		//GUI_ClearRect(114, 248, 31*2, 109);
		GUI_ClearRect(114, 248, 114+31*2, 248+109);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[mmenu_attr->sports_data[0]%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
	} else if(mmenu_attr->sports_data[0] >= 100)
	{	
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[mmenu_attr->sports_data[0]/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[mmenu_attr->sports_data[0]/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[mmenu_attr->sports_data[0]%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
	} else {

		GUI_SetBkColor(0x00000000);
	
		GUI_ClearRect(114, 248, 114+31*2, 248+109);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[mmenu_attr->sports_data[0]/10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[mmenu_attr->sports_data[0]%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
	}
}


/*
	画DISTANCE
*/

static void draw_distanceHR_data(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 232;
	__s32 setNUM_y = 248;
	void *pbmp;
	__u16 distance_decimal; //小数
	__u16 distance_int; 	//整数

	GUI_LyrWinSel(mmenu_attr->layer);
	
	if (!unitFlag) { //true 为 英里
		distance_decimal = mmenu_attr->sports_data[1] * 5 / 80000 ;
	}else { //false 为 公里
		distance_decimal = mmenu_attr->sports_data[1] / 10000  ;
	}
	//__msg("huang####	distance_decimal = %d \n", distance_decimal);
	esKRNL_TimeDly(1);
	if (distance_decimal > 9 && distance_decimal <100) {
		distance_decimal = distance_decimal%10;
	} else if (distance_decimal >= 100) {
		distance_decimal = distance_decimal % 100 % 10;
	} else if (distance_decimal > 999) {
		mmenu_attr->sports_data[1] = 0;
	}

	if (!unitFlag) {
		distance_int  = mmenu_attr->sports_data[1] *5 / 800000;
	} else {
		distance_int  = mmenu_attr->sports_data[1] / 100000;
	}
	//__msg("huang####	distance_int = %d \n", distance_int);
	esKRNL_TimeDly(1);
	/*整数部分*/
	if (distance_int > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[distance_int/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		
	} else {
		GUI_SetBkColor(0x00000000);

		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+31, setNUM_y+109);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[distance_int%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);

	/*小数部分*/
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[distance_decimal]);
	GUI_BMP_Draw(pbmp,setNUM_x+31*2+19,setNUM_y);

	BT_DIST = distance_int*100 + distance_decimal*10;
}

/*
	画TIME
*/

static void draw_timeHR_data(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 359;
	__s32 setNUM_y = 117;
	void *pbmp;
	__s32 min;
	__s32 sec;

	GUI_LyrWinSel(mmenu_attr->layer);
	min = mmenu_attr->sports_data[2] / 60;
	sec = mmenu_attr->sports_data[2] % 60;
	BT_MIN = min;
	BT_SEC = sec;
	if (min > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_time_bmp[min/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

	
	} else {
		GUI_SetBkColor(0x00000000);

		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+67, setNUM_y+240);
	}
	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_time_bmp[min%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+67,setNUM_y);
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_time_bmp[sec/10]);
	GUI_BMP_Draw(pbmp,setNUM_x+67*2+41,setNUM_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_time_bmp[sec%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+67*3+41,setNUM_y);

}

/*
	画PACE
*/

static void draw_paceHR_data(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 682;
	__s32 setNUM_y = 248;
	void *pbmp;
	__s32 min;
	__s32 sec;

	GUI_LyrWinSel(mmenu_attr->layer);
	if (mmenu_attr->sports_data[3] < 6000) {
		min = mmenu_attr->sports_data[3] / 60;
		sec = mmenu_attr->sports_data[3] % 60;

		if (min > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[min/10]);
			GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
		} else {
			GUI_SetBkColor(0x00000000);

			GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+31, setNUM_y+109);
		}

		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[min%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[sec/10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31*2+19,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[sec%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31*3+19,setNUM_y);
	}

}

/*
	画LEVEL
*/

static void draw_level_data(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 844;
	__s32 setNUM_y = 248;
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);

	if (mmenu_attr->sports_data[4] < LEVEL_MIN) {
		mmenu_attr->sports_data[4] = LEVEL_MIN;
	} else if (mmenu_attr->sports_data[4] > MaxLevel) {
		mmenu_attr->sports_data[4] = MaxLevel;
	}
	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_level_bmp);
	GUI_BMP_Draw(pbmp,834, 202);
	if (mmenu_attr->sports_data[4] > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[mmenu_attr->sports_data[4]/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);

		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+31, setNUM_y+109);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[mmenu_attr->sports_data[4]%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);

	SetRunLevel(mmenu_attr->p_cmd_data,(__u16) mmenu_attr->sports_data[4]);
	if (BTSET_FLAG) {
		Send_BT_inc_level(LEVEL_CMD,(__u8) mmenu_attr->sports_data[4]);
	}


}

static void draw_inc_data(mmenu_attr_t *mmenu_attr)
{

	__s32 setNUM_x = 130;
	__s32 setNUM_y = 248;
	
	void *pbmp;
	if ((errCode >> 4) != 0x8) {
#ifdef AC00280
	GUI_LyrWinSel(mmenu_attr->layer);

	if (mmenu_attr->sports_data[5] < INCLINE_MIN) {
		mmenu_attr->sports_data[5] = INCLINE_MIN;
	} else if (mmenu_attr->sports_data[5] > INCLINE_MAX) {
		mmenu_attr->sports_data[5] = INCLINE_MAX;
	}
	//pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_level_bmp);
	//GUI_BMP_Draw(pbmp,834, 202);
	if (mmenu_attr->sports_data[5] > 9) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[mmenu_attr->sports_data[5]/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);

		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+31, setNUM_y+109);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[mmenu_attr->sports_data[5]%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);

	SetRunINC(mmenu_attr->p_cmd_data,(__u16) mmenu_attr->sports_data[5]);
#else ifdef AC00285
	

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[STRIDE_INC_TABLE[mmenu_attr->sports_data[5]-1]/100]);
	GUI_BMP_Draw(pbmp,105,248);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[STRIDE_INC_TABLE[mmenu_attr->sports_data[5]-1]%100/10]);
	GUI_BMP_Draw(pbmp,136,248);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[11]);
	GUI_BMP_Draw(pbmp,167,248);
	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_white_bmp[STRIDE_INC_TABLE[mmenu_attr->sports_data[5]-1]%100%10]);
	GUI_BMP_Draw(pbmp,186,248);
	SetRunSTRDIE(mmenu_attr->p_cmd_data,(__u16)(mmenu_attr->sports_data[5]-1));
#endif
		}else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->inc_error_bmp);
		GUI_BMP_Draw(pbmp,129,248);
	}
	if (BTSET_FLAG) {
		Send_BT_inc_level(INC_CMD,(__u8) mmenu_attr->sports_data[5]);
	}
}



/*
	画PULSE
*/

static void draw_pulseHR_data(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 37;
	__s32 setNUM_y = 430;
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);


	if(mmenu_attr->sports_data[6] >= 100)
	{
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[mmenu_attr->sports_data[6]/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[mmenu_attr->sports_data[6]/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[mmenu_attr->sports_data[6]%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);

		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+31, setNUM_y+109);

		if (mmenu_attr->sports_data[6] > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[mmenu_attr->sports_data[6]/10]);
			GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);
		} else {
			GUI_SetBkColor(0x00000000);

			GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+31*2, setNUM_y+109);
		}
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[mmenu_attr->sports_data[6]%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
	}
	BT_PLUSE = (__u8) mmenu_attr->sports_data[6];
}

/*
	画Target HR
*/

static void draw_TargHR_data(mmenu_attr_t *mmenu_attr, __bool flag)
{
	__s32 setNUM_x = 285;
	__s32 setNUM_y = 430;
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);


	if(TargHR >= 100)
	{
		if (flag) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[TargHR/100]);
			GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[TargHR/10%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[TargHR%100%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_targHR_bmp[0]);
			GUI_BMP_Draw(pbmp,270, 562); //targHR 图标
		} else {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_yellow_bmp[TargHR/100]);
			GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_yellow_bmp[TargHR/10%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_yellow_bmp[TargHR%100%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_targHR_bmp[1]);
			GUI_BMP_Draw(pbmp,270, 562); //targHR 图标
		}
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+31, setNUM_y+109);
		if (flag) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_yellow_bmp[TargHR/10%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_yellow_bmp[TargHR%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_targHR_bmp[0]);
			GUI_BMP_Draw(pbmp,270, 562); //targHR 图标
		}else {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_yellow_bmp[TargHR/10%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_yellow_bmp[TargHR%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_targHR_bmp[1]);
			GUI_BMP_Draw(pbmp,270, 562); //targHR 图标
		}
	}

	Send_BT_targetHR(TargHR);
}

/*
	画HR
*/

static void draw_hr_data(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 154;
	__s32 setNUM_y = 430;
	void *pbmp;
	__u8 hr_profile;

	

	GUI_LyrWinSel(mmenu_attr->layer);

	if(mmenu_attr->sports_data[7] < 50 ) {
		hr_profile = 0;
	} else if (mmenu_attr->sports_data[7] >= 50 && mmenu_attr->sports_data[7] <= 54) {
		hr_profile = 1;
	}else if (mmenu_attr->sports_data[7] >= 55 && mmenu_attr->sports_data[7] <= 59) {
		hr_profile = 2;
	}else if (mmenu_attr->sports_data[7] >= 60 && mmenu_attr->sports_data[7] <= 64) {
		hr_profile = 3;
	}else if (mmenu_attr->sports_data[7] >= 65 && mmenu_attr->sports_data[7] <= 69) {
		hr_profile = 4;
	}else if (mmenu_attr->sports_data[7] >= 70 && mmenu_attr->sports_data[7] <= 74) {
		hr_profile = 5;
	}else if (mmenu_attr->sports_data[7] >= 75 && mmenu_attr->sports_data[7] <= 79) {
		hr_profile = 6;
	}else if (mmenu_attr->sports_data[7] >= 80 && mmenu_attr->sports_data[7] <= 84) {
		hr_profile = 7;
	}else if (mmenu_attr->sports_data[7] >= 85 && mmenu_attr->sports_data[7] <= 89) {
		hr_profile = 8;
	}else if (mmenu_attr->sports_data[7] >= 90 ) {
		hr_profile = 9;
	} 
	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_hrprofile_bmp[hr_profile]);
	GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
}

/*
	画CALORIES
*/

static void draw_caloriesHR_data(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 764;
	__s32 setNUM_y = 430;
	void *pbmp;
	__s32 curCORL;
	
	
	GUI_LyrWinSel(mmenu_attr->layer);

	curCORL = mmenu_attr->sports_data[8] / 100000;
	
	if (curCORL > 999) {
		curCORL = 0;
		mmenu_attr->sports_data[8] = 0;
	}
	if(curCORL >= 100)
	{
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[curCORL/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[curCORL/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[curCORL%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+31, setNUM_y+109);
		if (curCORL > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[curCORL/10]);
			GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);
		} else {
			GUI_SetBkColor(0x00000000);

			GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+31*2, setNUM_y+109);
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[curCORL%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
	}
	BT_CAL = curCORL*10;
}


/*
	画WALLS
*/

static void draw_wallsHR_data(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 889;
	__s32 setNUM_y = 430;
	void *pbmp;

	GUI_LyrWinSel(mmenu_attr->layer);

//	__msg("huang######  draw_wallsHR_data \n");

	if(mmenu_attr->sports_data[9] >= 100)
	{
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[mmenu_attr->sports_data[9]/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[mmenu_attr->sports_data[9]/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[mmenu_attr->sports_data[9]%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
	} else {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+31, setNUM_y+109);
		if (mmenu_attr->sports_data[9] > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[mmenu_attr->sports_data[9]/10%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+31,setNUM_y);
		}else {
			GUI_SetBkColor(0x00000000);
			GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+31*2, setNUM_y+109);
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[mmenu_attr->sports_data[9]%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+31*2,setNUM_y);
	}

	
}

/*
	初始化运动数据
	/*
		0:rpm  1:distance  2:time  3:pace  4:level 
		5:incline 6:pulse 7:hr  8:calories  9:walls
	*/


static void init_sportsHR_data(mmenu_attr_t *mmenu_attr)
{
	int timeUnit;
	int timeRe;
	__s32 i;

	for (i = 0 ; i < 10; i++) {
		mmenu_attr->sports_data[i] = 0;
	}

	oldDist = 0;
	RunTime = 0;
	SumSpd = 0;
	incErrorCount = 0;
	sportsAllTime = 0;
	SumIncline = 0;
	SumLevel = 0;
	SumPace = 0;
	SumPulse = 0;
	SumRpm = 0;
	SumWatt = 0;
	RpmFlag = EPDK_FALSE;
	HRTooHighCount = 0;
	FirstHRFlag = EPDK_TRUE;
	HRReduce = EPDK_FALSE;
	reaStandby = EPDK_FALSE;
	reduceTime = 60;
	reachTime = 0;
	reachTime_1= 0;
	mainTainTime = 0;
	HRProfileTime = 0;
	SumCount = 0;
	HR0 = 0;
	HR60 = 0;
	HR75 = 0;
	Goal = 0;
	ROR1 = 0;
	ROR2 = 0;
	//stopCount = 0;
	HRbackHomeCount = 0;
	reaching = EPDK_FALSE;
	mainTain = EPDK_FALSE;
	reachTime60Flag = EPDK_FALSE;
	reachTime75Flag = EPDK_FALSE;
	reachLoop = EPDK_FALSE;
 	HeartFlag = EPDK_FALSE;
	hwWarring = EPDK_FALSE;
	HRTooHigh = EPDK_FALSE;
	E2ErrorFlag = EPDK_FALSE;
	LevelError = EPDK_FALSE;
	RPM_Flag = EPDK_FALSE;
	
	mmenu_attr->curFocus = 0;
	mmenu_attr->sports_data[4] = 1;
	mmenu_attr->sports_data[5] = 1;
	mmenu_attr->sports_data[2] = timeTmp*60; //设置的时间

	if (!timeTmp) {
		timeFlag = 1; //设置为0，则正数
		for (i = 0 ; i < 18; i++) {
			mmenu_attr->sports_profile[i].time_unit = 60;
		}
		
	} else {
		timeUnit = mmenu_attr->sports_data[2] /18;
		timeRe = mmenu_attr->sports_data[2] %18;
		for (i = 0 ; i < 18; i++) {
			if (timeRe > 0) {
				mmenu_attr->sports_profile[i].time_unit = timeUnit + 1;
			} else {
				mmenu_attr->sports_profile[i].time_unit = timeUnit;
			}
			timeRe--;
		}
	}
	
}

/*
	画当前profile
	flag 为true ，level
	flag为false ， incline
*/

static void draw_curHR_profile(mmenu_attr_t *mmenu_attr,__bool flag)
{
	
}


/*
	画profile
	flag 为true ，level
	flag为false ， incline
*/

static void draw_hr_profile(mmenu_attr_t *mmenu_attr,__bool flag)
{
	__s32 setNUM_x = 404;
	__s32 setNUM_y = 464;
	void *pbmp;
	__s32 i;
	__s32 profile_value;

	GUI_LyrWinSel(mmenu_attr->layer);
	GUI_SetBkColor(0x00000000);

	GUI_ClearRect(404, 425, 404+355, 425+135);
	if (flag) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_hrProfile_1_bmp);
		GUI_BMP_Draw(pbmp,setNUM_x, setNUM_y);
	} else {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_hrProfile_2_bmp);
		GUI_BMP_Draw(pbmp,setNUM_x, setNUM_y);
	}
	
}

/*
	根据计算的watt值寻找当前watt
*/

static void getLevel(mmenu_attr_t *mmenu_attr) 
{
	__u8 Rpm_h;
	__u8 Rpm_l;
	__u8 RpmIndex;
	__u8 i;
	__u8 j;
	__s16 watt;

	if (mmenu_attr->sports_data[0] < 15) {
		mmenu_attr->sports_data[0] = 15;
	}

	if ( mmenu_attr->sports_data[0] > 99) {
		Rpm_h = mmenu_attr->sports_data[0] / 10;
		Rpm_l = mmenu_attr->sports_data[0] % 100 % 10;
	} else {
		Rpm_h = mmenu_attr->sports_data[0] / 10;
		Rpm_l = mmenu_attr->sports_data[0] % 10;
	}
	if (Rpm_l < 5) {
		RpmIndex = (Rpm_h*10 - 15) / 5;
	} else {
		RpmIndex = (Rpm_h*10 + 5 - 15) /5;
	}
	
	mmenu_attr->sports_data[9] = 1667 *weight *((5*TargHR) - (mmenu_attr->sports_data[0]*10))*5/9000000;
	watt = 5*TargHR - mmenu_attr->sports_data[0]*10;
	
	if (watt <= 0) {
		mmenu_attr->sports_data[9] = wattTable[0][0];
		mmenu_attr->sports_data[4] = 1;
		return;
	}
	for (i=0; i<20; i++) {
		if (mmenu_attr->sports_data[9] < wattTable[0][RpmIndex]) {
			mmenu_attr->sports_data[4] = 1;
			return;
		}
		if (mmenu_attr->sports_data[9]<=wattTable[i][RpmIndex]) {
			mmenu_attr->sports_data[4] = i+1;
			__msg("huang##### level = %d  \n", i+1);
			return;
		}
		
	}
	

}

static void draw_HRProfile(mmenu_attr_t *mmenu_attr,int num)
{
	__s32 setNUM_x = 404;
	__s32 setNUM_y = 425;
	void *pbmp;


	GUI_LyrWinSel(mmenu_attr->layer);

	GUI_SetBkColor(0x00000000);

	GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+355, setNUM_y+135);
	
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->HRProfile_bmp[num]);
	GUI_BMP_Draw(pbmp,setNUM_x, setNUM_y);

	
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


/*
	显示sports界面
*/

static void paint_sports_hr_all(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	GUI_RECT gui_rect;

	RPMTme = 0;
	
	if(EPDK_FALSE == mmenu_attr->res_init)
    {
        init_sports_hr_res(mmenu_attr);
    }

	GUI_LyrWinSel(mmenu_attr->layer);
	GUI_SetBkColor(0);

    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	
	gui_rect.x0 = 0;
    gui_rect.y0 = 70;  
    gui_rect.x1 = 1024;
    gui_rect.y1 = 600;
    GUI_ClearRectEx(&gui_rect);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
	GUI_BMP_Draw(pbmp,0, 101); //白色方框

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_incline_bmp);
	GUI_BMP_Draw(pbmp,120, 202);//inc 图标

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_distance_bmp);
	GUI_BMP_Draw(pbmp,247, 202);//distance 图标

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[11]);
	GUI_BMP_Draw(pbmp,294, 248);//distance point 图标

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_time_bmp[10]);
	GUI_BMP_Draw(pbmp,480, 117);// time colon 图标

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_pace_bmp);
	GUI_BMP_Draw(pbmp,713, 202);//pace 图标

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_blue_bmp[10]);
	GUI_BMP_Draw(pbmp,744, 248);//pace colon 图标

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_pulse_bmp);
	GUI_BMP_Draw(pbmp,23, 562);//pulse 图标

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_hr_bmp);
	GUI_BMP_Draw(pbmp,146, 562); //hr 图标

	

	pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_1);
	GUI_BMP_Draw(pbmp,404, 562); //manual 图标

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_calories_bmp);
	GUI_BMP_Draw(pbmp,750, 562);// calories 图标

	pbmp = dsk_theme_hdl2buf(mmenu_attr->sports_watts_bmp);
	GUI_BMP_Draw(pbmp,875, 562); // watts 图标

	if (BTSET_FLAG) {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->bt_bmp);
		GUI_BMP_Draw(pbmp,973, 101); 
	}

	draw_inc_data(mmenu_attr);

	draw_distanceHR_data(mmenu_attr);

	draw_timeHR_data(mmenu_attr);

	draw_paceHR_data(mmenu_attr);

	draw_level_data(mmenu_attr);

	draw_pulseHR_data(mmenu_attr);

	draw_TargHR_data(mmenu_attr,EPDK_TRUE);

	draw_hr_data(mmenu_attr);

	draw_caloriesHR_data(mmenu_attr);

	//count_first_watt_data;
	draw_wallsHR_data(mmenu_attr);

	draw_hr_profile(mmenu_attr,EPDK_TRUE);
	 
	
}

/*
	display键切换显示
*/
static void display_show(mmenu_attr_t *mmenu_attr, __bool flag)
{
	RECT	rect;
	
		rect.x = 0;
		rect.y = 0;
		rect.width = 1024;
		rect.height = 600;
	
		if (flag) {
			lry = home_32bpp_layer_create(&rect, 2);
			
			if( !lry )
			{
				__err("new layer create error !\n");
			}
			GUI_LyrWinSetSta(lry, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSel(lry);
			GUI_LyrWinSetTop(lry);
			GUI_SetBkColor(0xEE000000);
			
			GUI_ClearRect(0, 0, 1024, 600);
			draw_pop_display(mmenu_attr);
		} else {
			GUI_LyrWinSetSta(lyrThree, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSel(lyrThree);
			GUI_SetBkColor(0x00000000);
				
			GUI_ClearRect(0, 0, 1024, 600);
			GUI_LyrWinDelete(lyrThree);
			lyrThree = 0;
			
			GUI_LyrWinSetSta(lry, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSel(lry);
			GUI_SetBkColor(0x00000000);
				
			GUI_ClearRect(0, 0, 1024, 600);
			GUI_LyrWinDelete(lry);	
			lry = 0;
		}

}

/*
	主界面按键响应
*/
 __s32  sportsHRbar_key_proc(__gui_msg_t *msg, com_data_para_t *com_data)
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

	if (mmenu_attr->sports_data[0] >= 15) {
		RpmFlag =EPDK_TRUE;
	}

	if (hwWarring == EPDK_TRUE && com_data->WirHr > 0) {
		GUI_LyrWinSel(lry);
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(0, 0, 1024, 600);
		GUI_LyrWinDelete(lry);
		lry = 0;
		GUI_SetTimer(h_deswin_old, SPORTS_HR_TIME_ID, 100, NULL);
		GUI_SetTimer(h_deswin_old, SPORTS_HR_PROFILE_ID, 50, NULL);

		hwWarring = EPDK_FALSE;
		
	}
	mmenu_attr->sports_data[0] = com_data->curRpm;
	BT_RPM = (__u8) mmenu_attr->sports_data[0];
	//if (com_data->WirHr) {
		mmenu_attr->sports_data[6] = com_data->WirHr;
		BT_HR = 0x02;
	//} else {
		//mmenu_attr->sports_data[6] = com_data->HandHr;
		//BT_HR = 0x01;
	//}
	if (!mmenu_attr->sports_data[6]) {
		BT_HR = 0x00;
	}
	//mmenu_attr->sports_data[6] = com_data->HandHr;
	mmenu_attr->sports_data[7] = mmenu_attr->sports_data[6] * 100 / (220 - Age);
	HrFlag = com_data->HrFlag;
	errCode = com_data->error_code;
	
	
	if (!com_data->E2) {
		if ((errCode & 0x0F) == 0x01 || (errCode & 0x0F) == 0x02) {
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
		
	} else {
		LevelError = EPDK_TRUE;
	}
	
	
	keycode = msg->dwAddData1;
	switch(keycode)
	{
		case FAN_KEY:
			fanFlag = !fanFlag;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			//__msg("huang#####  keyTonFlag = %d\n", keyTonFlag);
			SetFanOnOff(mmenu_attr->p_cmd_data,fanFlag);
			break;
		case ENTER_KEY: //切换incline与level
		{
			#if 0
			level_incline_flag = !level_incline_flag;
			draw_level_incline_data(mmenu_attr,level_incline_flag);
			draw_hr_profile(mmenu_attr,level_incline_flag);	
			#endif

			break;
		}	

		case STOP_KEY:
		{
			RECT	rect;

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
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (hwWarring == EPDK_TRUE) {
				GUI_LyrWinSel(mmenu_attr->layer);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinSel(lry);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinDelete(lry);
				lry = 0;
				main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data);
				break;
			}

			stopCount ++;
		//	__msg("huang#####  stopCount = %d \n",stopCount);
			if (stopCount == 1) {
				if (!display_falg) {
					display_show(mmenu_attr,EPDK_FALSE);
					dispalyCount = 0;
					display_falg = EPDK_TRUE;
				}
				rect.x = 0;
				rect.y = 0;
				rect.width = 1024;
				rect.height = 600;
				lry = home_32bpp_layer_create(&rect, 2);
				
				if( !lry )
				{
					__err("new layer create error !\n");
				}
				GUI_LyrWinSetSta(lry, GUI_LYRWIN_STA_ON);
				GUI_LyrWinSel(lry);
				GUI_LyrWinSetTop(lry);
				GUI_SetBkColor(0xEE000000);
				
				GUI_ClearRect(0, 0, 1024, 600);
				SetRunLevel(mmenu_attr->p_cmd_data,1);
				#ifdef AC00280
					SetRunINC(mmenu_attr->p_cmd_data,1);
				#else ifdef AC00285
					SetRunSTRDIE(mmenu_attr->p_cmd_data,0);
				#endif
				draw_pop_pause(mmenu_attr);
				if (pauseModeFlag) {
					GUI_SetTimer(msg->h_deswin, HR_PAU_BACK_ID, 100, NULL);
				}
				GUI_KillTimer(msg->h_deswin, SPORTS_HR_TIME_ID);
				if( GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID)) {
					GUI_KillTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID);
				}
				break;
			}else if (stopCount == 2){
				if(GUI_IsTimerInstalled(msg->h_deswin,HR_PAU_BACK_ID)) {
					GUI_KillTimer(msg->h_deswin, HR_PAU_BACK_ID);
					HRbackHomeCount = 0;
				}
				GUI_SetBkColor(0xEE000000);	
				GUI_ClearRect(0, 0, 1024, 600);
				draw_pop_stop(mmenu_attr);
				
				runnedDist = mmenu_attr->sports_data[1]/10000 - oldDist;
				save_total(sportsAllTime%120,runnedDist);
				
				GUI_SetTimer(msg->h_deswin, HR_BACK_HOME_ID, 100, NULL);
				break;
			} else if (stopCount == 3) {
				GUI_KillTimer(msg->h_deswin, HR_BACK_HOME_ID);
				//backHomeCount = 0;
				GUI_LyrWinSel(mmenu_attr->layer);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				
				GUI_LyrWinSel(lyrThree);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinDelete(lyrThree);
				lyrThree = 0;
				GUI_LyrWinSel(lry);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinDelete(lry);
				lry = 0;
				//GUI_LyrWinSel(mmenu_attr->layer);
				stopCount = 0;
				draw_save(mmenu_attr);
				main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data);
			}

			
		}

		case START_KEY:
		{
			if (E2ErrorFlag) {
				break;
			}
			if (stopCount == 2) { //按下stop后start才起作用
				//GUI_KillTimer(msg->h_deswin, BACK_HOME_ID);
				init_sports_hr_res(mmenu_attr);
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinDelete(lyrThree);
				lyrThree = 0;
				
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinDelete(lry);
				lry = 0;
				SetRunLevel(mmenu_attr->p_cmd_data,mmenu_attr->sports_data[4]);
				#ifdef AC00280
					SetRunINC(mmenu_attr->p_cmd_data,mmenu_attr->sports_data[5]);
				#else ifdef AC00285
					SetRunSTRDIE(mmenu_attr->p_cmd_data,(__u16)(mmenu_attr->sports_data[5] -1));
				#endif
				GUI_SetTimer(msg->h_deswin, SPORTS_HR_TIME_ID, 100, NULL);
				if( !GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID)) {
					GUI_SetTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID, 50, NULL);
				}
				stopCount = 0;
				//draw_level_incline_data(mmenu_attr,EPDK_TRUE);
				//draw_profile(mmenu_attr,EPDK_TRUE);
				if (BTSET_FLAG) {
					Send_BT_status(4);
				}
			
			} else if (stopCount == 1){
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);	
				
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinDelete(lry);
				lry = 0;
				GUI_SetTimer(msg->h_deswin, SPORTS_HR_TIME_ID, 100, NULL);
				SetRunLevel(mmenu_attr->p_cmd_data,mmenu_attr->sports_data[4]);
				#ifdef AC00280
					SetRunINC(mmenu_attr->p_cmd_data,mmenu_attr->sports_data[5]);
				#else ifdef AC00285
					SetRunSTRDIE(mmenu_attr->p_cmd_data,(__u16)(mmenu_attr->sports_data[5] -1));
				#endif
				
				if( !GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID)) {
					GUI_SetTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID, 50, NULL);
				}
				if (BTSET_FLAG) {
					Send_BT_status(4);
				}
				stopCount = 0;
			}
			
			break;
		}
		
			
		case DOWN_LONG_KEY:
		case DOWN_QUICK_LONG_KEY:
		case INCDEC_LONG_KEY:
		case INCDEC_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case DOWN_KEY: //incline -
		case INCDEC_KEY:
		{
			if (E2ErrorFlag || incErrorCount || !sportsAllTime) {
				break;
			}
			if (hwWarring || (hrSigTime > 0) || (stopCount>0)) {
				break;
			}
			mmenu_attr->sports_data[5]--;
		#ifdef AC00280
			if (mmenu_attr->sports_data[5] < INCLINE_MIN) {
				mmenu_attr->sports_data[5] = INCLINE_MIN;
				return EPDK_OK;
			}
		#else ifdef AC00285
			if (mmenu_attr->sports_data[5] < 1) {
				mmenu_attr->sports_data[5] = 1;
				return EPDK_OK;
			}
		#endif
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
			draw_inc_data(mmenu_attr);
			
			break;
		}
		
			
		case UP_LONG_KEY:
		case UP_QUICK_LONG_KEY: 
		case INCPLUSE_LONG_KEY:
		case INCPLUSE_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case UP_KEY: //incline +
		case INCPLUSE_KEY:
		{
			if (E2ErrorFlag || incErrorCount|| !sportsAllTime) {
				break;
			}
			
			if (hwWarring || (hrSigTime > 0) || (stopCount>0)) {
				break;
			}
			mmenu_attr->sports_data[5]++;
		#ifdef AC00280
			if (mmenu_attr->sports_data[5] > 20) {
				mmenu_attr->sports_data[5] = 20;
				return EPDK_OK;
			}
		#else ifdef AC00285
			if (mmenu_attr->sports_data[5] > 13) {
				mmenu_attr->sports_data[5] = 13;
				return EPDK_OK;
			}
		#endif
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
			draw_inc_data(mmenu_attr);
			break;
		}	
	
		
		case FAST_LONG_KEY:
		case FAST_LONG_QUICK_KEY:
		case LEVELPLUSE_LONG_KEY:
		case LEVELPLUSE_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case FAST_KEY: //level +
		case LEVELPLUSE_KEY:
			
		{
			if (E2ErrorFlag || LevelError|| !sportsAllTime) {
				break;
			}
			if (hwWarring || (hrSigTime > 0) || (stopCount>0)) {
				break;
			}
			TargHR ++;
			if (TargHR > TARGHR_MAX) {
				TargHR = TARGHR_MAX;
				return EPDK_OK;
			}
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
			YellowTarHRCount = 0;
			YellowTarHR = EPDK_TRUE;
			draw_TargHR_data(mmenu_attr,EPDK_FALSE);
			break;
		}		
	
		case SLOW_LONG_KEY:
		case SLOW_LONG_QUICK_KEY:
		case LEVELDEC_LONG_KEY:
		case LEVELDEC_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case SLOW_KEY: //level -
		case LEVELDEC_KEY:	
		{
			if (E2ErrorFlag || LevelError|| !sportsAllTime) {
				break;
			}
			if (hwWarring || (hrSigTime > 0) || (stopCount>0)) {
				break;
			}
			TargHR --;
			if (TargHR < TARGHR_MIN) {
				TargHR = TARGHR_MIN;
				return EPDK_OK;
			}
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
			YellowTarHRCount = 0;
			YellowTarHR = EPDK_TRUE;
			draw_TargHR_data(mmenu_attr,EPDK_FALSE);
			
			break;
		}
#if 0
		case LEVEL_1_KEY : 
		{
			mmenu_attr->sports_data[4] = 1;
			if (mmenu_attr->sports_data[4] > MaxLevel) {
				mmenu_attr->sports_data[4] = MaxLevel;
				return EPDK_OK;
			}
			for (i = mmenu_attr->curFocus ; i  < 18; i++) {
				mmenu_attr->sports_profile[i].level_value_unit = mmenu_attr->sports_data[4];
			}	
			draw_level_data(mmenu_attr);
			
			break;
		}	
		
		case LEVEL_4_KEY : 
		{
			mmenu_attr->sports_data[4] = 4;
			if (mmenu_attr->sports_data[4] > MaxLevel) {
				mmenu_attr->sports_data[4] = MaxLevel;
				return EPDK_OK;
			}
			for (i = mmenu_attr->curFocus ; i  < 18; i++) {
				mmenu_attr->sports_profile[i].level_value_unit = mmenu_attr->sports_data[4];
			}	
			draw_level_data(mmenu_attr);
			
			break;
		}	

		case LEVEL_3_KEY : 
		{
			mmenu_attr->sports_data[4] = 3;
			if (mmenu_attr->sports_data[4] > MaxLevel) {
				mmenu_attr->sports_data[4] = MaxLevel;
				return EPDK_OK;
			}
			for (i = mmenu_attr->curFocus ; i  < 18; i++) {
				mmenu_attr->sports_profile[i].level_value_unit = mmenu_attr->sports_data[4];
			}	
			
			draw_level_data(mmenu_attr);
			
			break;
		}	

		case LEVEL_5_KEY : 
		{
			mmenu_attr->sports_data[4] = 5;
			if (mmenu_attr->sports_data[4] > MaxLevel) {
				mmenu_attr->sports_data[4] = MaxLevel;
				return EPDK_OK;
			}
			for (i = mmenu_attr->curFocus ; i  < 18; i++) {
				mmenu_attr->sports_profile[i].level_value_unit = mmenu_attr->sports_data[4];
			}	
			draw_level_data(mmenu_attr);
			
			break;
		}	

		case LEVEL_7_KEY : 
		{
			mmenu_attr->sports_data[4] = 7;
			if (mmenu_attr->sports_data[4] > MaxLevel) {
				mmenu_attr->sports_data[4] = MaxLevel;
				return EPDK_OK;
			}
			for (i = mmenu_attr->curFocus ; i  < 18; i++) {
				mmenu_attr->sports_profile[i].level_value_unit = mmenu_attr->sports_data[4];
			}	
			draw_level_data(mmenu_attr);
			
			break;
		}	

		case LEVEL_9_KEY : 
		{
			mmenu_attr->sports_data[4] = 9;
			if (mmenu_attr->sports_data[4] > MaxLevel) {
				mmenu_attr->sports_data[4] = MaxLevel;
				return EPDK_OK;
			}
			for (i = mmenu_attr->curFocus ; i  < 18; i++) {
				mmenu_attr->sports_profile[i].level_value_unit = mmenu_attr->sports_data[4];
			}	
			draw_level_data(mmenu_attr);
		
			
			break;
		}	

		case LEVEL_12_KEY : 
		{
			mmenu_attr->sports_data[4] = 12;
			if (mmenu_attr->sports_data[4] > MaxLevel) {
				mmenu_attr->sports_data[4] = MaxLevel;
				return EPDK_OK;
			}
			for (i = mmenu_attr->curFocus ; i  < 18; i++) {
				mmenu_attr->sports_profile[i].level_value_unit = mmenu_attr->sports_data[4];
			}	
			draw_level_data(mmenu_attr);
			
			break;
		}	
#endif
		case DISPLAY_KEY:
		{
			if (E2ErrorFlag) {
				break;
			}
			if (hwWarring || (hrSigTime > 0) || (stopCount>0)) {
				break;
			}
			
			if (stopCount == 0)
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
				display_show(mmenu_attr,display_falg);
				display_falg = !display_falg;
	
			break;
		}

	case BT_DISCONNECT:
			GUI_LyrWinSel(mmenu_attr->layer);
			GUI_SetBkColor(0x00000000);
			GUI_ClearRect(0, 0, 1024, 600);
			if (lry!=0) {
				GUI_LyrWinSel(lry);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinDelete(lry);
			}

			if (lyrThree!=0) {
				GUI_LyrWinSel(lyrThree);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinDelete(lyrThree);
			}
			if( GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_TIME_ID)) {
					GUI_KillTimer(msg->h_deswin, SPORTS_HR_TIME_ID);
			}

			if( GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID)) {
					GUI_KillTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID);
			}

			if( GUI_IsTimerInstalled(msg->h_deswin, HR_BACK_HOME_ID)) {
					GUI_KillTimer(msg->h_deswin, HR_BACK_HOME_ID);
			}

			if( GUI_IsTimerInstalled(msg->h_deswin, HR_PAU_BACK_ID)) {
					GUI_KillTimer(msg->h_deswin, HR_PAU_BACK_ID);
			}
			main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data); 
			break;
			
	}

	return EPDK_OK;
}
	
/*
	
*/
static __s32 _sports_hr_bar_Proc(__gui_msg_t *msg)
{
   // home_uipara_t* home_ui_para;
    
  //  home_ui_para = home_get_ui_para(GUI_GetScnDir());
  //  if(!home_ui_para)
   // {
   // __here__;
    //    __err("home_ui_para is null..\n");
    //    return ;
   // }   
	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
			return EPDK_OK;
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

			//SetRunLevel(mmenu_attr->p_cmd_data,1);
			//SetRunINC(mmenu_attr->p_cmd_data,1);

			__msg("huang####  sports  GUI_MSG_DESTROY \n");
			mmenu_para = (mmenu_para_t *)GUI_WinGetAttr(msg->h_deswin);
			mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			uninit_sports_hr_res(mmenu_attr);
			uninit_HR_summary_res(mmenu_attr);
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
			__here__;
            if(!mmenu_attr)
            {
                return EPDK_FAIL;                
            }
			eLIBs_memset((void *)mmenu_attr, 0, sizeof(mmenu_attr));
			init_sports_hr_res(mmenu_attr);

		
			__here__;
			
			mmenu_attr->font = mmenu_para->mmenu_font;
			mmenu_attr->layer = mmenu_para->layer;
			//mmenu_attr->item_w = home_ui_para->item_width;
			mmenu_attr->focus_item = mmenu_para->focus_id;
			mmenu_attr->first_item = 0;
			mmenu_attr->curFocus = 0;
            
			
			
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
    		
			init_sportsHR_data (mmenu_attr);//初始化运动数据
			__msg("huang#####  hr = %d \n",HR_pulse);
			if (HR_pulse) {
				GUI_SetTimer(msg->h_deswin, SPORTS_HR_TIME_ID, 100, NULL);
				GUI_SetTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID, 50, NULL);
			} else {
				hwWarring = EPDK_TRUE;
				draw_pop_hrwarring(mmenu_attr);
			}
			paint_sports_hr_all(mmenu_attr);
    		return EPDK_OK;
	    }

	case GUI_MSG_KEY:
    {
		return EPDK_OK;
	}
    case GUI_MSG_TOUCH:	
	{
        return EPDK_OK;
	}			
    case GUI_MSG_TIMER:
    {       
		mmenu_attr_t *mmenu_attr;
		__u8 i;
		void *pbmp;
	

		mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);
		if (!mmenu_attr)
		{
			__err("mmenu_attr is null...\n");
            return EPDK_OK;
		}
		if (msg->dwAddData1 == SPORTS_HR_PROFILE_ID) {
			HeartFlag = !HeartFlag;
			HRProfileTime -- ;
			if (HRProfileTime < 0) {
				HRProfileTime = 0;
			}
			if (HRProfileTime == 0) {
				draw_hr_profile(mmenu_attr,HeartFlag);
			}
			
		}
		
		if (msg->dwAddData1 == SPORTS_HR_TIME_ID) {
		
			
			if ((errCode >> 4) == 0x8) {
				
				if (!incErrorCount) {
					GUI_LyrWinSel(mmenu_attr->layer);
					GUI_SetBkColor(0x00000000);
					GUI_ClearRect(105, 248, 130+31*4, 248+109);
					pbmp = dsk_theme_hdl2buf(mmenu_attr->inc_error_bmp);
					GUI_BMP_Draw(pbmp,129,248);
					incErrorCount = 1;
				}
				
			}

			RunTime++;
			if (RunTime >= (mmenu_attr->sports_profile[mmenu_attr->curFocus].time_unit-2) && RunTime <=  mmenu_attr->sports_profile[mmenu_attr->curFocus].time_unit)
			{
				SumLevel += mmenu_attr->sports_data[4];
				#ifdef AC00280
				SumIncline += mmenu_attr->sports_data[5];
				#else ifdef AC00285
				SumIncline += STRIDE_INC_TABLE[mmenu_attr->sports_data[5]-1];
				#endif
				SumCount++;
			}

			if (mmenu_attr->sports_profile[mmenu_attr->curFocus].time_unit < RunTime) {
				RunTime = 1;
				mmenu_attr->curFocus ++; //计算当前段数
				if (mmenu_attr->curFocus > 17) {
					mmenu_attr->curFocus = 0;
				}

			}

			if (timeFlag) {
				mmenu_attr->sports_data[2] ++;
				if (mmenu_attr->sports_data[2] == 5999) {
					mmenu_attr->sports_data[2] = -1;
					 return EPDK_OK;
				}
			}else {
				mmenu_attr->sports_data[2] --;
				if (mmenu_attr->sports_data[2] == -1) {
					mmenu_attr->sports_data[2] = 0;
					if( GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID))	 {
						GUI_KillTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID);
					}
				
					runnedDist = mmenu_attr->sports_data[1]/10000 - oldDist;
					save_total(sportsAllTime%120,runnedDist);
					open_buffer(mmenu_attr->p_cmd_data,3,keyTonFlag);
				
					draw_pop_TargetTime(mmenu_attr);
					GUI_SetTimer(msg->h_deswin, HR_BACK_HOME_ID, 100, NULL);
					stopCount = 2;
					GUI_KillTimer(msg->h_deswin, SPORTS_HR_TIME_ID);
					init_sportsHR_data(mmenu_attr);
					break;
				}
			}	

				draw_timeHR_data(mmenu_attr);
				if (!display_falg) {
					dispalyCount++;
					if (dispalyCount == 3) {
						display_show(mmenu_attr,EPDK_FALSE);
						dispalyCount = 0;
						display_falg = EPDK_TRUE;
					}
				}
				if (!mmenu_attr->sports_data[0]) {
					
					RPMTme ++;
					if (!RPM_Flag) {
						curLevel = mmenu_attr->sports_data[4];
						mmenu_attr->sports_data[4] = 1;
						draw_level_data(mmenu_attr);
					}
					RPM_Flag = EPDK_TRUE;
				if (RPMTme == 300) {
					
					GUI_KillTimer(msg->h_deswin, SPORTS_HR_TIME_ID);
					if( GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID)) {
							GUI_KillTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID);
					}
					SetRunLevel(mmenu_attr->p_cmd_data,1);
				
					#ifdef AC00280
						SetRunINC(mmenu_attr->p_cmd_data,1);
					#else ifdef AC00285
						SetRunSTRDIE(mmenu_attr->p_cmd_data,0);
					#endif
					GUI_LyrWinSel(mmenu_attr->layer);
					GUI_SetBkColor(0x00000000);
					GUI_ClearRect(0, 0, 1024, 600);
					main_cmd2manuel(msg->h_deswin,ID_MAIN,0,SP_Data);
					break;
					
				}
				//SetRunLevel(mmenu_attr->p_cmd_data,17);
				//break;
			}else {
				RPMTme = 0;
				if (RPM_Flag) {
					mmenu_attr->sports_data[4] = curLevel;
					draw_level_data(mmenu_attr);
				}
				RPM_Flag = EPDK_FALSE;
			}
			/*心跳判断各种模式   start*/
			
			
			if (RpmFlag && FirstHRFlag) { // 有输入过RPM才能进行第一次心跳判
				if (mmenu_attr->sports_data[6] >= (TargHR * 6 / 5)) { //大于等于目标心跳的20%,10s后退出
					if( GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID)) {
							GUI_KillTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID);
					}
					if (!HRTooHigh) {
						open_buffer(mmenu_attr->p_cmd_data,3,keyTonFlag);
						draw_HRProfile(mmenu_attr,1);
					}
					HRTooHigh = EPDK_TRUE;
				} else if (mmenu_attr->sports_data[6] > TargHR && mmenu_attr->sports_data[6] < (TargHR * 6 / 5)) {
					HRReduce = EPDK_TRUE; 
					
				
 				} else if (mmenu_attr->sports_data[6] <= TargHR) {
					if (mmenu_attr->sports_data[6] > (TargHR-5)){
							HRProfileTime = 6;
							if (HRProfileTime == 6) {
								draw_HRProfile(mmenu_attr,3);
							}
							mainTain = EPDK_TRUE;//进入MAINTAIN状态
							
						} else {
							reaStandby = EPDK_TRUE;
							
						}
				}

				FirstHRFlag = EPDK_FALSE;
				
			}
		if (!RPM_Flag) {
			if (HRTooHigh) {
				HRTooHighCount++;
				//if (HRTooHighCount == 5) {
				//	open_buffer(mmenu_attr->p_cmd_data,3,keyTonFlag);
				
					//draw_HRProfile(mmenu_attr,1);
				//}

				if (HRTooHighCount == 5) {
					GUI_KillTimer(msg->h_deswin, SPORTS_HR_TIME_ID);
					if (GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID)) {
						GUI_KillTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID);
					}
					GUI_LyrWinSel(mmenu_attr->layer);
					GUI_SetBkColor(0x00000000);
					GUI_ClearRect(0, 0, 1024, 600);
					main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data);
					return EPDK_OK;
				}
			}

			if(HRReduce) {
				if (reduceTime == 60) {
					HRProfileTime = 6;
					draw_HRProfile(mmenu_attr,2);
				}
				reduceTime--;
				if (reduceTime == 0) {
					reduceTime = 60;
				} 
			}

			if (mmenu_attr->sports_data[6] >= (TargHR * 6 / 5)) { //大于等于目标心跳的20%,10s后退出
					if( GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID)) {
							GUI_KillTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID);
					}
					if (!HRTooHigh) {
						draw_HRProfile(mmenu_attr,1);
					}
					HRTooHigh = EPDK_TRUE;
			}

			if (reaStandby) {
				reachTime++;
				if (reachTime == 5) {
					getLevel(mmenu_attr);
					draw_level_data(mmenu_attr);
					reaching = EPDK_TRUE; //进入reaching状态
					//reachTime = 0;
					reaStandby = EPDK_FALSE;
					
				}
			}

			if (reaching) {
				if (mmenu_attr->sports_data[6] > (TargHR-5)){
					HRProfileTime = 6;
					if (HRProfileTime == 6) {
						draw_HRProfile(mmenu_attr,3);
					}
					mainTain = EPDK_TRUE;
					if (mmenu_attr->sports_data[6] > (TargHR+5)){
						HRProfileTime = 100;
						if (HRProfileTime == 100) {
							mmenu_attr->sports_data[4]--;
							draw_level_data(mmenu_attr);	
							HRProfileTime = 0;
						}
					}
				}

				reachTime++;
				if (reachTime == 62) {
					
					if (mmenu_attr->sports_data[6] != 0) {
						HR60 = mmenu_attr->sports_data[6];
						ROR1 = (TargHR - HR0) - (TargHR - HR60);
						Goal = ROR1*3+HR60-TargHR;
			
						if (Goal >= -5 && Goal <=5) {
							HRProfileTime = 6;
							if (HRProfileTime == 6) {
								draw_HRProfile(mmenu_attr,3);
							}
							mainTain = EPDK_TRUE;
						} else if (Goal > 15) {
							mmenu_attr->sports_data[4] -= 3;
							draw_level_data(mmenu_attr);
						} else if (Goal > 7 && Goal <=15) {
							mmenu_attr->sports_data[4] -= 2;
							draw_level_data(mmenu_attr);
						} else if (Goal > 5 && Goal <=7) {
							mmenu_attr->sports_data[4] --;
							draw_level_data(mmenu_attr);
						}else if (Goal < -40) {
							mmenu_attr->sports_data[4] += 3;
							draw_level_data(mmenu_attr);
						}else if (Goal >= -40 && Goal <-20) {
							mmenu_attr->sports_data[4] += 2;
							draw_level_data(mmenu_attr);
						}else if (Goal >= -20 && Goal <-5) {
							mmenu_attr->sports_data[4] ++;
							draw_level_data(mmenu_attr);
						}
						
					}else {
						reachTime--;
						
					}
				}
					
					if (reachTime == 77) {
						
						if (mmenu_attr->sports_data[6] != 0) {
							if (!reachLoop) {
								HR75 = mmenu_attr->sports_data[6];
								ROR2 = HR75 -  HR60;
								Goal = ROR2*3+HR75-TargHR;
								//__msg("huang#### HR 60  Goal = %d \n",Goal);
							}

							if (Goal >= -5 && Goal <=5) {
								HRProfileTime = 6;
								if (HRProfileTime == 6) {
									draw_HRProfile(mmenu_attr,3);
								}
								mainTain = EPDK_TRUE;
							} else if (Goal > 15) {
								mmenu_attr->sports_data[4] -= 3;
								draw_level_data(mmenu_attr);
							} else if (Goal > 7 && Goal <=15) {
								mmenu_attr->sports_data[4] -= 2;
								draw_level_data(mmenu_attr);
							} else if (Goal > 5 && Goal <=7) {
								mmenu_attr->sports_data[4] --;
								draw_level_data(mmenu_attr);
							}else if (Goal < -40) {
								mmenu_attr->sports_data[4] += 3;
								draw_level_data(mmenu_attr);
							}else if (Goal >= -40 && Goal <-20) {
								mmenu_attr->sports_data[4] += 2;
								draw_level_data(mmenu_attr);
							}else if (Goal >= -20 && Goal <-5) {
								mmenu_attr->sports_data[4] ++;
								draw_level_data(mmenu_attr);
							}
							HR60 = HR75;
							reachTime =62;
							reachLoop = EPDK_TRUE;
						}else {
							reachTime --;
						}
					
					
				}
				
			}

			if (mainTain) {
				reaching = EPDK_FALSE;
				

				mainTainTime ++;

				if (mainTainTime == 15) {
					if (mmenu_attr->sports_data[6] > (TargHR+3)) {
						mmenu_attr->sports_data[4] --;
						draw_level_data(mmenu_attr);
					} 

					if (mmenu_attr->sports_data[6] < (TargHR-3)) {
						mmenu_attr->sports_data[4] ++;
						draw_level_data(mmenu_attr);
					} 

					mainTainTime = 0;

			}
		
				
			}
		}
			if (mmenu_attr->sports_data[6] == 0 && HRTooHigh == EPDK_FALSE) {
				if( GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID))	 {
					GUI_KillTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID);
				}
				if (!hrSigTime) {
					draw_HRProfile(mmenu_attr,4);
				}
				hrSigTime++;
				if (hrSigTime == 30) {
					GUI_KillTimer(msg->h_deswin, SPORTS_HR_TIME_ID);
					//if(! GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID)) {
					//	GUI_SetTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID, 50, NULL);
					//}
					GUI_LyrWinSel(mmenu_attr->layer);
					GUI_SetBkColor(0x00000000);
					GUI_ClearRect(0, 0, 1024, 600);
					main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data);	
					break;
				}
				RpmFlag = EPDK_FALSE;
				
			} else if (mmenu_attr->sports_data[6] != 0 && HRTooHigh == EPDK_FALSE){
				RpmFlag = EPDK_TRUE;
				hrSigTime = 0;
				if(! GUI_IsTimerInstalled(msg->h_deswin, SPORTS_HR_PROFILE_ID)) {
					GUI_SetTimer(msg->h_deswin, SPORTS_HR_PROFILE_ID, 50, NULL);
				}
			}
		
			/*心跳判断各种模式   end*/

		
			
			

			if (YellowTarHR) {
				YellowTarHRCount++;
				if (YellowTarHRCount == 3) {
					draw_TargHR_data(mmenu_attr,EPDK_TRUE);
					YellowTarHRCount = 0;
					YellowTarHR = EPDK_FALSE;
				}
			}
			
		

			if (RunedMaxLevel < mmenu_attr->sports_data[4]) {
				RunedMaxLevel = mmenu_attr->sports_data[4];
			}

	

		
			count_watt_data(mmenu_attr);
			count_sports_data(mmenu_attr);
			//SumLevel += mmenu_attr->sports_data[4];
			SumPace += mmenu_attr->sports_data[3];
			SumPulse += mmenu_attr->sports_data[6];
			SumRpm += mmenu_attr->sports_data[0];
			SumWatt += mmenu_attr->sports_data[9];
			
			//draw_rpmHR_data(mmenu_attr);
			
			draw_distanceHR_data(mmenu_attr);
		
			draw_paceHR_data(mmenu_attr);
			
			draw_pulseHR_data(mmenu_attr);
		
			draw_hr_data(mmenu_attr);

			draw_caloriesHR_data(mmenu_attr);
		
			draw_wallsHR_data(mmenu_attr);

			if ((sportsAllTime>0) && (sportsAllTime%120 == 0)) {
				runnedDist = mmenu_attr->sports_data[1]/10000 - oldDist;
				oldDist = runnedDist;
				save_total(120, runnedDist);
			}
			
			if (sportsAllTime == 1) {
				HR0 = mmenu_attr->sports_data[6];
			}
			if (BTSET_FLAG) {
				Send_BT_DATA();
			}
		}
		if (msg->dwAddData1 == HR_PAU_BACK_ID) {
			
		
			HRbackHomeCount ++;

			if (HRbackHomeCount > 180) {
				GUI_KillTimer(msg->h_deswin, HR_PAU_BACK_ID);
				GUI_LyrWinSel(mmenu_attr->layer);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinSel(lry);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinDelete(lry);
				lry = 0;

				runnedDist = mmenu_attr->sports_data[1]/10000 - oldDist;
				save_total(sportsAllTime%120,runnedDist);
				main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data);
			}


		}

		
		if (msg->dwAddData1 == HR_BACK_HOME_ID) {
			HRbackHomeCount ++;

			if (HRbackHomeCount > 180) {
				GUI_KillTimer(msg->h_deswin, HR_BACK_HOME_ID);
				GUI_LyrWinSel(mmenu_attr->layer);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				if (stopCount == 2) {
					GUI_LyrWinSel(lyrThree);
					GUI_SetBkColor(0x00000000);
					GUI_ClearRect(0, 0, 1024, 600);
					GUI_LyrWinDelete(lyrThree);
					lyrThree = 0;
				}
				GUI_LyrWinSel(lry);
				GUI_SetBkColor(0x00000000);
				GUI_ClearRect(0, 0, 1024, 600);
				GUI_LyrWinDelete(lry);
				lry = 0;
				//GUI_LyrWinSel(mmenu_attr->layer);
				HRbackHomeCount = 0;
				
				main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data);
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

__s32 sports_hr_bar_win_create(H_WIN h_parent, mmenu_para_t *para)
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
	mmenu_para->focus_id = para->focus_id;
	timeFlag = para->timeFlag;
	timeTmp  = para->time;
	TargHR = para->targHR;
	BTSET_FLAG = para->BTSET_Flag;
	curID    = para->curID;
	HrFlag  = para->HRFlag;
	HR_pulse = para->HR0;
	Age = para->age;
	weight = para->weight;
	esKRNL_TimeDly(3);
	pageData[0] = 0x60;
	readPage(writeData,pageData);
	unitFlag = writeData[0];
	keyTonFlag = writeData[3];
	pauseModeFlag =writeData[2];
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	"sports_hr_bar_win",
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = SPORTS_HR_BAR_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_sports_hr_bar_Proc);
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

void sports_hr_bar_win_delete(H_WIN mm_win)
{
	GUI_FrmWinDelete(mm_win);
}

void sports_hr_bar_res_uninit(H_WIN hwin)
{    
    mmenu_attr_t * mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(hwin);

    if(mmenu_attr)
    {
        uninit_sports_hr_res(mmenu_attr);
    }
}

void sports_hr_bar_res_init(H_WIN hwin)
{
    mmenu_attr_t * mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(hwin);

    if(mmenu_attr)
    {
        init_sports_hr_res(mmenu_attr);
    }
}

