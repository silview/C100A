/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              apps Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : Shell
* File    : startx.c
*
* By      : Steven
* Version : vx.xx.xx
* Date    : (please press f7 here)
**********************************************************************************************************************
*/

#include    "Esh_shell_private.h"

static __u8 mid_mod_lemon;
static __mp * mod_lemon;

static __u8 mid_mod_desktop;
static __mp * mod_desktop;

static __s32 __close_logo(void)
{
	ES_FILE      * p_disp;	
    __u32 arg[3];

    if(!logo_layer)
    {
        return -1;
    }

	/*open display driver handle*/
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		Esh_Wrn("open display device fail!\n");
		return EPDK_FAIL;
	}	
	//close logo
	arg[0] = logo_layer;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_CLOSE,0,(void*)arg);

	eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_RELEASE,0,(void*)arg);

	eLIBs_fclose(p_disp);   
    
	return EPDK_OK;
}

static __s32 __release_logo_buf(void)
{
    if(picture)
    {
        if(picture->Buffer)
        {
	        esMEMS_Bfree(picture->Buffer, picture->BufferSize);
            picture->Buffer = NULL;
        }
	    eLIBs_free(picture);
        picture = NULL;
    }
    
    return EPDK_OK;
}
static void __exec_startx(void)
{
    __u32 bid, sid, pid, chip;
	__u32 arg[3];
	
	//__getc();
		
    __msg("after close logo\n");

	Esh_printf("1\n");
    mid_mod_lemon = esMODS_MInstall(BEETLES_APP_ROOT"mod\\orange.mod", 0);

    Esh_printf("2\n");

	if(!mid_mod_lemon)
	{
		Esh_Error("orange.mod install fail!\n");
	}

    Esh_printf("3\n");
	mod_lemon = esMODS_MOpen(mid_mod_lemon, 0);

	if(mod_lemon == 0)
	{
		Esh_Error("orange.mod start fail!\n");
	}
	Esh_printf(".orange Installed and Openned!........................................................................................\n");
	
	mid_mod_desktop = esMODS_MInstall(BEETLES_APP_ROOT"mod\\desktop.mod", 0);

	if( !mid_mod_desktop )
	{
		Esh_Error("desktop.mod install fail!\n");
	}
    Esh_printf("4\n");

    if(1)
    {//∑¿÷πlogo¡¢øÃœ˚ ß
        ES_FILE      * p_disp;

        /*open display driver handle*/
        p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
        if(p_disp)
        {
            /*start hardware cache layer*/
            eLIBs_fioctrl(p_disp, DISP_CMD_START_CMD_CACHE, SEL_SCREEN, 0);
            eLIBs_fclose(p_disp);        
        }        
    }

    __close_logo();
    
	mod_desktop = esMODS_MOpen(mid_mod_desktop, 0);

	if( mod_desktop == 0)
	{
		Esh_Error("desktop.mod start fail!\n");
	}

    esKRNL_TimeDly(50);
    if(1)
    {
        ES_FILE      * p_disp;

        /*open display driver handle*/
        p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
        if(p_disp)
        {
            /*start hardware cache layer*/
            eLIBs_fioctrl(p_disp, DISP_CMD_EXECUTE_CMD_AND_STOP_CACHE, SEL_SCREEN, 0);
            eLIBs_fclose(p_disp);        
        }        
    }

    __release_logo_buf();

    Esh_printf("5\n");

	Esh_printf(".Desktop Installed and Openned!........................................................................................\n");

    /*enable device monitor */
    if(0){
        ES_FILE     *pHwsc;

        pHwsc = eLIBs_fopen("b:\\HWSC\\hwsc", "rb+");
        if(pHwsc)
        {
            eLIBs_fioctrl(pHwsc, DEV_IOC_USR_HWSC_ENABLE_MONITOR, 0, NULL);
            eLIBs_fclose(pHwsc);
            __inf(".start device monitor .......................................\n");
        }
        else
        {
            __err("try to open b:\\HWSC\\hwsc failed!\n");
        }
    }
}
/*
*****************************************************************************
*                            dostartx
*
*  Description: start lemon.mod
*
*
*  Parameters:
*
*  Return value:
*****************************************************************************
*/
int dostartx(struct op *t , const char **args)
{
    /* do startx core: write by Steven */
    __exec_startx();

    /* no any error */
	return DOSUCCEED;
}
