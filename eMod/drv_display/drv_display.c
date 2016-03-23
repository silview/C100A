#include "drv_display_i.h"

#define PIO_BASE                    0xf1c20800


#if DRV_VCOM_TIMER
#define KNOB_USE_HW_TIMER		1
#define VCOM_MAX_PERIOD			30

//默认占空比
#define DRV_VCOM_DEFAULT_LEVEL	5		

static __u32 vcom_io = 0;
static __s32 vcom_en = 0;

static __s32 sta_mask = 0;

static __s32 duty_period = VCOM_MAX_PERIOD;	//周期总数
static __s32 duty_percent = DRV_VCOM_DEFAULT_LEVEL;	//占空比
static __s32 duty_cnt = 0;

#if  KNOB_USE_HW_TIMER
static __hdle  g_vcom_timer_hdl = NULL;
#else
static __krnl_stmr_t *g_vcom_timer_hdl = NULL;
#endif

#endif

__disp_drv_t    g_disp_drv;

void DRV_lcd_power_on(__u32 sel)
{
    ES_FILE * hd 	= NULL;
    __s32 ret 	= 0;

    hd = eLIBs_fopen("b:\\HID\\POWER", "w");
    if(hd == NULL){
        __wrn("open power driver fail\n");
        return;
    }

    ret = eLIBs_fioctrl(hd, DRV_POWER_CMD_VSTS_SET, POWER_VOL_SW1,(void *)POWER_VOL_ON);
    if(ret != EPDK_OK){
        __wrn("eLIBs_fioctrl DRV_POWER_CMD_VSTS_SET failed\n");
    }

    eLIBs_fclose(hd);
    hd = NULL;
    
    return;
}

void DRV_lcd_power_off(__u32 sel)
{
    ES_FILE *  hd 	= NULL;
    __s32 ret 	= 0;

    hd = eLIBs_fopen("b:\\HID\\POWER", "w");
    if(hd == NULL){
        __wrn("open power driver fail\n");
        return;
    }

    ret = eLIBs_fioctrl(hd, DRV_POWER_CMD_VSTS_SET, POWER_VOL_SW1,(void *)POWER_VOL_OFF);
    if(ret != EPDK_OK){
        __wrn("eLIBs_fioctrl DRV_POWER_CMD_VSTS_SET failed\n");
    }

    eLIBs_fclose(hd);
    hd = NULL;
    
    return;
}


__s32 DRV_lcd_open(__u32 sel)
{    
    __u32 i = 0;
    __lcd_flow_t *flow;
    
    BSP_disp_lcd_open_before(sel);

    flow = BSP_disp_lcd_get_open_flow(sel);
    for(i=0; i<flow->func_num; i++)
    {
        flow->func[i].func(sel);
        esKRNL_TimeDly(flow->func[i].delay / 10);
    }

    BSP_disp_lcd_open_after(sel);

    return EPDK_OK;
}

__s32 DRV_lcd_close(__u32 sel)
{    
    __u32 i = 0;
    __lcd_flow_t *flow;
    
    BSP_disp_lcd_close_befor(sel);

    flow = BSP_disp_lcd_get_close_flow(sel);
    for(i=0; i<flow->func_num; i++)
    {
        flow->func[i].func(sel);
        esKRNL_TimeDly(flow->func[i].delay / 10);
    }

    BSP_disp_lcd_close_after(sel);

    return EPDK_OK;
}

__s32 DRV_scaler_begin(__u32 sel)
{
    __u8 err;

    esKRNL_SemPend(g_disp_drv.scaler_finished_sem[sel],0x50,&err);

	if(err == OS_NO_ERR)
	{
    	return EPDK_OK;
   	}
   	else
   	{
   		__wrn("writeback error\n");
   		return EPDK_FAIL;
   	}
}

void DRV_scaler_finish(__u32 sel)
{
    esKRNL_SemPost(g_disp_drv.scaler_finished_sem[sel]);
}


void DRV_tve_interrup(__u32 sel)
{
}

__s32 DRV_hdmi_set_mode(__disp_tv_mode_t mode)
{
	ES_FILE * p_hdmi;
	__s32 ret;
	
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {
        __err("open hdmi device fail!\n");
    }
	ret  = eLIBs_fioctrl(p_hdmi, HDMI_CMD_SET_VIDEO_MOD, mode, 0);
	if(ret != EPDK_OK)
	{
		__wrn("set hdmi mode fail!\n");
		eLIBs_fclose(p_hdmi);
		return EPDK_FAIL;
	}
	eLIBs_fclose(p_hdmi);

	return EPDK_OK;
}

__s32 DRV_Hdmi_open(void)
{
    ES_FILE * p_hdmi;
    __s32 ret;
    
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {
        __err("open hdmi device fail!\n");
    }
    ret  = eLIBs_fioctrl(p_hdmi, HDMI_CMD_OPEN, 0, 0);
    if(ret != EPDK_OK)
    {
        __wrn("set hdmi mode fail!\n");
        eLIBs_fclose(p_hdmi);
        return EPDK_FAIL;
    }
    eLIBs_fclose(p_hdmi);

    return EPDK_OK;
}

__s32 DRV_Hdmi_close(void)
{
    ES_FILE * p_hdmi;
    __s32 ret;
    
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {
        __err("open hdmi device fail!\n");
    }
    ret  = eLIBs_fioctrl(p_hdmi, HDMI_CMD_CLOSE, 0, 0);
    if(ret != EPDK_OK)
    {
        __wrn("set hdmi mode fail!\n");
        eLIBs_fclose(p_hdmi);
        return EPDK_FAIL;
    }
    eLIBs_fclose(p_hdmi);

    return EPDK_OK;
}

__s32 DRV_hdmi_mode_support(__u8 mode)
{
    ES_FILE * p_hdmi;
    __s32 ret;
    
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {
        __err("open hdmi device fail!\n");
    }
    ret  = eLIBs_fioctrl(p_hdmi, HDMI_CMD_MOD_SUPPORT, mode, 0);
    if(ret != EPDK_OK)
    {
        __wrn("set hdmi mode fail!\n");
        eLIBs_fclose(p_hdmi);
        return EPDK_FAIL;
    }
    eLIBs_fclose(p_hdmi);

    return EPDK_OK;
}
__s32 DRV_hdmi_get_HPD_status(void)
{
    ES_FILE * p_hdmi;
    __s32 ret;
    
    p_hdmi = eLIBs_fopen("b:\\BUS\\HDMI", "r+");
    if(!p_hdmi)
    {
        __err("open hdmi device fail!\n");
    }
    ret  = eLIBs_fioctrl(p_hdmi, HDMI_CMD_GET_HPD_STATUS, 0, 0);
    if(ret != EPDK_OK)
    {
        __wrn("set hdmi mode fail!\n");
        eLIBs_fclose(p_hdmi);
        return EPDK_FAIL;
    }
    eLIBs_fclose(p_hdmi);

    return EPDK_OK;
}

#if DRV_VCOM_TIMER
__s32 DRV_lcd_set_set_duty(__s32 duty)
{
	eLIBs_printf(" -- %s, %d, duty = %d -- \n", __FILE__, __LINE__, duty);
	duty = 10;
	duty_percent = duty;
	
	return EPDK_OK;
}

__s32 DRV_lcd_get_set_duty(void)
{
	return duty_percent;
}

static void vcom_timmer_proc(void)
{
#if 1
	if((duty_cnt++) < duty_percent)
	{
		if(!sta_mask)
		{
			esPINS_WritePinData(vcom_io, 1, NULL);
			sta_mask= !sta_mask;
		}
	}
	else
	{
		if(sta_mask)
		{
			esPINS_WritePinData(vcom_io, 0, NULL);
			sta_mask= !sta_mask;
		}
	}

	if(duty_cnt >= duty_period)
		duty_cnt = 0;
#else
		if(!sta_mask)
		{
			esPINS_WritePinData(vcom_io, 1, NULL);
		}
		else
		{
			esPINS_WritePinData(vcom_io, 0, NULL);
		}
		sta_mask= !sta_mask;

#endif
}

static __s32 vcom_timer_init(void)
{	
	__hw_tmr_type_t timer_para;

	__wrn("knob_timer_init...\n");
	
#if KNOB_USE_HW_TIMER	

	eLIBs_memset(&timer_para, 0, sizeof(__hw_tmr_type_t));
	timer_para.precision = CSP_TMRC_TMR_PRECISION_MILLI_SECOND;
	timer_para.leastCount = 0;				
		
	g_vcom_timer_hdl = esTIME_RequestTimer(&timer_para,(__pCBK_t)vcom_timmer_proc, NULL, "knob_hw_timer");

	if(NULL == g_vcom_timer_hdl)
	{
		__wrn("create hw timer failed...\n");
		return EPDK_FAIL;
	}
	__here__;
	
#else	
	g_vcom_timer_hdl = esKRNL_TmrCreate(
				1, 
				OS_TMR_OPT_PRIO_LOW | OS_TMR_OPT_PERIODIC,
				(OS_TMR_CALLBACK)vcom_timmer_proc,
				NULL);
	if (NULL == g_vcom_timer_hdl)
	{
		__wrn("create sw timer fail...\n");
		return EPDK_FAIL;
	}
	
#endif
	return EPDK_OK;
}

static __s32 vcom_timer_start(void)
{
#if KNOB_USE_HW_TIMER	
	if(NULL == g_vcom_timer_hdl)
	{
		return EPDK_FAIL;
	}
	esTIME_StartTimer(g_vcom_timer_hdl, 1, CSP_TMRC_TMR_MODE_CONTINU);	
#else
	if(NULL == g_vcom_timer_hdl)
	{
		return EPDK_FAIL;
	}
	esKRNL_TmrStart(g_vcom_timer_hdl);
#endif
	return EPDK_OK;
}
#endif


__mp * DRV_DISP_MOpen(__u32 mid, __u32 mode)
{
    g_disp_drv.mid = mid;
    g_disp_drv.used = 1;
    
    return (__mp *)&g_disp_drv;
}
 
__s32 DRV_DISP_MClose(__mp * pDrv)
{
    return EPDK_OK;
}
__u32 DRV_DISP_MRead(void *pBuffer, __u32 size, __u32 count, __mp * pdev)
{
    return EPDK_OK;
}
__u32 DRV_DISP_MWrite(const void *pBuffer, __u32 size, __u32 count, __mp * pdev)
{
    return EPDK_OK;
}

__s32 DRV_DISP_MIoctrl(__mp* pDev, __u32 cmd, __s32 aux, void *pbuffer)
{ 
    return   EPDK_OK;;
}

__s32 DRV_DISP_MInit(void)
{
    __disp_bsp_init_para para;
    __hdle hReg;
    __u32 i = 0;
     user_gpio_set_t  gpio_set[1];
	__s32            ret;
   
    para.base_image0    = 0xf1e60000;
    para.base_scaler0   = 0xf1e00000;
    para.base_lcdc0     = 0xf1c0c000;
    para.base_tvec      = 0xf1c0a000;
    para.base_ccmu      = 0xf1c20000;
    para.base_sdram     = 0xf1c01000;
    para.base_pioc      = 0xf1c20800;
    para.base_timer		= 0xf1c21000;	/*	PWM module register base	*/

    para.power_on       = DRV_lcd_power_on;
    para.power_off      = DRV_lcd_power_off;
    para.scaler_begin   		= DRV_scaler_begin;
    para.scaler_finish  		= DRV_scaler_finish;
    para.tve_interrup   		= DRV_tve_interrup;
	para.hdmi_set_mode  		= DRV_hdmi_set_mode;
	para.Hdmi_open  			= DRV_Hdmi_open;
	para.Hdmi_close  			= DRV_Hdmi_close;
	para.hdmi_mode_support		= DRV_hdmi_mode_support;
	para.hdmi_get_HPD_status	= DRV_hdmi_get_HPD_status;
	para.disp_int_process       = disp_int_process;

	eLIBs_memset(&g_disp_drv, 0, sizeof(__disp_drv_t));
    g_disp_drv.scaler_finished_sem[0] = esKRNL_SemCreate(0);
    if(!g_disp_drv.scaler_finished_sem[0])
    {
        __wrn("create scaler_finished_sem[0] fail!\n");
        return EPDK_FAIL;
    }  
    g_disp_drv.scaler_finished_sem[1] = esKRNL_SemCreate(0);
    if(!g_disp_drv.scaler_finished_sem[1])
    {
        __wrn("create scaler_finished_sem[1] fail!\n");
        return EPDK_FAIL;
    }    

    for(i = 0; i<MAX_EVENT_SEM; i++)
    {
    	g_disp_drv.event_sem[0][i] = NULL;
     	g_disp_drv.event_sem[1][i] = NULL;
    }

    BSP_disp_init(&para);
    BSP_disp_open();

	g_disp_drv.reg_operate_vaild = 1;
	
   	hReg = esDEV_DevReg("DISP", "DISPLAY", &dispdev_ops, 0);
    if(!hReg)
    {
        __inf("esDEV_DevReg() fail!\n");
    	return EPDK_FAIL;
    }

	#if DRV_VCOM_TIMER

	//Soft Vcom Control;

	ret = esCFG_GetKeyValue("softvcom_para", "soft_vcom_en", &vcom_en, 1);
	if (!ret)
	{
		eLIBs_printf(" -- %s, %d, vcom_en = %d -- \n", __FILE__, __LINE__, vcom_en);
	
		if(vcom_en)
		{
			ret = esCFG_GetKeyValue("softvcom_para", "vcom_io", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
			if (!ret)
			{
				vcom_io = esPINS_PinGrpReq(gpio_set, 1);
				if (!vcom_io)
				{
					__wrn("request output_col pin failed\n");
					return EPDK_FAIL;
				}
			}
			else
			{
				__wrn("fetch para from script failed\n");
				return EPDK_FAIL;
			}
		}
	}
	else
	{
		__wrn("fetch para from script failed\n");
		return EPDK_FAIL;
	}

	ret = esPINS_SetPinPull(vcom_io, PIN_PULL_DOWN, NULL);
	if (ret)
	{
		__wrn("pull output_col failed\n");
		return EPDK_FAIL;
	}

    //申请定时器
	vcom_timer_init();

    //让定时器开始跑
	vcom_timer_start();
#endif	

    return EPDK_OK;        
}

__s32 DRV_DISP_MExit(void)
{        
	__u8 err;

    BSP_disp_close();
    BSP_disp_exit(g_disp_drv.exit_mode);

#if DRV_VCOM_TIMER

#if  KNOB_USE_HW_TIMER
	if(g_vcom_timer_hdl)
	{
		esTIME_StopTimer(g_vcom_timer_hdl);
		esTIME_ReleaseTimer(g_vcom_timer_hdl);
		g_vcom_timer_hdl = NULL;
	}

#else
    if (esKRNL_TmrStateGet(g_vcom_timer_hdl) == OS_TMR_STATE_RUNNING)
	{
		esKRNL_TmrStop(g_vcom_timer_hdl, OS_TMR_OPT_NONE, NULL);
		esKRNL_TmrDel(g_vcom_timer_hdl);
		g_vcom_timer_hdl = NULL;
    }


#endif
    esPINS_PinGrpRel(vcom_io, 0);
#endif		

	esKRNL_SemDel(g_disp_drv.scaler_finished_sem[0], 0, &err);
	esKRNL_SemDel(g_disp_drv.scaler_finished_sem[1], 0, &err);

    return EPDK_OK;
} 

 
