/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dsk_power.h
* By      	: Andy.zhang
* Func		: power interface 
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "..\\functions_i.h"

#define 	BATTERY_AVER_CNT 	20

/*
 * 剩余电量level, 判断低点
 */
static __u32 g_battery[BATTERY_AVER_CNT];
static __u32 g_battery_cnt = 0; 

static __u32 speaker_open_status;

//电压级别 DRV_POWER_CMD_BATADC_GET
__s32 dsk_power_get_battery_level(power_level_e *p_level)	
{
	/*
	ES_FILE  *power;
	__drv_power_battery_status_t1 pbstatus;
	power_level_e level;
	
	
	power = eLIBs_fopen("b:\\HID\\POWER", "w");
	if( !power )
	{
		__msg(" Power Driver cannot be open !\n");
		*p_level = DSK_POWER_LEVEL_0;
		return EPDK_FAIL;		
	}       		
	eLIBs_fioctrl(power, DRV_POWER_CMD_BATADC_GET, 0, &pbstatus);
	eLIBs_fclose(power);

	if(!pbstatus.battery_exist)
		return -1;
	
	//cal last five aver volume
	{
		__u32 i;
		__u32 total_battery = 0;
	
		if(g_battery_cnt >= BATTERY_AVER_CNT)
		{	
			for(i = 0; i < g_battery_cnt - 1; i++)
			{
				g_battery[i] = g_battery[i + 1];
			}
			g_battery_cnt = BATTERY_AVER_CNT;
		}
		else
		{
			g_battery_cnt++;
		}
		g_battery[g_battery_cnt - 1] = pbstatus.bat_vol;
		
		for(i = 0; i < g_battery_cnt; i++)
		{
			total_battery += g_battery[i];
		}

		pbstatus.bat_vol = total_battery/g_battery_cnt;
	}
		
	if(pbstatus.bat_vol < 3500)
		level  = DSK_POWER_LEVEL_0;
	else if(pbstatus.bat_vol >= 3500 && pbstatus.bat_vol < 3550)
		level = DSK_POWER_LEVEL_1;
	else if(pbstatus.bat_vol >= 3550 && pbstatus.bat_vol < 3625)
		level = DSK_POWER_LEVEL_2;
	else if(pbstatus.bat_vol >= 3625 && pbstatus.bat_vol < 3750)
		level = DSK_POWER_LEVEL_3;
	else if(pbstatus.bat_vol >= 3750 && pbstatus.bat_vol < 3950)
		level = DSK_POWER_LEVEL_4;
	else
		level = DSK_POWER_LEVEL_5;
	
	*p_level = level;
	*/
	return EPDK_OK;
}

/*
 * 工作电压level,(工具栏显示电池电量级别)
 */ 
 //电量级别 DRV_POWER_CMD_GET_FUELGUAGE
__s32 dsk_power_get_voltage_level(power_level_e *p_level)
{
#if 0
	ES_FILE  						*power;
	power_level_e 					vol_level;
	__drv_power_fuelguage_t  		info;
	int 							rest_vol;
    
	power = eLIBs_fopen("b:\\HID\\POWER", "w");
	if( !power )
	{
		__msg(" Power Driver cannot be open !\n");
		*p_level = DSK_POWER_LEVEL_0;
		return EPDK_FAIL;		
	}
	eLIBs_fioctrl(power, DRV_POWER_CMD_GET_FUELGUAGE, 0, &info );		
		
	eLIBs_fclose( power );
	
	rest_vol = info.rest_vol;

//    __msg("info.rest_vol=%d\n", info.rest_vol);
		
	if( rest_vol >=100)
		vol_level = DSK_POWER_LEVEL_5;
	else if( rest_vol >=80 )
		vol_level = DSK_POWER_LEVEL_4;
	else if( rest_vol >=60 )
		vol_level = DSK_POWER_LEVEL_3;
	else if( rest_vol >=40 )
		vol_level = DSK_POWER_LEVEL_2;
	else if( rest_vol >=20 )
		vol_level = DSK_POWER_LEVEL_1;
	else if( rest_vol >=0 )
		vol_level = DSK_POWER_LEVEL_0;
	else
		vol_level = DSK_POWER_LEVEL_0;
	
	*p_level = vol_level;
	#endif
	return EPDK_OK;	
}

__bool dsk_power_is_low(void)
{
#if 0
	ES_FILE *power;
	__s32	 ret;
	power_level_e level;    
	__drv_power_battery_status_t3 bat_sta;	
    static __s32 low_power_cnt = 0;
 
	power = eLIBs_fopen("b:\\HID\\POWER", "w");
	if( !power )
	{
		__msg(" Power Driver cannot be open !\n");
		return EPDK_FALSE;		
	}		   
	
	ret  = dsk_power_get_voltage_level(&level);
	if( ret<0)
	{
		eLIBs_fclose( power );
		return EPDK_FALSE;
	}
			
	eLIBs_memset(&bat_sta, 0, sizeof(__drv_power_battery_status_t3));
	eLIBs_fioctrl(power, DRV_POWER_CMD_GET_DCIN, 0, &bat_sta);
	
//	eLIBs_fclose( power );
        
    __msg("***********bat_sta.charge_status=%d, level=%d***********\n", bat_sta.charge_status, level);

    __msg("low_power_cnt=%d\n", low_power_cnt);
	if(bat_sta.charge_status == 0 && level == DSK_POWER_LEVEL_0 )	// 低电   //在此处扩音器没有充电检测脚
	{			
        low_power_cnt++;
		
		if((low_power_cnt == 10) || (low_power_cnt == 90) || (low_power_cnt == 140) || (low_power_cnt == 160) || (low_power_cnt == 170))
		{
			//Turn On BackLight;	Ethan;
			{
				__gui_msg_t msg;

				msg.id 			= DSK_MSG_SCREEN_OPEN;
				msg.h_srcwin 		= NULL;
				msg.h_deswin		= GUI_WinGetHandFromName("init");
				msg.dwAddData1	= (__u32)0;
				msg.dwAddData2	= (__u32)0;
				
				GUI_SendNotifyMessage(&msg);
			}			__here__;
			{
				__gui_msg_t 		ntymsg;

				ntymsg.id			=  DSK_MSG_LOW_POWER_TIP;
				ntymsg.h_srcwin 		=  NULL;
				ntymsg.h_deswin 		=  GUI_WinGetHandFromName("beetles"); 
				ntymsg.dwAddData1	=  NULL;
				ntymsg.dwAddData2	=  NULL;
				ntymsg.dwReserved	=  NULL;
				
				GUI_SendNotifyMessage(&ntymsg); 
			}			
		}
        if(low_power_cnt > 180)
        {
            __msg("low_power_cnt > 10!!!\n");
    		return EPDK_TRUE;
        }
	}
	else
	{
        low_power_cnt = 0;
//        __here__;		
	}
    
	eLIBs_fclose( power );
	#endif
    return EPDK_FALSE;
	
}

__s32 dsk_power_set_off_time(__u32 time)
{
//	__gui_msg_t msg;
//	__s32 ret;
//	
//	g_memset(&msg, 0, sizeof(__gui_msg_t));
//	msg.id 			= DSK_MSG_SET_AUTO_OFF;
//	msg.h_srcwin 	= NULL;
//	msg.h_deswin	= GUI_WinGetHandFromName(APP_DESKTOP_MWIN);
//	msg.dwAddData1	= (__u32)time;
//	msg.dwAddData2	= (__u32)0;
//	
//	ret = GUI_SendNotifyMessage(&msg);
//	
//	return ret;
	return EPDK_OK;
}

__s32 dsk_power_get_battary_state(power_state_e *state)
{
    /*
	ES_FILE *power;
	__drv_power_fuelguage_t  	info;
	
	power = eLIBs_fopen("b:\\HID\\POWER", "w");
	if( !power )
	{
		__msg(" Power Driver cannot be open !\n");
		return EPDK_FALSE;		
	}	

 	eLIBs_fioctrl(power, DRV_POWER_CMD_GET_FUELGUAGE, 0, &info);	
 	
	if(info.charge_status & 0x2)	
	{
		if(info.charge_status & 0x4)
		{
			if(info.charge_status & 0x1)
			{
				*state = DSK_POWER_BATTARY_CHARGE;				
			}
			else
			{
				*state = DSK_POWER_BATTARY_FULL;	
			}
		}
		else
		{
			*state = DSK_POWER_BATTARY_FULL;
		}
	}
	else
	{
		*state = DSK_POWER_BATTARY_FULL;
	}
	
	eLIBs_fclose( power );
	*/
	return EPDK_OK;
}

//__s32 dsk_power_off(void);
//{
//	__gui_msg_t msg;
//	
//	msg.id 			= DSK_MSG_POWER_OFF;
//	msg.h_srcwin 	= NULL;
//	msg.h_deswin	= GUI_WinGetHandFromName(APP_DESKTOP_MWIN);
//	msg.dwAddData1	= 0;
//	msg.dwAddData2  = 0;
//	msg.p_arg		= 0;
//	
//	return GUI_SendNotifyMessage(&msg);
//}



void dsk_speaker_turnoff(void)    
{           
    ES_FILE *pfile;
    
    __inf("speaker control off.....\n");
    pfile = eLIBs_fopen("b:\\HID\\POWER", "r+");
	if(pfile == 0)
    {
	    __msg("b:\\HID\\POWER cannot open \n");
	    return;
    }
    //Get the previous status
    speaker_open_status=eLIBs_fioctrl(pfile,DRV_POWER_CMD_PWREPT_GET,POWER_EPT_3,0);
    //Pull Low
    eLIBs_fioctrl(pfile, DRV_POWER_CMD_PWREPT_SET, POWER_EPT_3, (void *)((POWER_EPT_MOS_N) | (POWER_EPT_GATE_ON)));
    eLIBs_fclose(pfile);
}

void dsk_speaker_turnon(void)  
{
    ES_FILE *pfile;
    __inf("speaker control On.....\n");
    
    pfile = eLIBs_fopen("b:\\HID\\POWER", "r+");
	if(pfile == 0)
    {
	    __err("b:\\HID\\POWER cannot open \n");
	    return;
    }
    // Get the previous status
    speaker_open_status=eLIBs_fioctrl(pfile,DRV_POWER_CMD_PWREPT_GET,POWER_EPT_3,0);
    //Pull High
    eLIBs_fioctrl(pfile, DRV_POWER_CMD_PWREPT_SET, POWER_EPT_3, (void *)((POWER_EPT_MOS_P) | (POWER_EPT_GATE_ON))); 
    eLIBs_fclose(pfile);
}

void dsk_speaker_resume(void)
{
	ES_FILE *pfile;
    __inf("speaker control resume.....\n");
    pfile = eLIBs_fopen("b:\\HID\\POWER", "r+");
	if(pfile == 0)
    {
	    __msg("b:\\HID\\POWER cannot open \n");
	    return;
    }
    eLIBs_fioctrl(pfile, DRV_POWER_CMD_PWREPT_SET, POWER_EPT_3, (void *)speaker_open_status);     //return the previous status
    eLIBs_fclose(pfile);
}

__bool dsk_get_charge_state(void)
{
	#if 0
	ES_FILE *power; 
	__drv_power_battery_status_t3 bat_sta;	
    
	power = eLIBs_fopen("b:\\HID\\POWER", "w");
	if( !power )
	{
		__msg(" Power Driver cannot be open !\n");
		return EPDK_FALSE;		
	}		   
			
	eLIBs_memset(&bat_sta, 0, sizeof(__drv_power_battery_status_t3));
	eLIBs_fioctrl(power, DRV_POWER_CMD_GET_DCIN, 0, &bat_sta);
	
	eLIBs_fclose( power );          
      
	if(0 == bat_sta.charge_status)	// 非充电  //cai modify for 电池检测
	{			
        __here__;
		return EPDK_FALSE;
	}
	else//充电
	{
        __here__;
		return EPDK_TRUE;
	}
	#endif
	return EPDK_FALSE;
}
__bool dsk_power_is_full(void)
{
    ES_FILE  						*power;
	__drv_power_fuelguage_t  		info;
	
	power = eLIBs_fopen("b:\\HID\\POWER", "rb");
	if( !power )
	{
		__msg(" Power Driver cannot be open !\n");		
		return EPDK_FALSE;		
	}
    
	eLIBs_fioctrl(power, DRV_POWER_CMD_GET_FUELGUAGE, 0, &info );			
	eLIBs_fclose( power );
    if(1 == info.is_full_charge)
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    }
}

#if 1//wqx 20130408
#define PIOC_MEM_BASE   (0xf1c20800)

#define PIO_REG_CFG(n, i)               ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x00))
#define PIO_REG_DLEVEL(n, i)            ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x14))
#define PIO_REG_PULL(n, i)              ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + ((i)<<2) + 0x1C))
#define PIO_REG_DATA(n) 	            ((volatile unsigned int *)( PIOC_MEM_BASE + ((n)-1)*0x24 + 0x10))


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

void dsk_power_onoff_ctl(__s32 flag)
{
#if 0
    if(1 == flag)
    {
        pull_up_gpio(3, 8);//power on
    }
    else
    {
                pull_down_gpio(3, 8);//power off
        
    }
#endif	
    
}

#endif


