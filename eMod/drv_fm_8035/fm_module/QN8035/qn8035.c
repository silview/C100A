/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2009-2010, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : rda5807.c
* By      : ligang
* Version : V1.00
*********************************************************************************************************
*/
#include  "qn8035.h"
#include "epdk.h"
#include  "drv_fm_i.h"

#if(FM_MODULE_TYPE == FM_MODULE_TYPE_QN8035)

#define  FM_QN8035_ADDR 	(0x20>>1)
#define READ	1
#define WRITE	0

UINT8    qnd_PrevMode;
UINT8    qnd_Country  = COUNTRY_CHINA ;
UINT16   qnd_CH_START = 8700;
UINT16   qnd_CH_STOP  = 10800;
UINT8    qnd_CH_STEP  = 1;
UINT8  qnd_ChipID;
UINT8 qnd_IsQN8035B;

UINT8   qnd_AutoScanAll = 0;
UINT8   qnd_ChCount;
UINT8  qnd_div1; 
UINT8  qnd_div2;
UINT8  qnd_nco;
UINT16  qnd_ChList[QN_CCA_MAX_CH];
UINT8    qnd_StepTbl[3]={5,10,20};
QND_SeekCallBack qnd_CallBackFunc = 0;

#if  0
__bool OperationRDAFM_2w(__u8 addr,__u8 rw,  __u8 *data,__s32 n_byte)
{	
	__twi_dev_para_ex_t  piic;        //要写入TEA5767的数据  
	__s32 ret;

	//__u8 addr = 0xff;

	eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));
	
//	return 1;
	if(rw == 1)
	{
		piic.slave_addr_flag = TWI_SLAVE_ADDRESS_07BIT;
		piic.slave_addr 	 = (0x21>>1);
		piic.byte_addr_width = 0;
		piic.byte_addr		 = &addr;
		piic.data			 = data;
		piic.byte_count 	 = n_byte;
		
		ret = eLIBs_fioctrl(fiic, TWI_READ_SPEC_RS, 0, (void *)&piic);	

		if(ret != EPDK_OK)
		{
			__msg("fm i2c read error~\n");
		}

		return ret;
	}			
	else
	{
		piic.slave_addr_flag = TWI_SLAVE_ADDRESS_07BIT;
		piic.slave_addr 	 = (0x20>>1);
		piic.byte_addr_width = 0;
		piic.byte_addr		 = &addr;
		piic.data			 = data;
		piic.byte_count 	 = n_byte;
		
		ret = eLIBs_fioctrl(fiic, TWI_WRITE_SPEC_RS, 0, (void *)&piic);

		if(ret != EPDK_OK)
		{
			__msg("fm i2c write error~\n");
		}

		return ret;
	}		
	return 1;
}

#else
__bool ReadRDAFM(__u8 addr, __u8 *data,__s32 n_byte)
{
	__twi_dev_para_ex_t  piic;        //要写入TEA5767的数据  
	__s32 ret;

	__u8 addr1 = addr;

	eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));


	{
		//addr = 0x0a;
		//n_byte = 4;
		piic.slave_addr_flag = TWI_SLAVE_ADDRESS_07BIT;
		piic.slave_addr 	 = (0x20>>1);	//(0x22>>1);//(0x20>>1);
			piic.byte_addr_width = 1;
			piic.byte_addr		 = &addr1;
			piic.data			 = data;
			piic.byte_count 	 = n_byte;			
			ret = eLIBs_fioctrl(fiic, TWI_READ_SPEC_RS, 0, (void *)&piic);
			if(ret == EPDK_FAIL)
			{
				__msg("fm i2c read error~\n");
			}			
			
		}
	return ret;
}
__bool WriteRDAFM(__u8 addr,  __u8 *data,__s32 n_byte)
{	
	__twi_dev_para_ex_t  piic;        //要写入TEA5767的数据  
	__s32 ret;

	__u8 addr1 = addr;

	eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));
	
//	return 1;
	{
		{
			
			int i = 0;
			__u8 *pData = data;
			//for(i = 0; i < 10000; i++)
			{
				piic.slave_addr_flag = TWI_SLAVE_ADDRESS_07BIT;
				piic.slave_addr 	 = (0x20>>1);//(0x20>>1);//(0x22>>1);//
				piic.byte_addr_width = 1;
				piic.byte_addr		 = &addr1;
				piic.data			 = data;
				piic.byte_count 	 = n_byte;
				
				//for(i = 0; i < n_byte; i++, pData++)
				{
				//	__msg("write data is : %x\n", *pData);
				}
				
				pData = data;
				{		
					ret = eLIBs_fioctrl(fiic, TWI_WRITE_SPEC_RS, 0, (void *)&piic);

					if(ret != EPDK_OK)
					{
						__msg("fm i2c write error~\n");
					}
					else
					{
						__msg("fm i2c write succed~\n");
					}
					//esKRNL_TimeDly(5);
					//__msg("fm write data from 5807p\n");
				}
			}
			//__wait__;
		}
		
		return ret;
	}		
	return 1;
}

#endif
extern UINT8 QND_WriteReg(UINT8 adr, UINT8 value)
{
	__u8  ret;
	__u8  cnt =8;
	//fm_iic_read(adr,&reg);
     //  OperationRDAFM_2w(adr,READ,&reg,1);
       while(cnt)
       {
		ret = WriteRDAFM(adr,&value,1);
		if (ret == EPDK_OK)
		{
		      break;
		}
		cnt--;
       }

	//fm_iic_write(adr, value);
      // OperationRDAFM_2w(adr,WRITE,&value,1);
	return EPDK_OK;
}
extern UINT8 QND_ReadReg(UINT8 adr)
{
	__u8  reg;
	__u8  ret;
	__u8  cnt =8;
	//fm_iic_read(adr,&reg);
     //  OperationRDAFM_2w(adr,READ,&reg,1);
       while(cnt)
       {
		ret = ReadRDAFM(adr,&reg,1);
		if (ret == EPDK_OK)
		{
		      break;
		}
		cnt--;
       }
	
	return reg;
}
extern __bool GetFMID(void)
{
	unsigned char temp;
	temp=QND_ReadReg(0x06);

__msg("temp=%d\n",temp);
	if ((QND_ReadReg(0x06) & 0xff) == 0x84)
		{
			//gFM_DEV_HAVEMUN = 1;
			return 1;
		}
		else{
			//gFM_DEV_HAVEMUN = 0;
			return 0;
		}
		


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
    QNF_SetRegBit(0x1b,0x08,0x00);  //Let NFILT adjust freely
    QNF_SetRegBit(0x2c,0x3f,0x12);
    QNF_SetRegBit(0x1d,0x40,0x00);
    QNF_SetRegBit(0x41,0x0f,0x0a);
    QND_WriteReg(0x45,0x50);
    QNF_SetRegBit(0x3e,0x80,0x80);
    QNF_SetRegBit(0x41,0xe0,0xc0);
    if(qnd_ChipID == CHIPSUBID_QN8035A0)
    {
        QNF_SetRegBit(0x42,0x10,0x10);
    }
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
void QNF_SetMute(UINT8 On)
{
//eLIBs_printf("QNF_SetMute  On = %d\n",On);
	if(On)
	//if(0)
	{
//	eLIBs_printf("11111111111");
        if(qnd_ChipID == CHIPSUBID_QN8035A0)
        {
//        eLIBs_printf("2222222222",QND_ReadReg(REG_DAC));
            QNF_SetRegBit(REG_DAC, 0x0b, 0x0b);
        }
        else
        {//eLIBs_printf("33333333333");
            QNF_SetRegBit(0x4a, 0x20, 0x20);
        }
	}
	else
	{//eLIBs_printf("4444444444");
        if(qnd_ChipID == CHIPSUBID_QN8035A0)
        {//eLIBs_printf("555555555555");
            QNF_SetRegBit(REG_DAC, 0x0b, 0x00);
        }
        else
        {//eLIBs_printf("6666666666");
            QNF_SetRegBit(0x4a, 0x20, 0x00);
        }
		//eLIBs_printf("\nqnd_ChipID = %d",qnd_ChipID);
	}
}

/**********************************************************************
//置寄存器reg对应的bitMask中高电平的位为data_val相对应的位
//例如置寄存器reg第8为1,则bitMask为0x80,data_val为0x80即可
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
	ch	=  tStep ;
	tCh= QND_ReadReg(CH);	 
	ch = (ch<<8)+tCh;
	return CHREG2FREQ(ch);
}

/**********************************************************************
UINT8 QNF_SetCh(UINT16 freq)
**********************************************************************
Description: set channel frequency 

Parameters:
    freq:  channel frequency to be set
Return Value:
    1: success
**********************************************************************/
void QNF_SetCh(UINT16 freq) 
{
	// calculate ch parameter used for register setting
	UINT8 tStep;
	UINT8 tCh;
	UINT16 f; 
	UINT16 pll_dlt;
		
    if(freq == 8550)
    {

        QND_WriteReg(XTAL_DIV1, QND_XTAL_DIV1_855);
		QND_WriteReg(XTAL_DIV2, QND_XTAL_DIV2_855);
		QND_WriteReg(NCO_COMP_VAL, 0x69);
		freq = 8570;
    }
    else
    {
        QND_WriteReg(XTAL_DIV1, qnd_div1);
        QND_WriteReg(XTAL_DIV2, qnd_div2);
        QND_WriteReg(NCO_COMP_VAL, qnd_nco);
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
        eg: 7600 for 76.00MHz.
    stop
        Set the frequency (10kHz) where scan to be stopped,
        eg: 10800 for 108.00MHz
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
	esKRNL_TimeDly((ms/10)+1);
#if 0
 	UINT16 i,k;
    for(i=0; i<3000;i++) 
    {    
        for(k=0; k<ms; k++) 
        {

        }
    }
#endif
}

/**********************************************************************
UINT8 QND_GetRSSI(UINT16 ch)
**********************************************************************
Description:    Get the RSSI value
Parameters:
Return Value:
RSSI value  of the channel setted
**********************************************************************/
UINT8 QND_GetRSSI(UINT16 ch) 
{    
    QNF_SetRegBit(REG_REF,ICPREF,0x0a);
    QNF_ConfigScan(ch, ch, qnd_CH_STEP); 
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
        QND_Delay(ENABLE_2K_SPEED_PLL_DELAY);
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
void QN_ChipInitialization()
**********************************************************************
Description: chip first step initialization, called only by QND_Init()

Parameters:
None
Return Value:
None
**********************************************************************/
void QN_ChipInitialization()
{
/*
   while(1)
   {
      __u8  i;
      __u8  tmp;
	  
       for(i=0;i<100;i++)
       {
            QND_WriteReg(0x07,i);  
	     tmp = QND_ReadReg(0x07);
	     __msg("tmp=%d\n",tmp);	 
       }
   }
   */
    QND_WriteReg(0x00, 0x81);
    QND_Delay(10);
    qnd_ChipID = QND_ReadReg(CID1) & 0x03;
    qnd_IsQN8035B = QND_ReadReg(0x58) & 0x1f;
    /*********User sets chip working clock **********/
    if(qnd_IsQN8035B == 0x13)
    {
        QND_WriteReg(0x58,0x13);
        //Following is where change the input clock type.as crystal or oscillator.
        QNF_SetRegBit(0x58,0x80,QND_INPUT_CLOCK);            
    }
    //Following is where change the input clock wave type,as sine-wave or square-wave.
    QNF_SetRegBit(0x01,0x80,QND_SINE_WAVE_CLOCK);//QND_SINE_WAVE_CLOCK //QND_DIGITAL_CLOCK

    QND_WriteReg(XTAL_DIV0, QND_XTAL_DIV0);
    QND_WriteReg(XTAL_DIV1, QND_XTAL_DIV1);
    QND_WriteReg(XTAL_DIV2, QND_XTAL_DIV2);    
    QND_Delay(10);
    QND_WriteReg(0x54, 0x47);//mod PLL setting
    QND_WriteReg(SMP_HLD_THRD, 0xc4);//select SNR as filter3,SM step is 2db    
    QNF_SetRegBit(0x40,0x70,0x70);
    QND_WriteReg(0x33, 0x9c);//set HCC Hystersis to 5db
    QND_WriteReg(0x2d, 0xd6);//notch filter threshold adjusting
    QND_WriteReg(0x43, 0x10);//notch filter threshold enable    
    QNF_SetRegBit(SMSTART,0x7f,SMSTART_VAL);
    QNF_SetRegBit(SNCSTART,0x7f,SNCSTART_VAL);
    QNF_SetRegBit(HCCSTART,0x7f,HCCSTART_VAL);
    if(qnd_ChipID == CHIPSUBID_QN8035A1)
    {
		 QNF_SetRegBit(0x47,0x0c,0x08); 
    }   
    //these variables are used in QNF_SetCh() function.
    qnd_div1 = QND_ReadReg(XTAL_DIV1);
    qnd_div2 = QND_ReadReg(XTAL_DIV2);
    qnd_nco = QND_ReadReg(NCO_COMP_VAL);
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
UINT8 QND_Init()
{
    QN_ChipInitialization();
    return 1;
}

/**********************************************************************
//设置系统模式
//QND_MODE_SLEEP----standby模式 
//QND_MODE_WAKEUP------收台模式
**********************************************************************/
void QND_SetSysMode(UINT16 mode)
{	 
	UINT8 val;
	switch(mode)		
	{		 
		case QND_MODE_SLEEP:					   //set sleep mode    
			QNF_SetRegBit(REG_DAC, 0x08, 0x00);    //make sure Power down control by FSM control
			QNF_SetRegBit(SYSTEM1, STNBY_RX_MASK, STNBY_MODE);		   
			break;		  
		case QND_MODE_WAKEUP:					   //set wakeup mode		
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

/**********************************************************************
void QND_TuneToCH(UINT16 ch)
**********************************************************************
Description: Tune to the specific channel. call QND_SetSysMode() before 
call this function
Parameters:
ch
Set the frequency (10kHz) to be tuned,
eg: 101.30MHz will be set to 10130.
Return Value:
None
**********************************************************************/
void QND_TuneToCH(UINT16 ch) 
{   
    UINT8 reg;
    UINT8 imrFlag = 0;
    
    QNF_SetRegBit(REG_REF,ICPREF,0x0a);
    QNF_RXInit();
    QNF_SetMute(1);
    if(qnd_IsQN8035B == 0x13)
	{
		if((ch==7630)||(ch==8580)||(ch==9340)||(ch==9390)||(ch==9530)||(ch==9980)||(ch==10480))
		{
			imrFlag = 1;
		}
	}	
    else if((qnd_ChipID == CHIPSUBID_QN8035A0)||(qnd_ChipID == CHIPSUBID_QN8035A1))
    {
        if((ch==6910)||(ch==7290)||(ch==8430))
        {
            imrFlag = 1;
        }        
        else if(qnd_ChipID == CHIPSUBID_QN8035A1)
		{
			if((ch==7860) || (ch==10710))
            {
                imrFlag = 1;
            }
		}		
    }	
    if(imrFlag)
    {
        QNF_SetRegBit(CCA, IMR, IMR);
    }
    else
    {
        QNF_SetRegBit(CCA, IMR, 0x00);  
    }    
    QNF_ConfigScan(ch, ch, qnd_CH_STEP); 
    QNF_SetCh(ch);
    
    if((qnd_ChipID == CHIPSUBID_QN8035A0)||(qnd_ChipID == CHIPSUBID_QN8035A1))
    {
        QNF_SetRegBit(SYSTEM1, CHSC, CHSC);  
    }        
    else
    {
        QNF_SetRegBit(0x55, 0x80, 0x80);    
        QND_Delay(ENABLE_2K_SPEED_PLL_DELAY);
        QNF_SetRegBit(0x55, 0x80, 0x00);    
    }
    
    //Auto tuning
    QND_WriteReg(0x4f, 0x80);
    reg = QND_ReadReg(0x4f);
    reg >>= 1;
    QND_WriteReg(0x4f, reg);
    if(qnd_ChipID == CHIPSUBID_QN8035A0)
    {
        QND_Delay(CH_SETUP_DELAY_TIME+300);
    }
    else
    {
        QND_Delay(CH_SETUP_DELAY_TIME);
    }
    QNF_SetRegBit(REG_REF,ICPREF,0x00);
    QNF_SetMute(0);    
}

/**********************************************************************
void QND_SetCountry(UINT8 country)
***********************************************************************
Description: Set start, stop, step for RX and TX based on different
             country
Parameters:
country:
Set the chip used in specified country:
    CHINA:
    USA:
    JAPAN:
Return Value:
    None     
**********************************************************************/
void QND_SetCountry(UINT8 country)
{
	qnd_Country = country;
	switch(country)
	{
		case COUNTRY_CHINA:
			qnd_CH_START = 8750;
			qnd_CH_STOP = 10800;
			qnd_CH_STEP = 1;
			break;
		case COUNTRY_USA:
			qnd_CH_START = 8810;
			qnd_CH_STOP = 10790;
			qnd_CH_STEP = 2;
			break;
		case COUNTRY_JAPAN:
			qnd_CH_START = 7600;
			qnd_CH_STOP = 9000;
			qnd_CH_STEP = 1;
			break;
		default:
			break;		 
	}
}

/***********************************************************************
Description: set call back function which can be called between seeking 
channel
Parameters:
func : the function will be called between seeking
Return Value:
None
**********************************************************************/
void QND_SetSeekCallBack(QND_SeekCallBack func)
{
    qnd_CallBackFunc = func;
}

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
                                  };
                                  
    rssi_snr_TH = rssi_snr_TH_tbl[th];
    rssiTH = (UINT8) (rssi_snr_TH >> 8);
    snrTH = (UINT8) (rssi_snr_TH & 0xff);
    QND_WriteReg(0x4f, 0x00);//enable auto tunning in CCA mode
    QNF_SetRegBit(REG_REF,ICPREF,0x0a);
    QNF_SetRegBit(GAIN_SEL,0x08,0x08);//NFILT program is enabled 
	//selection filter:filter3
    QNF_SetRegBit(CCA1,0x30,0x30);
    //Enable the channel condition filter3 adaptation,Let ccfilter3 adjust freely  
    QNF_SetRegBit(SYSTEM_CTL2,0x40,0x00);
    QND_WRITE(CCA_CNT1,0x00);
    QNF_SetRegBit(CCA_CNT2,0x3f,0x03);
	//selection the time of CCA FSM wait SNR calculator to settle:20ms
	//0x00:	    20ms(default)
	//0x40:	    40ms
	//0x80:	    60ms
	//0xC0:	    100m
    QNF_SetRegBit(CCA_SNR_TH_1 , 0xc0, 0xc0);  
    //selection the time of CCA FSM wait RF front end and AGC to settle:20ms
    //0x00:     10ms
	//0x40:     20ms(default)
    //0x80:     40ms
	//0xC0:     60ms
    QNF_SetRegBit(CCA_SNR_TH_2, 0xc0, 0xc0);  
    QNF_SetRegBit(CCA, 0x3f, 0x0a);  //setting RSSI threshold for CCA
	QNF_SetRegBit(CCA_SNR_TH_1 , 0x3f, 0x08); //setting SNR threshold for CCA

}

/***********************************************************************
UINT16 QND_RXValidCH(UINT16 freq, UINT8 step);
***********************************************************************
Description: to validate a ch (frequency)(if it's a valid channel)
Freq: specific channel frequency, unit: 10Khz
  eg: 108.00MHz will be set to 10800.
Step:  
  FM:
  QND_FMSTEP_100KHZ: set leap step to 100kHz
  QND_FMSTEP_200KHZ: set leap step to 200kHz
  QND_FMSTEP_50KHZ:  set leap step to 50kHz
Return Value:
  0: not a valid channel
  other: a valid channel at this frequency
***********************************************************************/
UINT8 QND_RXValidCH(UINT16 freq, UINT8 step)
{
    UINT8 regValue;
    UINT8 timeOut = 40; //time out is 200ms
    UINT8 isValidChannelFlag = 0;
    QNF_ConfigScan(freq, freq, step);
    QNF_SetCh(freq);
	QND_WriteReg(0x4f, 0x80);
    regValue = QND_ReadReg(0x4f);
    regValue = (regValue >> 1);
    QND_WriteReg(0x4f, regValue);
    QNF_SetRegBit(SYSTEM1,RXCCA_MASK,RX_CCA);//enter channel scan mode,channel frequency is decided by internal CCA
    while(1)
    {
        regValue = QND_ReadReg(SYSTEM1);
        if((regValue & CHSC) == 0) break;
        QND_Delay(5);   //delay 5ms
        //if it was time out,chip would not normally work.
        if((timeOut--) == 0) 
        {
             __msg("timeOut=======0\n");
		return -1;
        }
    }
    //TRACE("CHSC:%d,timeOut:%d \n",regValue&CHSC,timeOut);
    //reading out the rxcca_fail flag of RXCCA status 
    isValidChannelFlag = ((QND_ReadReg(STATUS1) & RXCCA_FAIL) ? 0:1);
    if(isValidChannelFlag)    
    {
        if(qnd_CallBackFunc)
        qnd_CallBackFunc(freq, BAND_FM);
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
            else if(validCH == -1)
            {
                return -1;
            }
        } while (validCH == 0);  
        QND_TuneToCH(freq); 
    }
    else
    {
        if(qnd_AutoScanAll == 0)
        {
            QNF_SetMute(1);
            QND_RXSetTH(db);
        }
        do
        {
            QNF_ConfigScan(pStart, pStop, step);
        	//enter CCA mode,channel index is decided by internal CCA
            QNF_SetRegBit(SYSTEM1,RXCCA_MASK,RX_CCA);
            timeOut = 400;  //time out is 2S
           	while(1)
            {
                regValue = QND_ReadReg(SYSTEM1);
                //if it seeks a potential channel, the loop will be quited     
                if((regValue & CHSC) == 0) break;
                QND_Delay(5);   //delay 5ms
                if((timeOut--) == 0) return -1;
            } 
            //TRACE("CHSC:%d,timeOut:%d \n",regValue&CHSC,timeOut);            
            //reading out the rxcca_fail flag of RXCCA status 
            isValidChannelFlag = (QND_ReadReg(STATUS1) & RXCCA_FAIL ? 0:1);
            freq = QNF_GetCh();
			if(isValidChannelFlag == 0)
			{
                pStart = freq + (up ? stepValue : -stepValue);
            }
        }
        while ((isValidChannelFlag == 0) && (up ? (pStart<=pStop):(pStart>=pStop)));
        if(isValidChannelFlag)
        {
            if(qnd_CallBackFunc)
                qnd_CallBackFunc(freq, BAND_FM);
        }
        else
        {
            freq = 0;
        }
        if (qnd_AutoScanAll == 0)
        {
            QND_TuneToCH(freq ? freq: stop);
        }
    }
   //  DBG(("freq-->%D\n",freq));
    return freq;
}

/**********************************************************************
UINT8 QND_RXSeekCHAll(UINT16 start, UINT16 stop, UINT16 step, UINT8 db, UINT8 up)
**********************************************************************
Description:    Automatically scans the complete FM or AM band and detects 
            all the available  channels(AM or FM, it will be determine by 
            the workmode which set by QND_SetSysmode). A threshold value 
            needs to be passed in for the channel detection.
Parameters:
    start
        Set the frequency (10kHz) where scan will be started,
        eg: 76.00MHz will be set to 7600.
    stop
        Set the frequency (10kHz) where scan will be stopped,
        eg: 108.00MHz will be set to 10800.
    Step
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
  The channel count found by this function
  0: no channel found 
**********************************************************************/
UINT8 QND_RXSeekCHAll(UINT16 start, UINT16 stop, UINT8 step, UINT8 db, UINT8 up) 
{
	UINT16 freq;
	UINT16 temp;
	UINT8  stepValue;
	
	stop = stop > qnd_CH_STOP ? qnd_CH_STOP : stop;
	QNF_SetMute(1);    
	qnd_AutoScanAll = 1;
	qnd_ChCount = 0;
	up=(start<stop) ? 1 : 0;
	stepValue = qnd_StepTbl[step];
	QND_RXSetTH(db);
	freq=start;
	do
	{
		if(qnd_ChipID == CHIPSUBID_QN8035A0)
		{
			temp = QND_RXValidCH(freq, step);
            if(temp == -1)
            {
                break;
            }
            else if(temp == 1) 
            {
				qnd_ChList[qnd_ChCount++] = freq;
			}
			freq += (up ? stepValue : -stepValue);
		}
		else
		{
			temp = QND_RXSeekCH(freq, stop, step, db, up);
            if(temp == -1)
            {
                break;
            }
            else if(temp)  
			{
				qnd_ChList[qnd_ChCount++] = temp;
			}
			else
			{
				temp = stop;
			}
			freq = temp + (up ? stepValue : -stepValue);
		}  
	}
	while((up ? (freq<=stop):(freq>=stop)) && (qnd_ChCount < QN_CCA_MAX_CH));	
	QND_TuneToCH((qnd_ChCount >= 1)? qnd_ChList[0] : stop); 
	qnd_AutoScanAll = 0;
	return qnd_ChCount;
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
				if (option == 0)	//audio is muted when the volume is adjusted to minimum 
				{
					QNF_SetRegBit(VOL_CTL, 0x80, 0x80); //mute audio 
				}
				else
				{
					QNF_SetRegBit(VOL_CTL, 0x80, 0x00); // disable mute audio 
				}
				regVal = (UINT8)(option/6);  
				QNF_SetRegBit(VOL_CTL, 0x07, regVal);	//set analog gain
				regVal = (UINT8)(option%6);
				QNF_SetRegBit(VOL_CTL, 0x38, (UINT8)((5-regVal)<<3));	//set digital gain					  
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
void QND_RDSHighSpeedEnable(UINT8 on) 
**********************************************************************
Description: Enable or disable chip to work with RDS related functions.
Parameters:
  on: 
    1: enable 4x rds to receive/transmit RDS data.
    0: disable 4x rds, enter normal speed.
Return Value:
  none
**********************************************************************/
	void QND_RDSHighSpeedEnable(UINT8 on)
	{
		QNF_SetRegBit(INT_CTRL, 0x08, on?0x08:0x00);
	}

/***********************************
char QND_RDSModeDetect(void)       
************************************
Description: Automatic check the RDS mode which is 1kb/s or 4kb/s mode. 

Parameters:                       
None                              
Return Value:                     
1: 4kb/s RDS signal is detected   
0: 1kb/s RDS signal is detected   
-1: No RDS signal is detected     
************************************/

INT8 QND_RDSModeDetect(void)
{                                   
    INT8 i;                        
    UINT8 val;                        
    QND_RDSEnable(1);                 
    for (i=1; i>=0; i--)          
    {                               
        QND_RDSHighSpeedEnable(i);  
        QND_Delay(1500);            
        val=QND_RDSDetectSignal();  
        if (val & 0x10) return i;   
    }                                 
    QND_RDSEnable(0);                 
    return (INT8)EPDK_FAIL;                        
}                                     

/**********************************************************************
UINT8 QND_DetectRDSSignal(void)
**********************************************************************
Description: detect the RDSS signal .

Parameters:
    None
Return Value:
    the value of STATUS2
**********************************************************************/
	UINT8 QND_RDSDetectSignal(void)
	{
		UINT8 val = QND_ReadReg(STATUS2);	  
		return val;
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
	void QND_RDSLoadData(UINT8 *rdsRawData, UINT8 upload)
	{
		UINT8 i;
		UINT8 temp;
		{		
			for (i = 0; i <= 7; i++) 
			{
				temp = QND_ReadReg(RDSD0 + i);
				rdsRawData[i] = temp;
			}
		}
	}

/**********************************************************************
UINT8 QND_RDSCheckBufferReady(void)
**********************************************************************
Description: Check chip RDS register buffer status before doing load/unload of
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
	UINT8 timeOut = 100;
		
    rdsUpdated = QND_ReadReg(STATUS2);
    while(1)
    {
        val = QND_ReadReg(STATUS2)^rdsUpdated;
        if(val&RDS_RXUPD) return QND_RDS_BUFFER_READY;
    	QND_Delay(1);
    	if((timeOut--) == 0) return QND_RDS_BUFFER_NOT_READY;
	}

}
__s32  fm_init(void)
{
	__msg("------------------------------------\n");
	__msg("-----------<<FM INIT>>--------------\n");
	__msg("------------------------------------\n");

#if 1
{
	__s32 ret = eLIBs_fioctrl(fiic, TWI_SET_SCL_CLOCK, 0, (void *)TWI_SCLK_100KHZ);	
	
	if(ret == EPDK_OK)
	{
		__msg("set iic clock as 100Khz successfully\n");
	}
}	
	GetFMID();

	QND_Init();

	QND_SetCountry(COUNTRY_CHINA);



	//QND_SetSysMode(QND_MODE_WAKEUP);
	QND_SetSysMode(QND_MODE_FM|QND_MODE_RX);

	//QND_RXConfigAudio(QND_CONFIG_MONO,0);
	//QND_RXConfigAudio(QND_CONFIG_MUTE,0);
       //TunerVolume(gSys.PlayArg.Volume);
	//QND_RXConfigAudio(QND_CONFIG_VOLUME,47);

      //	tuner.CurFreq=10430;
	//QND_TuneToCH(10430);
#else
	QND_WriteReg(0x00, 0x81); 
	esKRNL_TimeDly(1);
	
    QNF_SetRegBit(0x01,0x80,QND_SINE_WAVE_CLOCK );
	
 	qnd_ChipID = QND_ReadReg(CID1) & 0x03;
	// change crystal frequency setting here
	QND_WriteReg(XTAL_DIV0, QND_XTAL_DIV0);
	QND_WriteReg(XTAL_DIV1, QND_XTAL_DIV1);
	QND_WriteReg(XTAL_DIV2, QND_XTAL_DIV2);	  
	esKRNL_TimeDly(1);  
	QND_WriteReg(0x54, 0x47);//mod PLL setting
	QND_WriteReg(0x19, 0x40);//AGC setting
	QND_WriteReg(0x2d, 0xD6);//notch filter threshold adjusting
	QND_WriteReg(0x43, 0x10);//notch filter threshold enable 
	__msg("qnd_ChipID = %x\n",qnd_ChipID);
	if(qnd_ChipID == CHIPSUBID_QN8035A1)
	{
		QNF_SetRegBit(0x47,0x0C,0x08); 
	}
	//these variables are used in QNF_SetCh() function.
	qnd_R16 = QND_ReadReg(0x16);
	__msg("qnd_R16=%d\n",qnd_R16);
	qnd_R17 = QND_ReadReg(0x17);
	__msg("qnd_R16=%d\n",qnd_R17);
	qnd_R46 = QND_ReadReg(0x46);
	__msg("qnd_R16=%d\n",qnd_R46);
	
	QND_SetSysMode(QND_MODE_FM|QND_MODE_RX);
	{
       __u8   status;
	  status = QND_ReadReg(STATUS1) ;
	__msg("status = %x\n",status);
	}
      QND_WriteReg(0x00, 0x11);//mod PLL setting
	fm_play(104300);
#endif
    return EPDK_OK;
}

__s32  fm_exit(void)
{
	__msg("------------------------------------\n");
	__msg("-----------<<FM EXIT>>--------------\n");
	__msg("------------------------------------\n");
	QND_SetSysMode(QND_MODE_SLEEP);
    return EPDK_OK;
}
/**********************************************
* function:      fm_auto_search
*
* description:   自动搜索，支持向上搜索和向下搜索
*
* notes:         搜索到一个频点后退出，返回值是一个XX.X MHz单位
*
**********************************************/
__s32  fm_auto_search(__s32 freq, __u32 search_dir)
{
	UINT16 freq_now,freq_sign;
	//__s32 sign;
	//sign = 1;
    __msg("^^^^^^freq = %d^^^^^^\n",freq);
	//if(freq < 104000)
		QND_RXSetTH(0);	//0~9//设置门限
	//else
	//{
	//	sign = 0;
		//QND_RXSetTH(1);
	//}
	freq_now = (UINT16)(freq/10);//8750;
	freq_now = freq_now - (freq_now%10);
	QNF_SetMute(1);
	
   	while(1)
    {
		if((freq_now <= (qnd_CH_START-60))||(freq_now >= qnd_CH_STOP))
			return EPDK_FAIL;
		__msg("~~~~~freq_now = %d\n",freq_now);
		freq_sign = QND_RXValidCH(freq_now,1);

		//eLIBs_printf("cunrent	freq_now = %d\n",freq_now);
		
		if(freq_sign != 0)
		{
			//QND_TuneToCH(freq_now);
			//__msg("■■■■freq_now = %d■■■■■\n",freq_now);
			//__msg("~~~~~freq_now = %d\n",(__s32)freq_now);
			QNF_SetMute(0);
			//eLIBs_printf("search succed	freq_now = %d\n",freq_now);
			return (__s32)(freq_now*10);
			
		}
/*		
#if 1//mhy
		else
		{
			QNF_SetMute(0);
			return (freq_now*10)|0xff000000;
		}
#else*///hkchange
		 if(search_dir == DRV_FM_SEARCH_DOWN)
        {
            freq_now -= 10;
				return (freq_now*10)|0xff000000;//hkchange
        }
        else
        {
            freq_now += 10;
				return (freq_now*10)|0xff000000;//hkchange
        }
//#endif   //hkchange
		//eLIBs_printf("cunrent	freq_now = %d\n",freq_now);
 	}
	QNF_SetMute(0);
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
__s32 fm_play(__s32 ch)
{
    ch = ch/10;
	__msg("-----------<<FM PLAY>>-------------\n");
	__msg("-------ch = %d------------------\n",ch);
	//QND_SetSysMode(QND_MODE_WAKEUP);
	ch = ch - (ch%10);
	
	QND_TuneToCH((UINT16)(ch)); 
	
    return EPDK_OK;
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
{
    return fm_play(freq);
}
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
	QND_SetCountry(COUNTRY_JAPAN);
	 freq_range->fm_area_min_freq = FM_SEARCH_JAP_MIN_FREQ;
        freq_range->fm_area_max_freq = FM_SEARCH_JAP_MAX_FREQ;
    }
    else if(area == DRV_FM_AREA_CHN_US_EUP)
    {
    	 QND_SetCountry(COUNTRY_CHINA);
        freq_range->fm_area_min_freq = FM_SEARCH_CHN_MIN_FREQ;
        freq_range->fm_area_max_freq = FM_SEARCH_CHN_MAX_FREQ;
    }
    else
    {
        return EPDK_FAIL;
    }

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
	__msg("-----------------------------------\n");
	__msg("-------<<FM STEREO CHOOSE>>--------\n");
	__msg("-----------------------------------\n");

    return EPDK_OK;
};

/**********************************************
* function:      fm_mute
*
* description:   静音功能
*
* notes:
*
**********************************************/     //OK
__s32 fm_mute(__s32 voice_onoff)
{
	__msg("-----------------------------------\n");
	__msg("-----------<<FM MUTE>>-------------\n");
	__msg("-----------------------------------\n");
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
	__msg("-----------------------------------\n");
	__msg("------<<FM SIGNAL LEAVEL>>---------\n");
	__msg("-----------------------------------\n");


    return EPDK_OK;
}
__s32  fm_send_on(void)
{
    return EPDK_FAIL;
}

__s32  fm_send(__s32 freq)
{
    return EPDK_FAIL;
}

__s32  fm_send_off(void)
{
    return EPDK_FAIL;
}

__s32  fm_pa_gain(__u8 pagain)
{
    return EPDK_FAIL;
}

__s32  fm_get_status(void)
{
    return FM_DRV_STATUS_RECEIVE;
}

#endif
