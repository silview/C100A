/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           calendar app sample
*
*						        	(c) Copyright 2006-2009, ANDY, China
*											 All Rights Reserved
*
* File    : sset_menu.c
* By      :lyn
* Version : V1.00
*********************************************************************************************************
*/
#include "sset_i.h"

static	tbar_para_t		*pmenu_para = 0;

/*
************************************************************************************************************************
*                       				SSET_MenuTbar_Create
*
*Description: 创建dsk_tbar
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
H_WIN SSET_MenuTbar_Create( H_WIN hparent, H_WIN hlayer, __sset_item_t *pitem )
{
	__u32						i = 0;
	__u32						tbar_width;
	H_WIN						h_win;
	tbar_item_t					*pmenu_item;
	__sset_item_t				*user_para;
	__sset_item_attr_t			*pitem_attr;
	__sset_uipara_t				*puipara;	
	__sset_create_para_t		*psset; 			
	
	puipara = (__sset_uipara_t *)GUI_WinGetAddData(hparent);
	psset = (__sset_create_para_t *)GUI_WinGetAttr(hparent);
	pmenu_para = (tbar_para_t *)esMEMS_Balloc( sizeof(tbar_para_t) );
	if(!pmenu_para)
	{
		__err("malloc error \n");
		return 0;
	}
	eLIBs_memset( pmenu_para, 0, sizeof(tbar_para_t) );	
	
	user_para = pitem;
	while(user_para)
	{
		i++;
		user_para = user_para->next;
	}

	pmenu_item = (tbar_item_t *)esMEMS_Balloc( sizeof(tbar_item_t)*i );
	if(!pmenu_item)
	{
		__err("malloc error \n");
		return 0;
	}	
	eLIBs_memset( pmenu_item, 0, sizeof(tbar_item_t)*i );
		
	pmenu_para->item			= pmenu_item;
	pmenu_para->item_nr			= i;

	pmenu_para->frame_size.width 	= puipara->menu_tbar_item.w;
	pmenu_para->frame_size.height	= puipara->menu_tbar_item.h;
	pmenu_para->bmp_pos.x    		= puipara->menu_tbar_bmp.x;
	pmenu_para->bmp_pos.y   		= puipara->menu_tbar_bmp.y;
	pmenu_para->text_pos.x	   		= puipara->menu_tbar_txt.x;
	pmenu_para->text_pos.y			= puipara->menu_tbar_txt.y;

	pmenu_para->focus_color		= /*GUI_RED*/APP_COLOR_YELLOW;
	pmenu_para->unfocus_color	= /*GUI_YELLOW*/APP_COLOR_WHITE;
	pmenu_para->draw_font		= SWFFont;
	user_para = pitem;
	
	tbar_width = pmenu_para->item_nr * pmenu_para->frame_size.width;	
	if(tbar_width < puipara->tbar_lyr.scnw)
		pmenu_para->scn_width = tbar_width ;
	else	
		pmenu_para->scn_width = puipara->tbar_lyr.scnw;
	pmenu_para->src_start = puipara->menu_tbar_item.x;

	for(i = 0; i < pmenu_para->item_nr; i++)
	{	
		if(user_para->item_type != TYPE_TBAR)
		{
			return 0;
		}			
		pitem_attr = &user_para->item_attr;
		
		if(user_para->item_sta == 1)
		{
			if(pitem_attr->icon[0])
			{
// 				(pmenu_item+i)->focus_bmp = (void *)esMEMS_Balloc(puipara->menu_tbar_bmp.w*puipara->menu_tbar_bmp.h*4 + 0x100); 
// 				GetRes(psset->icon_res, 
// 					   psset->icon_style, 	  
// 					   pitem_attr->icon[0],					  
// 					   (pmenu_item+i)->focus_bmp,	  
// 					   puipara->menu_tbar_bmp.w*puipara->menu_tbar_bmp.h*4 + 0x100);
				Create_BMP_RES_Pt((pmenu_item+i)->focus_bmp, pitem_attr->icon[0]);
			}
			if(pitem_attr->icon[1])
			{
// 				(pmenu_item+i)->unfocus_bmp = (void *)esMEMS_Balloc(puipara->menu_tbar_bmp.w*puipara->menu_tbar_bmp.h*4 + 0x100); 
// 				GetRes(psset->icon_res, 
// 					   psset->icon_style,   
// 					   pitem_attr->icon[1],					  
// 					   (pmenu_item+i)->unfocus_bmp,	  
// 					   puipara->menu_tbar_bmp.w*puipara->menu_tbar_bmp.h*4 + 0x100);
				Create_BMP_RES_Pt((pmenu_item+i)->unfocus_bmp, pitem_attr->icon[1]);
			}

            if(pitem_attr->string[0])
            {
                get_menu_text(pitem_attr->string[0], (pmenu_item+i)->text, sizeof((pmenu_item+i)->text));
            }
		}
		else
		{
			if(pitem_attr->icon[2])
			{
// 				(pmenu_item+i)->focus_bmp = (void *)esMEMS_Balloc(puipara->menu_tbar_bmp.w*puipara->menu_tbar_bmp.h*4 + 0x100); 
// 				GetRes(psset->icon_res, 
// 					   psset->icon_style, 	  
// 					   pitem_attr->icon[2],					  
// 					   (pmenu_item+i)->focus_bmp,	  
// 					   puipara->menu_tbar_bmp.w*puipara->menu_tbar_bmp.h*4 + 0x100);
				Create_BMP_RES_Pt((pmenu_item+i)->focus_bmp, pitem_attr->icon[2]);
			}
			if(pitem_attr->icon[3])
			{
// 				(pmenu_item+i)->unfocus_bmp = (void *)esMEMS_Balloc(puipara->menu_tbar_bmp.w*puipara->menu_tbar_bmp.h*4 + 0x100); 
// 				GetRes(psset->icon_res, 
// 					   psset->icon_style,   
// 					   pitem_attr->icon[3],					  
// 					   (pmenu_item+i)->unfocus_bmp,	  
// 					   puipara->menu_tbar_bmp.w*puipara->menu_tbar_bmp.h*4 + 0x100);
				Create_BMP_RES_Pt((pmenu_item+i)->unfocus_bmp, pitem_attr->icon[3]);
			}			

            if(pitem_attr->string[1])
            {
                get_menu_text(pitem_attr->string[1], (pmenu_item+i)->text, sizeof((pmenu_item+i)->text));
            }
		}
        
		user_para = user_para->next;		
	}
	
	h_win = tbar_frm_create( hparent, hlayer, pmenu_para );

	return h_win;	
}

/*
************************************************************************************************************************
*                       				SSET_MenuTbar_Destroy
*
*Description: 销毁dsk_tbar
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_MenuTbar_Destroy( H_WIN h_win )
{
	__u32						i = 0;
	tbar_item_t					*pmenu_item = 0;
	__sset_uipara_t				*puipara;	

	puipara = (__sset_uipara_t *)GUI_WinGetAddData(GUI_WinGetParent(h_win));
	if(pmenu_para)
	{
		pmenu_item = pmenu_para->item;
		if(pmenu_item)
		{
			for(i = 0; i < pmenu_para->item_nr; i++)
			{
				if((pmenu_item+i)->focus_bmp)
				{
//					esMEMS_Bfree((pmenu_item+i)->focus_bmp, puipara->menu_tbar_bmp.w*puipara->menu_tbar_bmp.h*4 + 0x100);
					Destroy_BMP_RES_Pt((pmenu_item+i)->focus_bmp);
					(pmenu_item+i)->focus_bmp = NULL;
				}
				if((pmenu_item+i)->unfocus_bmp)
				{
//					esMEMS_Bfree((pmenu_item+i)->unfocus_bmp, puipara->menu_tbar_bmp.w*puipara->menu_tbar_bmp.h*4 + 0x100);
					Destroy_BMP_RES_Pt((pmenu_item+i)->unfocus_bmp);
					(pmenu_item+i)->unfocus_bmp = NULL;
				}			
			}
			esMEMS_Bfree( pmenu_item, sizeof(tbar_item_t)*pmenu_para->item_nr );
			pmenu_item = NULL;
		}
		esMEMS_Bfree( pmenu_para, sizeof(tbar_para_t) );
		pmenu_para = NULL;
	}

	tbar_frm_delete( h_win );

	h_win = NULL;

	return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				SSET_MenuTbar_SetFocus
*
*Description: 设置MENU的焦点
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_MenuTbar_SetFocus(H_WIN h_win, __u32 fcs_id)
{

	return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				SSET_MenuTbar_UpdateItem
*
*Description: 
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_MenuTbar_UpdateItem( H_WIN h_win, __u32 item_no )
{
	
	return EPDK_OK;
}

