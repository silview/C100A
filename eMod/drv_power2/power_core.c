/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                Power Driver
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : power_core.c
* By      : Sunny
* Version : v1.0
* Date    : 2011-4-30
* Descript: no pmu power dirver.
* Update  : date                auther      ver     notes
*           2011-4-30 10:07:53  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "drv_power_i.h"

//以下定义为AD采用服务
#define POWER_ADC_SPMPLE_NUM 6//6次AD采样取平均
static __u32 g_adc_val[POWER_ADC_SPMPLE_NUM]={0};
static __s32 g_cur_adc_sample_num = 0;
__s32 g_adc_exp = 1027;
static __u32 CalcVoltage(__u32 ADCVal)
{
    return ADCVal * 2000 / 63 * g_adc_exp/1000.0 * (BAT_HIGHT_RESISITOR + BAT_LOW_RESISITOR) / BAT_LOW_RESISITOR;
}

static __u32 PowerGetADSampleVal(void)
{
    __s32 cnt;
    volatile __u32          *tmpAddr;
    volatile __u32           tmpVal;

    //enable LADC
    tmpAddr = LRADC_CRTL_ADDR;
    tmpVal = *tmpAddr;
    tmpVal = tmpVal | 0x01;
    *tmpAddr = tmpVal;
    
    //wait data ready
    tmpVal = LRADC_DATA_VALUE;
    tmpAddr = LRADC_INTSTAT_ADDR;
    *tmpAddr |= 0x1;
    cnt = 0;
    while (1)
    {
        if ((*tmpAddr & 0x1) == 1)
        {
            //data ready
            break;
        }
        if(cnt++ > 2)
        {
            break;
        }
        esKRNL_TimeDly(10);
    }

    if((*tmpAddr & 0x1) == 1)
    {
        //获取ADC采样值
        tmpVal = LRADC_DATA_VALUE;        
    }
    else
    {
        __wrn("********get adc val fail...************\n");
        tmpVal = 0x3f;        
    }
     
    __wrn("********tmpVal=%d********\n", tmpVal);
        
    tmpVal = tmpVal & 0x3f;
    
    return tmpVal;    
}

static __u32 PowerGetAverage(__u32* val, __s32 num)
{
    __u32 total;
    __s32 i;

    if(num <= 0)
    {
        return 0;
    }
    
    total = 0;
    for(i = 0 ; i < num ; i++)
    {
        total += val[i];
    }

    return total/num;
}

void PowerTask(void *parg)
{    
    __drv_power_fuelguage_t *pFuelguage;
    __u32                    tmpVal;
    __u32                    cur_adc_val;
    __u32                    Voltage;
    
    if (parg == NULL)
    {
        __wrn("invalid argument for power timer task\n");
        return ;
    }
    
    pFuelguage = (__drv_power_fuelguage_t *)parg;

    cur_adc_val = PowerGetADSampleVal();

    if(g_cur_adc_sample_num < POWER_ADC_SPMPLE_NUM)
    {              
        g_adc_val[g_cur_adc_sample_num++] = cur_adc_val;        
    }
    else
    {
        __s32 i;

        for(i = 0 ; i < POWER_ADC_SPMPLE_NUM-1 ; i++)
        {
            g_adc_val[i] = g_adc_val[i+1];
        }
        g_adc_val[POWER_ADC_SPMPLE_NUM-1] = cur_adc_val;
    }

    tmpVal = PowerGetAverage(g_adc_val, g_cur_adc_sample_num);   

    //__msg("********tmpVal=%d********\n", tmpVal);

    //__msg("********tmpVal*k=%f********\n", tmpVal* 2000 / 63 * 1.0276);
    
    
    //计算实际电池电压(mV)
    Voltage = CalcVoltage(tmpVal);

    __msg("Voltage=%d\n", Voltage);
    
    pFuelguage->is_low_power = 0;
    pFuelguage->is_full_charge = 0;
#if 0
    if(Voltage < 3400)//低电关机电压
    {
        pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL0;
        pFuelguage->is_low_power = 1;        
    }
    else if(Voltage <= 3550)
    {        
    	pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL0;
    	
    	//发送低电关机系统消息
    	//__msg("low power voltage detect...\n");
    	//esKSRV_SendMsg(KMSG_USR_KEY_POWEROFF, KMSG_PRIO_HIGH);
    }
    else if((Voltage > 3550) && (Voltage <= 3650))
    {
        //电池电量为20%
        pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL1;
    }
    else if((Voltage > 3650) && (Voltage <= 3750))
    {
        //电池电量为40%
        pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL2;
    }
    else if((Voltage > 3750) && (Voltage <= 3850))
    {
        //电池电量为60%
        pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL3;
    }
    else if((Voltage > 3850) && (Voltage <= 3950))
    {
        //电池电量为80%
        pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL4;
    }
    else if(Voltage > 3950 && Voltage <= 4150)
    {
        //电池电量为100%
    	pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL5;
    }
    else
    {
        //电池电量为100%
    	pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL5;
        pFuelguage->is_full_charge = 1;
    }
#else
    if(Voltage <= 3096)//低电关机电压
    {
        pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL0;
        //pFuelguage->is_low_power = 1;   //cai modify for usb插入后还是提示低电关机        
    }
    else if((Voltage <= 3200) && (Voltage > 3096))
    {
        //电池电量为20%
        pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL1;
    }
    else if((Voltage <= 3300) && (Voltage > 3200))
    {
        //电池电量为40%
        pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL2;
    }
    else if((Voltage > 3300) && (Voltage <= 3420))
    {
        //电池电量为60%
        pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL3;
    }
    else if((Voltage > 3420) && (Voltage <= 3550))
    {
        //电池电量为80%
        pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL4;
    }
    else if(Voltage > 3550 && Voltage <= 3701)
    {
        //电池电量为100%
    	pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL5;
    }
	else if(Voltage > 3701)		//4.1V
    {
        //电池电量为100%
    	pFuelguage->rest_vol = NO_PMU_BATTERY_LEVEL5;
        pFuelguage->is_full_charge = 1;
    }
#endif
    pFuelguage->charge_status = 0x07;
    //__msg(".......66666..........pstatus->charge_status=%d......\n",pFuelguage->charge_status);
    //剩余时间暂时不支持
    pFuelguage->rest_time = 0;
    
    //__msg("PowerTask, rest vol = %d\n", pFuelguage->rest_vol);
}

