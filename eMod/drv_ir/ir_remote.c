/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                             audio driver Module
*
*                                    (c) Copyright 2009-2010, kevin China
*                                             All Rights Reserved
*
* File    : ir_remote.c
* By      : victor
* Version : V1.0
* Date    : 2010-12-31
*********************************************************************************************************
*/

#include "drv_ir_remote_i.h"
#include "ir_remote.h"
#include "ir_remote_keymap.h"

 __u32 *ir_solution_keymapping = NULL;

//static __ir_key_setting_t *pIrkey = NULL;   //cai modify
__ir_key_setting_t *pIrkey = NULL;   //cai++ for ir power

#if 0
    #define __here__            eLIBs_printf("@L%d(%s)\n", __LINE__, __FILE__);
	#define __msg(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
	#define __wrn(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
	#define __err(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
#else
    #define __msg(...)
#endif

/*
****************************************************************************************************
*
*  FunctionName:           lradckey_keydown
*
*  Description:
*              按键按下，调用ioctrl发送按键按下消息。回调函数。   
*
*  Parameters:
*       keyvalue :  按键值
*
*  Return value:
*            无。
*  Notes:
*           
****************************************************************************************************
*/
static  void Irkey_keydown(__u32 keyvalue)
{
    __ir_key_msg_t msg;
    msg.key_value = ir_solution_keymapping[keyvalue];
    msg.flag = 1;

__msg("..........Irkey_keydown....................keyvalue=%d\n",keyvalue);
    DEV_IR_KEY_Ioctrl(0, DRV_IRKEY_CMD_PUTKEY, 0, &msg);
  	return;
}

/*
****************************************************************************************************
*
*  FunctionName:           lradckey_keyup
*
*  Description:
*              按键抬起，调用ioctrl发送按键抬起消息。回调函数。   
*
*  Parameters:
*       keyvalue :  按键值
*
*  Return value:
*            无。
*  Notes:
*           
****************************************************************************************************
*/
static  void Irkey_keyup(__u32 keyvalue)
{
    __ir_key_msg_t msg;
    msg.key_value = ir_solution_keymapping[keyvalue];
    msg.flag = 0;
__msg("..........Irkey_keyup....................keyvalue=%d\n",keyvalue);
    DEV_IR_KEY_Ioctrl(0, DRV_IRKEY_CMD_PUTKEY, 0, &msg);
    return;
}

/*
****************************************************************************************************
*
*  FunctionName:           Lradc_key_Init
*
*  Description:
*             lradc 按键初始化。
*  Parameters:
*       无。
*
*  Return value:
*             成功返回EPDK_OK,失败返回EPDK_FAIL.
*  Notes:
*           
****************************************************************************************************
*/
__s32 Ir_key_Init(void)
{
    if(pIrkey == NULL)
    {
        pIrkey = (__ir_key_setting_t *)eLIBs_malloc(sizeof(KEYC_SETTING));
        if(pIrkey == NULL)
        {
            __err("ir key driver memory malloc error!\n");
            return EPDK_FAIL;
        }
    	eLIBs_memset(pIrkey, 0, sizeof(__ir_key_setting_t));
    }
    else
    {
        __wrn("ir key has been initial already!\n");
        return EPDK_FAIL;
    }
   __msg("按键初始化开始\n"); 
    //initial ir key 
    pIrkey->irq_flags    = 0;
    pIrkey->irq_no       = IR_IRQ_NUM;
    pIrkey->irq_prio     = 0;    
    
    pIrkey->remote_controller = NEC_REMOTE_CONTROLLER;
    pIrkey->user_code         = NEC_IR_ADDR_CODE3;/* 如果为0则认为不检查用户码 */  //cai modify
    //pIrkey->user_code		= 0;  //cai++
    pIrkey->holdkey           = 0;
    
    pIrkey->rp_keytime = 130; /* repeat key interval times: 95ms */
    pIrkey->keyup_times = 3; /* detect key up times: 连续按285ms抬起,单键按190ms抬起 ms*/
    
    pIrkey->key_down = Irkey_keydown;
    pIrkey->key_up   = Irkey_keyup;
	__msg("按键映射\n");
#if 0
    if(pIrkey->user_code == 0)
    {
	     ir_keymap[0x14] = KPAD_IR_POWER;   //使用该按键，用来进入待机模式
	     ir_keymap[0x4c] = KPAD_MENU;
	     ir_keymap[0x15] = KPAD_RETURN;  //弹出菜单功能
		 ir_keymap[0x11] = KPAD_IR_LEFT;
		 ir_keymap[0x48] = KPAD_ENTER;   //弹出菜单功能
		 ir_keymap[0x13] = KPAD_IR_DOWN;   //KPAD_IR_VOICEDOWN;
		 ir_keymap[0x17] = KPAD_FM;//KPAD_IR_MODE;
		 ir_keymap[0x12] = KPAD_IR_UP;  //KPAD_IR_VOICEUP;
		 ir_keymap[0x10] = KPAD_IR_RIGHT;//KPAD_FM;
    	}

#elif 0
    if(pIrkey->user_code == NEC_IR_ADDR_CODE1)
    {
        ir_keymap[3]  = KPAD_MENU;
        ir_keymap[13] = KPAD_RETURN;/* learning */
        ir_keymap[18] = KPAD_UP;
        ir_keymap[20] = KPAD_RIGHT;
        ir_keymap[21] = KPAD_ENTER;
        ir_keymap[22] = KPAD_LEFT;
        ir_keymap[24] = KPAD_DOWN;        
    }
#elif 1//wqx 20130514
	if(pIrkey->user_code == NEC_IR_ADDR_CODE3)
	{
		ir_keymap[0x45] = KPAD_MUTE;
		ir_keymap[0x46] = KPAD_IR_TVOUT;   
		ir_keymap[0x47] = KPAD_IR_KOK;  
		ir_keymap[0x44] = KPAD_MENU;
		ir_keymap[0x40] = KPAD_IR_VOICEUP;
		ir_keymap[0x43] = KPAD_RETURN;
		ir_keymap[0x07] = KPAD_LEFT;  
		ir_keymap[0x15] = KPAD_REPLACE_ENTER;
		ir_keymap[0x09] = KPAD_RIGHT;  
		ir_keymap[0x16] = KPAD_NUM0;
		ir_keymap[0x19] = KPAD_IR_VOICEDOWN;   
		ir_keymap[0x0d] = KPAD_MODE;
		ir_keymap[0x0c] = KPAD_NUM1;
		ir_keymap[0x18] = KPAD_NUM2;
		ir_keymap[0x5e] = KPAD_NUM3;
		ir_keymap[0x08] = KPAD_NUM4;
		ir_keymap[0x1c] = KPAD_NUM5;
		ir_keymap[0x5a] = KPAD_NUM6;
		ir_keymap[0x42] = KPAD_NUM7;
		ir_keymap[0x52] = KPAD_NUM8;
		ir_keymap[0x4a] = KPAD_NUM9;
	}

#else
	//if(pIrkey->user_code == 0)
	if(pIrkey->user_code == NEC_IR_ADDR_CODE1)
	{
        ir_keymap[18]  = KPAD_MUTE;
        ir_keymap[26] = KPAD_IR_TVOUT;   //未添加此功能
        ir_keymap[30] = KPAD_IR_POWER;   //使用该按键，用来进入待机模式
        ir_keymap[1] = KPAD_PHOTO;
        ir_keymap[2] = KPAD_MUSIC;
        ir_keymap[3] = KPAD_MOVIE;
        ir_keymap[4] = KPAD_PAUSE;  
	 ir_keymap[5] = KPAD_IR_UP;
	 ir_keymap[6] = KPAD_MENU;  //弹出菜单功能
	 ir_keymap[7] = KPAD_IR_LEFT;
	 ir_keymap[8] = KPAD_ENTER;   //弹出菜单功能
	 ir_keymap[9] = KPAD_IR_RIGHT;
	 ir_keymap[10] = KPAD_SETTING;
	 ir_keymap[27] = KPAD_IR_DOWN;
	 ir_keymap[31] = KPAD_CALENDAR;
	 ir_keymap[12] = KPAD_IR_VOICEUP;
	 ir_keymap[13] = KPAD_FM;
	 ir_keymap[14] = KPAD_MEDIA_PRE;
	 ir_keymap[0] = KPAD_IR_VOICEDOWN;
	 ir_keymap[15] = KPAD_RECORD;
	 ir_keymap[25] = KPAD_MEDIA_NEXT;
	}
#endif
	if(pIrkey->user_code == NEC_IR_ADDR_CODE4)
	{
        ir_keymap[0]  = KPAD_MUSIC;
        ir_keymap[1] = KPAD_MOVIE;   
        ir_keymap[2] = KPAD_FM;  
		
	 ir_keymap[4] = KPAD_NUM1;
	 ir_keymap[5] = KPAD_NUM2;
	 ir_keymap[6] = KPAD_NUM3;
	 
	 ir_keymap[8] = KPAD_NUM4;
	 ir_keymap[9] = KPAD_NUM5;
	 ir_keymap[0x0a] = KPAD_NUM6;

	 ir_keymap[0x0C] = KPAD_REPLACE_ENTER;
	 ir_keymap[0x0D] = KPAD_VOICEDOWN;
	 ir_keymap[0x0E] = KPAD_MENU;

	 ir_keymap[0x10] = KPAD_LEFT;
	 ir_keymap[0x11] = KPAD_ENTER;
	 ir_keymap[0x12] = KPAD_RIGHT;

	 ir_keymap[0x14] = KPAD_MUTE;
	 ir_keymap[0x15] = KPAD_VOICEUP;
	 ir_keymap[0x16] = KPAD_MODE;

	 ir_keymap[0x18] = KPAD_SETTING;
	 ir_keymap[0x19] = KPAD_REPLACE_ENTER;
	 ir_keymap[0x1A] = KPAD_RETURN;

	}
    ir_solution_keymapping = ir_keymap;
    
   return BSP_IR_key_init(pIrkey);
}

/*
****************************************************************************************************
*
*  FunctionName:           Lradc_key_Exit
*
*  Description:
*              释放资源，调用bsp退出函数。
*
*  Parameters:
*       无。
*
*  Return value:
*            成功返回EPDK_OK,失败返回EPDK_FAIL
*  Notes:
*           
****************************************************************************************************
*/
__s32 Ir_key_Exit(void)
{	
    __inf("lradc key drv exit!\n");
	if(pIrkey)
	{
	    BSP_IR_key_exit(pIrkey);
	    
        eLIBs_free(pIrkey);
        pIrkey = NULL;
        return EPDK_OK;
	}
	return  EPDK_FAIL;
}
