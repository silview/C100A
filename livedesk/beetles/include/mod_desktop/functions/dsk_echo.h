#ifndef __COM_ECHO_H__
#define __COM_ECHO_H__

#include "apps.h"

//type:AUDIO_REC_USR_MIC/AUDIO_REC_USR_FM....
//auto_clean_buf:0代表不会自动清空buffer(录音应用)，为1代表自动清空buffer(非录音的应用)
//详细调用方法参见SDK的app_root_scene.c和record_view_doc.c
__s32 dsk_audio_echo_open(__audio_rec_user_t type, __s32 auto_clean_buf, __s32 gain);

//详细调用方法参见SDK的app_root_scene.c和record_view_doc.c
__s32 dsk_audio_echo_close(void);

//该函数一般不要调用，按照默认的就行了
//0<= pre_gain <= 3
__s32 dsk_audio_echo_set_mic_pre_gain(__s32 pre_gain);

//该函数一般不要调用，按照默认的就行了
//0<= gain <= 7
__s32 dsk_audio_echo_set_mic_gain(__s32 gain);

//该接口暂时不用，CPU跑不够，增加mic音量，-32到32，可以调节mic音量
__s32 dsk_audio_echo_gain_incr(void);

//该接口暂时不用，CPU跑不够，减小mic音量 -32到32，可以调节mic音量
__s32 dsk_audio_echo_gain_decr(void);

//该函数不调用，调试用
__s32 dsk_audio_echo_pt_regs(__u32 start, __u32 end);

/*aux: 0<= aux <= 10 设置混响延时次数，默认为3*/
__s32 dsk_audio_echo_set_delay_num(__s32 aux);

/*aux:10<= aux <= 3000,默认为100，以毫秒为单位*/
__s32 dsk_audio_echo_set_delay_time(__s32 aux);

/*是否抑制MIC啸叫，aux: 0:disable ec, 1: enable ec*/
__s32 dsk_audio_echo_enable_ec(__s32 aux);
/*该接口暂时不可用，CPU跑不够，设置变调模式:
mode: 0xff:用户模式，1:变女声 2:变男声 3:变童声
level:如果mode=0xff,那么level为变调等级-8<=level<=8, 如果mode!=0xff，那么level无效*/
__s32 dsk_audio_echo_set_pitch_mode(__s32 mode, __s32 level);

__s32 dsk_audio_echo_is_open(void);

/*是否打开混响输出，aux: 0:disable , 1: enable */
__s32 dsk_audio_echo_enable_output(__s32 aux);



#endif//__COM_ECHO_H__


