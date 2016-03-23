/************************************************Copyright(c)**************************************
**                                   Quintic(Beijing) Microelectronics Co,Ltd.
**                                   
**                                   http:\\www.quinticcorp.com 
**
**--------------------File Info----------------------------------------------------------------
** File Name:    qnws.c
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
//#include "qndriver.h"
//#include "qnio.h"
//#include "qnwsrx.h"
#include "qnws.h"
#include "RDA_FMG1208_drv.h"
#if(FM_MODULE_TYPE == FM_MODULE_TYPE_G1208)

unsigned char g_AAEnable = FALSE; // air audio enable
unsigned char g_AAMode = AA_MODE_WS; // default wireless speaker mode
unsigned char g_fsm = FSM_CCA;

unsigned char g_mute_st;
unsigned char g_currstate = STATE_MUTE;
unsigned short g_mutecount = 0;
unsigned char g_mutef = TRUE;
unsigned short g_fmFrequency;
unsigned char g_noisefloor;
unsigned char g_mutebyrds = FALSE;
unsigned char g_peakcount = 0;
unsigned char g_audiostate = 0;       // 0: audio is playing, 1: audio pause

unsigned short g_maxfreq = 0;         //store the freqency with max RSSI
unsigned char  g_maxrssi = 0;         //store the right channel's rssi value.

unsigned char g_exitflag = 0;         // 1: exit from while(g_AAEnable==TRUE). 0: not exit

unsigned char rds_data[8];            //store the rds registers' value
unsigned char button_flag = 0;        //button press flag,0=not press,1=press
unsigned char toggle_flag = 0;        //a flag indicate system received a rds after recived a button information 


unsigned char g_increase_flag = 0;    //a flag set 1,when the system store a increase number already.
unsigned char increase_store[2];      //a array store increase number, increase_store[0]=rds[2]=increase_low_8-bit
                                      //increase_store=rds[3]=increase_high_8-bit

__s32 QNA_global_init(void)
{
    __s32 i = 0;
									   
																			   
	g_currstate = STATE_MUTE; 												   
	g_mutecount = 0;															   
	g_mutef = TRUE;															   
	g_fmFrequency = 0;															   
	g_noisefloor = 0; 															   
	g_mutebyrds = FALSE;														   
	g_peakcount = 0;															   
	g_audiostate = 0; 	  // 0: audio is playing, 1: audio pause			   
																			   
	g_maxfreq = 0;		 //store the freqency with max RSSI 				   
	g_maxrssi = 0;		 //store the right channel's rssi value.			   
																			   
	g_exitflag = 0;		  // 1: exit from while(g_AAEnable==TRUE). 0: not exit 

	for(i=0; i<8; i++)
	{
		rds_data[i]= NULL;			  //store the rds registers' value	
	}
				   
	button_flag = 0;		  //button press flag,0=not press,1=press			   
	toggle_flag = 0;		  //a flag indicate system received a rds after recived
																			   
																			   
	g_increase_flag = 0;	  //a flag set 1,when the system store a increase numbe
	for(i=0; i<2; i++)
	{
		increase_store[i]= NULL;			  //a array store increase number, increase_store[0]=rd
	}	
		 
									//increase_store=rds[3]=increase_high_8-bit

    return EPDK_OK;									

}

unsigned char QNA_AudioDetecting(void)
{
    unsigned char peak;

    peak = QND_ReadReg(0x44);
    if (peak >= 4) 
    {
        g_peakcount = 0;
        g_audiostate = 0;
    }
    else if (peak < 2)
    {
        if (g_peakcount >= PEAK_LOW_COUNT) 
        {
            g_audiostate = 1;
        }
        else 
        {
            g_peakcount++;
        }
    }
    else 
    {
        g_peakcount = 0;
    }

    //TRACE("%d \n", peak);
    return g_audiostate;
}

unsigned char QNA_SNRCheck(void)
{
    unsigned char snr;
    unsigned char rssi;
    unsigned char ret;

    snr = QND_ReadReg(0x02);
    if ((snr >= MUTE_SNR_TH + 5) && (snr < 66)) 
    {
        ret = SNR_GOOD;
    }
    else if (snr < MUTE_SNR_TH)
    {

        rssi = QND_ReadReg(0x03);

        if ((rssi < g_noisefloor + SHUT_RSSI_TH) && (snr < SHUT_SNR_TH))
        {
            ret = SNR_SHUT;
            return ret;
        }

        if (rssi >= g_noisefloor + MUTE_RSSI_TH + 5) 
        {
            ret = SNR_GOOD;    
        }
        else if ((rssi >= g_noisefloor + SHUT_RSSI_TH) && (rssi < g_noisefloor + MUTE_RSSI_TH))
        {
            ret = SNR_BAD;
        }
        else 
        {
            ret = SNR_HOLD;
        }        
    }
    else 
    {
        ret = SNR_HOLD;
    }

    return ret;
}

unsigned char QNA_GetNosieFloor(void)
{
    unsigned char rssi;
    unsigned char step;
    unsigned char max = 0x00;
    unsigned char i;
    unsigned short add = 0;

    step =  (STOP_FREQ - START_FREQ) / 5;

    for (i = 0; i < 5; i++) 
    {
        rssi = QND_GetRSSI(START_FREQ + i*step);
        if (rssi > max) 
        {
            max = rssi;
        }
        add += rssi;
    }
    rssi = (add - max) / 4;
    QND_TuneToCH(g_fmFrequency);

	__wrn("g_fmFrequency = %d \n",g_fmFrequency);
    return rssi;

}

//In the MUTE CHECK state
//Get the button information from RDS register
//get the RDS registers' value
//with the rdsNerr help, get the right RDS information
//0		   : no rds information or all rds check error
//volume+  : 0x01  = IOB0=VOL+/INCREASE
//voluem-  : 0x02  = IOA3=VOL-
//next	   : 0x03  = IOB2=NEXT
//previous : 0x04  = IOB7=previous
//mode	   : 0x05  = IOB1=MODE/increase

unsigned char QNA_GetButton( void )
{
    unsigned char ret = 0;
    //unsigned char rds_data[8];
    //unsigned char i;
    unsigned char temp_status2 = 0;             // a temp value,stroe STATUS2 value
    if(g_fsm == FSM_MUTE_CHECK)                 //if the system is in mute check state,then you can receive button information
    {
        if(button_flag) 	                      //weather the RDS buffers are ready
        {
            button_flag = 0;	                  //clear the button_flag
            temp_status2 = QND_ReadReg(0x13);	  //read the STATUS2 register
            if(!(temp_status2 & 0x08))		      //block 1 is no error
            {
                if(rds_data[0] == rds_data[1])
                {
                    return rds_data[0];
                }
            }
            if(!(temp_status2 & 0x04))
            {
                if(rds_data[2] == rds_data[3])
                {
                    return rds_data[2];
                }
            }
            if(!(temp_status2 & 0x02))
            {
                if(rds_data[4] == rds_data[5])
                {
                    return rds_data[4];
                }
            }
			return 0;
                
        }
        else
        {
            return 0;
        }   
    }
    else
    {
        return 0;
    }
}




#ifndef QN8035_ONLY_FM

unsigned char QNA_AirAudio(void)
{
    unsigned char  snr;
    unsigned short ws_start, ws_stop;
    unsigned short ws_freq;
    unsigned char  rds_check_c;
    unsigned char  ws_status;
    unsigned char  temp_rssi = 0;	        // a temp  value store ws_freq's rssi
    unsigned char  valid_channelflag = 0;	//weather have a valid channel,1: have, 0 : no have
    unsigned char  i;
	unsigned char  increase_temp[2];        //read the increase number from rds
	
	#if ENABLE_PILOT_CHECK    
    unsigned char pilot;
	#endif

    ws_start = START_FREQ;
    ws_stop = STOP_FREQ;
    rds_check_c = 0;

    //QNA_global_init();
	
    while (g_AAEnable == TRUE)
    {
        QND_Delay(10);

	
		__wrn("enter QNA_AirAudio g_fsm = %d..............................\n",g_fsm);
        switch (g_fsm) 
        {
            case FSM_CCA: // step1 : find a real channel
                if(ws_start <= ws_stop)
                {
                    ws_freq = QND_RXSeekCH(ws_start, ws_stop, QND_FSTEP_200KHZ, 0, 1);
					__wrn("##############################################\n");
					__wrn("ws_freq = %d \n",(__s32)ws_freq);
					__wrn("##############################################\n");
                    //if (ws_freq >= START_FREQ) 
                    if(ws_freq != 0)
                    {       
                        g_fsm = FSM_RDS_CHECK;
                        rds_check_c = 0;
						__wrn("enable rds.........................\n");
                        QND_RDSEnable(1);
                        ws_start = ws_freq + 20;
                    }
                    else 
                    {
                        if(valid_channelflag == 0)
                        {
                            return 1;
                        }
                        else
                        {
                            //if the ws_start != START_FREQ, and don't find the channel,
                            //its meanning that it already has a channel  
                            ws_start = START_FREQ;		
                            g_fsm = FSM_TUNE_CHANNEL;	//all freqency has been scaned
                        }
	                
                    }
                }
                else	
                {
                    if(valid_channelflag == 0)	//no have a valid channel
                    {
                        __wrn("no have a valid channel.....\n");
                        return 1;
                    }
                    else
                    {
                        ws_start = START_FREQ;
                        g_fsm = FSM_TUNE_CHANNEL;	//all freqency has been scaned
                        __wrn("/all freqency has been scaned..........\n");
                    }
                }
            break;

            case FSM_RDS_CHECK: // step2: get rds and check the channel is valid or not
                if (QND_RDSCheckBufferReady()) 
                {
                    //
                    __wrn("...............FSM_RDS_CHECK...............\n");
                    ws_status = QNA_WSGetRdsInfo(ws_freq, DEFAULT_KEY, increase_temp);
					__wrn("ws_status1 = %d\n",ws_status);
                    if (ws_status & 0x80)
                    { 	
                      //
                        __wrn("g_increase_flag = %d \n",g_increase_flag);
                        //pairing successful
                       if(g_increase_flag == 1)
                        {
                            if((increase_temp[0] == increase_store[0]) && (increase_temp[1] == increase_store[1]))    //if the value read from rds equal the value stored
                            {
                                //if the increase numbers are right, store the current freqency,and tune to this freqency.
                                valid_channelflag = 1;
                                g_maxfreq = ws_freq;
                                g_fsm = FSM_TUNE_CHANNEL;
                            }
                            else
                            {
                                g_fsm = FSM_CCA;        //if the increase numbers is wrong,continue to cca .
                            }
                        
                        }
                        else
                        {
                            valid_channelflag = 1;	            //duiring the band , have a valid channel.
                            temp_rssi = QND_GetRSSI(ws_freq);
                            if(g_maxrssi < temp_rssi)           //if the new channel's rssi greater than the store vale,replace the store(freqency and rssi)
                            {
                                g_maxfreq = ws_freq;            //store the freqency with successful pairing .
                                g_maxrssi = temp_rssi;          //replace the rssi value	
                            
                                increase_store[0] = increase_temp[0];
                                increase_store[1] = increase_temp[1];
                                g_increase_flag = 1;            //if find a paired right channel,store its increase numbers and set the flag = 1
                            }
                            g_fsm = FSM_CCA;                    //go back ,continue to scan
                            break;
                        }
                    }
                }
                rds_check_c++;
                if (rds_check_c >= WAITTING4RDS) 
                {
                    g_fsm = FSM_CCA;
                }
            break;
            case FSM_TUNE_CHANNEL:	
                //set the 8035 tune to the channel store in g_maxfreq
			
                g_fmFrequency = g_maxfreq;
              //  QND_TuneToCH(g_fmFrequency);
			
                g_fsm = FSM_MUTE_CHECK;
                g_mutef = (ws_status & 0x40) == 0x40 ? TRUE : FALSE;
				__wrn("g_mutef = %d \n",g_mutef);
                g_mutebyrds = g_mutef;

			

                if (g_AAMode == AA_MODE_WS) 
                {
                    QNF_SetRegBit(0x00,0x04,0x00);  // stereo
                    QND_WriteReg(0x32, 0x80);       // disable SNC, disable SM, Enable HCC
                    QND_WriteReg(0x35, 0x80);
                    QND_WriteReg(0x31, 0x00);
                    QND_WriteReg(0x36, 0x10);
                    QND_WriteReg(0x43, 0x00);
                    g_noisefloor = QNA_GetNosieFloor();
                }
                else if (g_AAMode == AA_MODE_WM) 
                {
                    QNF_SetRegBit(0x00,0x04,0x00);  // stereo
                    QND_WriteReg(0x32, 0xB0);       // MAX SNC, disable SM, Enable HCC
                    QND_WriteReg(0x35, 0x80);
                    QND_WriteReg(0x31, 0xE0);
                    QND_WriteReg(0x36, 0x10);     
                    g_noisefloor = QNA_GetNosieFloor();
                }

				//qn8035_set_vol(47);//0-16
				//QND_RXConfigAudio(QND_CONFIG_VOLUME,47);
				//fm_mute(DRV_FM_VOICE_OFF);
				//QNF_SetMute(0);

				//while(1)
					__wrn("die...\n");;	
            break;
            case FSM_MUTE_CHECK:    // step3: work on wireless speaker mode, polling rds for mute/unmute

                #if ENABLE_PILOT_CHECK
                pilot = QND_ReadReg(STATUS1) & ST_MO_RX;    // 0: stereo, pilot exist

				__wrn("pilot = %d \n",pilot);
                if (pilot)                                  //发射端总是处于stereo模式，如果检测到pilot=1，则说明发射端关机或信号丢失， 
                {     
                   //此时，就不用在检测SNR，直接g_mutecount++,g_mutef=true即可。
                    __wrn("pilot = %d \n",pilot);
                    g_mutef = TRUE;
                    g_mutecount++;                          //信号丢失的情况下，置位g_mutef实现快速mute，g_mutecount累加，以实现等待一段时间后重新CCA
                }
                else 
                #endif
                {
                    //----------------------------------------------------------------------------------------------
                    //正常状态下，发射端是一直处于stereo模式，pilot信号一直存在，变量pilot=0所以if(pilot)是不成立的，
                    //要执行else中SNR检测。
                    //----------------------------------------------------------------------------------------------
                    snr = QNA_SNRCheck();
					__wrn("snr = %d \n",snr);
                    if (snr == SNR_GOOD)
                    {
                        g_mutecount = 0;

					     __wrn("check rds mute\n");
                        // check rds mute
                        if (QND_RDSCheckBufferReady()) 
                        {
                            __wrn("QND_RDSCheckBufferReady OK..........\n");
                            for(i=0;i<8;i++)
                            {
                                rds_data[i]=QND_ReadReg(0x0B+i);    //read rds registers
                            }
                            if((rds_data[6] == 0x55) && (rds_data[7] == 0xAA))  //confirm the rds is button information
                            {
                                if(toggle_flag == 0)
                                {
                                    button_flag = 1;	//set up the button_flag
                                    toggle_flag = 1;
                                }
							
                            }
                            else
                            {
                                toggle_flag = 0;	//clear the toggle_flag,
                                ws_status = QNA_WSGetRdsInfo(g_fmFrequency, DEFAULT_KEY, increase_temp);
								__wrn("ws_status = %d \n",ws_status);
                                if (ws_status & 0x80)
	                              { 
                                    //pairing successful
                                    g_mutef = (ws_status & 0x40) == 0x40 ? TRUE : FALSE;
									__wrn("g_mutef = %d \n",g_mutef);
                                    g_mutebyrds = g_mutef;
                                }
                            }
                        
                        }

						__wrn("g_AAMode = %d g_mutebyrds = %d \n",g_AAMode,g_mutebyrds);
                        if ((g_AAMode == AA_MODE_WM) && (g_mutebyrds == FALSE)) 
                        {
                            g_mutef = QNA_AudioDetecting();
                        }   
                    }
                    else if (snr == SNR_BAD)
                    {
                        g_mutecount++;
						__wrn("g_mutecount = %d \n",g_mutecount);
                    }
                    else if (snr == SNR_SHUT) 
                    {  
                        // mute immediately (eg: FMT power off)
                        g_mutef = TRUE;
                        g_mutecount++;
						__wrn(".............\n");
                    }
                }
            
                //----------------------------------------------------------------------------------------------
                //根据g_mutecount的值，来判断是否指示系统进入mute状态，g_mutef = true
                //或者指示系统reset，
                //----------------------------------------------------------------------------------------------
                if (g_mutecount >0 &&  g_mutecount%WAITTING4MUTE == 0 )          //累计SNR bad到一定数值之后，系统进入mute 
                {
                    __wrn("g_mutecount = %d \n",g_mutecount);
                    g_mutef = TRUE;
                }
                else if (g_mutecount >= WAITTING4RESET)    //并不是mute之后就马上开始CCA，而是等待一段时间后，若SNR仍然bad，则重新开始CCA， 
                {                                          //所以WAITTING4RESET的值会比WAITTING4MUTE的值大。
                    g_mutecount = 0;
                    g_fsm = FSM_CCA;

                    //if the channel becomes bad, the system return to CCA,
                    //then ,reset the ws_start and ws_stop to the original start freqency 
                    // and stop freqency.
                    //reset the all scan flag.
                    ws_start = START_FREQ;	
                    ws_stop = STOP_FREQ;
                    g_maxrssi = 0;			//clear the rssi value store in g_maxrssi
                    valid_channelflag = 0;	//scan again,clear flag
                }

			    __wrn("g_currstate = %d g_mutef = %d\n",g_currstate,g_mutef);
                //----------------------------------------------------------------------------------------------
                //根据当前状态标志位g_currstate和g_mutef的值，来配置当前系统应该怎么样配置。
                //----------------------------------------------------------------------------------------------
                if ((g_currstate == STATE_MUTE) && (g_mutef == FALSE))          //系统处于mute且g_mutef=false，则配置系统进入unmute 
                {
                    g_currstate = STATE_UNMUTE;
                    //pull_down_gpio(3, 4);//关功放	
                    QNF_SetMute(0);
					//esKRNL_TimeDly(10);
					//pull_up_gpio(3, 4);//关功放					
                    //DISABLE_MUTE();
                    
					__wrn(".......000\n");
                }
                else if ((g_currstate == STATE_UNMUTE) && (g_mutef == TRUE))    //系统处于unmute且g_mutef=true，则配置系统进入mute         
                {
                    //pull_down_gpio(3, 4);//关功放	
                    
                    g_currstate = STATE_MUTE;              //更新系统当前状态。                    
					//pull_down_gpio(3,4);
					                  
                    QNF_SetMute(1);
					//esKRNL_TimeDly(5);  
					//esKRNL_TimeDly(10);
					//pull_up_gpio(3, 4);//关功放		
					__wrn(".......1111\n");
                    //ENABLE_MUTE();
					g_noisefloor = QNA_GetNosieFloor();    //系统调至mute后，需要更新一次噪底。

                }
            
            break;
            default:
                g_fsm = FSM_CCA;
            break;
        }


        if (g_fsm == FSM_MUTE_CHECK) 
        {
            __wrn("return 0\n");
            return 0;
        }

        if(g_exitflag)
        {
            __wrn("return 1\n");
            return 1;
        }
        // add user code here, eg: exit by key
    
    }
    return 1;
}
#endif

#endif
