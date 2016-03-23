/*
************************************************************************************************************************
*                                                        CUCKOO
*                                                   the Audio Player
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : misc.c
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2008.11.08
*
* Description :
* 
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2008.11.08       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __misc_c
#define  __misc_c

#include "Misc.h"

#ifdef KEY_VOICE
#define LANG_COUNT 3

static char* g_short_key_wav_file[SHORT_KEY_WAVE_FILE_NUM][LANG_COUNT]=
{
	{"d:\\res\\sounds\\chs\\fm.wav",			  	  "d:\\res\\sounds\\chs\\fm.wav",		     		"d:\\res\\sounds\\chs\\fm.wav",		   },
	{"d:\\res\\sounds\\chs\\photo.wav",		    "d:\\res\\sounds\\chs\\photo.wav", 	   		"d:\\res\\sounds\\chs\\photo.wav",	   },
	{"d:\\res\\sounds\\chs\\movie.wav",		    "d:\\res\\sounds\\chs\\movie.wav", 	   		"d:\\res\\sounds\\chs\\movie.wav",	   },
	{"d:\\res\\sounds\\chs\\music.wav",		    "d:\\res\\sounds\\chs\\music.wav", 	   		"d:\\res\\sounds\\chs\\music.wav",	   },
	{"d:\\res\\sounds\\chs\\setting.wav", 	    "d:\\res\\sounds\\chs\\setting.wav",	   	"d:\\res\\sounds\\chs\\setting.wav",	   },
	{"d:\\res\\sounds\\chs\\ebook.wav",		    "d:\\res\\sounds\\chs\\ebook.wav", 	   		"d:\\res\\sounds\\chs\\ebook.wav",	   },
	{"d:\\res\\sounds\\chs\\record.wav",		    "d:\\res\\sounds\\chs\\record.wav",	   		"d:\\res\\sounds\\chs\\record.wav",	   },
	{"d:\\res\\sounds\\chs\\prev.wav",		  	  "d:\\res\\sounds\\chs\\prev.wav",		   		"d:\\res\\sounds\\chs\\prev.wav", 	   },
	{"d:\\res\\sounds\\chs\\next.wav",		  	  "d:\\res\\sounds\\chs\\next.wav",		   		"d:\\res\\sounds\\chs\\next.wav", 	   },
	{"d:\\res\\sounds\\chs\\mute_off.wav",	    "d:\\res\\sounds\\chs\\mute_off.wav",	 		"d:\\res\\sounds\\chs\\mute_off.wav",    },
	{"d:\\res\\sounds\\chs\\mute_on.wav", 	    "d:\\res\\sounds\\chs\\mute_on.wav",	   	"d:\\res\\sounds\\chs\\mute_on.wav",	   },
	{"d:\\res\\sounds\\chs\\next_song.wav",	  "d:\\res\\sounds\\chs\\next_song.wav",  	"d:\\res\\sounds\\chs\\next_song.wav",   },
	{"d:\\res\\sounds\\chs\\prev_song.wav",	  "d:\\res\\sounds\\chs\\prev_song.wav",  	"d:\\res\\sounds\\chs\\prev_song.wav",   },
	{"d:\\res\\sounds\\chs\\play.wav",		  	  "d:\\res\\sounds\\chs\\play.wav",		   		"d:\\res\\sounds\\chs\\play.wav", 	   },
	{"d:\\res\\sounds\\chs\\pause.wav",		    "d:\\res\\sounds\\chs\\pause.wav", 	   		"d:\\res\\sounds\\chs\\pause.wav",	   },
	{"d:\\res\\sounds\\chs\\record_start.wav",	"d:\\res\\sounds\\chs\\record_start.wav", "d:\\res\\sounds\\chs\\record_start.wav", },
	{"d:\\res\\sounds\\chs\\record_pause.wav", "d:\\res\\sounds\\chs\\record_pause.wav", "d:\\res\\sounds\\chs\\record_pause.wav",},
	{"d:\\res\\sounds\\chs\\record_stop.wav", 	"d:\\res\\sounds\\chs\\record_stop.wav",  "d:\\res\\sounds\\chs\\record_stop.wav", },
};

typedef struct __WAVE_HEADER1
{
    __u32       uRiffFcc;       // four character code, "RIFF"
    __u32       uFileLen;       // file total length, don't care it

    __u32       uWaveFcc;       // four character code, "WAVE"

    __u32       uFmtFcc;        // four character code, "fmt "
    __u32       uFmtDataLen;    // Length of the fmt data (=16)
    __u16       uWavEncodeTag;  // WAVE File Encoding Tag
    __u16       uChannels;      // Channels: 1 = mono, 2 = stereo
    __u32       uSampleRate;    // Samples per second: e.g., 44100
    __u32       uBytesPerSec;   // sample rate * block align
    __u16       uBlockAlign;    // channels * bits/sample / 8
    __u16       uBitsPerSample; // 8 or 16

    __u32       uDataFcc;       // four character code "data"
    __u32       uSampDataSize;  // Sample data size(n)

} __wave_header_t1;
#endif


#define COM_GET_R_VAL(color) ( ((__u32)color)&0xff )
#define COM_GET_G_VAL(color) ( (((__u32)color)>>8)&0xff )
#define COM_GET_B_VAL(color) ( (((__u32)color)>>16)&0xff )
#define COM_GET_ALPHA_VAL(color) ( (((__u32)color)>>24)&0xff )

#define PIOC_MEM_BASE   (0xf1c20800)

#define PIO_REG_CFG(n, i)               ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x00))
#define PIO_REG_DLEVEL(n, i)            ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x14))
#define PIO_REG_PULL(n, i)              ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x1C))
#define PIO_REG_DATA(n) 	            ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + 0x10))

//libaiao, 2011.5.15
//根据指定长度从src中截取字符串
//返回值为窃取字符串的长度(byte为单位)
__s32 GUI_GetStringByWidth(char *dst, const char GUI_UNI_PTR *src, __u32 width) 
{
    const char GUI_UNI_PTR * s = src;
    U16 r = 0,w = 0, len = 0;
	
	
	r   = GUI_UC_GetCharCode(s);
	if(r == 0)
	{
		return 0;
	}
	while(r != 0)
	{
		if(w >= width)
		{
			break;
		}	
    	r   = GUI_UC_GetCharCode(s+len);
	    len  += GUI_UC_GetCharSize(s+len);		//指向下一个字符
		w += GUI_GetCharDistX(r);
	}
	eLIBs_strncpy(dst, src, len);
    return len;
}

//libaiao, 2011.5.15
//extern __s32 GUI_GetStringByWidth(char *dst, const char GUI_UNI_PTR *src, __u32 width);
//根据可显示区域截取字串
//如果字串宽度大于显示区域,则在可显示字串后加三个省略号
//注意该函数只判断宽度，不判断高度
//append为追加在后面的省略号
__s32 GetClippedString(GUI_RECT *region, char *src_str, char *dst_str, const char *append)
{
	__u32 src_width = 0, dst_width = 0, append_width = 0;	
	__u32 len = 0, char_width = 0;
	__u32 gap = 0;
	
	dst_width = region->x1 - region->x0;	
	src_width = GUI_GetStringDistX(src_str);
	if(dst_width >= src_width)
	{
		eLIBs_strcpy(dst_str, src_str);
		return EPDK_OK;
	}
	append_width = GUI_GetStringDistX(append);
	if(append_width >= dst_width)
	{
		eLIBs_strcpy(dst_str, append);
		return EPDK_OK;
	}
	gap = dst_width - append_width;
	GUI_GetStringByWidth(dst_str, src_str, gap);
	//GUI_DispStringLen();
	eLIBs_strcat(dst_str, append);
	return EPDK_OK;
}

void time2time( __u32 second_time, cuckoo_time_t *format_time )
{
	__u32  total_minute;
	
	total_minute = second_time/60;
	format_time->second = second_time%60;
	format_time->minute = total_minute%60;
	format_time->hour   = total_minute/60;	
}

void   uint2str( __u32 input, char * str )
{
	char stack[11] ;
	int i ;
	int j ;
	int count;
	
	
	if( input == 0 )
	{
	__msg("come to here to convert the num\n");
		str[0] = '0';
		str[1] = '0';
		str[2] = '\0';                   // 'str' must end with '\0'
		return ;
	}
	
	for( i = 0; input > 0; ++i )
	{
		stack[i] = input%10 + '0';       // characters in reverse order are put in 'stack' .
		input /= 10;
	}                                    // at the end of 'for' loop, 'i' is the number of characters.
	count = i;
	
	for( j = 0, i = 0; i < ( 2-count); i++ )    // pad '0'
		str[j++] = '0';
	for( i = count, --i; i >= 0; --i, ++j )
		str[j] = stack[i];		
	str[j] = '\0';		                 // 'str' must end with '\0'
}

static void __time2str( const cuckoo_time_t *time, char *str )
{
	uint2str( time->hour,   str );
	str[2] = ':';
	uint2str( time->minute, str+3 );
	str[5] = ':';
	uint2str( time->second, str+6 );
}

void  time2str( __u32 ms, char *str )
{
	__u32  second;
	cuckoo_time_t  time;
	
	second = ms /1000;
	time2time( second, &time );
	__time2str( &time, str );
}


//根据要显示的格式转换
void time2str_by_format(__u32 ms, char *str, time_format_e format)
{
	__u32  second;
	cuckoo_time_t  time;
	
	second = ms /1000;
	time2time( second, &time );
	
	switch(format)
	{
		case TIME_S:
			uint2str( time.second, str);		
			break;
		case TIME_MS:
			uint2str( time.minute, str);
			str[2] = ':';
			uint2str( time.second, str+3);		
			break;
		case TIME_HMS:
			time2str(ms, str);
			break;
		case TIME_AUTO_HMS:
			uint2str( time.minute, str);
			str[2] = ':';
			uint2str( time.second, str+3);	
			if(time.hour != 0)
			{
				time2str(ms, str);
			}
			break;
		default:
			break;
	}
	
}
	

//转换文件size 到字符串
//add by libaiao, 2011.5.8
void filesize2str(__u32 size, char *str)
{
	__msg("size = %d\n" , size );
	if (size < 1024)	// 小于1 k
	{
		eLIBs_sprintf(str, "%d B",size);
	}
	else if ( size < (1024*1024))	// 小于 1 M
	{
		eLIBs_sprintf(str, "%d K",size/1024);
	}
	else if (size < (1024*1024*1024))	// 小于 1 G
	{
		//eLIBs_sprintf(str, "%d.%d M",size/(1024*1024), (size%(1024*1024))*100/(1024*1024)/10);
		eLIBs_sprintf(str, "%d.%d%d M",size/(1024*1024), ((size%(1024*1024))/1024)*1000/1024/100,((size%(1024*1024))/1024)*1000/1024%100/10);	//保留两位小数

	}
	else
	{
		//eLIBs_sprintf(str, "%d.%d G",size/(1024*1024*1024),(size%(1024*1024*1024))*100/(1024*1024*1024)/10);
		eLIBs_sprintf(str, "%d.%d%d G",size/(1024*1024*1024),(size%(1024*1024*1024))/(1024*1024)*1000/1024/100,(size%(1024*1024*1024))/(1024*1024)*1000/1024%100/10);

	}
}





//传入24时制小时值，传出12时制小时值，返回0为am，非0为pm
__s32 hour24_to_hour12(__s32* hour)
{
	if (*hour == 0)
	{//凌晨12点
		*hour = 12;
	} 
	else if (*hour == 12)
	{//中午12点
		return 1;
	}
	else if (12 < *hour)
	{
		*hour -= 12;
		return 1;
	}
	return 0;
}

//查询app_name名称的应用是否存在
//查询背景音乐是否存在可以用is_app_exist(APP_MUSIC)
__s32 is_app_exist(char* app_name)
{
    H_WIN root, child;
	char  winname[256];  
    __bool exist;
   
    root  = GUI_WinGetHandFromName(APP_ROOT);	

    if(!root)
    {
        return 0;
    }
    
    child = GUI_WinGetFirstChild(root);

    exist = 0;
    
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 256);
		GUI_WinGetName(child, winname);
				
		__msg("winname = %s \n", winname);
		
		if( 0 == eLIBs_strcmp( app_name, winname))		
		{            
			exist = 1;
            break;
		}
		
		child = GUI_WinGetNextBro(child);
	}

    return exist;
}

__u32 get_memory_info(char* title, void* pExtPara)
{
	static __u32 last_free_size = 0;
	__u32 free_size = 0, total_size = 0;
	__u32 size;

	//__msg("\n");
	//__msg("=====================title: %s====================\n", title);	
	free_size = esMEMS_FreeMemSize();
	if(free_size > last_free_size)
	{
		size = free_size - last_free_size;
		if(last_free_size == 0)
		{
			__msg("=====================malloc memory size is %d=======\n", last_free_size);		
		}
		else
		{
			__msg("=====================release memory size is %d=======\n", size);		
		}
	}
	else if(free_size <= last_free_size)
	{
		size = last_free_size - free_size;
		__msg("=====================malloc memory size is %d=======\n", size);		
	}
	
	total_size = esMEMS_TotalMemSize();
	__msg("=====================free memory size is %d=======\n", free_size);
	__msg("=====================used memory size is %d=======\n", total_size-free_size);
	__msg("=====================total memory size is %d=======\n", total_size);
	last_free_size = free_size;
}


void * My_Palloc(__u32 npage, __u32 mode)
{
    void* p;

    //if(npage)
    
    p = esMEMS_Palloc( npage, mode);
    __wrn("palloc p=%x, npage=%d\n", p, npage);
    return p;
}

void My_Pfree(void *mblk, __u32 npage)
{    
    __wrn("Pfree p=%x\n", mblk);
    esMEMS_Pfree(mblk, npage);
}

void* My_Balloc(__u32 size)
{
	static __u32 index = 0;    
	void* p = esMEMS_Balloc(size);
    if(52 == size)
    {
       // __wrn("********My_Balloc******\n");
    }
	__wrn("Balloc index=%d p=%x, size=%d\n", index, (__u32)p, size);
	index++;
	return p;
}

void  My_Bfree(void *mblk, __u32 size)
{
	__wrn("Bfree p=%x, size=%d\n",(__u32)mblk, size);
	esMEMS_Bfree(mblk, size);
}

void* My_Malloc(__mems_heap_t *heap, __u32 size)
{
	static __u32 index = 0;
	void* p = esMEMS_Malloc(heap,  size);
	__wrn("Malloc index=%d p=%x\n", index, (__u32)p);
	index++;
	return p;
}


void  My_Mfree(__mems_heap_t *heap, void *f)
{
	__wrn("Mfree p=%x\n",(__u32)f);
	esMEMS_Mfree(heap, f);
}


__s32 test_flush_nor(__s32 i)
{
#if 0
	__s32 result;
	ES_FILE* fp = NULL;
    char buf[3000]={0};
    char path[] = "e:\\test.bin";

    eLIBs_printf("huang    i=%d\n", i);
    
    if(1)//
    {    	
    	fp = eLIBs_fopen(path, "wb");
    	if(fp == NULL)
    	{
    		eLIBs_printf("open file fail:%s\n", path);
    		return EPDK_FAIL;
    	}
    	
    	result = eLIBs_fwrite(buf, 1, sizeof(buf), fp);
    	if(result < sizeof(buf))
    	{
            eLIBs_fclose(fp);
    		eLIBs_printf("write file fail:%s\n", path);
    		return EPDK_FAIL;
    	}
    	
    	eLIBs_fclose(fp);


        {//
            __s32 i, num;
            ES_FILE* hFile;

            for(i = 0 ; i < 1 ; i++)
            {
                //flush user disk data to NOR flash
                hFile = eLIBs_fopen("b:\\STORAGE\\SPINOR", "rb");
                if (hFile == NULL)
                {
                    eLIBs_printf("open NOR file failed\n");
                    return EPDK_FAIL;
                }
                eLIBs_printf("i=%d flush file ....\n", i);
                eLIBs_fioctrl(hFile, DEV_IOC_USR_FLUSH_CACHE, 0, 0);
                
                eLIBs_printf("i=%d flush end ....\n", i);
                eLIBs_fclose(hFile);

                esKRNL_TimeDly(2);
            }
        }
    }  
#endif
	return EPDK_OK;
}

#ifdef KEY_VOICE
//pbuffer:数据buffer
//aux:模式
static __s32 app_root_play_wav_buf(__cedar_pcm_info_t* pbuffer, __cedar_play_aux_wav_mode_t aux)
{
    __s32 ret;
    ES_FILE* hAudio;
    __s32 data_size;
    __audio_dev_para_t    	au_para;
    __cedar_pcm_info_t*     pwav_inf;
    __cedar_play_aux_wav_mode_t mode;
    __s32 state;
    
            
    __wrn("app_root_play_wav_buf begin...\n");
    //..state = dsk_amplifier_is_on();
   //.. dsk_amplifier_onoff(1);
    esKRNL_TimeDly(80);
    
    ret = EPDK_OK;
    hAudio = NULL;
       
    hAudio = eLIBs_fopen("b:\\AUDIO\\PLAY", "r+");
    if(NULL == hAudio)
    {
        ret = EPDK_FAIL;
        goto end_play_wav_buf;
    }

    pwav_inf = (__cedar_pcm_info_t*)pbuffer;
    if(NULL == pwav_inf)
    {
        ret = EPDK_FAIL;
        goto end_play_wav_buf;
    }

    mode = (__cedar_play_aux_wav_mode_t)aux;

    au_para.bps = 16;
    au_para.chn = pwav_inf->Chan;
    au_para.fs = pwav_inf->SampleRate;
    
    eLIBs_fioctrl(hAudio, AUDIO_DEV_CMD_SET_PARA, 0, &au_para);
    eLIBs_fioctrl(hAudio, AUDIO_DEV_CMD_REG_USERMODE, AUDIO_PLAY_USR_SLAVE, 0);
    eLIBs_fioctrl(hAudio, AUDIO_DEV_CMD_START, 0, 0);
    data_size = eLIBs_fioctrl(hAudio, AUDIO_DEV_CMD_QUERY_BUFSIZE, AUDIO_DEV_QUERY_BUF_SIZE_DATA, 0);
    if(CEDAR_PLAY_AUX_WAV_MODE_IGNORE == mode)
    {
        if(data_size > 0)
        {
        }
        else
        {
            eLIBs_fwrite(pwav_inf->PCMPtr, 1, pwav_inf->PcmLen*sizeof(__u16)*pwav_inf->Chan
                , hAudio);
        }
    }
    else if(CEDAR_PLAY_AUX_WAV_MODE_TRUNCATE == mode)
    {
        eLIBs_fioctrl(hAudio, AUDIO_DEV_CMD_FLUSH_BUF, 0, 0);
        eLIBs_fwrite(pwav_inf->PCMPtr, 1, pwav_inf->PcmLen*sizeof(__u16)*pwav_inf->Chan
                , hAudio);
    }
    else if(CEDAR_PLAY_AUX_WAV_MODE_APPEND == mode)
    {
        eLIBs_fwrite(pwav_inf->PCMPtr, 1, pwav_inf->PcmLen*sizeof(__u16)*pwav_inf->Chan
                , hAudio);
    }
    else
    {
        eLIBs_fwrite(pwav_inf->PCMPtr, 1, pwav_inf->PcmLen*sizeof(__u16)*pwav_inf->Chan
                , hAudio);
    }
    
    do{//等待发音结束
		__s32 cnt;
		__s32 data_size;

		cnt = 0;

		while(cnt < 50)
		{
			data_size = eLIBs_fioctrl(hAudio, AUDIO_DEV_CMD_QUERY_BUFSIZE, AUDIO_DEV_QUERY_BUF_SIZE_DATA, 0);
			if(data_size <= 0)
			{
				break;
			}

			cnt++;
			esKRNL_TimeDly(10);
		}		
	}while(0);

	esKRNL_TimeDly(20);
	if(1 == state)
    {
        //..dsk_amplifier_onoff(1);
    }
	else
	{
        //..dsk_amplifier_onoff(0);
	}
 
    end_play_wav_buf:;

    if(hAudio)
    {
        eLIBs_fclose(hAudio);
        hAudio = NULL;
    }

    __wrn("app_root_play_wav_buf end...\n");
    
    return ret;
}

//pbuffer:文件名
//aux:模式
static __s32 app_root_play_wav_file(char* pbuffer, __cedar_play_aux_wav_mode_t aux)
{
    #define READ_FROM_BUFFER 1
    
    __s32 ret;
    char*    path;
    ES_FILE* fp;
    __wave_header_t1 wav;
    __s32 read_num;
    char* pwav_buf;//解压缩后wav buffer
    __s32 wav_len;//解压缩后wav 长度
    __cedar_pcm_info_t pcm_info;
    char* sound_buf;//整个文件的内容
    __s32 sound_len;//整个文件的长度

    char* data_buf;//data的buffer
    __s32 data_len;
    
    __wrn("app_root_play_wav_file begin...\n");          
    
    fp = NULL;            
    ret = EPDK_OK;
    pwav_buf = NULL;
    sound_buf = NULL;
    data_buf = NULL;
    sound_len = 0;
    
    path = (char*)pbuffer;
    if(NULL == path)
    {
        __wrn("wav file name is null...\n");
        ret = EPDK_FAIL;
        goto end_play_wav_file;
    }
    
    fp = eLIBs_fopen(path, "rb");
    if(NULL == fp)
    {
        __wrn("open wav file fail...\n");
        ret = EPDK_FAIL;
        goto end_play_wav_file;
    }

    eLIBs_fseek(fp, 0, SEEK_END);
    sound_len = eLIBs_ftell(fp);

    sound_buf = esMEMS_Balloc(sound_len);
    if(NULL == sound_buf)
    {
        __wrn("esMEMS_Balloc fail...\n");
        ret = EPDK_FAIL;
        goto end_play_wav_file;
    }

    eLIBs_fseek(fp, 0, SEEK_SET);
    read_num = eLIBs_fread(sound_buf, 1, sound_len, fp);
    if(read_num != sound_len)
    {
        __wrn("read wav file fail...\n");
        ret = EPDK_FAIL;
        goto end_play_wav_file;
    }
        
    do{//uncompress
        if(EPDK_TRUE != AZ100_IsCompress(sound_buf, sound_len))
        {
            __here__;
            pwav_buf = sound_buf;
            wav_len = sound_len;

            sound_buf = NULL;//buffer转交给pwav_buf
            break;
        }

        {
            wav_len = AZ100_GetUncompressSize(sound_buf, sound_len);
            pwav_buf = esMEMS_Balloc(wav_len);
            if(NULL == pwav_buf)
            {
                __wrn("esMEMS_Balloc fail...\n");
                ret = EPDK_FAIL;
                goto end_play_wav_file;
            }                        

            ret = AZ100_Uncompress(sound_buf, sound_len, pwav_buf, wav_len);
            if(EPDK_FAIL == ret)
            {
                __wrn("AZ100_Uncompress fail...\n");
                ret = EPDK_FAIL;
                goto end_play_wav_file;
            }   
        }
        
    }while(0);


    #if READ_FROM_BUFFER
    eLIBs_memcpy(&wav, pwav_buf, sizeof(wav));
    #else    
    eLIBs_fseek(fp, 0, SEEK_SET);
    read_num = eLIBs_fread(&wav, 1, sizeof(wav), fp);
    if(sizeof(wav) != read_num)
    {
        __wrn("read wav file fail...\n");
        ret = EPDK_FAIL;
        goto end_play_wav_file;
    }
    #endif

    if(EPDK_YES != eLIBs_memcmp("RIFF", &wav.uRiffFcc, 4))
    {
        __wrn("invalid riff wav file format...\n");
        ret = EPDK_FAIL;
        goto end_play_wav_file;
    }

    if(16 != wav.uBitsPerSample)
    {
        __wrn("bitPerSample invalid:%d\n", wav.uBitsPerSample);
        ret = EPDK_FAIL;
        goto end_play_wav_file;
    }


#if READ_FROM_BUFFER
    eLIBs_memset(&pcm_info, 0, sizeof(__cedar_pcm_info_t));
    pcm_info.Chan = wav.uChannels;
    pcm_info.PcmLen = wav.uSampDataSize/(wav.uBitsPerSample/8)
                        /wav.uChannels;
    pcm_info.preamp = 0;
    pcm_info.SampleRate = wav.uSampleRate;
    pcm_info.PCMPtr = (__u16*)(((__u8*)pwav_buf)+sizeof(wav));

#else
    data_buf = esMEMS_Balloc(wav.uSampDataSize);
    if(NULL == data_buf)
    {
        __wrn("mem not enough...\n");
        ret = EPDK_FAIL;
        goto end_play_wav_file;
    }

    read_num = eLIBs_fread(data_buf, 1, wav.uSampDataSize, fp);
    if(wav.uSampDataSize != read_num)
    {
        __wrn("read wav file fail...\n");
        ret = EPDK_FAIL;
        goto end_play_wav_file;
    }
   
    eLIBs_memset(&pcm_info, 0, sizeof(__cedar_pcm_info_t));
    pcm_info.Chan = wav.uChannels;
    pcm_info.PcmLen = wav.uSampDataSize/(wav.uBitsPerSample/8)
                        /wav.uChannels;
    pcm_info.preamp = 0;
    pcm_info.SampleRate = wav.uSampleRate;
    pcm_info.PCMPtr = (__u16*)data_buf;

#endif

    ret = app_root_play_wav_buf(&pcm_info, aux);
    
    end_play_wav_file:;
    if(fp)
    {
        eLIBs_fclose(fp);
        fp = NULL;
    }

    if(sound_buf)
    {
        esMEMS_Bfree(sound_buf, sound_len);
        sound_buf = NULL;
    }

    if(pwav_buf)
    {
        esMEMS_Bfree(pwav_buf, wav_len);
        pwav_buf = NULL;
    }

    if(data_buf)
    {
        esMEMS_Bfree(data_buf, data_len);
        data_buf = NULL;
    }
    

    __wrn("app_root_play_wav_file end...\n");

    return ret;             
}

__s32 app_root_play_app_sounds(short_key_sound_t type)
{
    char wav_file[1024];

//	return EPDK_OK;
	
    if(SHORT_KEY_WAVE_FILE_PLAY == type
        ||SHORT_KEY_WAVE_FILE_PAUSE == type
        ||SHORT_KEY_WAVE_FILE_MUTE_ON== type
        ||SHORT_KEY_WAVE_FILE_MUTE_OFF == type
        //||SHORT_KEY_WAVE_FILE_RECORD_START == type
        //||SHORT_KEY_WAVE_FILE_RECORD_PAUSE == type
        //||SHORT_KEY_WAVE_FILE_RECORD_STOP == type
        )
    {
        __wrn("not supported now...\n");
        return EPDK_FAIL;
    }

	{
		reg_system_para_t* para;
		__u32 e_lang;
		
		para = dsk_reg_get_para_by_app(REG_APP_SYSTEM);
		if(NULL == para)
		{
			e_lang = EPDK_LANGUAGE_ENM_CHINESES;
		}
		else
		{
			e_lang = para->language;
		}
		
		if (EPDK_LANGUAGE_ENM_ENGLISH == e_lang)//英文
		{
			eLIBs_strcpy(wav_file, g_short_key_wav_file[type][0]);          	
		}
		else if (EPDK_LANGUAGE_ENM_CHINESES == e_lang)//简体中文
		{
			eLIBs_strcpy(wav_file, g_short_key_wav_file[type][1]);          	
		}
		else if (EPDK_LANGUAGE_ENM_CHINESET == e_lang)//繁体中文
		{
			eLIBs_strcpy(wav_file, g_short_key_wav_file[type][2]);          	
		}
		else//默认简体中文
		{
			eLIBs_strcpy(wav_file, g_short_key_wav_file[type][0]);          	
		}	    
	}

	dsk_amplifier_onoff(1);
	esKRNL_TimeDly(10);

#if 1
    //会死机该接口会死机，先不用
    if(0)//(EPDK_TRUE == robin_is_open())
    {
        robin_play_aux_file(wav_file, CEDAR_PLAY_AUX_WAV_MODE_TRUNCATE);
    }
    else
    {
        app_root_play_wav_file(wav_file, CEDAR_PLAY_AUX_WAV_MODE_TRUNCATE);
    }	
#else
	dsk_keytone_init(wav_file);
	dsk_keytone_on();
	dsk_keytone_wait_audio_finish();
		
	dsk_keytone_exit();
	dsk_keytone_init(BEETLES_APP_ROOT"res\\sounds\\chord.wav");                    
#endif
/*
	do{//等待发音结束
		__s32 cnt;
		__s32 data_size;
		ES_FILE* hAudio;

		hAudio = eLIBs_fopen("b:\\AUDIO\\PLAY", "r+");	
		if(NULL == hAudio)
		{
			break;
		}

		cnt = 0;
		while(cnt < 50)
		{
			data_size = eLIBs_fioctrl(hAudio, AUDIO_DEV_CMD_QUERY_BUFSIZE, AUDIO_DEV_QUERY_BUF_SIZE_DATA, 0);
			if(data_size <= 0)
			{
				break;
			}

			cnt++;
			esKRNL_TimeDly(10);
		}

		if(hAudio)
	    {
	        eLIBs_fclose(hAudio);
	        hAudio = NULL;
	    }
	}while(0);
*/
	esKRNL_TimeDly(30);
	dsk_amplifier_onoff(0);
            
    return EPDK_OK;
}
#endif


static __s32 com_copy_rect_32bpp(__u32* pDst, __s32 dst_x, __s32 dst_y, __s32 dst_line_w, __u32* pSrc,
                __s32 src_x, __s32 src_y, __s32 src_line_w, __s32 w, __s32 h)
{
    __s32 i, j;

    for(i = 0 ; i < w ; i++)
    {
        for(j = 0 ; j < h ; j++)
        {
            DATA_OF_ADRR(__u32, pDst, dst_x+i, dst_y+j, dst_line_w) = DATA_OF_ADRR(__u32, pSrc, src_x+i, src_y+j, src_line_w);
        }
    }
    
    return EPDK_OK;
}

static __s32 com_copy_rect_8bpp(char* pDst, __s32 dst_x, __s32 dst_y, __s32 dst_line_w, char* pSrc,
                __s32 src_x, __s32 src_y, __s32 src_line_w, __s32 w, __s32 h)
{
    __s32 i, j;

    src_line_w = (src_line_w+3)/4*4;
   // dst_line_w = (dst_line_w+3)/4*4;
    
    for(i = 0 ; i < w ; i++)
    {
        for(j = 0 ; j < h ; j++)
        {
            DATA_OF_ADRR(__u8, pDst, dst_x+i, dst_y+j, dst_line_w) = 0xf0;//DATA_OF_ADRR(__u8, pSrc, src_x+i, src_y+j, src_line_w);
        }
    }
    
    return EPDK_OK;
}

//////////////////////////////////////////////////////////////////////////

/************************************************************************/
// pFileData --- 资源函数获得的数据(也是32位bmp文件数据)
// x，y      --- 目标坐标
// x0, y0, x1, y1 --- 源区域(即bmp图片某一区域)
/************************************************************************/
__s32 APP_BMP_DrawEx(H_LYR hlyr, const void * pFileData, int x, int y, int x0, int y0, int x1, int y1)
{
	__s32 Ret = EPDK_OK;
	__u32 *pSrc32;
    char *pSrc8;
	int bmp_w, bmp_h;
	int w, h;
    GUI_RECT rt;    
    FB fb;
    __u32* pfb_addr;
    __s32 i, j;
    __s32 nBitCount;
    __s32 offbits;

    if(!hlyr)
    {
        __err("invalid para...\n");
        return EPDK_FAIL;
    }
    
    GUI_LyrWinGetFB(hlyr, &fb);
    
	//检查区域范围
	bmp_w = GUI_BMP_GetXSize(pFileData);
	if ( ! ValueBetweenRangeEx(0, x0, x1, bmp_w))
	{
		return EPDK_FAIL;
	}

	bmp_h = GUI_BMP_GetYSize(pFileData);
	if ( ! ValueBetweenRangeEx(0, y0, y1, bmp_h))
	{
		return EPDK_FAIL;
	}	

    if(x1-x0+x >= fb.size.width
        || y1-y0+y >= fb.size.height)
    {
        return EPDK_FAIL;
    }

    nBitCount = *((char*)pFileData + 28);
    
    offbits = *(__u32*)((char*)pFileData+10);

    pSrc8 = (char*)pFileData + offbits;
    
    pSrc32 = (__u32*)pSrc8;

    pfb_addr = (__u32*)(fb.addr[0]);       
    
    if(32 == nBitCount)
    {
        com_copy_rect_32bpp(pfb_addr, x, y, fb.size.width, pSrc32, x0, y0, bmp_w, x1-x0+1, y1-y0+1);       
    }
    else if(8 == nBitCount)
    {
        com_copy_rect_8bpp((char*)pfb_addr, x, y, fb.size.width, pSrc8, x0, y0, bmp_w, x1-x0+1, y1-y0+1);       
    }
    else
    {
        __err("not 32bpp or 8bpp...\n");
    }
    

	return Ret;
}

__s32 app_set_title(__s32 res_id)
{
	char title[32] = {0};

	dsk_langres_get_menu_text(res_id, title, sizeof(title));

	return gscene_hbar_set_title(title, sizeof(title));
}


__s32 com_set_backcolor(__u32 color)
{		
    dsk_display_set_backcolor(
    	(COM_GET_B_VAL(color)<<24) 
    	| (COM_GET_G_VAL(color)<<16)
    	| (COM_GET_R_VAL(color)<<8)
    	| (COM_GET_ALPHA_VAL(color)<<0) );

    return EPDK_OK;
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

//设置gpio为输入
//pull: 0:高阻 1:上拉  2:下拉
void set_input_gpio(__u32 port, __u32 port_num, __s32 pull)
{
    volatile  __u32    *tmp_addr;
    __u32               tmp_data;
    __u32               tmp1;
    __u32               tmp2;
    

    //设置为输入
    tmp1 = (port_num - ((port_num>>3)<<3))<<2;
    tmp_addr = PIO_REG_CFG(port, (port_num>>3));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x07 << tmp1);
    //tmp_data |=  (0x01 << tmp1);
    *tmp_addr = tmp_data;

    //设置pull属性
    tmp2 = (port_num - ((port_num>>4)<<4))<<1;
    tmp_addr = PIO_REG_PULL(port, (port_num>>4));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x03 << tmp2);
    tmp_data |=  (pull << tmp2);
    *tmp_addr = tmp_data;          
    
}


__s32 get_gpio_status(__u32 port, __u32 port_num, __u32 pull)
{
    volatile  __u32    *tmp_addr;
    __u32               tmp_data;
    __s32               ret;
    __u32               tmp1;
    __u32               tmp2;

    //设置为输入
    tmp1 = (port_num - ((port_num>>3)<<3))<<2;
    tmp_addr = PIO_REG_CFG(port, (port_num>>3));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x07 << tmp1);
    tmp_data |=  (0x00 << tmp1);
    *tmp_addr = tmp_data;

	if(pull)
	{
	    //设置为上拉
	    tmp2 = (port_num - ((port_num>>4)<<4))<<1;
	    tmp_addr = PIO_REG_PULL(port, (port_num>>4));
	    tmp_data = *tmp_addr;
	    tmp_data &= ~(0x03 << tmp2);
	    tmp_data |=  (0x01 << tmp2);
	    *tmp_addr = tmp_data; 
	}
    else
	{
	    //设置为下拉
	    tmp2 = (port_num - ((port_num>>4)<<4))<<1;
	    tmp_addr = PIO_REG_PULL(port, (port_num>>4));
	    tmp_data = *tmp_addr;
	    tmp_data &= ~(0x03 << tmp2);
	    tmp_data |=  (0x02 << tmp2);
	    *tmp_addr = tmp_data;
	}
  
    
    tmp_addr = PIO_REG_DATA(port);
    tmp_data = *tmp_addr;

//    __wrn("tmp_data=0x%x\n", tmp_data);
    ret = (tmp_data & (1 << port_num))>>port_num;
    
    return ret;
}

int print_layer_info(void)
{
	ES_FILE * dispfile;
	__s32 ret = 0;
	__u32 pbuffer[3] = {0};
    dispfile = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
    if(!dispfile)
    {
        eLIBs_printf("open display device fail!\n");
        return EPDK_FAIL;
    }

	if(1)
	{
		__disp_layer_info_t     layer_para;
		__s32 i = 0;
		char * wm[6] = {"NORMAL","PALETTE","INTER_BUF","GAMMA","SCALER","reserved"};
		char * format[17] = {"1-bpp","2-bpp","4-bpp","8-bpp","rgb655","rgb565","rgb556",
							"argb1555","rgba5551","rgb888","argb8888",
							"YUV444","YUV422","YUV420","YUV411","CSI RGB","reserved"};
		char * yuv_mode[6] = {"non-macro block planar","interleaved","non-macro block UV combined",
							"macro block planar","macro block UV combined","reserved"};

		eLIBs_printf(".Layers Information Begin.................................................................................................\n");
		for(i=0; i<4; i++)
		{
			__s32 pixel_fmt = 0;
			__s32 pixel_mode = 0;
			__s32 pixel_seq = 0;

			pbuffer[0] = i+100;
			pbuffer[1] = (__u32)&layer_para;
	    	ret = eLIBs_fioctrl(dispfile, DISP_CMD_LAYER_GET_PARA, 0, (void*)pbuffer);
	    	if(ret != EPDK_OK)
	    	{
		        eLIBs_printf(".\tLayer %d:close\n",i);
		        continue;
		    }
	    	eLIBs_printf(".\tLayer %d:open\n",i);
	    	
	    	eLIBs_printf(".\t\twork mode:%s\n",((__s32)layer_para.mode>=0 && layer_para.mode<5)?wm[layer_para.mode]:wm[5]);
	    	eLIBs_printf(".\t\tpriority:%d\n",layer_para.prio);

		    pixel_fmt = layer_para.fb.format;
		    pixel_mode = layer_para.fb.mode;
		    pixel_seq = layer_para.fb.seq;
	    	eLIBs_printf(".\t\tformat:%s,",(pixel_fmt>=0 && pixel_fmt<16)?format[pixel_fmt]:format[16]);
	    	eLIBs_printf("\tmode:%s,",(pixel_mode>=0 && pixel_mode<5)?yuv_mode[pixel_mode]:yuv_mode[5]);
	    	

		    if(pixel_fmt == DISP_FORMAT_ARGB8888 && pixel_mode == DISP_MOD_INTERLEAVED)
		    {
		    	eLIBs_printf("\tpixels sequence:%s,",(pixel_seq==DISP_SEQ_ARGB)?"ARGB":"BGRA");
		    }
		    else if(pixel_fmt == DISP_FORMAT_YUV422 && pixel_mode == DISP_MOD_INTERLEAVED)
		    {
		    	     if(pixel_seq==DISP_SEQ_UYVY) eLIBs_printf("\tpixels sequence:UYVY,");
		    	else if(pixel_seq==DISP_SEQ_YUYV) eLIBs_printf("\tpixels sequence:YUYV,");
		    	else if(pixel_seq==DISP_SEQ_VYUY) eLIBs_printf("\tpixels sequence:VYUY,");
		    	else if(pixel_seq==DISP_SEQ_YVYU) eLIBs_printf("\tpixels sequence:YVYU,");
		    }
		    else if(pixel_fmt == DISP_FORMAT_YUV444 && pixel_mode == DISP_MOD_INTERLEAVED)
		    {
		    	     if(pixel_seq==DISP_SEQ_AYUV) eLIBs_printf("\tpixels sequence:AYUV,");
		    	else if(pixel_seq==DISP_SEQ_VUYA) eLIBs_printf("\tpixels sequence:VUYA,");
		    }
		    else if(pixel_fmt == DISP_FORMAT_YUV420 && (pixel_mode == DISP_MOD_NON_MB_UV_COMBINED || pixel_mode == DISP_MOD_MB_UV_COMBINED))
		    {
		    	     if(pixel_seq==DISP_SEQ_UVUV) eLIBs_printf("\tpixels sequence:UVUV,");
		    	else if(pixel_seq==DISP_SEQ_VUVU) eLIBs_printf("\tpixels sequence:VUVU,");
		    }
		    else
		    {
		    	eLIBs_printf("\tpixels sequence:%d,",pixel_seq);
		    }
		    eLIBs_printf("\tB R channel swap:%s\n",layer_para.fb.br_swap==0?"RGB":"BGR");

			if(pixel_mode == DISP_MOD_INTERLEAVED)
			{
				eLIBs_printf(".\t\tframe buffer address 0:%x\n",layer_para.fb.addr[0]);
			}
		    else if(pixel_mode == DISP_MOD_NON_MB_UV_COMBINED || pixel_mode == DISP_MOD_MB_UV_COMBINED)
		    {
		    	eLIBs_printf(".\t\tframe buffer address 0:%x,\taddress 1:%x\n",layer_para.fb.addr[0],layer_para.fb.addr[1]);
		    }
		    else
		    {
		    	eLIBs_printf(".\t\tframe buffer address 0:%x,\taddress 1:%x,\tddress 2:%x\n",layer_para.fb.addr[0],layer_para.fb.addr[1],layer_para.fb.addr[2]);
		    }
		    
	    	eLIBs_printf(".\t\tsource window(x:%d,\ty:%d,\twidth:%d,\theight:%d)\n",layer_para.src_win.x,layer_para.src_win.y,layer_para.src_win.width,layer_para.src_win.height);
	    	eLIBs_printf(".\t\tscreen window(x:%d,\ty:%d,\twidth:%d,\theight:%d)\n",layer_para.scn_win.x,layer_para.scn_win.y,layer_para.scn_win.width,layer_para.scn_win.height);

	    	eLIBs_printf(".\t\tcolor key:%s\n",layer_para.ck_enable==0?"disable":"enable");
	    	eLIBs_printf(".\t\talpha:%s,\talpha value:%x\n",layer_para.alpha_en==0?"disable":"enable",layer_para.alpha_val);
	    	eLIBs_printf(".\t\tpipe:%d\n",layer_para.pipe);
	    }
		eLIBs_printf(".Layers Information End.................................................................................................\n");
	}
    
    eLIBs_fclose(dispfile);

    return EPDK_OK;
}

#if 1	//wqx 20130913
char key_tone_filename[]="d:\\res\\sounds\\chord.wav";

__s32 com_audio_play_aux_wav_file(char* filename, 
    __audio_play_aux_wav_mode_t mode)
{
    __s32 ret;
    ES_FILE* fp;

    // return EPDK_OK;//112350
    
    ret = EPDK_FAIL;
    
    fp = eLIBs_fopen("b:\\AUDIO\\PLAY", "rb");
 		 __here__;
   
    if(fp)
    {
        ret = eLIBs_fioctrl(fp, AUDIO_DEV_CMD_PLAY_AUX_WAV_FILE, mode, (void*)filename);
        eLIBs_fclose(fp);
        fp = NULL;
 		 __here__;
   }

    
    return ret;
}

static __s32 com_audio_wait_aux_wav_finish(void)
{
    __s32 ret;
    __s32 data_size;
    ES_FILE* fp;

   // return EPDK_OK;//112350

    __here__;
    fp = eLIBs_fopen("b:\\AUDIO\\PLAY", "rb");
    
    if(fp)
    {
        __here__;
        while(1)
        {
            ret = eLIBs_fioctrl(fp, AUDIO_DEV_CMD_GET_AUX_WAV_DATA_SIZE, 0, 0);
            if(0 == ret)
            {
                break;
            }

            esKRNL_TimeDly(1);
        }        
        __here__;

        eLIBs_fioctrl(fp, AUDIO_DEV_CMD_FLUSH_BUF, 0, 0);

        __here__;
        while(1)
        {
            data_size = eLIBs_fioctrl(fp, AUDIO_DEV_CMD_QUERY_BUFSIZE, AUDIO_DEV_QUERY_BUF_SIZE_DATA, 0);
			if(data_size <= 0)
			{
				break;
			}

            esKRNL_TimeDly(1);
        }

        __here__;
        
        eLIBs_fclose(fp);
        fp = NULL;
    }

    __here__;
    
    return ret;
}
__s32 app_root_play_keytone(void)
{
        __bool mute_status = dsk_amplifier_is_on();

#if 1//wqx 20130926
	reg_system_para_t* para;

        para = dsk_reg_get_para_by_app(REG_APP_SYSTEM);
	if(para->keytone!=1)	// 1==on	2==off
	return EPDK_OK;
#endif	
		 __here__;
	dsk_amplifier_onoff(1);
	if(!mute_status)
	esKRNL_TimeDly(50);
	if(EPDK_FAIL==com_audio_play_aux_wav_file(key_tone_filename,AUDIO_PLAY_AUX_WAV_MODE_MIX))
	{
		__wrn("\n\ncom_audio_play_aux_wav_file fail\n\n");
	}
	else
	{
		__msg("\n\ncom_audio_play_aux_wav_file ok\n\n");
	}
	if(EPDK_FAIL==com_audio_wait_aux_wav_finish())
	{
		__wrn("\n\com_audio_wait_aux_wav_finish fail\n\n");
	}
	else
	{
		__msg("\n\com_audio_wait_aux_wav_finish ok\n\n");
	}
		
       // esKRNL_TimeDly(50);
        if(1 == mute_status)
        {
            	dsk_amplifier_onoff(1);
		 __here__;
       }
        else
        {
            	dsk_amplifier_onoff(0);
  		 __here__;
     	}            
	return EPDK_OK;
}

#endif

#endif     //  ifndef __misc_c

/* end of misc.c */
