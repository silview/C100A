/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : rda5820.h
* By      : xueli
* Version : V1.00
*********************************************************************************************************
*/
#ifndef  _RDA_FMG1208_H_
#define  _RDA_FMG1208_H_

#include "epdk.h"

/****************************Chips ID definition*******************************/
#define CHIPID_QN8000                   0x00
#define CHIPID_QN8005                   0x20
#define CHIPID_QN8005B1                 0x21
#define CHIPID_QN8006                   0x30
#define CHIPID_QN8006LB                 0x71
#define CHIPID_QN8007B1                 0x11
#define CHIPID_QN8007                   0x10
#define CHIPID_QN8006A1                 0x30
#define CHIPID_QN8006B1                 0x31
#define CHIPID_QN8016                   0xe0
#define CHIPID_QN8016_1                 0xb0
#define CHIPID_QN8015                   0xa0
#define CHIPID_QN8065                   0xa0
#define CHIPID_QN8067                   0xd0
#define CHIPID_QN8065N                  0xa0
#define CHIPID_QN8027                   0x40
#define CHIPID_QN8025                   0x80
#define CHIPID_QN8035                   0x84
#define CHIPSUBID_QN8035A0              0x01
#define CHIPSUBID_QN8035A1              0x02
#define CHIPID_QN8026                   0x3C
#define CHIPID_QN8036                   0x34
/***************************************End************************************/

// RDS, TMC
#define QND_EUROPE_FLEXIBILITY_DISABLE  0
#define QND_EUROPE_FLEXIBILITY_ENABLE   1
#define QND_RDS_OFF                     0
#define QND_RDS_ON                      1
#define QND_RDS_BUFFER_NOT_READY        0
#define QND_RDS_BUFFER_READY            1


// tune
#define QND_FSTEP_50KHZ                 0
#define QND_FSTEP_100KHZ                1
#define QND_FSTEP_200KHZ                2


#define FREQ2CHREG(freq)                ((freq-6000)/5)
#define CHREG2FREQ(ch)                  (ch*5+6000)



#define TRUE           1
#define FALSE          0


typedef __u8 UINT8;
typedef __u16 UINT16;
typedef __s16 INT16;
typedef __s8 INT8;


/**************************minor feature selection*****************************/
#define QN_CCA_MAX_CH                   20

#define _QNCONFIG_H_
/******************************country selection*******************************/
#define COUNTRY_CHINA                   0
#define COUNTRY_USA                     1
#define COUNTRY_JAPAN                   2
/***************************************End************************************/

#define QND_MODE_SLEEP                  0     
#define QND_MODE_WAKEUP                 1

// RX / TX value is using upper 8 bit
#define QND_MODE_RX                     0x8000
#define QND_MODE_TX                     0x4000
// AM / FM value is using lower 8 bit 
// need to check datasheet to get right bit 
#define QND_MODE_FM                     0x0000

#define BAND_FM                         0

// tune
#define QND_FSTEP_50KHZ                 0
#define QND_FSTEP_100KHZ                1
#define QND_FSTEP_200KHZ                2

// audio output format
#define QND_OUTPUT_ANALOG               0
#define QND_OUTPUT_IIS                  1

// stereo mode
#define QND_TX_AUDIO_MONO               0x10
#define QND_TX_AUDIO_STEREO             0x00

#define QND_RX_AUDIO_MONO               0x20
#define QND_RX_AUDIO_STEREO             0x00

#define QND_CONFIG_MONO                 0x01
#define QND_CONFIG_MUTE                 0x02    
#define QND_CONFIG_SOFTCLIP             0x03
#define QND_CONFIG_AUTOAGC              0x04
#define QND_CONFIG_AGCGAIN              0x05
#define QND_CONFIG_EQUALIZER            0x06
#define QND_CONFIG_VOLUME               0x07          
#define QND_CONFIG_BASS_QUALITY         0x08
#define QND_CONFIG_BASS_FREQ            0x09
#define QND_CONFIG_BASS_GAIN            0x0a
#define QND_CONFIG_MID_QUALITY          0x0b
#define QND_CONFIG_MID_FREQ             0x0c
#define QND_CONFIG_MID_GAIN             0x0d
#define QND_CONFIG_TREBLE_FREQ          0x0e
#define QND_CONFIG_TREBLE_GAIN          0x0f

#define QND_ENABLE_EQUALIZER            0x10
#define QND_DISABLE_EQUALIZER           0x00


#define QND_CONFIG_AUDIOPEAK_DEV        0x11
#define QND_CONFIG_PILOT_DEV            0x12
#define QND_CONFIG_RDS_DEV              0x13


#define _QNDRIVER_H
#define QN_8035
#define CRY_24M 0x24
#define CRY_32K 0x32
#define SYS_CRY CRY_24M
//假台多可以打开
#define PILOT_CCA                       0  // 0: disable stereo judgement for the country has many mono FM stations
//一定打开
#define USING_VALID_CH                  1
//#define CCA_PILOT_SNR_FILTER            35//25


//#define FREQ2CHREG(freq)                ((freq-6000)/5)
//#define CHREG2FREQ(ch)                  (ch*5+6000)

/**********************************QN8035's clock source selection**************
1.QN8035's default clock source is 32768HZ.
2.setting QN8035's clock source and clock source type(like sine-wave clock or digital clock).
3.user need to modify clock source according to actual hardware platform.
4.clock formula,the details please refer to the QN8035's datasheet
  XTAL_DIV = Round(Clock/32768);
  PLL_DLT = Round((28500000*512*XTAL_DIV)/Clock)-442368
*******************************************************************************/
#define QND_SINE_WAVE_CLOCK             0x00    //inject sine-wave clock
#define QND_DIGITAL_CLOCK               0x80    //inject digital clock,default is inject digital clock

#if(SYS_CRY == CRY_32K)
//crystal clock is 32768HZ
#define QND_XTAL_DIV0                   0x01
#define QND_XTAL_DIV1                   0x08
#define QND_XTAL_DIV2                   0x5C
#endif

/*#if(SYS_CRY == CRY_12M)
//crystal clock is 12MHZ
#define QND_XTAL_DIV0                   0x6E
#define QND_XTAL_DIV1                   0x01
#define QND_XTAL_DIV2                   0x54
#endif*/

#if(SYS_CRY == CRY_24M)
//crystal clock is 24MHZ
#define QND_XTAL_DIV0                   0xDC
#define QND_XTAL_DIV1                   0x02
#define QND_XTAL_DIV2                   0x54   //3
#endif

//following is definition step
#define QND_STEP_CONSTANT               0x40 //(QND_FSTEP_100KHZ << 6)
#define CHIPID_QN8035                   0x84


/*******************************************************************************
Performance configuration 
*******************************************************************************/
#define SMSTART_VAL                     10//19
#define HCCSTART_VAL                    10//25
#define SNCSTART_VAL                    51

enum {
    // Bit[15-8] of the word: RSSI Threshold
    // Bit[7-0] of the word: SNR Threshold
    // e.g. 0x1E06 => RSSI_TH = 0x1E, SNR_TH = 0x06
    // notice: the rang of RSSI is 0x0A ~ 0x3F
    // notice: the rang of SNR is 0x00 ~ 0x3F
   
    CCA_SENSITIVITY_LEVEL_0 = 0x1E06,
    CCA_SENSITIVITY_LEVEL_1 = 0x1E07,   //if using the pilot as CCA,reference this item.
    CCA_SENSITIVITY_LEVEL_2 = 0x1E08,
    CCA_SENSITIVITY_LEVEL_3 = 0x1E09,
    CCA_SENSITIVITY_LEVEL_4 = 0x1E0A,   //if not using the pilot as CCA,reference this item.
    CCA_SENSITIVITY_LEVEL_5 = 0x1E0B,       
    CCA_SENSITIVITY_LEVEL_6 = 0x1E0C,
    CCA_SENSITIVITY_LEVEL_7 = 0x1E0D,
    CCA_SENSITIVITY_LEVEL_8 = 0x1E0E,
    CCA_SENSITIVITY_LEVEL_9 = 0x1E0F
};
#define CCA_PILOT_SNR_FILTER            25 
#define CCA_PILOT_READ_DELAY            60
#define CCA_PILOT_READ_COUNT            10
#define CCA_PILOT_READ_COUNT_DELAY      2
#define CCA_PILOT_TH                    3
/***************************************End************************************/


/*******************************************************************************
limitation configuration 
*******************************************************************************/
#define QND_READ_RSSI_DELAY             10
#define SLEEP_TO_WAKEUP_DELAY_TIME      500       
#define CH_SETUP_DELAY_TIME             200  
#define CCA_TIME_OUT					100
/***************************************End************************************/

/*******************************************************************************
 definition register 
*******************************************************************************/
#define SYSTEM1         				0x00
#define CCA             				0x01
#define SNR            					0x02
#define RSSISIG         				0x03
#define STATUS1         				0x04
#define CID1            				0x05
#define CID2            				0x06
#define	CH								0x07
#define	CH_START						0x08
#define	CH_STOP							0x09
#define	CH_STEP							0x0A
#define	RDSD0							0x0B
#define	RDSD1							0x0C
#define	RDSD2							0x0D
#define	RDSD3							0x0E
#define	RDSD4							0x0F
#define	RDSD5							0x10
#define	RDSD6							0x11
#define	RDSD7							0x12
#define	STATUS2							0x13
#define	VOL_CTL							0x14
#define	XTAL_DIV0						0x15
#define	XTAL_DIV1						0x16
#define	XTAL_DIV2						0x17
#define INT_CTRL						0x18
#define SMP_HLD_THRD					0x19
#define	RXAGC_GAIN						0x1A
#define GAIN_SEL						0x1B
#define	SYSTEM_CTL1						0x1C
#define	SYSTEM_CTL2						0x1D
#define RDSCOSTAS						0x1E
#define REG_TEST						0x1F
#define STATUS4							0x20
#define RDSAGC2							0x21
#define	CCA1							0x27
#define	CCA2							0x28
#define	CCA3							0x29
#define	CCA4							0x2A
#define	CCA5							0x2B
#define PLT1                            0X2F
#define	PLT2                            0x30
#define	SMSTART							0x34
#define	SNCSTART						0x35
#define	HCCSTART						0x36
#define	CCA_CNT1					    0x37
#define	CCA_CNT2					    0x38
#define	CCA_SNR_TH_1					0x39
#define	CCA_SNR_TH_2					0x3A
#define NCCFIR3         				0x40
#define REG_REF                         0x49
#define REG_DAC							0x4C
/***************************************End************************************/

/*******************************************************************************
 definition operation bit of register
*******************************************************************************/
#define CCA_CH_DIS      				0x01
#define CHSC            				0x02
#define RDSEN           				0x08
#define CH_CH		    				0x03
#define CH_CH_START     				0x0c
#define CH_CH_STOP      				0x30
#define STNBY_MODE           		    0x20
#define RX_MODE         				0x10
#define IMR             				0x40
#define RDS_RXUPD       				0x80
#define ST_MO_RX                        0x01
#define STNBY_RX_MASK                   0x30
#define RXCCA_MASK                      0x03
#define RX_CCA                          0x02
#define RXCCA_FAIL                      0x08
#define RX_MONO                         0x04
#define ICPREF                          0x0F
/***************************************End************************************/

#include  "drv_fm_i.h"

#if(FM_MODULE_TYPE == FM_MODULE_TYPE_G1208)

#define  FM_MAX_FREQ               108000
#define  FM_MIN_FREQ                87000

typedef struct __FM_SETTING_INFO
{
    __s32   min_freq;
    __s32   max_freq;
    __s8    area_select;
    __s8    stereo_select;
    __s8    signal_level;
    __s8    reserved;

}__fm_setting_info_t;

#define _QNSYS_H_

#define CHANNEL_FILTER

// Triple frequency converter for high frequency (triple) channel receiver
// usage: convert high frequency to normal frequency without FM chip working range
// example: 
//        QND_TuneToCH(TRIPLE_FREQ_CONVERTER(high_freq));
// 
#define TRIPLE_FREQ_CONVERTER(trifreq)  (((((trifreq * 10 + 225) / 3 + 225)/10+2)/5)*5)

// external driver interface 
// logical layer
/*****************************************************************************
Driver API Macro Definition
*****************************************************************************/
#define QNM_GetMonoMode() \
        QND_ReadReg(STATUS1) & ST_MO_RX        
#define QNM_GetRssi() \
        QND_ReadReg(RSSISIG)
#define QND_READ(adr)    QND_ReadReg(adr)
#define QND_WRITE(adr, value)  QND_WriteReg(adr, value)


extern UINT8 QND_ReadReg(UINT8 adr);
extern UINT8 QND_WriteReg(UINT8 adr, UINT8 value);

extern void   QNF_SetRegBit(UINT8 reg, UINT8 bitMask, UINT8 data_val) ;


extern UINT16 QNF_GetCh() ;
extern void  QND_Delay(UINT16 ms) ;
extern UINT8 QND_GetRSSI(UINT16 ch) ;
extern UINT8 QND_Init(void) ;
extern void  QND_TuneToCH(__s32 ch) ;
extern void  QND_SetSysMode(UINT16 mode) ;
extern void  QND_SetCountry(UINT8 country) ;
extern void  QNF_SetMute(UINT8 On);

#define QN_RX
#define _QNRX_H_
typedef void  (*QND_SeekCallBack)(UINT16 ch, UINT8 bandtype);
extern UINT8   qnd_ChCount;
extern UINT16  qnd_ChList[QN_CCA_MAX_CH];
extern UINT8   qnd_StepTbl[3];

extern void   QND_SetSeekCallBack(QND_SeekCallBack func);
extern void   QND_RXConfigAudio(UINT8 optiontype, UINT8 option) ;
extern void   QND_RXSetTH(UINT8 th);
extern UINT8 QND_RXValidCH(UINT16 freq, UINT8 step); 
extern UINT16 QND_RXSeekCH(UINT16 start, UINT16 stop, UINT8 step, UINT8 db, UINT8 up) ;
extern UINT8  QND_RXSeekCHAll(UINT16 start, UINT16 stop, UINT8 step, UINT8 db, UINT8 up) ;
#define QN_RDS

#define _QNRDS_H_

extern UINT8 QND_RDSEnable(UINT8 on) ;
extern UINT8 QND_RDSCheckBufferReady(void) ;
void QND_RDSHighSpeedEnable(UINT8 on) ;
INT8 QND_RDSModeDetect(void)  ;
extern UINT8 QND_RDSDetectSignal(void) ;
extern void  QND_RDSLoadData(UINT8 *rdsRawData, UINT8 upload) ;

/**************************************************************************************************
unsigned char QNA_WSGetRdsInfo(unsigned short ch, unsigned char key, unsigned char* rds_data)
***************************************************************************************************
Description: Decrypt and parse the rds data
Parameters: rds data buffer
Return Value:
bit7|bit6~bit5|bit4|bit3|bit2|bit1|bit0|
pairing status|mute flag|0|0|0|pilot error|key error|
bit7: 1 pairing successful, 0 pairing failed
bit6~bit5: 01 unmute, 10 mute, 00 reserved 
****************************************************************************************************/
extern unsigned char QNA_WSGetRdsInfo(unsigned short ch, unsigned char key, unsigned char *ptr_increase);


#ifdef QN8035_ONLY_FM
extern   __s32  fm_init(void);
#else
extern   __s32  fm_init(__s32 flag);
#endif
extern   __s32  fm_exit(void);
extern   __s32  fm_play(__s32 freq);
extern   __s32  fm_auto_search(__s32 freq, __u32 search_dir);
extern   __s32  fm_manual_search(__s32 freq, __u32 search_dir);
extern   __s32  fm_area_choose(__s32 area, void *pbuffer);
extern   __s32  fm_stereo_choose(__s32 audio_method);
extern   __s32  fm_send(__s32 freq);
extern   __s32  fm_mute(__s32 voice_onoff);
extern   __s32  fm_signal_level(__s32 signal_level);

extern   __s32  fm_send_on(void);
extern   __s32  fm_send(__s32 freq);
extern   __s32  fm_send_off(void);
extern   __s32  fm_pa_gain(__u8 pagain);

extern   __s32  fm_get_status(void);

#endif


#endif /*_RDA_FMG1208_H_*/
