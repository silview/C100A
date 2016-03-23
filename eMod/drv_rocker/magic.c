/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : pfhead.h
* By      : Steven.ZGJ
* Version : V1.00
*********************************************************************************************************
*/
#include "drv_rocker_i.h"

#pragma arm section rodata="MAGIC"
const __mods_mgsec_t modinfo =
{
    {'e','P','D','K','.','m','o','d'},		//.magic
    0x01000000,                				//.version
    EMOD_TYPE_DRV_RK,                       //.type
    0xF0000,                                //.heapaddr
    0x400,                      			//.heapsize
	{                                       //.mif
	    &DRV_RK_MInit,
	    &DRV_RK_MExit,
	    &DRV_RK_MOpen,
	    &DRV_RK_MClose,
	    &DRV_RK_MRead,
    	&DRV_RK_MWrite,
	    &DRV_RK_MIoctrl
	}
};
#pragma arm section
