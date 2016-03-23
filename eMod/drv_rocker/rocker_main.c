/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           ir key driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : rocker_main.c
* By      : Jerry
* Version : V1.00
*********************************************************************************************************
*/

#include "drv_rocker_i.h"
#include "epdk.h"

__rk_private_data_set_t     rk_private;
static __s32 once_key=0;
static __s32 i=0;
static __s32 times=0;
static __s32 poweroff_count=0;
static __s32 pw_pin=0;
static __s32 action_flag=0, acc_debounce_cnt = 0;

static __u32 output_col = 0;
static __u32 output_col1 = 0;

 #define    C_ACC_DEBOUNCE_TIME         10                //10*60 ms,超过此时间，认为ACC断电

__s32	g_echo_level = 0;
__s32	g_echo_chk_flag = 0;			//控制 按键和混响旋钮 交替查询;		Ethan; 

typedef enum _echo_level{
	KR_ECHO_LV_0 = 0,
	KR_ECHO_LV_1,
	KR_ECHO_LV_2,
	KR_ECHO_LV_3,
	KR_ECHO_LV_4,
	KR_ECHO_LV_5,
	KR_ECHO_LV_6,
	KR_ECHO_LV_7
};

static user_gpio_set_t Input_1[1] =
{
	"input_1",//Pc13
	3,
	13,
	0,
	0,
	0,
	0
};
__hdle         h_input_1 = NULL;

static user_gpio_set_t Input_2[1] =
{
	"input_2",//Pc14
	3,
	14,
	0,
	0,
	0,
	0
};
__hdle         h_input_2 = NULL;

 __s32 get_key_type(void)// 0: yi first key 1:yx second key
 {
	__s32 ret;
	 __s32 key_type;
	ret = esCFG_GetKeyValue("user_name_show", "yx_key", &key_type, 1);
	return key_type;
 }
 __s32  read_key_value(__s32 *data, __u32 cnt)
{
  __s32 i,j,k,key_value;
   for(i=0;i<cnt-1;i++)
  for(j=0;j<cnt-1-i;j++)
  if(data[j]>data[j+1])
   {
     k=data[j];
     data[j]=data[j+1];
     data[j+1]=k;
    }
  key_value=data[1];
  return key_value;
}
/*
****************************************************************************************************
*
*             rk_default_cb
*
*  Description:
*       rk_default_cb
*
*  Parameters:
*
*  Return value:
*       none
*
****************************************************************************************************
*/
__s32  rk_default_cb(void * arg)             //RK的默认回调函数
{
    __ev_rk_msg_t * drv_rk_msg;
	__s32 x_value,y_value,value_key=0,cnt;
	__s32 value_x;
	drv_rk_msg=(__ev_rk_msg_t*) arg;
        x_value=drv_rk_msg->x_value;
	  y_value=drv_rk_msg->y_value;
      //__inf("x = %d , y = %d \n", x_value,y_value);
     if((x_value==2047)&&(y_value==2047)&&(once_key==0))
	 return 0;	
   else if(x_value!=2047)
    	{
    	//  __inf("x_value=%d\n",x_value);
    	 
	  if(rk_private.times<3)
	  {
	   rk_private.times++;
         rk_private.temp_x[i]=x_value;
	   i++;
	   __inf("x_value=%d\n",x_value);
	   }
	  else
	 {
	  rk_private.times=0;
	  value_x=read_key_value(rk_private.temp_x,3);
	   i=0;
	  __inf("value_x=%d\n",value_x);
         if((value_x>1)&&(value_x<30))
         value_key=7;//KPAD_CLK;
        else if((value_x>40)&&(value_x<130))
         value_key=7;//KPAD_MODE;
	  else if((value_x>180)&&(value_x<300))
	  value_key=7;//KPAD_NUM7;        //LOUD
	  else if((value_x>380)&&(value_x<510))
	  value_key=7;//KPAD_MOVIE;
	  else if((value_x>550)&&(value_x<700))
	  value_key=7;//KPAD_MUTE;
	  else if((value_x>750)&&(value_x<900))
	  value_key=7;//KPAD_EQ;
	  else if((value_x>950)&&(value_x<1300))
	  value_key=7;//KPAD_MUSIC;
         else 
	   return 0;
	  }
	}
	else if(y_value!=2047)
        {
         __inf("y_value=%d\n",y_value);
         if((y_value>1)&&(y_value<50))
	    value_key=7;//KPAD_NUM1;
          else if((y_value>70)&&(y_value<130))
	    value_key=7;//KPAD_NUM2;
	   else if ((y_value>150)&&(y_value<200))
	   value_key=7;//KPAD_NUM3;
	   else if ((y_value>220)&&(y_value<300))
	   value_key=7;//KPAD_NUM4;
	   else if ((y_value>330)&&(y_value<410))
	   value_key=7;//KPAD_NUM5;
	   else if ((y_value>430)&&(y_value<510))
	    value_key=7;//KPAD_NUM6;
	   else if((y_value>550)&&(y_value<700))
	  {
	  value_key=7;//KPAD_BAND;
	   //__inf("y_value=%d\n",y_value);
	   }
	   else if((y_value>750)&&(y_value<900))
	  {
	  value_key=7;//KPAD_PREV;
	// __inf("prev_value=%d\n",y_value);
	   }
	   else if((y_value>950)&&(y_value<1100))
	   {
	   value_key=7;//KPAD_NEXT;
	 // __inf("next_value=%d\n",y_value);
	   }
	   else if ((y_value>1150)&&(y_value<1390))
	   {
        //__inf("y_value=%d\n",y_value);
	   value_key=7;//KPAD_SEL;
	   	}
	   else
	   	return 0;
	   
	    }
	
	else
           value_key=0;
	     
	    if( value_key!=0)
	   {
	  esINPUT_SendEvent(rocker_drv.p_input_dev,     
		                      EV_KEY,
		                     value_key,
		                      1
		                      );				
																		
            esINPUT_SendEvent(rocker_drv.p_input_dev,
                              EV_SYN,
                              SYN_REPORT,
                              0
                              ); 
		     once_key=value_key;
                  value_key=0;
		  
	          return 0;
		}
	   else if(once_key!=0)
         {
	esINPUT_SendEvent(rocker_drv.p_input_dev,     
		                      EV_KEY,
		                     once_key,
		                      0
		                      );				
																		
            esINPUT_SendEvent(rocker_drv.p_input_dev,
                              EV_SYN,
                              SYN_REPORT,
                              0
                              ); 
                 once_key=0; 
		      
		//__inf("up---\n");
		}
    
    return 0;
}
__u8  scan_acc_pin(void)
{
    __s32 pin_status = 0;
    pin_status = esPINS_ReadPinData(pw_pin, NULL);
    if((pin_status)&&(!action_flag))
    {               
        acc_debounce_cnt++;
        if(acc_debounce_cnt >= C_ACC_DEBOUNCE_TIME)     //ACC断电超过DEBOUNCE_TIME，则认为关机
        {
            esKRNL_TimeDly(1);//yoursmile 2011_11_07  吾知点解
            __msg("send acc power off \n");
    		esINPUT_SendEvent(rocker_drv.p_input_dev,
    				              EV_KEY,
    				              KPAD_POWER,
    				              2
    				              );				
    																		
    		esINPUT_SendEvent(rocker_drv.p_input_dev,
    				              EV_SYN,
    				              SYN_REPORT,
    				              0
    				              );		
            
    		esKRNL_TimeDly(1);//yoursmile 2011_11_07  吾知点解   
    		
    		            
            
    		action_flag=1;
        }
    	//_inf("acc pin action-----\n");
    }
    else
    {
        acc_debounce_cnt = 0;
        action_flag = 0;
    }
return 1;

}

void rk_send_key_message(__s32 key_pad, __s32 key_status)
{
    esINPUT_SendEvent(rocker_drv.p_input_dev,     
                      EV_KEY,
                      key_pad,
                      key_status
                      );				
													
    esINPUT_SendEvent(rocker_drv.p_input_dev,
                      EV_SYN,
                      SYN_REPORT,
                      0
                      ); 
}

__s32 data_filter(__s32 *input, __s32  *output ,int arr_len)
{
__s32 tmp,i,j,half,rec,k=0;
__s32 *data;
 data=input;
 i=arr_len;
 for(i=0;i<arr_len-1;i++)
 for(j=0;j<arr_len-i-1;j++)
 {
  if(data[j]>data[j+1])
 {
  tmp=data[j];
  data[j]=data[j+1];
  data[j+1]=tmp;
  }
  }
  half=arr_len/2;
  for(i=0;i<arr_len;i++)
  {
  if(data[half]>data[i])
  rec=data[half]-data[i];
  else
  rec=data[i]-data[half];
  if(rec<30)
   {
   output[k]=data[i];
   k++;
   }
   
  }
  return k;
}

/*
****************************************************************************************************
*
*             rocker_data_task
*
*  Description:
*       rocker_data_task
*
*  Parameters:
*
*  Return value:
*       none
*
****************************************************************************************************
*/
// 0R 1K 2.2K 3.9K 5.6K 8.2K 12K
 __s32 get_x1_channe1_keypad(__s32 ad_data)//y1=
{
	int value_key = 0;
	if(ad_data > 2000)
	{
	    return 0;

	}

	if((ad_data>=100)&&(ad_data<300))////1
	{
	value_key=KPAD_NUM1;
	}
	else if((ad_data>=300)&&(ad_data<500))////1
	{
	value_key=KPAD_NUM2;
	}	
	else if((ad_data>=500)&&(ad_data<700))////1
	{
	value_key=KPAD_NUM3;
	}
	else if((ad_data>=700)&&(ad_data<900))////2
	{
	value_key=KPAD_NUM4;
	}
	else if((ad_data>=900)&&(ad_data<1100))////6
	{
	value_key=KPAD_NUM5;
	}
	else
	{
	    return 0;        	   
	}
	
	return value_key;
} 

 
 __s32 get_x2_channe1_keypad(__s32 ad_data)//y2=
{
	int value_key = 0;
	if(ad_data > 2000)
	{
	    return 0;
	}

	if((ad_data>=0)&&(ad_data<100))////1
	{
		value_key=KPAD_NUM1;
	}	
	else if((ad_data>=150)&&(ad_data<300))////1
	{
		value_key=KPAD_NUM2;
	}
	else if((ad_data>=350)&&(ad_data<500))////3
	{
		value_key=KPAD_NUM3;
	}
	else if((ad_data>=550)&&(ad_data<700))////4
	{
		value_key=KPAD_NUM4;
	}
	else if((ad_data>=750)&&(ad_data<900))////4
	{
		value_key=KPAD_NUM5;
	}
	else if((ad_data>=950)&&(ad_data<1100))////1
	{
		value_key=KPAD_NUM6;
	}
	else if((ad_data>=1150)&&(ad_data<1300))////3
	{
		value_key=KPAD_NUM7;
	}
	//else if((ad_data>=1350)&&(ad_data<1500))////4
	//{
	//	value_key=KPAD_NUM8;
	//}
	//else if((ad_data>=1550)&&(ad_data<1700))////4
	else if((ad_data>=1300)&&(ad_data<1950))////4
	{
		value_key=KPAD_NUM8;
	}	
	else
	{
	    return 0;        	   
	}
	
	return value_key;
}

 __s32 get_y2_channe1_keypad(__s32 ad_data)//y2=
{
	if(ad_data > 2000)
	{
	    return KR_ECHO_LV_0;
	}

	if(ad_data>=0&&ad_data<100)
	{
		return KR_ECHO_LV_0;
	}
	else if(ad_data>=100&&ad_data<300)
	{
		return KR_ECHO_LV_1;
	}
	else if(ad_data>=300&&ad_data<500)
	{
		return KR_ECHO_LV_2;
	}
	else if(ad_data>=500&&ad_data<700)
	{
		return KR_ECHO_LV_3;
	}
	else if(ad_data>=700&&ad_data<900)
	{
		return KR_ECHO_LV_4;
	}
	else if(ad_data>=900&&ad_data<1100)
	{
		return KR_ECHO_LV_5;
	}
	else if(ad_data>=1100&&ad_data<1300)
	{
		return KR_ECHO_LV_6;
	}
	else if(ad_data>=1300&&ad_data<1600)
	{
		return KR_ECHO_LV_7;
	}
	else
	{
		return KR_ECHO_LV_7;
	}
} 

  __s32 get_y1_channe1_keypad(__s32 ad_data)//y2=
{
	int value_key = 0;
	if(ad_data > 2000)
	{
	    return 0;
	}

	if((ad_data>=0)&&(ad_data<100))////1
	{
		value_key=KPAD_FM;
	}
	else if((ad_data>=150)&&(ad_data<300))////1
	{
		value_key=KPAD_MUSIC;
	}	
	else if((ad_data>=350)&&(ad_data<500))////1
	{
		value_key=KPAD_MOVIE;
	}	
	else if((ad_data>=550)&&(ad_data<700))////1
	{
		value_key=KPAD_RETURN;
	}
	else if((ad_data>=750)&&(ad_data<900))////4
	{
		value_key=KPAD_NUM8;
	}
	else if((ad_data>=950)&&(ad_data<1100))////1
	{
		value_key=KPAD_NUM9;
	}
	else if((ad_data>=1150)&&(ad_data<1300))////3
	{
		value_key=KPAD_NUM0;
	}
#if 0	
	else if((ad_data>=750)&&(ad_data<900))////2
	{
		value_key=KPAD_RETURN;
	}
	//else if((ad_data>=950)&&(ad_data<1100))////3
	//{
	//	value_key=KPAD_RECORD;
	//}
	else if((ad_data>=1150)&&(ad_data<1300))////4
	{
		value_key=KPAD_VOICEDOWN;
	}	
	//else if((ad_data>=1350)&&(ad_data<1700))////4
	else if((ad_data>=1350)&&(ad_data<1800))////4
	{
		value_key=KPAD_RECORD;
	}	
#endif
	else
	{
	    return 0;        	   
	}
	
	return value_key;
}

  
static void  rocker_data_task(void *p_arg)
{
    __s32           ret, times, ad_data, vaild_num, sum;
    __s32           count = 0,num=0;
    __ev_rk_msg_t   drv_rk_msg;
    
	static int x1_key_down_flag = 0, x1_key_pad = 0;
	static int x2_key_down_flag = 0, x2_key_pad = 0;
	static int y1_key_down_flag = 0, y1_key_pad = 0;
	static int y2_key_down_flag = 0, y2_key_pad = 0;

	static int col_down_flag = 0, col1_down_flag = 0;
	static int cur_key = 0, last_key = 0, key_cnt = 0;

	static int y2_last_pad = 0, y2_check_cnt = 0;
	
    __s32 databuf[8];

	__u32 adchannel_set=RK_CHANNEL_X2;
    __s32 key_type;
    key_type=get_key_type();
    RK_ClearFifo();
    RK_SelectChannel(RK_CHANNEL_X2);
    while(1)
    {
    	if(esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ)
    	{
	   		//杀线程;
	   		esKRNL_TDel(OS_PRIO_SELF);
    	}

		if(g_echo_chk_flag == 0)
		{
			switch(adchannel_set)
			{
#if 0				
				case RK_CHANNEL_X1:
				{
					RK_SelectChannel(RK_CHANNEL_X1);
					ret = RK_QueryIntPending();
					if(ret & RK_DATA_PENDING)
					{
						eLIBs_memset(rk_private.crt_x, 0, sizeof(rk_private.crt_x));
						RK_GetData(rk_private.crt_x, 8);
						RK_ClearIntPending(ret);
						RK_ClearFifo();
						eLIBs_memset(databuf, 0, sizeof(databuf));
						vaild_num = data_filter(rk_private.crt_x,databuf, 8);
						sum = 0;
						for(num = 0; num < vaild_num; num++)
						{
							sum +=databuf[num];
						}
						ad_data = sum / vaild_num;

						if((x1_key_down_flag == 0)&&(ad_data<2000))
						{
							__msg("### X1 ad_data = %d \n", ad_data);
							x1_key_pad = get_x1_channe1_keypad(ad_data);
							__msg("### x1_key_pad = %d ###\n", x1_key_pad);
				                    if(x1_key_pad != 0)
				                    {
				                        x1_key_down_flag = 1;
				                        rk_send_key_message(x1_key_pad, 1);        //down
				                        break;
				                    }
						}
						else if((x1_key_down_flag==1)&&(ad_data>2000))
						{
							adchannel_set=RK_CHANNEL_X2;
							x1_key_down_flag = 0;
							rk_send_key_message(x1_key_pad, 0);    
							x1_key_pad=0;
							break;
						}
						else if((x1_key_down_flag==0)&&(ad_data>2000))
						{
							adchannel_set=RK_CHANNEL_X2;
						}
					}
					//RK_DisableInt(RK_DATA_IRQ);
				}
				break;
#endif
				case RK_CHANNEL_X2:
				{
					RK_SelectChannel(RK_CHANNEL_X2);
					ret = RK_QueryIntPending();
					if(ret & RK_DATA_PENDING)
					{
						eLIBs_memset(rk_private.crt_x, 0, sizeof(rk_private.crt_x));
						RK_GetData(rk_private.crt_x, 8);
						RK_ClearIntPending(ret);
						RK_ClearFifo();
						eLIBs_memset(databuf, 0, sizeof(databuf));
						vaild_num = data_filter(rk_private.crt_x,databuf, 8);
						sum = 0;
						for(num = 0; num < vaild_num; num++)
						{
							sum +=databuf[num];					
						}
						ad_data = sum / vaild_num;

						if((x2_key_down_flag == 0)&&(ad_data<2000))
						{
							__msg("### X2 ad_data = %d \n", ad_data);
							x2_key_pad = get_x2_channe1_keypad(ad_data);
				                    if(x2_key_pad != 0)
				                    {		                        
				                        x2_key_down_flag = 1;
				                        rk_send_key_message(x2_key_pad, 1);        //down
				                        break;
				                    }
						}
						else if((x2_key_down_flag==1)&&(ad_data>2000))
						{
							adchannel_set=RK_CHANNEL_Y1;	//RK_CHANNEL_Y2;
							x2_key_down_flag = 0;
				            rk_send_key_message(x2_key_pad, 0);    
							x2_key_pad=0;
							break;
						}
						else if((x2_key_down_flag==0)&&(ad_data>2000))
						{
							adchannel_set=RK_CHANNEL_Y1;	//RK_CHANNEL_Y2;
						}
					}
				}
				break;
				
				case RK_CHANNEL_Y1:
				{
					RK_SelectChannel(RK_CHANNEL_Y1);
					ret = RK_QueryIntPending();
					if(ret & RK_DATA_PENDING)
					{
						eLIBs_memset(rk_private.crt_y, 0, sizeof(rk_private.crt_y));
						RK_GetData(rk_private.crt_y, 8);
						RK_ClearIntPending(ret);
						RK_ClearFifo();
						eLIBs_memset(databuf, 0, sizeof(databuf));
						vaild_num = data_filter(rk_private.crt_y,databuf, 8);
						sum = 0;
						for(num = 0; num < vaild_num; num++)
						{
							sum +=databuf[num];					
						}
						ad_data = sum / vaild_num;

				if((y1_key_down_flag == 0)&&(ad_data<2000))
				{
					__msg("--y1 ad_data = %d \n", ad_data);
					y1_key_pad = get_y1_channe1_keypad(ad_data);
		                    if(y1_key_pad != 0)
		                    {		                        
		                        y1_key_down_flag = 1;
		                        rk_send_key_message(y1_key_pad, 1);        //down
		                        break;
		                    }
				}
				else if((y1_key_down_flag==1)&&(ad_data>2000))
				{
					adchannel_set=RK_CHANNEL_X2;
					y1_key_down_flag = 0;
					rk_send_key_message(y1_key_pad, 0);    
					y1_key_pad=0;
					break;
				}
				else if((y1_key_down_flag==0)&&(ad_data>2000))
				{
					adchannel_set=RK_CHANNEL_X2;
				}
			}
		}
		break;				
#if 0
				case RK_CHANNEL_Y2:
				{
					if((++y2_check_cnt)>2)
					{
						y2_check_cnt = 0;
					}
					else
						break;
					
					RK_SelectChannel(RK_CHANNEL_Y2);
					ret = RK_QueryIntPending();
					if(ret & RK_DATA_PENDING)
					{
						eLIBs_memset(rk_private.crt_y, 0, sizeof(rk_private.crt_y));
						RK_GetData(rk_private.crt_y, 8);
						RK_ClearIntPending(ret);
						RK_ClearFifo();
						eLIBs_memset(databuf, 0, sizeof(databuf));
						vaild_num = data_filter(rk_private.crt_y,databuf, 8);
						sum = 0;
						for(num = 0; num < vaild_num; num++)
						{
							sum +=databuf[num];
						}
						ad_data = sum / vaild_num;

						if((y2_key_down_flag == 0)&&(ad_data<2000))
						{
							
							__msg("### Y2 ad_data = %d \n", ad_data);
							y2_key_pad = get_y2_channe1_keypad(ad_data);
				                    if((y2_key_pad != 0) && (y2_key_pad != y2_last_pad))
				                    {
				                        y2_last_pad = y2_key_pad;
				                        y2_key_down_flag = 1;
				                        rk_send_key_message(y2_key_pad, 1);        //down
				                        break;
				                    }
						}
						else if((y2_key_down_flag==1)&&(ad_data>2000))
						{
							adchannel_set=RK_CHANNEL_X1;
							y2_key_down_flag = 0;
				            rk_send_key_message(y2_key_pad, 0);    
							y2_key_pad=0;
							break;
						}
						else if((y2_key_down_flag==0)&&(ad_data>2000))
						{
							adchannel_set=RK_CHANNEL_X1;
						}
					}
				}
				break;
#endif
				default:
					break;
			}
		}
		else
		{
			RK_SelectChannel(RK_CHANNEL_Y2);
			ret = RK_QueryIntPending();
			if(ret & RK_DATA_PENDING)
			{
				eLIBs_memset(rk_private.crt_y, 0, sizeof(rk_private.crt_y));
				RK_GetData(rk_private.crt_y, 8);
				RK_ClearIntPending(ret);
				RK_ClearFifo();
				eLIBs_memset(databuf, 0, sizeof(databuf));
				vaild_num = data_filter(rk_private.crt_y,databuf, 8);
				sum = 0;
				for(num = 0; num < vaild_num; num++)
				{
					sum +=databuf[num];
				}
				ad_data = sum / vaild_num;

				if((ad_data<2000))
				{
					__msg("### Y2 ad_data = %d \n", ad_data);
					g_echo_level = get_y2_channe1_keypad(ad_data);
				}

			}

			adchannel_set=RK_CHANNEL_X2;
		}
		

next_check:
	    esKRNL_TimeDly(4);
	
    }
}

/*
****************************************************************************************************
*
*             rocker_init
*
*  Description:
*       rocker_init
*
*  Parameters:
*
*  Return value:
*       none
*
****************************************************************************************************
*/
__s32 rocker_init(void)
{
    __s32            ret;
    user_gpio_set_t  gpio_set[1];
	
    rocker_drv.rk_dev.cb    = rk_default_cb;
    rocker_drv.rk_dev.cb_bk = rk_default_cb;

	RK_Init();
    RK_SetFifoDepth(RK_FIFO_DEPTH_08);
    RK_SetSampleRate(RK_SAMPLE_RATE_16K);
    rk_private.data_sem     = esKRNL_SemCreate(1);
	rk_private.data_tid     = esKRNL_TCreate(rocker_data_task, 0, 0x8000, KRNL_priolevel1);
   RK_EnableInt(RK_DATA_IRQ);

#if 0
	/* 申请input_row1 */
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "output_col", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        output_col = esPINS_PinGrpReq(gpio_set, 1);
        if (!output_col)
        {
            __wrn("request input_row1 pin failed\n");
            return EPDK_FAIL;
        }
    }
    else
    {
        __wrn("fetch para from script failed\n");
        return EPDK_FAIL;
    }

	//ret = esPINS_SetPinPull(output_col, PIN_PULL_UP, NULL);
	//if (ret)
	//{
	//	__wrn("pull input_row2 failed\n");
	//	return EPDK_FAIL;
	//}

	/* 申请input_row1 */
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "output_col1", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        output_col1 = esPINS_PinGrpReq(gpio_set, 1);
        if (!output_col1)
        {
            __wrn("request input_row1 pin failed\n");
            return EPDK_FAIL;
        }
    }
    else
    {
        __wrn("fetch para from script failed\n");
        return EPDK_FAIL;
    }

	//ret = esPINS_SetPinPull(output_col1, PIN_PULL_UP, NULL);
	//if (ret)
	//{
	//	__wrn("pull input_row2 failed\n");
	//	return EPDK_FAIL;
	//}
//#else
	//申请IO口
    h_input_1 =  esPINS_PinGrpReq(&Input_1[0],1);
    if (NULL == h_input_1)
    {
        eLIBs_printf("request led control pin failed\n");
    }
	esKRNL_TimeDly(2);

	//申请IO口
    h_input_2 =  esPINS_PinGrpReq(&Input_2[0],1);
    if (NULL == h_input_2)
    {
        eLIBs_printf("request led control pin failed\n");
    }	
#endif
    return EPDK_OK;
}
/*
****************************************************************************************************
*
*             rocker_exit
*
*  Description:
*       rocker_exit
*
*  Parameters:
*
*  Return value:
*       none
*
****************************************************************************************************
*/
__s32 rocker_exit(void)
{
    __u8  err;

    if(rk_private.data_tid)
    {   
        esKRNL_TDel(rk_private.data_tid);
        esKRNL_TimeDly(4);
    }
    esKRNL_SemDel(rk_private.data_sem, OS_DEL_ALWAYS, &err);
    RK_Exit();
   esPINS_PinGrpRel(pw_pin, 0);

#if 0
    esPINS_PinGrpRel(output_col, 0);
    esPINS_PinGrpRel(output_col1, 0);
//#else
	//释放IO口
    if (h_input_1)
        esPINS_PinGrpRel(h_input_1, 0);
    if (h_input_2)
        esPINS_PinGrpRel(h_input_2, 0);	
#endif
    return EPDK_OK;
}
