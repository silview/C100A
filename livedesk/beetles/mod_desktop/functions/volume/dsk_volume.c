/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dsk_volume.c
* By      	: Andy.zhang
* Func		: voice control
* Version	: v1.0
* ============================================================================================================
* 2009-7-8 10:02:17  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "..\\functions_i.h"

#define AUDIO_DRV_PATH "b:\\AUDIO\\PLAY"

static __bool dsk_volume_gate_on = EPDK_TRUE;
static __s32 dsk_volume = EPDK_TRUE;
#if 0
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

static __s32 _set_volume( __s32 volume );
static __s32 _get_volume( void );
static __s32 g_amplifier_status = 0;

//用于显示音量最大30, 但实际设置音量最大28;   	Ethan; 

static __bool rec_is_record = EPDK_FALSE;

static __s32 virtual_vol = 0;				//保存虚拟音量;
#define SUPPORT_VIRTUAL_VOLUME	1

#define PIOC_MEM_BASE   (0xf1c20800)

#define PIO_REG_CFG(n, i)               ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x00))
#define PIO_REG_DLEVEL(n, i)            ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x14))
#define PIO_REG_PULL(n, i)              ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x1C))
#define PIO_REG_DATA(n) 	            ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + 0x10))

static __s32 get_gpio_status(__u32 port, __u32 port_num)
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
#if 0
    //设置为上拉
    tmp2 = (port_num - ((port_num>>4)<<4))<<1;
    tmp_addr = PIO_REG_PULL(port, (port_num>>4));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x03 << tmp2);
    tmp_data |=  (0x01 << tmp2);
    *tmp_addr = tmp_data;      
#else
    //设置为下拉
    tmp2 = (port_num - ((port_num>>4)<<4))<<1;
    tmp_addr = PIO_REG_PULL(port, (port_num>>4));
    tmp_data = *tmp_addr;
    tmp_data &= ~(0x03 << tmp2);
    tmp_data |=  (0x02 << tmp2);
    *tmp_addr = tmp_data;
#endif    
    
    tmp_addr = PIO_REG_DATA(port);
    tmp_data = *tmp_addr;

    __msg("tmp_data=0x%x\n", tmp_data);
    ret = (tmp_data & (1 << port_num))>>port_num;
    
    return ret;
}

static void pull_down_gpio(__u32 port, __u32 port_num)
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

static void pull_up_gpio(__u32 port, __u32 port_num)
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


#if SUPPORT_VIRTUAL_VOLUME
static __s32 set_volume_convert(__s32 volume)
{
	virtual_vol = volume;
	if(volume>28)
		volume = 28;

	return volume;
}

static __s32 get_volume_convert()
{
	return virtual_vol;
}
#endif
/*
****************************************************************************************************
*Name        : dsk_volume_set_volume
*Prototype   : __s32 dsk_volume_set_volume(__s32 volume)
*Arguments   : volume     input. volume value, ranges AUDIO_DEVICE_VOLUME_MIN~~AUDIO_DEVICE_VOLUME_MAX.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the volume.
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_set(__s32 volume)	// 设置当前音量
{
	__s32        ret;
	__s32		tmp_volume;
	
	if( !dsk_volume_gate_on )
	{
	    return EPDK_FAIL;
	}
#if SUPPORT_VIRTUAL_VOLUME
	tmp_volume =   set_volume_convert(volume);
	volume = tmp_volume;
#endif	
	ret = _set_volume( volume );
	
	if( ret != EPDK_FAIL )
	{
#if SUPPORT_VIRTUAL_VOLUME
	    dsk_volume = virtual_vol;	
#else	
	    dsk_volume = volume;	
#endif
        {
            reg_system_para_t* para;
            para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
            if(para)
            {
                para->volume = dsk_volume;
            }
        }   
	}
	
	return ret;	
}



/*
****************************************************************************************************
*Name        : dsk_volume_get_volume
*Prototype   : __s32 dsk_volume_get_volume(void)
*Arguments   : void
*Return      : the current volume value. -1 means fail.
*Description : get the curret volume value.
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_get(void)		// 当前音量大小
{
	if( !dsk_volume_gate_on )
	{
	    return 0;
	}
#if SUPPORT_VIRTUAL_VOLUME
	return get_volume_convert();
#else
	return _get_volume();
#endif
}



/*
****************************************************************************************************
*Name        : dsk_volume_inc_volume
*Prototype   : __s32 dsk_volume_inc_volume(void);
*Arguments   : void
*Return      : the current volume value. -1 means fail.
*Description : increase the volume.
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_inc(void)		// 增大音量
{
	__s32		volume;
	__s32       ret;
	__s32 		tmp_volume;
	if( !dsk_volume_gate_on )
	{
	    return -1;
	}

#if SUPPORT_VIRTUAL_VOLUME
	volume = get_volume_convert();
#else
	volume = _get_volume();
#endif
	if( volume == -1 )
	{
	    return -1;
	}
	
	if( volume < AUDIO_DEVICE_VOLUME_MAX )
	{
		volume++;	
#if SUPPORT_VIRTUAL_VOLUME
	tmp_volume =   set_volume_convert(volume);
	volume = tmp_volume;
#endif	
		ret = _set_volume( volume );		
    	if( ret != EPDK_FAIL )
    	{
    	    dsk_volume = volume;

            {
                reg_system_para_t* para;
                para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
                if(para)
                {
                    para->volume = volume;
                }
            }   
    	    return volume;	
    	}
    	else
    	{
    	    return -1;
    	}
	}
	else
	{
	    return volume;
	}
}

/*
****************************************************************************************************
*Name        : dsk_volume_dec_volume
*Prototype   : __s32 dsk_volume_dec_volume(void);
*Arguments   : void
*Return      : the current volume value. -1 means fail.
*Description : increase the volume.
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_dec(void)		// 减小音量
{
	__s32		volume;
	__s32       ret;
	__s32		tmp_volume;
	
	if( !dsk_volume_gate_on )
	{
	    return -1;
	}
	
	volume = dsk_volume_get();
	if( volume == -1 )
	{
	    return -1;
	}
	
	if( volume > AUDIO_DEVICE_VOLUME_MIN )
	{
		volume--;	
#if SUPPORT_VIRTUAL_VOLUME
	tmp_volume =   set_volume_convert(volume);
	volume = tmp_volume;
#endif	
		ret = _set_volume( volume );		
    	if( ret != EPDK_FAIL )
    	{
    	    dsk_volume = volume;

            {
                reg_system_para_t* para;
                para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
                if(para)
                {
                    para->volume = volume;
                }
            }       
    	    return volume;	
    	}
    	else
    	{
    	    return -1;
    	}
	}
	else
	{
	    return volume;
	}
}



/*
****************************************************************************************************
*Name        : dsk_volume_on
*Prototype   : __s32 dsk_volume_on(void);
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : make the voice on.
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_on( void )
{
	__s32 tmp_volume;
	
	if( dsk_volume_gate_on )
	{
	    return EPDK_OK;
    }
#if SUPPORT_VIRTUAL_VOLUME
	tmp_volume =   set_volume_convert(virtual_vol);
	dsk_volume = tmp_volume;
#endif	    
    _set_volume( dsk_volume );
	
	dsk_volume_gate_on = EPDK_TRUE;
	
	return EPDK_OK;	
}


/*
****************************************************************************************************
*Name        : dsk_volume_off
*Prototype   : __s32 dsk_volume_off(void);
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : make the voice off(mute).
*Other       :
****************************************************************************************************
*/
__s32 dsk_volume_off( void )	         // close voice
{
	if( !dsk_volume_gate_on )
	{
	    return EPDK_OK;
    }
    
    _set_volume( 0 );              // mute
	
	dsk_volume_gate_on = EPDK_FALSE;
	
	return EPDK_OK;	
}



/*
****************************************************************************************************
*Name        : dsk_volume_is_on
*Prototype   : __bool dsk_volume_is_on( void )
*Arguments   : void
*Return      : the status of the voice, on or off(mute).
*Description : get the status of the voice, on or off(mute).
*Other       : 
****************************************************************************************************
*/
__bool dsk_volume_is_on( void )
{
    return 	dsk_volume_gate_on;
}



static __s32 _set_volume( __s32 volume )
{
	ES_FILE		*p_dac;	
	__s32        ret;

    if(volume < AUDIO_DEVICE_VOLUME_MIN
        || volume > AUDIO_DEVICE_VOLUME_MAX)
    {
        return EPDK_FAIL;
    }
    
	p_dac = eLIBs_fopen(AUDIO_DRV_PATH, "r+");
	if(!p_dac)
	{
		__err("open dac device fail!\n");
		return EPDK_FAIL;
	}
	
#if PA_HIGH_OR_LOW_MUTE
    if(AUDIO_DEVICE_VOLUME_MIN == volume)
    {
        pull_up_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//关闭功放
    }
    else
    {
        if(g_amplifier_status == 0)
        {
            pull_up_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//关闭功放
        }
        else
        {
            pull_down_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//打开功放
        }        
    }
#else
    if(AUDIO_DEVICE_VOLUME_MIN == volume)
    {
        pull_down_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//关闭功放
    }
    else
    {
        if(g_amplifier_status == 0)
        {
            pull_down_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//关闭功放
        }
        else
        {
            pull_up_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//打开功放
        }        
    }
#endif	
	ret = eLIBs_fioctrl(p_dac, AUDIO_DEV_CMD_SET_VOLUME, volume, 0);
		
	eLIBs_fclose(p_dac);   
	
	return ret;
}

static __s32 _get_volume( void )
{
	ES_FILE		*p_dac;	
	__s32        volume;
	
	p_dac = eLIBs_fopen(AUDIO_DRV_PATH, "r+");
	if(!p_dac)
	{
		__err("open dac device fail!\n");
		return -1;
	}
	
	volume = eLIBs_fioctrl(p_dac, AUDIO_DEV_CMD_GET_VOLUME, 0, 0);	
		
	eLIBs_fclose(p_dac);		
	
	return volume;
}

#if 1
void dsk_rec_onoff(__s32 flag)
{
	return;
}
#endif


__s32 dsk_amplifier_onoff(__s32 flag)//设置功放开关
{

    g_amplifier_status = flag;

#if PA_HIGH_OR_LOW_MUTE
    if(0 == flag)
    {
		pull_up_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//关闭功放	pc9
    }
    else
    {
        __s32 sta;
            
        reg_system_para_t* para;
        
        para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);                        
        
        if(para&& AUDIO_DEVICE_VOLUME_MIN == para->volume)//音量最小
		{
			pull_up_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//关闭功放
		}
        else
        {
             pull_down_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//打开功放
        }
    }
#else
    if(0 == flag)
    {
		pull_down_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//关闭功放	pc9
    }
    else
    {
        __s32 sta;
            
        reg_system_para_t* para;
        
        para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);                        
        
        if(para&& AUDIO_DEVICE_VOLUME_MIN == para->volume)//音量最小
		{
			pull_down_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//关闭功放
		}
        else
        {
             pull_up_gpio(PA_MUTE_IO_PORT, PA_MUTE_IO_PIN);//打开功放
        }
    }
#endif

    return EPDK_OK;   
}

__bool dsk_amplifier_is_on(void)//查询功放开关
{
    if(1 == g_amplifier_status)
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    }
}



void dsk_set_record_is_recording(__bool val)
{
	rec_is_record = val;
	return;
}

__bool dsk_get_record_is_recording(void)
{
	return rec_is_record;
}


