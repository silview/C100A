/*
************************************************************************************************************************
*                                                          lcd
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_display_lcd.c
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.09.02
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
* Gary.Wang      2010.09.02        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __DSK_DISPLAY_LCD_C__
#define  __DSK_DISPLAY_LCD_C__


#include "dsk_display_i.h"


/*
****************************************************************************************************
*Name        : dsk_display_lcd_on
*Prototype   : __s32 dsk_display_lcd_on(void)
*Arguments   : void
*Return      : EPDK_OK     succeed
*              EPDK_FAIL   fail
*Description : open the LCD display.
*Other       :
****************************************************************************************************
*/
__s32 dsk_display_lcd_on(void)
{
    __disp_output_type_t  	output;
	ES_FILE      		   *p_disp = NULL;
    
	/* open display driver */
	p_disp = eLIBs_fopen(DISP_DRV_PATH, "r+");
	if( p_disp == NULL )
	{
	    __err("Error in opening display driver.\n");
	    goto error;
	}

    /* get output type and format */
    
    output = (__disp_output_type_t)eLIBs_fioctrl(p_disp, DISP_CMD_GET_OUTPUT_TYPE, 0, 0);
   
	if( output == DISP_OUTPUT_TYPE_NONE )
	{
		eLIBs_fioctrl(p_disp, DISP_CMD_LCD_ON, 0, 0);	
	}
    
	/* close display driver */
    eLIBs_fclose(p_disp);
    p_disp = NULL;
    
    return EPDK_OK;
    
error:
    if( p_disp != NULL )
    {
        eLIBs_fclose(p_disp);
        p_disp = NULL;
    }
    return EPDK_FAIL;
}

/*
****************************************************************************************************
*Name        : dsk_display_lcd_off
*Prototype   : __s32 dsk_display_lcd_off(void)
*Arguments   : void
*Return      : EPDK_OK     succeed
*              EPDK_FAIL   fail
*Description : close the LCD display.
*Other       :
****************************************************************************************************
*/
__s32 dsk_display_lcd_off(void)
{
    __disp_output_type_t  	output;
	ES_FILE      		   *p_disp = NULL;
    
	/* open display driver */
	p_disp = eLIBs_fopen(DISP_DRV_PATH, "r+");
	if( p_disp == NULL )
	{
	    __err("Error in opening display driver.\n");
	    goto error;
	}

    /* get output type and format */
    
    output = (__disp_output_type_t)eLIBs_fioctrl(p_disp, DISP_CMD_GET_OUTPUT_TYPE, 0, 0);
    
	if( output == DISP_OUTPUT_TYPE_LCD )
	{		
		eLIBs_fioctrl(p_disp, DISP_CMD_LCD_OFF, 0, 0);	// note: 这句有延时
	}
    
	/* close display driver */
    eLIBs_fclose(p_disp);
    p_disp = NULL;
    
    return EPDK_OK;
    
error:
    if( p_disp != NULL )
    {
        eLIBs_fclose(p_disp);
        p_disp = NULL;
    }
    return EPDK_FAIL;
}


/*
****************************************************************************************************
*Name        : dsk_display_set_lcd_bright
*Prototype   : __s32 dsk_display_set_lcd_bright(__LION_BRIGHT_t level)
*Arguments   : level       input. bright level.
*Return      : EPDK_OK     succeed
*              EPDK_FAIL   fail
*Description : set the bright level of the LCD.
*Other       :
****************************************************************************************************
*/
__s32 dsk_display_set_lcd_bright(__lion_bright_t level)
{
    __disp_output_type_t  	output;
	ES_FILE      		   *p_disp = NULL;
	__disp_lcd_bright_t  	lcd_bright;
    __u32 arg[3];
	
	/* open display driver */
	p_disp = eLIBs_fopen(DISP_DRV_PATH, "r+");
	if( p_disp == NULL )
	{
	    __err("Error in opening display driver.\n");
	    goto error;
	}

    /* get output type and format */
    
    output = (__disp_output_type_t)eLIBs_fioctrl(p_disp, DISP_CMD_GET_OUTPUT_TYPE, 0, 0);
    
	if( output == DISP_OUTPUT_TYPE_LCD )
	{		
        switch(level)
    	{
    		case LION_BRIGHT_LEVEL1:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL0;
    			break;

    		case LION_BRIGHT_LEVEL2:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL1;
    			break;

    		case LION_BRIGHT_LEVEL3:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL2;
    			break;

    		case LION_BRIGHT_LEVEL4:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL3;
    			break;

    		case LION_BRIGHT_LEVEL5:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL4;
    			break;
    		
    		case LION_BRIGHT_LEVEL6:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL5;
    			break;
    		
    		case LION_BRIGHT_LEVEL7:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL6;
    			break;
    		
    		case LION_BRIGHT_LEVEL8:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL7;
    			break;
    		
    		case LION_BRIGHT_LEVEL9:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL8;
    			break;
    		
    		case LION_BRIGHT_LEVEL10:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL9;
    			break;
    		
    		case LION_BRIGHT_LEVEL11:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL10;
    			break;
    		
    		case LION_BRIGHT_LEVEL12:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL11;
    			break;
    		
    		case LION_BRIGHT_LEVEL13:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL12;
    			break;
    			
    		case LION_BRIGHT_LEVEL14:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL13;
    			break;
    			
    		case LION_BRIGHT_LEVEL15:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL14;
    			break;
    			
    		case LION_BRIGHT_LEVEL16:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL15;
    			break;
    			
    		default:
    			lcd_bright = DISP_LCD_BRIGHT_LEVEL11;
    			break;
    	}
		
		arg[0] = lcd_bright;
		arg[1] = 0;
		arg[2] = 0;
		eLIBs_fioctrl(p_disp, DISP_CMD_LCD_SET_BRIGHTNESS, 0, (void *)arg);		
    	
	}
    
	/* close display driver */
    eLIBs_fclose(p_disp);
    p_disp = NULL;
    
    return EPDK_OK;
    
error:
    if( p_disp != NULL )
    {
        eLIBs_fclose(p_disp);
        p_disp = NULL;
    }
    return EPDK_FAIL;
}


/*
****************************************************************************************************
*Name        : dsk_display_get_lcd_bright
*Prototype   : __LION_BRIGHT_t dsk_display_get_lcd_bright( void )
*Arguments   : void
*Return      : the bright level of the LCD.
*Description : get the bright level of the LCD.
*Other       :
****************************************************************************************************
*/
__lion_bright_t dsk_display_get_lcd_bright( void )
{
    __disp_output_type_t  	output;
	ES_FILE      		   *p_disp = NULL;
	__lion_bright_t			level;
	__disp_lcd_bright_t  	lcd_bright;
    
	/* open display driver */
	p_disp = eLIBs_fopen(DISP_DRV_PATH, "r+");
	if( p_disp == NULL )
	{
	    __err("Error in opening display driver.\n");
	    goto error;
	}

    /* get output type and format */
    level = LION_BRIGHT_LEVEL1;
    
    output = (__disp_output_type_t)eLIBs_fioctrl(p_disp, DISP_CMD_GET_OUTPUT_TYPE, 0, 0);
    
	if( output == DISP_OUTPUT_TYPE_LCD )
	{		
		lcd_bright = (__disp_lcd_bright_t)eLIBs_fioctrl(p_disp, DISP_CMD_LCD_GET_BRIGHTNESS, 0, 0);	
		switch(lcd_bright)
    	{
    		case DISP_LCD_BRIGHT_LEVEL0:
    			level = LION_BRIGHT_LEVEL1;
    			break;

    		case DISP_LCD_BRIGHT_LEVEL1:
    			level = LION_BRIGHT_LEVEL2;
    			break;

    		case DISP_LCD_BRIGHT_LEVEL2:
    			level = LION_BRIGHT_LEVEL3;
    			break;

    		case DISP_LCD_BRIGHT_LEVEL3:
    			level = LION_BRIGHT_LEVEL4;
    			break;

    		case DISP_LCD_BRIGHT_LEVEL4:
    			level = LION_BRIGHT_LEVEL5;
    			break;
    		
    		case DISP_LCD_BRIGHT_LEVEL5:
    			level = LION_BRIGHT_LEVEL6;
    			break;
    		
    		case DISP_LCD_BRIGHT_LEVEL6:
    			level = LION_BRIGHT_LEVEL7;
    			break;
    		
    		case DISP_LCD_BRIGHT_LEVEL7:
    			level = LION_BRIGHT_LEVEL8;
    			break;
    		
    		case DISP_LCD_BRIGHT_LEVEL8:
    			level = LION_BRIGHT_LEVEL9;
    			break;
    		
    		case DISP_LCD_BRIGHT_LEVEL9:
    			level = LION_BRIGHT_LEVEL10;
    			break;
    		
    		case DISP_LCD_BRIGHT_LEVEL10:
    			level = LION_BRIGHT_LEVEL11;
    			break;
    		
    		case DISP_LCD_BRIGHT_LEVEL11:
    			level = LION_BRIGHT_LEVEL12;
    			break;
    		
    		case DISP_LCD_BRIGHT_LEVEL12:
    			level = LION_BRIGHT_LEVEL13;
    			break;
    			
    		case DISP_LCD_BRIGHT_LEVEL13:
    			level = LION_BRIGHT_LEVEL14;
    			break;
    			
    		case DISP_LCD_BRIGHT_LEVEL14:
    			level = LION_BRIGHT_LEVEL15;
    			break;
    			
    		case DISP_LCD_BRIGHT_LEVEL15:
    			level = LION_BRIGHT_LEVEL16;
    			break;
    			
    		default:
    			level = LION_BRIGHT_;
    			break;
    	}
    			
	}
    
	/* close display driver */
    eLIBs_fclose(p_disp);
    p_disp = NULL;
    
    return level;
    
error:
    if( p_disp != NULL )
    {
        eLIBs_fclose(p_disp);
        p_disp = NULL;
    }
    return LION_BRIGHT_;
}

__s32 dsk_display_lcd_set_duty(__s32 duty)
{
    __disp_output_type_t  	output;
	ES_FILE      		   *p_disp = NULL;
	
	/* open display driver */
	p_disp = eLIBs_fopen(DISP_DRV_PATH, "r+");
	if( p_disp == NULL )
	{
	    __err("Error in opening display driver.\n");
	    return EPDK_FAIL;
	}

	eLIBs_fioctrl(p_disp, DISP_CMD_LCD_SET_DUTY, 0, &duty);		
	
    eLIBs_fclose(p_disp);
    p_disp = NULL;

    return EPDK_OK;
}

__s32 dsk_display_lcd_get_duty(void)
{
    __disp_output_type_t  	output;
	ES_FILE      		   *p_disp = NULL;
	__s32 ret;
	
	/* open display driver */
	p_disp = eLIBs_fopen(DISP_DRV_PATH, "r+");
	if( p_disp == NULL )
	{
	    __err("Error in opening display driver.\n");
	    return EPDK_FAIL;
	}

	ret = eLIBs_fioctrl(p_disp, DISP_CMD_LCD_GET_DUTY, 0, 0);		

    eLIBs_fclose(p_disp);
    p_disp = NULL;

    return ret;
}


//#if ADD_LCD_BRIGHT_SATURATION_MORE_SET
__s32  dsk_display_set_panel_bright(__s32  value)
{ 
	__s32       buf[3];
	int ret;

	ES_FILE* g_display_shell = NULL;
	
	g_display_shell  = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if( !g_display_shell )
	{
	    __msg("Open display driver failed, test program exits.\n");
	    return 0;
	}
	ret=eLIBs_fioctrl(g_display_shell, DISP_CMD_ENHANCE_ON, 0, (void *)0);
	   
	buf[0] = value;
	buf[1] = 0;
	buf[2] = 0;     
   	eLIBs_fioctrl(g_display_shell, DISP_CMD_SET_BRIGHT, 0, (void *)buf);

	//__msg("write bright[0x%x],[%d]\n", value, value);

	ret = eLIBs_fioctrl(g_display_shell, DISP_CMD_GET_BRIGHT, 0, (void *)0);
       __msg(" second read bright[0x%x][%d]\n", ret, ret);

	eLIBs_fclose(g_display_shell);
	g_display_shell = NULL;
	
	return ret;
}

__s32  dsk_display_get_panel_bright(void)
{ 
	__s32       buf[3];
	int ret;

	ES_FILE* g_display_shell = NULL;
	
	g_display_shell  = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if( !g_display_shell )
	{
	    __msg("Open display driver failed, test program exits.\n");
	    return 0;
	}
	ret=eLIBs_fioctrl(g_display_shell, DISP_CMD_ENHANCE_ON, 0, (void *)0);
	  
	ret = eLIBs_fioctrl(g_display_shell, DISP_CMD_GET_BRIGHT, 0, (void *)0);
       __msg(" second read bright[0x%x][%d]\n", ret, ret);

	eLIBs_fclose(g_display_shell);
	g_display_shell = NULL;
	
	return ret;
}

__s32  dsk_display_set_lcd_constract(__s32  value)
{ 
	__s32       buf[3];
	int ret;

	ES_FILE* g_display_shell = NULL;
	
	g_display_shell  = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if( !g_display_shell )
	{
	    __msg("Open display driver failed, test program exits.\n");
	    return 0;
	}
	ret=eLIBs_fioctrl(g_display_shell, DISP_CMD_ENHANCE_ON, 0, (void *)0);
	   
	buf[0] = value;
	buf[1] = 0;
	buf[2] = 0;     
   	eLIBs_fioctrl(g_display_shell, DISP_CMD_SET_CONTRAST, 0, (void *)buf);

	//__msg("write constract[0x%x],[%d]\n", value, value);

	ret = eLIBs_fioctrl(g_display_shell, DISP_CMD_GET_CONTRAST, 0, (void *)0);
       __msg(" second read constract[0x%x][%d]\n", ret, ret);

	eLIBs_fclose(g_display_shell);
	g_display_shell = NULL;
	
	return ret;
}

__s32  dsk_display_get_lcd_constract(void)
{ 
	__s32       buf[3];
	int ret;

	ES_FILE* g_display_shell = NULL;
	
	g_display_shell  = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if( !g_display_shell )
	{
	    __msg("Open display driver failed, test program exits.\n");
	    return 0;
	}
	ret=eLIBs_fioctrl(g_display_shell, DISP_CMD_ENHANCE_ON, 0, (void *)0);
	  
	ret = eLIBs_fioctrl(g_display_shell, DISP_CMD_GET_CONTRAST, 0, (void *)0);
       __msg(" second read constract[0x%x][%d]\n", ret, ret);

	eLIBs_fclose(g_display_shell);
	g_display_shell = NULL;
	
	return ret;
}

__s32  dsk_display_set_lcd_saturation(__s32  value)
{ 
	__s32       buf[3];
	int ret;

	ES_FILE* g_display_shell = NULL;
	
	g_display_shell  = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if( !g_display_shell )
	{
	    __msg("Open display driver failed, test program exits.\n");
	    return 0;
	}
	ret=eLIBs_fioctrl(g_display_shell, DISP_CMD_ENHANCE_ON, 0, (void *)0);
	   
	buf[0] = value;
	buf[1] = 0;
	buf[2] = 0;     
   	eLIBs_fioctrl(g_display_shell, DISP_CMD_SET_SATURATION, 0, (void *)buf);

	//__msg("write saturation[0x%x],[%d]\n", value, value);

	ret = eLIBs_fioctrl(g_display_shell, DISP_CMD_GET_SATURATION, 0, (void *)0);
       __msg(" second read saturation[0x%x][%d]\n", ret, ret);

	eLIBs_fclose(g_display_shell);
	g_display_shell = NULL;
	
	return ret;
}

__s32  dsk_display_get_lcd_saturation(void)
{ 
	__s32       buf[3];
	int ret;

	ES_FILE* g_display_shell = NULL;
	
	g_display_shell  = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if( !g_display_shell )
	{
	    __msg("Open display driver failed, test program exits.\n");
	    return 0;
	}
	ret=eLIBs_fioctrl(g_display_shell, DISP_CMD_ENHANCE_ON, 0, (void *)0);
	  
	ret = eLIBs_fioctrl(g_display_shell, DISP_CMD_GET_SATURATION, 0, (void *)0);
       __msg(" second read saturation[0x%x][%d]\n", ret, ret);

	eLIBs_fclose(g_display_shell);
	g_display_shell = NULL;
	
	return ret;
}

__s32  dsk_display_set_lcd_hue(__s32  value)
{ 
	__s32       buf[3];
	int ret;

	ES_FILE* g_display_shell = NULL;
	
	g_display_shell  = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if( !g_display_shell )
	{
	    __msg("Open display driver failed, test program exits.\n");
	    return 0;
	}
	ret=eLIBs_fioctrl(g_display_shell, DISP_CMD_ENHANCE_ON, 0, (void *)0);
	   
	buf[0] = value;
	buf[1] = 0;
	buf[2] = 0;     
   	eLIBs_fioctrl(g_display_shell, DISP_CMD_SET_HUE, 0, (void *)buf);

	//__msg("write saturation[0x%x],[%d]\n", value, value);

	ret = eLIBs_fioctrl(g_display_shell, DISP_CMD_GET_HUE, 0, (void *)0);
       __msg(" second read saturation[0x%x][%d]\n", ret, ret);

	eLIBs_fclose(g_display_shell);
	g_display_shell = NULL;
	
	return ret;
}

__s32  dsk_display_get_lcd_hue(void)
{ 
	__s32       buf[3];
	int ret;

	ES_FILE* g_display_shell = NULL;
	
	g_display_shell  = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if( !g_display_shell )
	{
	    __msg("Open display driver failed, test program exits.\n");
	    return 0;
	}
	ret=eLIBs_fioctrl(g_display_shell, DISP_CMD_ENHANCE_ON, 0, (void *)0);
	  
	ret = eLIBs_fioctrl(g_display_shell, DISP_CMD_GET_HUE, 0, (void *)0);
       __msg(" second read saturation[0x%x][%d]\n", ret, ret);

	eLIBs_fclose(g_display_shell);
	g_display_shell = NULL;
	
	return ret;
}

__s32  dsk_display_set_lcd_vcom(__s32  value)
{ 
	ES_FILE* fp = NULL;
	__s32 file_size;
	char path[] = BEETLES_VCOM_PATH;
	__s32 ret;	
	char *tmp;
	char vcom_buf[10];
	__s32 result=0;

    __disp_output_type_t  	output;
	ES_FILE      		   *p_disp = NULL;
	
	/* open display driver */
	p_disp = eLIBs_fopen(DISP_DRV_PATH, "r+");
	if( p_disp == NULL )
	{
	    __err("Error in opening display driver.\n");
	    return EPDK_FAIL;
	}

	eLIBs_fioctrl(p_disp, DISP_CMD_LCD_SET_DUTY, 0, &value);		
	
    eLIBs_fclose(p_disp);
    p_disp = NULL;

	//写文件
    fp = eLIBs_fopen(path, "w+");
    if(fp == NULL)
    {
        eLIBs_printf(" -- %s, %d, open file fail:%s\n",__FILE__, __LINE__, path);
		return EPDK_FAIL;
	}
	else
	{
		eLIBs_int2str_dec(value, vcom_buf);
		eLIBs_strcat(vcom_buf, "\0");
		result = eLIBs_fwrite(vcom_buf, 1, sizeof( char )*eLIBs_strlen(vcom_buf), fp);
		eLIBs_fclose(fp);
		fp = NULL;
	}
		
	return EPDK_OK;
}

__s32  dsk_display_get_lcd_vcom(void)
{ 
	ES_FILE* fp = NULL;
	__s32 file_size;
	char path[] = BEETLES_VCOM_PATH;
	__s32 ret;	
	char *tmp;
	__s32 vcom_default = 0;
		
	fp = eLIBs_fopen(path , "r");
	
	if(fp)
	{
		eLIBs_fseek( fp , 0 , SEEK_END );
		file_size = eLIBs_ftell( fp );
		eLIBs_printf(" -- %s, %d,  eLIBs_fread 1, file_size=%d -- \n",__FILE__, __LINE__, file_size);

		eLIBs_fseek( fp , 0 , SEEK_SET);
		tmp =  (char *)eLIBs_malloc( file_size * sizeof( char ) );
		eLIBs_fread( tmp , file_size , sizeof(char) , fp);

		eLIBs_printf(" -- %s, %d,  eLIBs_fread 2, tmp=%s -- \n",__FILE__, __LINE__, tmp);	
		vcom_default = eLIBs_atoi(tmp);

		eLIBs_fclose(fp);
		fp = NULL;
	}
	else
	{
		eLIBs_printf(" -- %s, %d,  Open Vcom.txt Fail !! -- \n",__FILE__, __LINE__);	
	}

	return vcom_default;
}

//#endif


#endif     //  ifndef __DSK_DISPLAY_LCD_C__

/* end of dsk_display_lcd.c */
