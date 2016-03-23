
#include "..\\functions_i.h"

static ES_FILE* g_fp_rec = NULL;
///C500提供fm gain、mic gain、mic pregain、linein gain以及mic直通接口。，以下接口有变化，其余dsk_audio_echo接口跟C100一样。


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//#include "Gain.h"


typedef struct  {
	
  __u32 micPreAmpGainCtl    : 3;//B2-B0
    
  __u32 micPreAmpEnBit      : 1;//B3
	__u32	FMtoLRMirerGain		: 3;//B6-B4 
	__u32 paspreedselect 		: 1;//b7
	__u32 adcInputSrcSel      : 6;//b13-b8. adc input source
	__u32 CosSlopTimeForAntipop:2;//b15-b14
	__u32 adcInputGainCtl     : 3;//b18-16, adc input gain
  __u32 reserved4           : 2;//b20-19
  __u32	LineintoLRMirerGain	: 3;//b23-21
  __u32 MictoLRMirerGain	: 3; //b26-24
  __u32 reserved5           : 4;//b30-b27
  __u32 adcEnBit            : 1;//b31
}AdcAnaCtl;

 //when value ==1,gain== 24db  3db/step
 //when value =0 gain=0;
 //value  rang is 0 -7
__s32 dsk_audio_echo_set_mic_pre_gain(__s32 value)
{
	volatile AdcAnaCtl* pAdcAnaCtl = (volatile AdcAnaCtl*)(0x24 + 0Xf1C23C00);
	pAdcAnaCtl->micPreAmpEnBit=1;
	if(value>7||value<0)
		return -1;
	pAdcAnaCtl->micPreAmpGainCtl=value;

	return 0;
}
 //value  rang is 0-7
 //	  -4.5 db - 6db 1.6 db /step
 
__s32 dsk_audio_echo_set_mic_gain(__s32 value)
{
	volatile AdcAnaCtl* pAdcAnaCtl = (volatile AdcAnaCtl*)(0x24 + 0Xf1C23C00);
	if(value>7||value<0)
			return -1;
	pAdcAnaCtl->MictoLRMirerGain=value;
	return 0;
}
 //value rang is 0-7
 //0db -14db 2 db/step
__s32 dsk_audio_set_fm_gain(__s32 value)
{
	volatile AdcAnaCtl* pAdcAnaCtl = (volatile AdcAnaCtl*)(0x24 + 0Xf1C23C00);
	if(value>7||value<0)
		return -1;
	pAdcAnaCtl->FMtoLRMirerGain=value;
	return 0;

}
  //value rang is 0-7
 //0db -14db 2 db/step
__s32 dsk_audio_set_linein_gain(__s32 value)
 {
	volatile AdcAnaCtl* pAdcAnaCtl = (volatile AdcAnaCtl*)(0x24 + 0Xf1C23C00);
	if(value>7||value<0)
		return -1;
	pAdcAnaCtl->LineintoLRMirerGain	=value;
	return 0;

 }




//type:AUDIO_REC_USR_MIC/AUDIO_REC_USR_FM....
//auto_clean_buf:0代表不会自动清空buffer(录音应用)，为1代表自动清空buffer(非录音的应用)
//详细调用方法参见SDK的app_root_scene.c和record_view_doc.c
__s32 dsk_audio_echo_open(__audio_rec_user_t type, __s32 auto_clean_buf, __s32 gain)
{
    __s32 aux;
    __audio_dev_para_t adev_para;
    
    if(g_fp_rec)
    {
        __wrn("rec hdl alreadly open...\n");
        return EPDK_FAIL;
    }
    
    g_fp_rec = eLIBs_fopen("b:\\AUDIO\\REC", "r+");    

    if(NULL == g_fp_rec)
    {
        __wrn("open rec hdl fail...\n");
        return EPDK_FAIL;
    }

    
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_REG_USERMODE, type, 0);

    adev_para.chn = 2;
    adev_para.bps = 16;
    adev_para.fs = 44100;
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_PARA, 0, (void *)&adev_para);

    //eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_REG_USERMODE, type, 0);
    
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_SAMPCNT, 0, 0);//清adc buffer计数器;

    aux = 1;
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_ECHO, aux, 0);

    aux = auto_clean_buf;
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_BUF_MODE, aux, 0);
    
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_REC_LOW_PASS, 1, 0);
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_START, 0, 0);

	if(gain == 0)
	{
	    dsk_audio_echo_set_mic_pre_gain(0);
	    dsk_audio_echo_set_mic_gain(7);
	}
	else
	{
	    dsk_audio_echo_set_mic_pre_gain(1);
	    dsk_audio_echo_set_mic_gain(0);
	}

	//dsk_audio_echo_set_delay_num(echo_delay);
	//dsk_audio_echo_set_delay_time(echo_time); 
    dsk_audio_direct_output_on();
    
    return EPDK_OK;
}

//详细调用方法参见SDK的app_root_scene.c和record_view_doc.c
__s32 dsk_audio_echo_close(void)
{
    __s32 aux;
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    aux = 0;
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_ECHO, aux, 0);

    aux = 0;
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_BUF_MODE, aux, 0);

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_REC_LOW_PASS, 0, 0);
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_STOP, 0, 0);
    
    if(g_fp_rec)
    {
        eLIBs_fclose(g_fp_rec);
        g_fp_rec = NULL;
    }
    
    dsk_audio_direct_output_off();
    
    return EPDK_OK;
}

/*
//该函数一般不要调用，按照默认的就行了
//0<= pre_gain <= 3
__s32 dsk_audio_echo_set_mic_pre_gain(__s32 pre_gain)
{
    pre_gain &= 0x03;
    
    *(volatile unsigned long*)(0xf1c23c28) &= ~(0x03<<25);
    *(volatile unsigned long*)(0xf1c23c28) |= (pre_gain<<25);
    
    return EPDK_OK;
}

//该函数一般不要调用，按照默认的就行了
//0<= gain <= 7
__s32 dsk_audio_echo_set_mic_gain(__s32 gain)
{
    gain &= 0x07;
    
    *(volatile unsigned long*)(0xf1c23c28) &= ~(0x07<<20);
    *(volatile unsigned long*)(0xf1c23c28) |= (gain<<20);
    return EPDK_OK;   
}
*/
//该接口暂时不用，CPU跑不够，增加mic音量，-32到32，可以调节mic音量
__s32 dsk_audio_echo_gain_incr(void)
{
    __s32 aux;

    __wrn("dsk_audio_echo_gain_incr...\n");
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    aux = eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_GET_ECHO_REC_GAIN, 0, 0);
    if(aux < 32)
    {
        aux++;
    }

    __wrn("dsk_audio_echo_gain_incr, aux=%d\n", aux);

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_REC_GAIN, aux, 0);
    
    return EPDK_OK;
}

//该接口暂时不用，CPU跑不够，减小mic音量 -32到32，可以调节mic音量
__s32 dsk_audio_echo_gain_decr(void)
{
    __s32 aux;    
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    aux = eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_GET_ECHO_REC_GAIN, 0, 0);
    if(aux > -32)
    {
        aux--;
    }

    __wrn("dsk_audio_echo_gain_decr, aux=%d\n", aux);
    
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_REC_GAIN, aux, 0);
    
    return EPDK_OK;
}

//该函数不调用，调试用
__s32 dsk_audio_echo_pt_regs(__u32 start, __u32 end)
{
    __u32 i;
    
    //eLIBs_printf("*****dsk_audio_echo_pt_regs begin******\n");

    for(i = start ; i <= end ; i+= 4)
    {
        //eLIBs_printf("0x%8x:0x%8x\n", i, *(volatile unsigned long*)i);
        ;
    }
    
    //eLIBs_printf("*****dsk_audio_echo_pt_regs end******\n");

    return EPDK_OK;
}

/*aux: 0<= aux <= 10 设置混响延时次数，默认为3*/
__s32 dsk_audio_echo_set_delay_num(__s32 aux)
{
    __wrn("dsk_audio_echo_set_delay_num...\n");
	
	//echo_delay=aux;
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_DELAY_NUM, aux, 0);
    
    return EPDK_OK;
}

/*aux:10<= aux <= 3000,默认为100，以毫秒为单位*/
__s32 dsk_audio_echo_set_delay_time(__s32 aux)
{
    __wrn("dsk_audio_echo_set_delay_num...\n");
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_DELAY_TIME, aux, 0);
    
    return EPDK_OK;
}

__s32 dsk_audio_echo_is_open(void)
{
    if(g_fp_rec)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*是否抑制MIC啸叫，aux: 0:disable ec, 1: enable ec*/
__s32 dsk_audio_echo_enable_ec(__s32 aux)
{
    __wrn("dsk_audio_echo_enable_ec, aux=%d...\n", aux);
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_ECHO_CANCEL, aux, 0);
    
    return EPDK_OK;
}
/*该接口暂时不可用，CPU跑不够，设置变调模式:
mode: 0xff:用户模式，1:变女声 2:变男声 3:变童声
level:如果mode=0xff,那么level为变调等级-8<=level<=8, 如果mode!=0xff，那么level无效*/
__s32 dsk_audio_echo_set_pitch_mode(__s32 mode, __s32 level)
{
    __wrn("dsk_audio_echo_set_pitch_mode, mode=%d, level=%d...\n", mode, level);
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ECHO_SET_PITCH_MODE, mode, (void*)level);
    
    return EPDK_OK;
}

/*是否打开混响输出，aux: 0:disable , 1: enable */
__s32 dsk_audio_echo_enable_output(__s32 aux)
{
    __wrn("dsk_audio_echo_enable_ec, aux=%d...\n", aux);
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }        

    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_ECHO_OUTPUT, aux, 0);
    
    if(0 == aux)
  	{
  		dsk_audio_direct_output_off();
  	}
  	else
	{  				
		dsk_audio_direct_output_on();
	}
    
    return EPDK_OK;
}


//mic 直通功能开，mic  采样信号(可能经过微小放大)直接通过功放输出
__s32 dsk_audio_direct_output_on(void)
{       
    (*(volatile __u32 *)(0xf1c23c00+0x20))|=(1<<28)|(1<<29);//enable analog output
    (*(volatile __u32 *)(0xf1c23c00+0x20))|=(1<<24)|(1<<25);//lect mix input
    (*(volatile __u32 *)(0xf1c23c00+0x20))|=(1<<20)|(1<<12);// mute the l and r channle mic
    (*(volatile __u32 *)(0xf1c23c00+0x24))|=(1<<3);
    
    return 0;
}

//mic 直通功能关, mic  采样信号经过2399  或者主控处理, 再通过功放输出
__s32 dsk_audio_direct_output_off(void)
{    
    (*(volatile __u32 *)(0xf1c23c00+0x20))&=(~(1<<20)|~(1<<12));// mute the l and r channle mic
       
    return 0;
}
/*设置BUFFER模式，aux:0代表不会自动清空buffer(录音应用)，为1代表自动清空buffer(非录音的应用)，默认不会自动清空*/
__s32 dsk_audio_echo_set_buf_mode(__s32 aux)
{
 	__wrn("dsk_audio_echo_enable_output, aux=%d...\n", aux);
    
    if(NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }       
    
    eLIBs_fioctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_BUF_MODE, aux, 0);
    
    return EPDK_OK;
}

