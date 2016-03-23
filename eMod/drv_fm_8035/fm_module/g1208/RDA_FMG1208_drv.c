/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : rda5820.c
* By      : xueli
* Version : V1.00
*********************************************************************************************************
*/
#include  "RDA_FMG1208_drv.h"
#include  "qnws.h"


#if(FM_MODULE_TYPE == FM_MODULE_TYPE_G1208)

void QND_TuneToCH(__s32 ch);

#if(FM_MODULE_TYPE == FM_MODULE_TYPE_G1208)
#if 0
#define __msg(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
#endif
__u8 VolValue=0;
#define  FM_G1208_ADDR         (0x20>>1)// (0x84)

static   __fm_setting_info_t       rda5820_info = {87000, 108000, 0, 0, 3, 0};


UINT8    qnd_ChipID;

UINT8   qnd_R16; 
UINT8   qnd_R17;
UINT8   qnd_R46;

UINT8    qnd_StepTbl[3]={5,10,20};


//////////////////////FDO////////////////////////////
__u32  g_QNA_AirAudio_Flag=1;



extern __s32 QNA_global_init(void);
extern unsigned short g_mutecount;
extern unsigned char g_mute_st;
extern unsigned char g_currstate;
extern unsigned char g_mutef;

static __u8 QNA_AirAudio_tid = 0;

#ifndef QN8035_ONLY_FM

static void QNA_AirAudio_Thread(void *p_arg)
{    	
	
	__s32 unmute_cnt = 0;
	__msg("+++QNA_AirAudio_Thread+++\n");
	
	QNA_global_init();
	
	while (0==g_exitflag) 	{
        	if (esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ) 	{
            		break;
	       }
			__here__;
		 QNA_AirAudio();
         if(1){
            __u8 pilot;
            
            pilot = QND_ReadReg(STATUS1) & ST_MO_RX;    // 0: stereo, pilot exist
            if (pilot) 
            {
                
				if(g_mutecount > WAITTING4MUTE && g_mute_st == EPDK_FALSE)
				{
					//pull_down_gpio(3,4);
					//esKRNL_TimeDly(5);
					QNF_SetMute(1);
					
					//pull_up_gpio(3,4);	
					
				}
				g_mutecount++;
				if(g_mutecount > 2)
					unmute_cnt = 0;
			
            }
            else 
            {
                unmute_cnt++;

				if(unmute_cnt > 2)//防噪音 
				{
					g_mutecount = 0;
					if(g_mute_st == EPDK_TRUE)
					{
						//pull_down_gpio(3,4);	
						//esKRNL_TimeDly(5);
						QNF_SetMute(0);
						//esKRNL_TimeDly(10);
						//pull_up_gpio(3,4);
					}
				}
            }
         }
		 QND_Delay(10);	 		
		__here__;		 
	}

	esKRNL_TDel(OS_PRIO_SELF);
}

#endif


///////////////////////////////////////////////////



static __s32 fm_write_data(__u8 addr, __u8 *data,__s32 n_byte)
{
	__twi_dev_para_ex_t  piic;        //要写入TEA5767的数据  
	__s32 ret;
	int i = 0;
	__u8 *pData = data;
	eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));
			
	
	//for(i = 0; i < 10000; i++)
	piic.slave_addr_flag = TWI_SLAVE_ADDRESS_07BIT;
	piic.slave_addr 	 = (0x20>>1);//(0x20>>1);//(0x22>>1);//
	piic.byte_addr_width = 1;
	piic.byte_addr		 = &addr;
	piic.data			 = data;
	piic.byte_count 	 = n_byte;
	pData = data;

	for(i=8; i>0; i--)
	{		
		ret = eLIBs_fioctrl(fiic, TWI_WRITE_SPEC_RS, 0, (void *)&piic);

		if(ret != EPDK_OK)
		{
		    if(i <= 0)
			 __msg("fm i2c write error~\n");
		}
		else
		{
			break;
		}
		
	}
	return ret;
}
static __s32 fm_read_data(__u8 addr, __u8 *data,__s32 n_byte)
{
	__twi_dev_para_ex_t  piic;        //要写入TEA5767的数据  
	__s32 ret;
	__s32 i;

  
	eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));
	//n_byte = 4;
	piic.slave_addr_flag = TWI_SLAVE_ADDRESS_07BIT;
	piic.slave_addr 	 = (0x20>>1);//(0x22>>1);	//(0x22>>1);//(0x20>>1);
	piic.byte_addr_width = 1;
	piic.byte_addr		 = &addr;
	piic.data			 = data;
	piic.byte_count 	 = n_byte;	

	for(i=8; i>0; i--)
	{
		ret = eLIBs_fioctrl(fiic, TWI_READ_SPEC_RS, 0, (void *)&piic);
		if(ret == EPDK_FAIL)
		{
		    if(i <= 0)
			__msg("fm i2c read error~\n");
		}
		else
		{
			break;
		}

	}
			
	{
		int i = 0;
		__u8 *pdata = data;
	}
	return ret;
}
UINT8 QND_WriteReg(UINT8 addr, UINT8 value)
{
	return fm_write_data(addr,&value,1);
}
__u8 QND_ReadReg(__u8 Addr)
{
	__u8 Data;
	if(EPDK_FAIL==fm_read_data(Addr,&Data,1))
		return 0;
	return Data;
}
void qn8035_set_vol(__u8 volume)
{
	__u8  regVal;
//  volume = volume*3;
  if(volume>47)
  {
  	volume = 47;//qn8035 volume range is from 0~47
  }
  VolValue=volume;
    regVal = QND_ReadReg(VOL_CTL);
	regVal = (regVal&0xC0)|(volume/6)|(5-volume%6<<3);
	QND_WriteReg(VOL_CTL,regVal);
}
__s32 qn8035_get_vol(void)
{
	return VolValue;
}
__s32 fm_vol_set(__u8 volume)
{
	qn8035_set_vol(volume);
	return EPDK_OK;
}
__s32 fm_vol_get(void)
{
	return qn8035_get_vol();
}
/**********************************************
* function:      fm_iic_write
*
* description:   iic write
*
* notes:         5字节连续写
*
**********************************************/

static void fm_rx_init(void)
{
	return;
}

static void fm_tx_init(void)
{
    return;
}

__s32 fm_get_status(void)
{
 
    return EPDK_OK;
}

/**********************************************************************
void QNF_SetRegBit(UINT8 reg, UINT8 bitMask, UINT8 data_val)
**********************************************************************
Description: set register specified bit

Parameters:
    reg:        register that will be set
    bitMask:    mask specified bit of register
    data_val:    data will be set for specified bit
Return Value:
    None
**********************************************************************/
void QNF_SetRegBit(UINT8 reg, UINT8 bitMask, UINT8 data_val)  
{
    UINT8 temp;
    temp = QND_ReadReg(reg);
    temp &= (UINT8)(~bitMask);
    temp |= data_val & bitMask;
    QND_WriteReg(reg, temp);
}


/**********************************************************************
void QNF_RXInit()
**********************************************************************
Description: set to SNR based MPX control. Call this function before 
             tune to one specific channel

Parameters:
None
Return Value:
None
**********************************************************************/
void QNF_RXInit()
{
    QNF_SetRegBit(0x1B,0x08,0x00);  //Let NFILT adjust freely
    QNF_SetRegBit(0x2C,0x3F,0x12);
    QNF_SetRegBit(0x1D,0x40,0x00);
    QNF_SetRegBit(0x41,0x0F,0x0A);
    QND_WriteReg(0x45,0x50);
    QNF_SetRegBit(0x40,0x70,0x70);
    QNF_SetRegBit(0x19,0x80,0x80);
    QNF_SetRegBit(0x3E,0x80,0x80);
    QNF_SetRegBit(0x41,0xE0,0xC0);
    if(qnd_ChipID == CHIPSUBID_QN8035A0)
    {
        QNF_SetRegBit(0x42,0x10,0x10);
    }
    QNF_SetRegBit(0x34, 0x7F,SMSTART_VAL);
    QNF_SetRegBit(0x35,0x7F,SNCSTART_VAL);
    QNF_SetRegBit(0x36,0x7F,HCCSTART_VAL);
}

/**********************************************************************
void QNF_SetMute(UINT8 On)
**********************************************************************
Description: set register specified bit

Parameters:
On:        1: mute, 0: unmute
Return Value:
None
**********************************************************************/
extern unsigned char g_mute_st;
void QNF_SetMute(UINT8 On)
{

	if(On)
	{
        if(qnd_ChipID == CHIPSUBID_QN8035A0)
        {
            QNF_SetRegBit(REG_DAC, 0x0B, 0x0B);
        }
        else
        {
            QNF_SetRegBit(0x4A, 0x20, 0x20);
        }
		g_mute_st = EPDK_TRUE  ;
	}
	else
	{
        if(qnd_ChipID == CHIPSUBID_QN8035A0)
        {
            QNF_SetRegBit(REG_DAC, 0x0B, 0x00);
        }
        else
        {
            QNF_SetRegBit(0x4A, 0x20, 0x00);
        }
		g_mute_st = EPDK_FALSE;
	}

}

/**********************************************************************
UINT16 QNF_GetCh()
**********************************************************************
Description: get current channel frequency

Parameters:
    None
Return Value:
    channel frequency
**********************************************************************/
UINT16 QNF_GetCh() 
{
    UINT8 tCh;
    UINT8  tStep; 
    UINT16 ch = 0;
    // set to reg: CH_STEP
    tStep = QND_ReadReg(CH_STEP);
    tStep &= CH_CH;
    ch  =  tStep ;
    tCh= QND_ReadReg(CH);    
    ch = (ch<<8)+tCh;
    return CHREG2FREQ(ch);
}

/**********************************************************************
void QNF_SetCh(UINT16 freq)
**********************************************************************
Description: set channel frequency 

Parameters:
    freq:  channel frequency to be set
Return Value:
    None
**********************************************************************/
void QNF_SetCh(UINT16 freq) 
{
    // calculate ch parameter used for register setting
    UINT8 tStep;
    UINT8 tCh;
    UINT16 f; 
    UINT16 pll_dlt;
    
    if(freq == 8540 || freq == 8550 || freq == 8560)
    {
        pll_dlt = (UINT16)qnd_R16 >> 3;       
        pll_dlt |=(UINT16)qnd_R17 << 5;    
        pll_dlt -= 1039;  
        QND_WriteReg(0x16, (UINT8)pll_dlt << 3);
        QND_WriteReg(0x17, (UINT8)(pll_dlt >> 5));         
        if(freq == 8540) QND_WriteReg(0x46, 0x9D);
        else if(freq == 8550) QND_WriteReg(0x46, 0x69);
        else QND_WriteReg(0x46, 0x34);
        freq = 8570;
    }
    else
    {
        QND_WriteReg(0x16, qnd_R16);
        QND_WriteReg(0x17, qnd_R17);
        QND_WriteReg(0x46, qnd_R46);
    }    
	//Manually set RX Channel index
    QNF_SetRegBit(SYSTEM1, CCA_CH_DIS, CCA_CH_DIS); 
    f = FREQ2CHREG(freq); 
    // set to reg: CH
    tCh = (UINT8) f;
    QND_WriteReg(CH, tCh);
    // set to reg: CH_STEP
    tStep = QND_ReadReg(CH_STEP);
    tStep &= ~CH_CH;
    tStep |= ((UINT8) (f >> 8) & CH_CH);
    QND_WriteReg(CH_STEP, tStep);    
}

/**********************************************************************
void QNF_ConfigScan(UINT16 start,UINT16 stop, UINT8 step)
**********************************************************************
Description: config start, stop, step register for FM/AM CCA or CCS

Parameters:
    start
        Set the frequency (10kHz) where scan to be started,
        e.g. 7600 for 76.00MHz.
    stop
        Set the frequency (10kHz) where scan to be stopped,
        e.g. 10800 for 108.00MHz
    step        
        1: set leap step to (FM)100kHz / 10kHz(AM)
        2: set leap step to (FM)200kHz / 1kHz(AM)
        0:  set leap step to (FM)50kHz / 9kHz(AM)
Return Value:
         None
**********************************************************************/
void QNF_ConfigScan(UINT16 start,UINT16 stop, UINT8 step)  
{
    // calculate ch para
    UINT8 tStep = 0;
    UINT8 tS;
    UINT16 fStart;
    UINT16 fStop;
    
    fStart = FREQ2CHREG(start);
    fStop = FREQ2CHREG(stop);
    // set to reg: CH_START
    tS = (UINT8) fStart;
    QND_WriteReg(CH_START, tS);
    tStep |= ((UINT8) (fStart >> 6) & CH_CH_START);
    // set to reg: CH_STOP
    tS = (UINT8) fStop;
    QND_WriteReg(CH_STOP, tS);
    tStep |= ((UINT8) (fStop >> 4) & CH_CH_STOP);
    // set to reg: CH_STEP
    tStep |= step << 6;
    QND_WriteReg(CH_STEP, tStep);
}

/**********************************************************************
UINT8 QND_GetRSSI(UINT16 ch)
**********************************************************************
Description:    Get the RSSI value
Parameters:
Return Value:
RSSI value  of the channel settled
**********************************************************************/
UINT8 QND_GetRSSI(UINT16 ch)  
{    
    QNF_SetRegBit(REG_REF,ICPREF,0x0a);
    QNF_ConfigScan(ch, ch, 1); 
    QNF_SetCh(ch);
    if(qnd_ChipID == CHIPSUBID_QN8035A0)
    {
        //Enter CCA mode, speed up the PLL locking. 
		QNF_SetRegBit(SYSTEM1, CHSC, CHSC); 
        QND_Delay(QND_READ_RSSI_DELAY+90); 
    }
    else if(qnd_ChipID == CHIPSUBID_QN8035A1)
    {
        //if this delay time effects the CCA time,it may be shorted to 20ms
        QND_Delay(50); 
    }
	else
    {
        QNF_SetRegBit(0x55, 0x80, 0x80);    
        QND_Delay(20);
        QNF_SetRegBit(0x55, 0x80, 0x00);    
    }
    QNF_SetRegBit(REG_REF,ICPREF,0x00);
    if(QND_ReadReg(RDSCOSTAS) & 0x80) 
    {
        return (QND_ReadReg(RSSISIG)+9);  
    }
    else 
    {
        return QND_ReadReg(RSSISIG);  
    }
}

/**********************************************************************
int QND_Delay()
**********************************************************************
Description: Delay for some ms, to be customized according to user
             application platform

Parameters:
        ms: ms counts
Return Value:
        None
            
**********************************************************************/
void QND_Delay(UINT16 ms)  
{
    volatile UINT16 i;
    
    /*while(ms--)
    {
        //this constant 15000 is just for MTK6253 running on 104M.
        for(i=0; i<15000; i++)
        {
        }
    }*/
   if(ms < 10)
   {
       esKRNL_TimeDly(1);
   }
   else
   {
       ms = ms/10 + 1;
	   esKRNL_TimeDly(ms);
   }
}

/**********************************************************************
int QND_Init()
**********************************************************************
Description: Initialize device to make it ready to have all functionality ready for use.

Parameters:
    None
Return Value:
    1: Device is ready to use.
    0: Device is not ready to serve function.
**********************************************************************/


/**********************************************************************
int QND_Init()
**********************************************************************
Description: Initialize device to make it ready to have all functionality ready for use.

Parameters:
    None
Return Value:
    1: Device is ready to use.
    0: Device is not ready to serve function.
**********************************************************************/
UINT8 QND_Init(void) 
{
#if 1
    QND_WriteReg(0x00, 0x81); 
    QND_Delay(10);
    qnd_ChipID = QND_ReadReg(CID1) & 0x03;
    // change crystal frequency setting here
    //setting input clock source type:sine-wave clock or digital clock
    QNF_SetRegBit(0x01,0x80,QND_SINE_WAVE_CLOCK);
    QND_WriteReg(0x15, QND_XTAL_DIV0);   // 
    QND_WriteReg(0x16, QND_XTAL_DIV1);///
    QND_WriteReg(0x17, QND_XTAL_DIV2);  ///  
    
    QND_Delay(10);  
    QND_WriteReg(0x54, 0x47);//mod PLL setting
    QND_WriteReg(0x19, 0x40);//AGC setting
    QND_WriteReg(0x2d, 0xD6);//notch filter threshold adjusting
    QND_WriteReg(0x43, 0x10);//notch filter threshold enable 
    if(qnd_ChipID == CHIPSUBID_QN8035A1)
    {
		 //QNF_SetRegBit(0x47,0x0C,0x08); 
    }
    //these variables are used in QNF_SetCh() function.
    qnd_R16 = QND_ReadReg(0x16);
    qnd_R17 = QND_ReadReg(0x17);
    qnd_R46 = QND_ReadReg(0x46);

    
#else
	QND_WriteReg(0x00, 0x81);
	QND_Delay(10);

	/*********User sets chip working clock **********/
	//Following is where change the input clock wave type,as sine-wave or square-wave.
	//default set is 32.768KHZ square-wave input.
	//QND_WriteReg(0x01,QND_SINE_WAVE_CLOCK);
	//Following is where change the input clock frequency.
	//QND_WriteReg(XTAL_DIV0, QND_XTAL_DIV0);
	//QND_WriteReg(XTAL_DIV1, QND_XTAL_DIV1);
	//QND_WriteReg(XTAL_DIV2, QND_XTAL_DIV2);
	/********User sets chip working clock end ********/
    //QNF_SetRegBit(0x01,0x80,QND_SINE_WAVE_CLOCK);
    QND_WriteReg(0x15, 0xDC);  //QND_XTAL_DIV0);
    QND_WriteReg(0x16, 0x02);//QND_XTAL_DIV1);
    QND_WriteReg(0x17, 0x54);//QND_XTAL_DIV2);  

	QND_WriteReg(0x54, 0x47);//mod PLL setting
	//select SNR as filter3,SM step is 2db
	QND_WriteReg(0x19, 0xc4);
	QND_WriteReg(0x40,0x70);	//set SNR as SM,SNC,HCC MPX
	QND_WriteReg(0x33, 0x9e);//set HCC and SM Hystersis 5db
	QND_WriteReg(0x2d, 0xd6);//notch filter threshold adjusting
	QND_WriteReg(0x43, 0x10);//notch filter threshold enable
	QND_WriteReg(0x47,0x39);
	//QND_WriteReg(0x57, 0x21);//only for qn8035B test
	//enter receiver mode directly
	QND_WriteReg(0x00, 0x11);
	//Enable the channel condition filter3 adaptation,Let ccfilter3 adjust freely
	QND_WriteReg(0x1d,0xa9);	
	QND_WriteReg(0x4f, 0x40);//dsiable auto tuning
	QND_WriteReg(0x34,SMSTART_VAL); ///set SMSTART
	QND_WriteReg(0x35,SNCSTART_VAL); ///set SNCSTART
	QND_WriteReg(0x36,HCCSTART_VAL); ///set HCCSTART
	QNF_SetMute(1);

#endif
    return EPDK_OK;
}


#ifdef QN8035_ONLY_FM
__s32  fm_init(void)//-----------ok
{
    __s32 i;
	__s32 tmp;
	__s32 ret;
/*    for(i=0; i<100; i++)
    {
		QND_WriteReg(0x07,i);

		tmp = QND_ReadReg(0x07);
		__wrn("i  = %d   tmp = %d \n",i, tmp);

	}*/
	
	ret = eLIBs_fioctrl(fiic, TWI_SET_SCL_CLOCK, 0, (void *)TWI_SCLK_100KHZ);	
	
	if(ret == EPDK_OK)
	{
		__msg("set iic clock as 100Khz successfully\n");
	}
	esKRNL_TimeDly(1);

    QND_Init();

	//QND_SetSysMode(QND_MODE_FM|QND_MODE_RX);
	QND_WriteReg(0x00, 0x11);  //QND_XTAL_DIV0);  //fm|rx mode
	///////QND_TuneToCH(8980);
	//qn8035_set_vol(47);//0-16
	QND_RXConfigAudio(QND_CONFIG_VOLUME,47);
	//fm_mute(DRV_FM_VOICE_OFF);
	QNF_SetMute(0);
	//while(1)
	__wrn(".......now  flag = %d ..\n",0);

/*
	if(flag)
	{
	    g_QNA_AirAudio_Flag = 1;
		if(g_QNA_AirAudio_Flag) {
			 g_AAEnable = EPDK_TRUE;		 // air audio 功能打开
				 //g_AAMode = AA_MODE_WS;	   // 无线音箱模式
			   g_AAMode = AA_MODE_WM;	  // 无线麦克风
				 g_fsm = FSM_CCA;
				 g_exitflag =0;
				 __wrn("create QNA_AirAudio_Thread;;;;;;;\n");
			  QNA_AirAudio_tid = esKRNL_TCreate(QNA_AirAudio_Thread, 0, 0x800, KRNL_priolevel6);	
		}
	}
	else
	*/
	{
		g_QNA_AirAudio_Flag = 0;
	}
	

    return EPDK_OK;
}
#else

//flag = 1:air audio  ; 0:fm
__s32  fm_init(__s32 flag)//-----------ok
{
    __s32 i;
	__s32 tmp;
	__s32 ret;
/*    for(i=0; i<100; i++)
    {
		QND_WriteReg(0x07,i);

		tmp = QND_ReadReg(0x07);
		__wrn("i  = %d   tmp = %d \n",i, tmp);

	}*/
	
	ret = eLIBs_fioctrl(fiic, TWI_SET_SCL_CLOCK, 0, (void *)TWI_SCLK_100KHZ);	
	
	if(ret == EPDK_OK)
	{
		__msg("set iic clock as 100Khz successfully\n");
	}
	esKRNL_TimeDly(1);

    QND_Init();

	//QND_SetSysMode(QND_MODE_FM|QND_MODE_RX);
	QND_WriteReg(0x00, 0x11);  //QND_XTAL_DIV0);  //fm|rx mode
	///////QND_TuneToCH(8980);
	//qn8035_set_vol(47);//0-16
	QND_RXConfigAudio(QND_CONFIG_VOLUME,47);
	//fm_mute(DRV_FM_VOICE_OFF);
	QNF_SetMute(0);
	//while(1)
	__wrn(".......now  flag = %d ..\n",flag);

	if(flag)
	{
	    g_QNA_AirAudio_Flag = 1;
		if(g_QNA_AirAudio_Flag) {
			 g_AAEnable = EPDK_TRUE;		 // air audio 功能打开
				 //g_AAMode = AA_MODE_WS;	   // 无线音箱模式
			   g_AAMode = AA_MODE_WM;	  // 无线麦克风
				 g_fsm = FSM_CCA;
				 g_exitflag =0;
				 __wrn("create QNA_AirAudio_Thread;;;;;;;\n");
			  QNA_AirAudio_tid = esKRNL_TCreate(QNA_AirAudio_Thread, 0, 0x800, KRNL_priolevel6);	
		}
	}
	else
	{
		g_QNA_AirAudio_Flag = 0;
	}
	

    return EPDK_OK;
}
#endif
__s32  fm_exit(void)//--------------------ok
{
	//__u8 Data=0x21;
	//fm_write_data(0x00,&Data,1);

    g_exitflag=1;
    QND_Delay(100);
	__msg("g_QNA_AirAudio_Flag = %d,QNA_AirAudio_tid =%d\n ",g_QNA_AirAudio_Flag,QNA_AirAudio_tid);
    if( g_QNA_AirAudio_Flag && QNA_AirAudio_tid )	 {
		
	     /*if(esKRNL_TDelReq(QNA_AirAudio_tid) != OS_TASK_DEL_REQ)      {     		      	
		       esKRNL_TDel(QNA_AirAudio_tid);
			   QNA_AirAudio_tid = 0;
	     }*/


		while( esKRNL_TDelReq(QNA_AirAudio_tid) != OS_TASK_NOT_EXIST )
		{
			__here__			
			esKRNL_TimeDly(5);
			//esKRNL_TimeDlyResume(anole_core.maintask);
		}
		QNA_AirAudio_tid = 0;


		 
    }		 
    QND_SetSysMode(QND_MODE_SLEEP);
    g_QNA_AirAudio_Flag=0;

	
	QND_WriteReg(0x00, 0x21);
	esKRNL_TimeDly(1);
    return EPDK_OK;
}


/**********************************************************************
void QND_SetSysMode(UINT16 mode)
***********************************************************************
Description: Set device system mode(like: sleep ,wakeup etc) 
Parameters:
mode:  set the system mode , it will be set by  some macro define usually:

SLEEP : set chip to sleep mode
WAKEUP: wake up chip 
RX:     set chip work on RX mode
Return Value:
None     
**********************************************************************/
void QND_SetSysMode(UINT16 mode)  
{    
    UINT8 val;
    switch(mode)        
    {        
        case QND_MODE_SLEEP:                       //set sleep mode    
            QNF_SetRegBit(REG_DAC, 0x08, 0x00);    //make sure Power down control by FSM control
            QNF_SetRegBit(SYSTEM1, STNBY_RX_MASK, STNBY_MODE);         
            break;        
        case QND_MODE_WAKEUP:                      //set wakeup mode        
            QNF_SetRegBit(REG_DAC, 0x08, 0x00);    //make sure Power down control by FSM control
            QNF_SetRegBit(SYSTEM1, STNBY_RX_MASK, RX_MODE);    
            QNF_SetMute(1);
            QND_Delay(SLEEP_TO_WAKEUP_DELAY_TIME); //avoid noise from sleep to wakeup mode during.
            QNF_SetMute(0);        
            break; 
        default:    
            val = (UINT8)(mode >> 8);        
            if (val)
            {
                val = val >> 3;
                if(val&0x10)
                    // set to new mode if it's not same as old
                    if((QND_ReadReg(SYSTEM1) & STNBY_RX_MASK) != val)
                    {
                        QNF_SetMute(1);
                        QNF_SetRegBit(SYSTEM1, STNBY_RX_MASK, val); 
                    }
            }    
            break;        
    }    
}



/**********************************************
* function:      fm_auto_search
*
* description:   自动搜索，支持向上搜索和向下搜索
*
* notes:         搜索到一个频点后退出，返回值是一个XX.X MHz单位
*
**********************************************/
	/***********************************************************************
	void QND_RXSetTH(UINT8 th)
	***********************************************************************
	Description: Setting the threshold value of automatic scan channel 
	th:
	  Setting threshold for quality of channel to be searched,
	  the range of th value:CCA_SENSITIVITY_LEVEL_0 ~ CCA_SENSITIVITY_LEVEL_9	
	Return Value:
	  None
	***********************************************************************/
	void QND_RXSetTH(UINT8 th) 
	{	
		UINT8 rssiTH;
		UINT8 snrTH;
		UINT16 rssi_snr_TH;
		UINT16 rssi_snr_TH_tbl [10] = { CCA_SENSITIVITY_LEVEL_0,CCA_SENSITIVITY_LEVEL_1,
										CCA_SENSITIVITY_LEVEL_2,CCA_SENSITIVITY_LEVEL_3,
										CCA_SENSITIVITY_LEVEL_4,CCA_SENSITIVITY_LEVEL_5,
										CCA_SENSITIVITY_LEVEL_6,CCA_SENSITIVITY_LEVEL_7,
										CCA_SENSITIVITY_LEVEL_8,CCA_SENSITIVITY_LEVEL_9 								   
									  };//越大越难收到台
									  
		rssi_snr_TH = rssi_snr_TH_tbl[th];
		rssiTH = (UINT8) (rssi_snr_TH >> 8);
		snrTH = (UINT8) (rssi_snr_TH & 0xff);
		QNF_SetRegBit(REG_REF,ICPREF,0x0A);
		QNF_SetRegBit(GAIN_SEL,0x08,0x08);//NFILT program is enabled 
		//selection filter:filter3
		QNF_SetRegBit(CCA1,0x30,0x30);
		//Enable the channel condition filter3 adaptation,Let ccfilter3 adjust freely  
		QNF_SetRegBit(SYSTEM_CTL2,0x40,0x00);
		QND_WRITE(PLT1,0x00);  
		QND_WRITE(CCA_CNT1,0x00);
		QNF_SetRegBit(CCA_CNT2,0x3F,0x03);//01~05   越大越容易收到台
		//selection the time of CCA FSM wait SNR calculator to settle:20ms
		//0x00: 	20ms(default)
		//0x40: 	40ms
		//0x80: 	60ms
		//0xC0: 	100m
		QNF_SetRegBit(CCA_SNR_TH_1 , 0xC0, 0xc0);  
		//selection the time of CCA FSM wait RF front end and AGC to settle:20ms
		//0x00: 	10ms
		//0x40: 	20ms(default)
		//0x80: 	40ms
		//0xC0: 	60ms							
		QNF_SetRegBit(CCA_SNR_TH_2, 0xC0, 0xc0);  
		QNF_SetRegBit(CCA, 0x3F, 0);  //setting RSSI threshold for CCA
		QNF_SetRegBit(CCA_SNR_TH_1 , 0x3F, 0x08); //setting SNR threshold for CCA
	}


/***********************************************************************
UINT8 QND_RXValidCH(UINT16 freq, UINT8 step);
***********************************************************************
Description: to validate a ch (frequency)(if it's a valid channel)
Freq: specific channel frequency, unit: 10Khz
  e.g. 108.00MHz will be set to 10800.
Step:  
  FM:
  QND_FMSTEP_100KHZ: set leap step to 100kHz
  QND_FMSTEP_200KHZ: set leap step to 200kHz
  QND_FMSTEP_50KHZ:  set leap step to 50kHz
Return Value:
  0: not a valid channel
  1: a valid channel at this frequency
***********************************************************************/
UINT8 QND_RXValidCH(UINT16 freq, UINT8 step) 
{
    UINT8 regValue;
    UINT8 timeOut = CCA_TIME_OUT;
    UINT8 isValidChannelFlag = 0; 
    UINT8 snr;
    UINT8 readCnt=0,stereoCount=0;

    QNF_ConfigScan(freq, freq, step);
	//enter CCA mode,channel index is decided by internal CCA
    QNF_SetRegBit(SYSTEM1,RXCCA_MASK,RX_CCA);
    do
    {
        regValue = QND_ReadReg(SYSTEM1);
        QND_Delay(5);   //delay 5ms
        timeOut--;
        //TRACE("CHSC:0x%02x timeOut:%d \n",regValue&CHSC,timeOut);
    } while((regValue & CHSC) && timeOut);//when seeking a channel or time out,be quited the loop
    //read out the rxcca_fail flag of RXCCA status 
    isValidChannelFlag = QND_ReadReg(STATUS1) & RXCCA_FAIL ? 0:1;
#if 1
    if(isValidChannelFlag)    
    {
        snr = QND_ReadReg(SNR); 
        QND_Delay(CCA_PILOT_READ_DELAY);        
        if(snr <= CCA_PILOT_SNR_FILTER)  
        {
           isValidChannelFlag = 0;  
           for(readCnt=0;readCnt<CCA_PILOT_READ_COUNT;readCnt++)
            {
                QND_Delay(CCA_PILOT_READ_COUNT_DELAY);
                stereoCount += ((QND_ReadReg(STATUS1) & ST_MO_RX) ? 0:1); 	      
                if(stereoCount >= CCA_PILOT_TH) 
                {
                    isValidChannelFlag = 1;
                    break;
                }
            }           
        }
        //TRACE("freq:%d snr:%d stereoCount:%d \n",freq,snr,stereoCount);
    }
#endif
    if(isValidChannelFlag)
    {
	  	//if(0==g_QNA_AirAudio_Flag)
		//wqx 20130329	QNF_SetMute(0);
		
         return 1;     
  
    }
    else
    {
        return 0;
    }
}


/***********************************************************************
UINT16 QND_RXSeekCH(UINT16 start, UINT16 stop, UINT16 step, UINT8 db, UINT8 up);
***********************************************************************
Description: Automatically scans the frequency range, and detects the 
first channel(FM, it will be determine by the system mode which set 
by QND_SetSysMode).
A threshold value needs to be passed in for channel detection.
Parameters:
start
Set the frequency (10kHz) where scan will be started,
e.g. 76.00MHz will be set to 7600.
stop
Set the frequency (10kHz) where scan will be stopped,
e.g. 108.00MHz will be set to 10800.
step
FM:
QND_FMSTEP_100KHZ: set leap step to 100kHz
QND_FMSTEP_200KHZ: set leap step to 200kHz
QND_FMSTEP_50KHZ:  set leap step to 50kHz
db:
Set threshold for quality of channel to be searched,
the range of db value:CCA_SENSITIVITY_LEVEL_0 ~ CCA_SENSITIVITY_LEVEL_19
up:
Set the seach direction :
Up;0,seach from stop to start
Up:1 seach from start to stop
Return Value:
The channel frequency (unit: 10kHz)
***********************************************************************/
UINT16 QND_RXSeekCH(UINT16 start, UINT16 stop, UINT8 step, UINT8 db, UINT8 up)  
{  
    UINT16 freq = start;
    UINT16 validCH;
    UINT8 stepValue;
    UINT16 pStart = start;
    UINT16 pStop = stop;    
    UINT8 regValue;
    UINT8 timeOut;
    UINT8 isValidChannelFlag = 0;
    UINT8 snr; 
    UINT8 readCnt=0,stereoCount=0;

    up=(start <= stop) ? 1 : 0;    
    stepValue = qnd_StepTbl[step];
    if(qnd_ChipID == CHIPSUBID_QN8035A0)
    {   
        QNF_SetMute(1);
        QND_RXSetTH(db);
        do
        {       
            validCH = QND_RXValidCH(freq, step);
            if (validCH == 0)
            {
                 if ((!up && (freq <= stop)) || (up && (freq >= stop)))
                 {
                   break;
                 }
                 else 
                 {
                    freq = freq + (up ? stepValue : -stepValue);
                 }
            }
        } while (validCH == 0);  
       // QND_TuneToCH(freq); 
    }
    else
    {

        QNF_SetMute(1);           
        QND_RXSetTH(db);
 
        do 
        {        
            QNF_ConfigScan(pStart, pStop, step);
        	//enter CCA mode,channel index is decided by internal CCA
            QNF_SetRegBit(SYSTEM1,RXCCA_MASK,RX_CCA);
            timeOut = CCA_TIME_OUT;
            do
            {
                regValue = QND_ReadReg(SYSTEM1);
                QND_Delay(5);   //delay 5ms
                timeOut--;
                //TRACE("CHSC:0x%02x timeOut:%d \n",regValue&CHSC,timeOut);
            } while((regValue & CHSC) && timeOut);//when it seeks a channel or the time is overtime,the loop is quited   
            isValidChannelFlag = QND_ReadReg(STATUS1) & RXCCA_FAIL ? 0:1;//reading the rxcca_fail flag of RXCCA status 
            freq = QNF_GetCh();
#if 0
            if(isValidChannelFlag)
            {   
                snr = QND_ReadReg(SNR);
                stereoCount = 0;
                QND_Delay(CCA_PILOT_READ_DELAY);        
                if(snr <= CCA_PILOT_SNR_FILTER)
                {
                   isValidChannelFlag = 0;  
                   for(readCnt=0;readCnt<CCA_PILOT_READ_COUNT;readCnt++)
                    {
                        QND_Delay(CCA_PILOT_READ_COUNT_DELAY);
                        stereoCount += ((QND_ReadReg(STATUS1) & ST_MO_RX) ? 0:1); 	      
                        if(stereoCount >= CCA_PILOT_TH) 
                        {
                            isValidChannelFlag = 1;
                            break;
                        }
                    }           
                }
                //TRACE("freq:%d snr:%d stereoCount:%d \n",freq,snr,stereoCount);
            }
#endif
			if(isValidChannelFlag == 0)
			{		
                pStart = freq + (up ? stepValue : -stepValue);                
            }      
        } 
        while ((isValidChannelFlag == 0) && (up ? (pStart<=pStop):(pStart>=pStop)));    

        if(isValidChannelFlag)
        {
           // QND_TuneToCH(freq);
           return freq;
        }
        else
        {
            freq = 0;
        }

    }
    return freq;
}

/************************************************************************
void QND_RXConfigAudio(UINT8 optiontype, UINT8 option )
*************************************************************************
Description: config audio 
Parameters:
  optiontype: option 
    QND_CONFIG_MONO; ‘option’control mono, 0: stereo receive mode ,1: mono receiver mode
    QND_CONFIG_MUTE; ‘option’control mute, 0:mute disable,1:mute enable    
    QND_CONFIG_VOLUME: 'option' control the volume gain,range : 0~47(-47db~0db)
    
Return Value:
    none
**********************************************************************/
void QND_RXConfigAudio(UINT8 optiontype, UINT8 option )  
{
    UINT8 regVal;
    switch(optiontype)
    {
        case QND_CONFIG_MONO:
            if (option)
                QNF_SetRegBit(SYSTEM1,RX_MONO,RX_MONO);
            else
                QNF_SetRegBit(SYSTEM1,RX_MONO,0x00);
            break;
        case QND_CONFIG_MUTE:
            if (option)
                QNF_SetMute(1); //mute audio 
            else
                QNF_SetMute(0); // disable mute audio 
            break;
        case QND_CONFIG_VOLUME:             
            if (option > 47)
            {
                option = 47;     
            }    
            if (option == 0)    //audio is muted when the volume is adjusted to minimum 
            {
                QNF_SetRegBit(VOL_CTL, 0x80, 0x80); //mute audio 
            }
            else
            {
                QNF_SetRegBit(VOL_CTL, 0x80, 0x00); // disable mute audio 
            }
            regVal = (UINT8)(option/6);  
            QNF_SetRegBit(VOL_CTL, 0x07, regVal);   //set analog gain
            regVal = (UINT8)(option%6);
            QNF_SetRegBit(VOL_CTL, 0x38, (UINT8)((5-regVal)<<3));   //set digital gain                    
            break;        
        default:
            break;
    }
}


/**********************************************************************
UINT8 QND_RDSEnable(UINT8 on) 
**********************************************************************
Description: Enable or disable chip to work with RDS related functions.
Parameters:
    on: 
        QND_RDS_ON:  Enable chip to receive/transmit RDS data.
        QND_RDS_OFF: Disable chip to receive/transmit RDS data.
Return Value:
           1: success
           0: failure
**********************************************************************/
UINT8 QND_RDSEnable(UINT8 on)  
{       
    if (on == QND_RDS_ON) 
    {
        QNF_SetRegBit(SYSTEM1,0x08,0x08);//enable RDS        
    } 
    else if (on == QND_RDS_OFF)
    {
        QNF_SetRegBit(SYSTEM1,0x08,0x00);// disable RDS       
    }
    else 
    {
        return 0;
    }
    return 1;
}

/**********************************************************************
void QND_RDSLoadData(UINT8 *rdsRawData, UINT8 upload)
**********************************************************************
Description: Load (TX) or unload (RX) RDS data to on-chip RDS buffer. 
             Before calling this function, always make sure to call the 
             QND_RDSBufferReady function to check that the RDS is capable 
             to load/unload RDS data.
Parameters:
  rdsRawData : 
    8 bytes data buffer to load (on TX mode) or unload (on RXmode) 
    to chip RDS buffer.
  Upload:   
    1-upload
    0--download
Return Value:
    None
**********************************************************************/
/*
void QND_RDSLoadData(UINT8 *rdsRawData, UINT8 upload)  
{
    UINT8 i;
    UINT8 temp;
    
    if (upload == 0) {       
        for (i = 0; i <= 7; i++) 
        {
            temp = QND_ReadReg(RDSD0 + i);
            rdsRawData[i] = temp;
        }
    }
}
*/
/**********************************************************************
UINT8 QND_RDSCheckBufferReady(void)
**********************************************************************
Description: Check chip RDS register buffer status before doing unload of
RDS data.

Parameters:
    None
Return Value:
    QND_RDS_BUFFER_NOT_READY: RDS register buffer is not ready to use.
    QND_RDS_BUFFER_READY: RDS register buffer is ready to use. You can now
    unload (for RX) data from RDS buffer
**********************************************************************/
UINT8 QND_RDSCheckBufferReady(void)  
{

    UINT8 val;
    UINT8 rdsUpdated;
    UINT8 timeOut = 10;
    
    rdsUpdated = QND_ReadReg(STATUS2);
    do 
    {
        val = QND_ReadReg(STATUS2)^rdsUpdated;
		QND_Delay(1);
		timeOut--;
    }while((!(val&RDS_RXUPD))&&timeOut);
    return timeOut ? QND_RDS_BUFFER_READY : QND_RDS_BUFFER_NOT_READY;   
}


/*
freq:
	
search_dir :
	DRV_FM_SEARCH_DOWN	向下搜索
	DRV_FM_SEARCH_UP		向上搜索


*/
//875
__s32  fm_auto_search(__s32 freq, __u32 search_dir)
{
    __u8 wfm_data[2] = {0, 0};
    __s32  flag, index = 0;
    __s8 rssi[4] = {0, 0, 0, 0};
	//DebugPrintf("--------fm_auto_search---------\n");
	//DebugPrintf("000--------freq=%d---------\n",freq);

	__u16 temp = (__u16)(freq / 100);
	
	if( g_QNA_AirAudio_Flag )  
	{
		return (temp * 100)|0xff000000;
	}

	
	while(1)
	{
		if(freq>108000||freq<87000)
			return EPDK_FAIL;
		if(search_dir==DRV_FM_SEARCH_DOWN)
		{
			fm_mute(DRV_FM_VOICE_OFF);
	    		QND_RXSetTH(1);
			if(QND_RXValidCH(freq,QND_FSTEP_100KHZ))
			{
				//DebugPrintf("--------111freq=%d---------\n",freq);
				return freq;
			}
			else
			{
				//DebugPrintf("--------222freq=%d---------\n",freq);
				return freq|0xff000000;
			}
		}
		else
		{
			fm_mute(DRV_FM_VOICE_OFF);
	    		QND_RXSetTH(1);
			if(QND_RXValidCH(freq/10,QND_FSTEP_100KHZ))
			{
				//DebugPrintf("--------333freq=%d---------\n",freq);
				return freq;
			}
			else
			{
				//DebugPrintf("--------444freq=%d---------\n",freq);
				return freq|0xff000000;
			}
		}
	}
  
}

/**********************************************
* function:      fm_play
*
* description:   根据输入的频率，播放此频率的
*                电台节目，即使没有节目只有
*                噪音，照样播出
*
* notes:         只返回成功
*
**********************************************/
#if 1
/**********************************************************************
void QND_TuneToCH(UINT16 ch)
**********************************************************************
Description: Tune to the specific channel. call QND_SetSysMode() before 
call this function
Parameters:
ch
Set the frequency (10kHz) to be tuned,
e.g. 101.30MHz will be set to 10130.
Return Value:
None
**********************************************************************/
void QND_TuneToCH(__s32 ch)  
{   
    UINT8 reg;
    UINT8 imrFlag = 0;
    
    QNF_SetRegBit(REG_REF,ICPREF,0x0A);
    QNF_RXInit();
    //QNF_SetMute(1);
    if((ch==8430)||(ch==7290)||(ch==6910))  
    {
    	imrFlag = 1;
    }
    else if(qnd_ChipID == CHIPSUBID_QN8035A1)
    { 
        if((ch==10710)||(ch==7860))
        {
            imrFlag = 1;
        }
    }
    if(imrFlag)
    {
        QNF_SetRegBit(0x01, 0x40, 0x40);
    }
    else
    {
        QNF_SetRegBit(CCA, IMR, 0x00);  
    }    
    QNF_ConfigScan(ch, ch, 1); 
    QNF_SetCh(ch);
    QNF_SetRegBit(0x00, 0x02, 0x02);  
    //Auto tuning
    QND_WriteReg(0x4F, 0x80);
    reg = QND_ReadReg(0x4F);
    reg >>= 1;
    QND_WriteReg(0x4F, reg);
    //if(qnd_ChipID == CHIPSUBID_QN8035A0)
    //{
    //    QND_Delay(CH_SETUP_DELAY_TIME+300);
    //}
    //else
    //{
    //    QND_Delay(CH_SETUP_DELAY_TIME);
    //}
    QNF_SetRegBit(REG_REF,ICPREF,0x00);
    QNF_SetMute(0);    
}
#else
void QND_TuneToCH(__s32 ch) 
{
    __u8 reg;

   // ch = ch * 10;
    //increase reference PLL charge pump current.
    QND_WriteReg(REG_REF,0x7A);
	
    /********** QNF_RXInit ****************/
    QND_WriteReg(0x1B,0x70);  //Let NFILT adjust freely
    //QNF_SetRegBit(0x2C,0x3F,0x12);  ///When SNR<ccth31, ccfilt3 will work
    //setting the threshold of Filter3 will be worked.
    QND_WriteReg(0x2C,0x52);	
    //QNF_SetRegBit(0x1D,0x40,0x00);  ///Let ccfilter3 adjust freely
    //QNF_SetRegBit(0x41,0x0F,0x0A);  ///Set a hcc index to trig ccfilter3's adjust
    QND_WriteReg(0x45,0x50);        ///Set aud_thrd will affect ccfilter3's tap number
    //QNF_SetRegBit(0x40,0x70,0x70);  ///snc/hcc/sm snr_rssi_sel; snc_start=0x40; hcc_start=0x30; sm_start=0x20
    QND_WriteReg(0x40,0x70);
    //QNF_SetRegBit(0x19,0x80,0x80);  ///Use SNR for ccfilter selection criterion
    //selecting SNR as filter3 filter condition
    //QND_WriteReg(0x19, 0xC2);
    //QNF_SetRegBit(0x3E,0x80,0x80);  ///it is decided by programming this register
    //QNF_SetRegBit(0x41,0xE0,0xC0);  ///DC notching High pass filter bandwidth; remove low freqency dc signals
    QND_WriteReg(0x41,0xCA);
    QND_WriteReg(0x34,SMSTART_VAL); ///set SMSTART
    QND_WriteReg(0x35,SNCSTART_VAL); ///set SNCSTART
    QND_WriteReg(0x36,HCCSTART_VAL); ///set HCCSTART
    /********** End of QNF_RXInit ****************/
	
    fm_mute(DRV_FM_VOICE_OFF);
#if USING_VALID_CH
    QNF_SetCh(ch);
#else
    QNF_SetCh(ch,ch);
#endif
    //enable CCA mode with user write into frequency
    //QND_WriteReg(0x00, 0x13);
    QND_WriteReg(0x00, 0x11); // by wth
#if 1
    //Auto tuning
    QND_WriteReg(0x4F, 0x80);
    reg = QND_ReadReg(0x4F);
    reg >>= 1;
    QND_WriteReg(0x4F, reg);
#endif
    ///avoid the "POP" noise.
    //Delay5Ms(CH_SETUP_DELAY_TIME/5);  //300ms
    esKRNL_TimeDly(30);
    ///decrease reference PLL charge pump current.
    QND_WriteReg(REG_REF,0x70);
	 fm_mute(DRV_FM_VOICE_ON);
}


#endif
__s32 fm_play(__s32 freq)
{//888  00
	freq=freq/10;
	//QND_RXSetTH(0);
	__msg("-----fm_play->freq=%d--------\n",freq);
	QND_TuneToCH(freq);
	/*if(QND_RXValidCH(freq/10))
		return freq;
	else
		return freq|0xff000000;*/
	QND_RXConfigAudio(QND_CONFIG_VOLUME,47);
	fm_mute(DRV_FM_VOICE_ON);
    return freq*10;
}
/**********************************************
* function:      fm_manual_search
*
* description:   手动搜索，使用fm_play
*                返回值是下一个搜索频率点
*
* notes:
*
**********************************************/
__s32 fm_manual_search(__s32 freq, __u32 search_dir)
#if 1//wqx 20130528
{

	__u16 temp = (__u16)(freq / 100);
	
	if( g_QNA_AirAudio_Flag )  
	{
		return (temp * 100)|0xff000000;
	}

	
	while(1)
	{
		if(freq>108000||freq<87000)
			return EPDK_FAIL;
		{
			fm_mute(DRV_FM_VOICE_OFF);
	    		QND_RXSetTH(0);
			if(QND_RXValidCH(freq/10,QND_FSTEP_100KHZ))
			{
				//DebugPrintf("--------333freq=%d---------\n",freq);
				return freq;
			}
			else
			{
				//DebugPrintf("--------444freq=%d---------\n",freq);
				return freq|0xff000000;
			}
		}
	}
  
}
#else
{
    __u16 temp = (__u16)(freq / 100);
	
    if( g_QNA_AirAudio_Flag )  
	{
		return (temp * 100)|0xff000000;
    }
    return fm_play(freq);
}
#endif
/**********************************************
* function:      fm_area_choose
*
* description:   地区选择，区别不同的起始和终止
*                频率
*
* notes:         输入正确地区返回成功，否则失败
*
**********************************************/                    //OK
__s32 fm_area_choose(__s32 area, void *pbuffer)
{
    __drv_fm_area_freq_t   *freq_range = (__drv_fm_area_freq_t *)pbuffer;

    if((area == DVR_FM_AREA_JAPAN) || (area == DRV_FM_AREA_CHN_SCHOOL))
    {
        rda5820_info.area_select = 1;
        rda5820_info.max_freq = FM_SEARCH_JAP_MAX_FREQ;
        rda5820_info.max_freq = FM_SEARCH_JAP_MIN_FREQ;

        freq_range->fm_area_min_freq = FM_SEARCH_JAP_MIN_FREQ;
        freq_range->fm_area_max_freq = FM_SEARCH_JAP_MAX_FREQ;
    }
    else if(area == DRV_FM_AREA_CHN_US_EUP)
    {
        rda5820_info.area_select = 0;
        rda5820_info.max_freq = FM_SEARCH_CHN_MAX_FREQ;
        rda5820_info.min_freq = FM_SEARCH_CHN_MIN_FREQ;

        freq_range->fm_area_min_freq = FM_SEARCH_CHN_MIN_FREQ;
        freq_range->fm_area_max_freq = FM_SEARCH_CHN_MAX_FREQ;
    }
    else
    {
        return EPDK_FAIL;
    }
	 rda5820_info.area_select = 0;
        rda5820_info.max_freq = FM_SEARCH_CHN_MAX_FREQ;
        rda5820_info.min_freq = FM_SEARCH_CHN_MIN_FREQ;

        freq_range->fm_area_min_freq = FM_SEARCH_CHN_MIN_FREQ;
        freq_range->fm_area_max_freq = FM_SEARCH_CHN_MAX_FREQ;
    return EPDK_OK;
}
/**********************************************
* function:      fm_stereo_choose
*
* description:   音质选择，立体声和普通声音
*
* notes:
*
**********************************************/                 //OK
__s32 fm_stereo_choose(__s32 audio_method)
{
    /*if(audio_method == DRV_FM_MONO)
    {
        rda5820_info.stereo_select = 1;
    }
    else if(audio_method == DRV_FM_STEREO)
    {
        rda5820_info.stereo_select = 0;
    }
    else
    {
        return EPDK_FAIL;
    }*/

    return EPDK_OK;
}

__s32 fm_pa_gain(__u8 pagain)
{
   
	return EPDK_OK;
}

__s32 fm_signal_gain(__u8 sigain)
{
   
	return EPDK_OK;
}

/**********************************************
* function:      fm_send
*
* description:   发射功能
*
* notes:
*
**********************************************/
__s32 fm_send_on(void)
{
	//fm_tx_init();

    return EPDK_OK;
}

__s32 fm_send(__s32 freq)//2======================================
{
 
    return EPDK_OK;
}

__s32 fm_send_off(void)
{
   
    return EPDK_OK;
}
/**********************************************
* function:      fm_mute
*
* description:   静音功能
*
* notes:
*
**********************************************/     //OK
__s32 fm_mute(__s32 voice_onoff)//2=================================
{
	__u8 data;
	if(voice_onoff==DRV_FM_VOICE_ON)
	{
		//data=0x10;//0x1b;
	   	//fm_write_data(REG_DAC,&data,1);
	   	QNF_SetMute(0);
	}
	else if(voice_onoff==DRV_FM_VOICE_OFF)
	{
		//data=0x1b;//0x10;
	   	//fm_write_data(REG_DAC,&data,1);
	   	QNF_SetMute(1);
	}
    return EPDK_OK;
}
/**********************************************
* function:      fm_signal_level
*
* description:   信号强度选择，要求信号强大越高，收到的电台越少
*                   要求信号强大越高，收到的电台越多，但无效电台也多
*
* notes:
*
**********************************************/             //OK
__s32 fm_signal_level(__s32 signal_level)
{
  
    return EPDK_OK;
}

#endif

#endif

