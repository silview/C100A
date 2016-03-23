/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									          shell application
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : Esh_shell.c
* By      : sunny
* Version : V2.0                                                            2009.3.2
*********************************************************************************************************
*/

#include    "Esh_shell_private.h"

#define MCU_TRY_TIMES		120

//支持logo拉伸
#define		SUPPORT_ZOOM_OUT_LOGO	0

__hdle logo_layer = NULL;
__bool pass_flag = EPDK_FALSE;
Picture_t * picture = NULL;

static void __Esh_Begin_Info(void)
{
    __inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
    __inf("&                                                                                                                   &\n");
    __inf("&                                                      FFBBEE                                                       &\n");
    __inf("&                          I BBBBBBBBBBBB            I BBBB                            MMI         BBLL             &\n");
    __inf("&                    BBBBB@STEVEN@BMMBBBBBB          BBBBI                           BBBBFF      BBBBFF             &\n");
    __inf("&                  BBBBBBBBMMBBBBBBMMEEBBLL        BBBBBB                          BBBBBB::    BBBBBB..             &\n");
    __inf("&                BBBBBBBBBB        LLBB.         BBBBBB                          BBBB  BB    EEBB  BB               &\n");
    __inf("&                BBBB..          BB            I BBBB.                           BBI FFLL    BB..FFFF               &\n");
    __inf("&              I BBFF                          BBBBLL                          BBBB  BB    BBBB  BB                 &\n");
    __inf("&              ::BBBBBB                      BBBBBB    BB          BBBBBBBB    BBLLBB::    BBLLBB::                 &\n");
    __inf("&                LLBBBBBBBBBB              FFBBBB  . BBBBBB    . BBBB  BBFF  BBBBBBEE    BBBBBBBB                   &\n");
    __inf("&                      BBBBBBEEBB          BBBB  LLBBBBBB      BBBB  EEBB    BBBBBB      BBBBBB                     &\n");
    __inf("&                              BBBB::    BBBB. EEBBI BB      BBBB  BBEE    . BBBB      . BBBB                       &\n");
    __inf("&                                BBBB    MMBBLLBB  BBLL    ..BBBBBB        I BB::      BBBB.       ..               &\n");
    __inf("&                              BBBBBB    BBEEBB::LLBB    BBFFBBEE      LLBBEEBB::    BBBBBB..    BB                 &\n");
    __inf("&          BB::          BBBBBBBBBBFF    BBBBFF  BBBBBBBB  I BBBB    BBBB  I BBBB::BB::FFBBBB..BB..                 &\n");
    __inf("&        BBBBBBBBMMBBBBBBBBMMMMBBBB      BBBB    BBBBBB      BBBBBBBBBB      BBBBBBI     BBBBBBI                    &\n");
    __inf("&        BBBB@V1.0@BBMMBBBBBBBB                                BBBB            BB          BB                       &\n");
    __inf("&            ::BBBBMMBB                                                                                             &\n");
    __inf("&                                                                                                                   &\n");
    __inf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");

    __inf("-----Please input cmd after \"work_dir>:\"-----------------------------------------------------------------------------\n");
    __inf("-----Input \"help\" command for more information!----------------------------------------------------------------------\n");
    __inf("-----Give your requirement or sugguestion to sunny for improvement---------------------------------------------------\n");
    __inf("---------------------------------------------------------------------------------------------------------------------\n");
    __inf("\n");
}

static __s32 __show_logo(void)
{
	ES_FILE      * p_disp;
	__s32		width, height;
	__disp_layer_info_t layer_para;
    __u32 arg[3];
    __s32 ret;
    
	    
	//parse logo bmp	
	picture = (Picture_t *)eLIBs_malloc(sizeof(Picture_t));
	if(picture == NULL)
	{
		__wrn("malloc logo bmp buffer failed!\n");
		return EPDK_FAIL;
	}
    eLIBs_memset(picture, 0, sizeof(Picture_t));
    
	ret = Parse_Pic_BMP_ByPath(BEETLES_APP_ROOT"res\\boot_ui\\logo.bmp", picture, NULL);
	if(-1 == ret)
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

	width  = eLIBs_fioctrl(p_disp, DISP_CMD_SCN_GET_WIDTH, 0, 0);
	height = eLIBs_fioctrl(p_disp, DISP_CMD_SCN_GET_HEIGHT, 0, 0);

	__msg("width = %d, height = %d\n", width, height);
    arg[0] = DISP_LAYER_WORK_MODE_NORMAL;
    arg[1] = 0;
    arg[2] = 0;
    logo_layer = eLIBs_fioctrl(p_disp, DISP_CMD_LAYER_REQUEST, 0, (void*)arg);

#if SUPPORT_ZOOM_OUT_LOGO
    layer_para.mode = DISP_LAYER_WORK_MODE_SCALER; 
#else
    layer_para.mode = DISP_LAYER_WORK_MODE_NORMAL; 
#endif
    layer_para.fb.addr[0]       = (__u32)picture->Buffer; 
    layer_para.fb.size.width    =  picture->Width;
    layer_para.fb.size.height    = picture->Height;
    layer_para.fb.mode          = DISP_MOD_INTERLEAVED;
    layer_para.fb.format        = DISP_FORMAT_ARGB8888;
    layer_para.fb.br_swap       = 0;
    layer_para.fb.seq           = DISP_SEQ_ARGB;
    layer_para.ck_enable        = 0;
    layer_para.alpha_en         = 1; 
    layer_para.alpha_val        = 0xff;
    layer_para.pipe             = 0; 
    layer_para.src_win.x        = 0;
    layer_para.src_win.y        = 0;
    layer_para.src_win.width    = picture->Width;
    layer_para.src_win.height   = picture->Height;

#if SUPPORT_ZOOM_OUT_LOGO
    layer_para.scn_win.x        = 0;
    layer_para.scn_win.y        = 0;
    layer_para.scn_win.width    = width;
    layer_para.scn_win.height   = height;
#else
    layer_para.scn_win.x        = (width-picture->Width)>>1;
    layer_para.scn_win.y        = (height-picture->Height)>>1;
    layer_para.scn_win.width    = picture->Width;
    layer_para.scn_win.height   = picture->Height;
#endif
	__msg("scn_win.x = %d, scn_win.y = %d\n", layer_para.scn_win.x, layer_para.scn_win.y);
    __msg("picture->Width = %d, picture->Height = %d\n", picture->Width, picture->Height);
    arg[0] = logo_layer;
    arg[1] = (__u32)&layer_para;
    arg[2] = 0;
    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_SET_PARA,0,(void*)arg);

    arg[0] = logo_layer;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_OPEN,0,(void*)arg);

	eLIBs_fioctrl(p_disp, DISP_CMD_LCD_ON, 0, 0);
	
	eLIBs_fclose(p_disp);

	return EPDK_OK;
}

static __s32 __key_graber(void *packet)
{		
	__u32 i;
	__input_event_packet_t *pPacket;
    __input_event_t        *pEventFrame;				
	
    if (packet == NULL)
    {
        __msg("invalid argument for call back\n");
        return EPDK_FAIL;
    }
    
    //dump packet events
    pPacket = (__input_event_packet_t *)packet;
    if (pPacket->event_cnt > INPUT_MAX_EVENT_NR)
    {
        __msg("invalid event count number\n");
        return EPDK_FAIL;
    }
	__msg("------------------------------------------------\n");
	for (i = 0; i < pPacket->event_cnt; i++)
	{
		pEventFrame = &(pPacket->events[i]);
		__msg("Event %d: type = %d, code = %d, value = %d\n", i + 1,
                                                                 pEventFrame->type,
                                                                 pEventFrame->code,
                                                                 pEventFrame->value
                                                                 );
		if(pEventFrame->type == EV_KEY) 
		{	  
			pass_flag = EPDK_TRUE;			
		}		
	}	
	__msg("------------------------------------------------\n");			
	
	return EPDK_OK;
}

static __s32 test_data[][2] =
{
    121545, 43251,
    40907, 64809,
    14724, 171149,
    115631, 73756
};

static __s32 test_read_nor(void)
{
	return 0;
	#if 0
    __s32 result;

    result = EPDK_TRUE;

    eLIBs_printf("********begin test_read_nor***********\n");
    
     do{//nor 读操作测试代码
        __s32 ret;
        __s32 i, j;
        __s32 cur_data;
        __s32 page_num;
        __s32 once_count;    
        __s32 start_pos;
        __s32 file_size;
        unsigned char * pbuf;
        char path[]=BEETLES_APP_ROOT"apps\\nor_reading_test.bin";
        ES_FILE* fp;
        __s32 test_num;

        //test_num = sizeof(test_data)/sizeof(test_data[0]);
        test_num = 10;
        
        //open file
        fp = eLIBs_fopen(path, "rb");
        if(!fp)
        {
            result = EPDK_FALSE;
            eLIBs_printf("open file fail, path=%s\n", path);
            break;
        }

        //get file size
        eLIBs_fseek(fp, 0, SEEK_END);
        file_size = eLIBs_ftell(fp);
        eLIBs_printf("file_size=%d\n", file_size);

        //cal page size
        page_num = (file_size+1023)>>10;
        eLIBs_printf("page_num=%d\n", page_num);

        //alloc page mem
        pbuf = esMEMS_Palloc(page_num, 0);
        if(!pbuf)
        {
            result = EPDK_FALSE;
            eLIBs_printf("esMEMS_Palloc fail\n");
            eLIBs_fclose(fp);
            break;
        }

        //begin test
        for(i = 0 ; i < test_num; i++)
        {
            eLIBs_printf("test3\n");
            //generate start reading pos
            start_pos = eLIBs_ramdom(file_size-2);
            //start_pos = test_data[i][0];
            //start_pos = 80;
            eLIBs_printf("test4\n");
            //generate reading count
            once_count = eLIBs_ramdom(file_size-start_pos);
            //once_count = test_data[i][1];
            //once_count = 45699;
            eLIBs_printf("test5\n");
            if(0 == once_count)
            {
                once_count = 1;
            }

            //seek and read
            eLIBs_printf("before eLIBs_fseek\n");
            eLIBs_fseek(fp, start_pos, SEEK_SET);
            eLIBs_printf("after eLIBs_fseek\n");
            eLIBs_printf("before eLIBs_fread\n");
            ret = eLIBs_fread(pbuf, 1,once_count, fp);
            eLIBs_printf("after eLIBs_fread\n");
            if(ret != once_count)
            {
                result = EPDK_FALSE;
                eLIBs_printf("eLIBs_fread fail, once_count=%d, ret=%d\n", once_count, ret);
                break;
            }

            //veryfy data
            cur_data = start_pos%256;
            for(j = 0 ; j < once_count ; j++)
            {
                if(cur_data != pbuf[j])
                {
                    break;
                }
                cur_data++;
                if(cur_data >= 256)
                {
                    cur_data = 0;
                }
            }

            if(j < once_count)//has err data
            {
                result = EPDK_FALSE;
                eLIBs_printf("***************err encounter, begin dump****************\n");
                eLIBs_printf("***************err encounter, begin dump****************\n");
                eLIBs_printf("***************err encounter, begin dump****************\n");
                eLIBs_printf("***************err encounter, begin dump****************\n");
                    
                eLIBs_printf("test bad, i=%d, start_pos=%d, once_count=%d, cur_data=%d, pbuf[%d]=%d, err_pos=%d, (start_pos+j)/1024=%f\n"
                    , i, start_pos, once_count, cur_data, j, pbuf[j], start_pos+j, (start_pos+j)/1024.0);
                __wait__;
                
                if(1){//dump data
                    __s32 k;
                    eLIBs_printf("***************err encounter, begin dump****************\n");
                    eLIBs_printf("***************err encounter, begin dump****************\n");
                    eLIBs_printf("***************err encounter, begin dump****************\n");
                    eLIBs_printf("***************err encounter, begin dump****************\n");
                    for(k = 0 ; k < once_count ; k++)
                    {
                        eLIBs_printf("pbuf[%.6d]=%X\n", k, pbuf[k]);
                    }
                    eLIBs_printf("***************end dump****************\n");
                }
                
                //break;
            }
            else//test alright
            {
                eLIBs_printf("test alright, i=%d, start_pos=%d, once_count=%d\n"
                    , i, start_pos, once_count);
            }
            eLIBs_printf("test1\n");
            esKRNL_TimeDly(2);
            eLIBs_printf("test2\n");
            
        }
        
        eLIBs_fclose(fp);
        esMEMS_Pfree(pbuf, page_num);       
        
    }while(0);

    eLIBs_printf("********end test_read_nor, result=%d***********\n", result);
    __wait__;
    return result;
	#endif
}

static __s32 test_flush_nor(__s32 i)
{
	#if 0
	__s32 result;
	ES_FILE* fp = NULL;
    char buf[3000]={0};
    char path[] = "e:\\test.bin";

    eLIBs_printf("i=%d\n", i);
    if(1)//
    {    	
    	fp = eLIBs_fopen(path, "wb");
    	if(fp == NULL)
    	{
    		eLIBs_printf("open file fail:%s\n", path);
    		return EPDK_FAIL;
    	}
    	
    	result = eLIBs_fwrite(buf, 1, sizeof(buf), fp);
    	if(result < sizeof(buf))
    	{
            eLIBs_fclose(fp);
    		eLIBs_printf("write file fail:%s\n", path);
    		return EPDK_FAIL;
    	}
    	
    	eLIBs_fclose(fp);


        {//
            __s32 i, num;
            ES_FILE* hFile;

            for(i = 0 ; i < 1 ; i++)
            {
                //flush user disk data to NOR flash
                hFile = eLIBs_fopen("b:\\STORAGE\\SPINOR", "rb");
                if (hFile == NULL)
                {
                    eLIBs_printf("open NOR file failed\n");
                    return EPDK_FAIL;
                }
                eLIBs_printf("i=%d flush file ....\n", i);
                eLIBs_fioctrl(hFile, DEV_IOC_USR_FLUSH_CACHE, 0, 0);
                
                eLIBs_printf("i=%d flush end ....\n", i);
                eLIBs_fclose(hFile);

                esKRNL_TimeDly(2);
            }
        }
    }  
	#endif
	return EPDK_OK;
}


__s32 shellmain(void *p_arg)
{
    __target_para_t target;
    __s32 result=0, i;
  	__s32 ret = EPDK_FAIL;

	ES_FILE* fp;
	__s32 vcom_val = 0;
	__s32 vcom_file = 0, vcom_default = VCOM_DEFAULT_LEVEL;
	
    __Esh_Begin_Info();     
    
    esKSRV_GetTargetPara(&target);
    if(target.debugenable == EPDK_TRUE)
    {
        __here__;
        esDEV_Plugin("drv\\sdmmc.drv", 0, 0, 1);  //112350 如果是调试，则加载sdmmc
    }  



    //esDEV_Plugin("\\drv\\key.drv", 0, 0, 1);
	//Esh_printf("huang    plugin adc key  \n");
    esDEV_Plugin("\\drv\\display.drv", 0, 0, 1);    
    
    if(0){
        __s32 i;
        
        for(i = 0 ; i < 300 ; i++)
        {
            test_flush_nor(i);
        }
    }
    //test_read_nor();   

	__msg(".Display Device Installed!...........................................................................................\n");

#ifdef BEETLES_SW_VCOM

	//VCOM取初值;
	{
		ES_FILE* fp = NULL;
		__s32 file_size;
		char path[] = BEETLES_VCOM_PATH;
		__s32 ret;	
		char *tmp;
			
		fp = eLIBs_fopen(path , "r");
		if(!fp)
		{
		    //eLIBs_printf(" -- %s, %d,  open file fail:%s\n",__FILE__, __LINE__,  path);
				char* config_path ="d:\\app_config.bin";
				 H_PARSER parser;
				__s32 ret;
				__s32 i;
				char keyname[64];
				int val;
				// char str_val[32];
				char vcom_buf[10];
				__here__;		
				do{
				    parser = NULL;

				    parser = esCFG_Init_Ex(config_path);
				    if(NULL == parser)
				    {
				        __wrn("esCFG_Init_Ex fail...\n");
				        break;
				    }	

				     eLIBs_memset(keyname, 0, sizeof(keyname));
				    eLIBs_strcpy(keyname, "system");
				    eLIBs_strcat(keyname, "/");
				    eLIBs_strcat(keyname, "lcd_vcom");                        
				    ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
				    __wrn("after esCFG_GetKeyValue_Ex, ret=%d\n", ret);
				    if(EPDK_OK == ret)
				    {
				        if(val < 0 || val > VCOM_MAX_LEVEL)
				        {
				            val = VCOM_DEFAULT_LEVEL;
				        }
						__here__;        
				        vcom_default = val;
						eLIBs_printf(" -- %s, %d,  eLIBs_fread 2, vcom_default=%d -- \n",__FILE__, __LINE__, vcom_default);	
						//vcom_default = eLIBs_atoi(tmp);						
				        __wrn("default vcom_default=%d\n", vcom_default);
				    } 
					
				    if(parser)
				    {
				        esCFG_Exit_Ex(parser);
				        parser = NULL;
				    }					
		       }while(0);	
		//写入txt文件
		#if 0
			{
				fp = eLIBs_fopen(path , "w+");
				if(fp)
				{
					eLIBs_int2str_dec(vcom_default, vcom_buf);
					eLIBs_strcat(vcom_buf, "\0");
					result = eLIBs_fwrite(vcom_buf, 1, sizeof( char )*eLIBs_strlen(vcom_buf), fp);
					eLIBs_fclose(fp);
					fp = NULL;
				}
				else
					eLIBs_printf(" -- %s, %d,  shell create Vcom.txt Fail !! -- \n",__FILE__, __LINE__);
			}
		#endif
		__here__;
		}
		else
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
	}
	//设置初值;
	if(!vcom_file)
	{
	
		{
		    __disp_output_type_t  	output;
			ES_FILE      		   *p_disp = NULL;
			
			/* open display driver */
			p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
			if( p_disp == NULL )
			{
			    __err("Error in opening display driver.\n");
			    return EPDK_FAIL;
			}

			eLIBs_fioctrl(p_disp, DISP_CMD_LCD_SET_DUTY, 0, &vcom_default);		
			
		    eLIBs_fclose(p_disp);
		    p_disp = NULL;
		}
	}
		
#endif	

    //esMEMS_Info();
	__show_logo();

    //test flush nor
 //   __wait__;
    //helloworld_main(NULL);


//test open file
{
 //   __hdle fp;
 //   fp = esFSYS_fopen(BEETLES_APP_ROOT"apps\\lang.bin", "r");
  //  __msg("fp=%x file=%s\n", fp, BEETLES_APP_ROOT"apps\\lang.bin");
 //   esFSYS_fclose(fp);
}   
    //__wait__;
	
    /* print shell begin information */    

   	Esh_printf(".....................................................................................................................\n");
   	Esh_printf(".   huang   Please press enter key(keypad or irkey) to continue!\n..............................................................\n");
   	Esh_printf(".....................................................................................................................\n");

    esKRNL_TimeDly(200);

#if 0
    esKSRV_GetTargetPara(&target);
	esDEV_Plugin("\\drv\\ir.drv",0,0,1);
	//esDEV_Plugin("\\drv\\matrixkey.drv",0,0,1);
	esDEV_Plugin("\\drv\\rocker.drv",0,0,1);
	
    __msg("target.debugenable == %d\n", target.debugenable);
    if(target.debugenable == EPDK_TRUE)
    {
	    __hdle  		h_keyGraber;
		//esDEV_Plugin("\\drv\\ir.drv", 0, 0, 1);
        //enable device monitor       
        if(0)
        {
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

		{			
			h_keyGraber = esINPUT_LdevGrab(INPUT_LKEYBOARD_DEV_NAME, (__pCBK_t)__key_graber, 0, 0);
		    if (h_keyGraber == NULL)
		    {
		        eLIBs_printf("shell grab logical keyboard failed\n");		        
		    }
		}
		
        while(1)
        {            	
            if(pass_flag)
        	{
        		if (h_keyGraber)
        		{
        			esINPUT_LdevRelease(h_keyGraber);
        		}
				
        		break;
        	}			
            esKRNL_TimeDly(10);
        }
		
    }
#else
#if 0
 	//enable device monitor
 	{
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
#endif
#endif

    /* Eshell initialize  */
    if ( ESH_FAILED == Esh_init() ) {
        Esh_Error("Eshell initialize failed\n");
        return ESH_FAILED;
    }
    /* execute setup first */
    Esh_StartUp();

	/*
	if(target.debugenable == EPDK_TRUE)
	{
		Esh_ReaderLoop();
	}
	*/

    /* Eshell exit */
    Esh_exit();

	/* kill shell self */
    esEXEC_PDel(EXEC_pidself);

    /* del shell main thread */
    Esh_Msg("shell main thread: Bye Bye!\n");
    esKRNL_TDel(OS_PRIO_SELF);
    esKRNL_TimeDly(1000);

    return ESH_SUCCEED;
}
