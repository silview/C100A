/************************************************Copyright(c)**************************************
**                                   Quintic(Beijing) Microelectronics Co,Ltd.
**                                   
**                                   http:\\www.quinticcorp.com 
**
**--------------------File Info----------------------------------------------------------------
** File Name:    qnws.h
** Last Modified Date: 2011-10-10
** Last Version:     
** Descriptions: 
**
**----------------------------------------------------------------------------------------
** Created by:   quintic AE
** Created Date: 2011-8-24
** Version:      1.0
** Descriptions: 
**
**************************************************************************************************/

#ifndef _QNWS_H_
#define _QNWS_H_

#define ENABLE_PILOT_CHECK    1


#define FSM_CCA           0
#define FSM_RDS_CHECK     1
#define FSM_MUTE_CHECK    2
#define FSM_TUNE_CHANNEL  3

#define DEFAULT_KEY       0xFF

//#define WAITTING4RDS      30
//#define WAITTING4MUTE     50
//#define WAITTING4RESET    100
#define WAITTING4RDS      5
#define WAITTING4MUTE     2//100
#define WAITTING4RESET    5//300

#define START_FREQ        10850
#define STOP_FREQ         11100

#define STATE_UNMUTE      0
#define STATE_MUTE        1

#define MUTE_SNR_TH       20 // snr thrshold for auto mute detection
#define SHUT_SNR_TH       10 // snr thrshold for auto mute detection

#define MUTE_RSSI_TH      22
#define SHUT_RSSI_TH      8

#define PEAK_LOW_COUNT    30


#define AA_MODE_WS        0 // air audio mode : wireless speaker
#define AA_MODE_WM        1 // air audio mode : wireless microphone

#define SNR_HOLD          0
#define SNR_GOOD          1
#define SNR_BAD           2
#define SNR_SHUT          3


extern unsigned char g_AAEnable;
extern unsigned char g_AAMode;
extern unsigned char g_fsm;

extern unsigned char g_exitflag;

extern unsigned char QNA_AirAudio(void);

#endif
