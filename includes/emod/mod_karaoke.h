/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : mod_karaoke.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-8-11
* Description:
    基于cedar的各插件而设计,故大量复用cedar的数据结构定义,数据类型定义,命令集.
********************************************************************************
*/
#ifndef _MOD_KARAOKE_H_
#define _MOD_KARAOKE_H_

#include "mod_cedar.h"
#include "mod_ginkgo.h"

typedef enum __KARAOKE_CMD_SET            /* 媒体播放中间件命令列表(IOCtrol)                              */
{
    //karaoke中间件,基于cedar core的各插件,与cedar平级
    KARAOKE_CMD_SET_MEDIAFILE = 0x2000,         /* 设置伴音的文件路径 */
    KARAOKE_CMD_SET_RECORD_MEDIAFILE,           /* 设置要录制的文件参数信息, 其中的采样率就会作为microphone的采样率, pbuffer = __ginkgo_media_file_inf* */
    KARAOKE_CMD_SET_CHAN_OUTPUT_MODE,           // 设置播放时的声道输出模式(左、右、立体声), aux = AudioChanOutputMode, return = EPDK_OK/EPDK_FAIL, (__audio_dev_chan_mode_t)
    KARAOKE_CMD_GET_MESSAGE_CHN,                // 得到karaoke的消息反馈通道

    KARAOKE_CMD_ENABLE_MUSIC = 0x2100,          //(1)music和microphone至少选一个, aux = 1:enable; aux=0:disable,通知mixer即可,必须在play之前即stop状态下设置好,中途设置无效.
    KARAOKE_CMD_ENABLE_MICROPHONE,              //(2)aux = 1:通知mixer, 启动audio_rec; aux=0:通知mixer,停止audio_rec,但可能会返回失败.如果只有麦克风的话.
    KARAOKE_CMD_ENABLE_RECORD,                  //(4)aux = 1: enable意味着加载录制插件,进行录制; 可能在播放中途才enable,加载插件进行录制. aux = 0:disable就是关闭插件,文件写结束.
    //KARAOKE_CMD_ENABLE_AUDIOPLYBK,              //(3)audio_plybk和record至少选一个, aux = 1:enable, aux=0:disable

    KARAOKE_CMD_PLAY = 0x2200,
    KARAOKE_CMD_STOP,   //整个停止,播放和录制
    KARAOKE_CMD_PAUSE,  //预留,但不实现. 这个命令指music和录制的暂停，但如果此时microphone说话，还是可以通过音箱传出.
    KARAOKE_CMD_GET_STATUS, 

    KARAOKE_CMD_GET_TOTAL_TIME = 0x2300,

    KARAOKE_CMD_ENABLE_ECHO = 0x2400,            //echo enable,回音消除使能；aux = 1: enable; aux =0: disable; 
    KARAOKE_CMD_ENABLE_ECHO_EFFECT,              //echo effect enable，混音效果使能，aux = 1: enable; aux =0: disable;
    KARAOKE_CMD_SET_ECHO_EFFECT_OUTPUT_MODE,     //设置混音效果模式；
    
    KARAOKE_CMD_SET_VID_LAYERHDL = 0x2504,         /* 设置视频图层句柄 pbuffer = video_layer_hdl                   */
    KARAOKE_CMD_SET_VID_WINDOW,                 /* 设置视频窗口属性 pbuffer = (__rect_t *)                      */
    
    KARAOKE_CMD_

} __karaoke_cmd_set_t;


typedef enum tag_KaraokeStatus          /* the status os cedar                  */
{
    KARAOKE_STAT_PLAY=0,                  /* play                                 */
    KARAOKE_STAT_PAUSE,                   //这个状态指music和录制的暂停，但如果此时microphone说话，还是可以通过音箱传出.
    KARAOKE_STAT_STOP,                    /* stop                                 */
    KARAOKE_STAT_
} KaraokeStatus;


typedef enum tag_AudioChanOutputMode
{
    AUDIO_CHANNEL_MODE_STEREO = 0,           /* 立体声模式                       */
    AUDIO_CHANNEL_MODE_LEFT,                 /* 左声模式                         */
    AUDIO_CHANNEL_MODE_RIGHT,                /* 右声模式                         */

    AUDIO_CHANNEL_MODE_
} AudioChanOutputMode;

typedef enum tag_KaraokeEchoOputMode
{
    AUDIO_ECHO_MODE_0 = 0,              /*do nothing                       */
    AUDIO_ECHO_MODE_1,                 /*模式1                         */
    AUDIO_ECHO_MODE_2,                   /* 模式2                     */
    AUDIO_ECHO_MODE_USER =  0xff,               /* 用户模式           */

    AUDIO_ECHO_MODE_
} KaraokeEchoOutputMode;


typedef enum tag_KaraokeFeedbackMsg           /* 定义媒体播放中间件反馈给应用程序的消息类型               */
{
//    CEDAR_ERROR_NOP                 =   0,  /* 空消息                                                   */
//
    //cedar feed back error type
    KARAOKE_ERROR_OPEN_FILE_FAIL      = -20,  /* 打开当前媒体文件失败                                     */
    KARAOKE_ERROR_FILE_FMT_UNKNOWN    = -19,           /* 无法识别当前媒体文件格式                                 */
    KARAOKE_ERROR_FILE_NOT_SUPPORT    = -18,           /* 当前媒体文件不支持                                       */
//    CEDAR_ERROR_FILE_DATA_ERROR,            /* 媒体文件数据错，无法解析                                 */
    KARAOKE_ERROR_NO_FILE_INPUT       = -16,              /* 未设置要播放的媒体文件                                   */
//    CEDAR_ERROR_READ_FILE_FAIL,             /* 读取文件数据失败                                         */
//
    KARAOKE_ERROR_START_PLG_FAIL      = -40,  /* 启动插件失败                                             */
//    CEDAR_ERROR_ADEC_ERROR,                 /* 音频解码插件解码错误                                     */
//    CEDAR_ERROR_VDEC_ERROR,                 /* 视频解码插件解码错误                                     */
//    CEDAR_ERROR_TDEC_ERROR,                 /* 字幕解码插件解码错误                                     */
//
//    CEDAR_ERROR_PIC_SIZE_TOO_LARGE  = -60,  /* 图片尺寸太大，无法支持解码                               */
//    CEDAR_ERROR_REQ_MEMORY_FAIL,            /* 申请内存失败                                             */
//
//    CEDAR_ERROR_UNKOWN               = -80,  /* 未知错误                                                 */
//

    KARAOKE_GINKGO_ERROR_CREATE_FILE_FAIL = -60,    //GINKGO_ERROR_CREATE_FILE_FAIL
    KARAOKE_GINKGO_ERROR_START_PLG_FAIL,            //GINKGO_ERROR_START_PLG_FAIL

    //cedar feed back status type
    KARAOKE_FEDBAK_NO_ERROR           =  1,   /* 空消息                                                   */
//    CEDAR_FEDBAK_PLAY_END,                  /* 播放媒体文件结束                                         */

    KARAOKE_ERROR_

} KaraokeFeedbackMsg;

typedef struct tag_KaraokeEchoAudioInf
{
    __s32       uInGain;                
    __s32       uOutGain;       
    __s32       uNumDelay;             
    __s32       uDelay[16];         
    __s32       uDecay[16];          

} KaraokeEchoAudioInf;

typedef struct tag_KaraokeEchoEffectInf
{
    //__s32       uEchoEnable;
    //__s32       uEchoEffectEnable; 
    
    KaraokeEchoOutputMode   uEchoOutputMode;       
    KaraokeEchoAudioInf     uEchoAudioInf;
} KaraokeEchoEffectInf;


//typedef struct tag_CedarAudioInputInf   //音频输入的参数,设置adc, 现在不需要, 等需要时再用
//{
//    __u32       uChn;                   /* 录音的声道模式，如果设置非法，则采用default值,类型为__ginkgo_audio_chn_t    */
//    __u32       uSampWidth;             /* 录音样本点的位宽，如果设置非法，则采用default值  */
//    __u32       uAudioSampRate;         /* 音频码流的采样率，如果设置非法，则采用default值  */
//    __u32       uAudioSource;           /* 音频源，如果设置非法，则采用default值, GINKGO_AUDIO_SRC_MIC */
//    __u32       uVolume;                /* 录音的音量值，如果设置非法，则采用default值      */
//    __u32       uAbsFmt;                /* 音频码流类型，if(uAbsFmt == GINKGO_ABS_TYPE_NONE), 将忽略其它相关音频参数                             */
//    __u32       uAudioBps;              /* 音频码流的码率，如果设置非法，则采用default值    */
//    __u32       reserved[12];
//} CedarAudioInputInf;
//__ginkgo_audio_inf_t

#endif  /* _MOD_KARAOKE_H_ */


