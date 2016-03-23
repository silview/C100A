/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2016-2020, Rounder, China
*											 All Rights Reserved
*
* File    	: silview_i2c.h
* By      	: rounder.huang
* Func		: i2c read write
* Version	: v1.0
* ============================================================================================================
* 2016-01-21 10:04:52  rounder.huang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#ifndef __I2C_H_
#define  __I2C_H_

#include "apps.h"
#include "beetles_app.h"


__s32 silview_i2c_init();
__s32 silview_i2c_deinit();
__u8 GT02_Check(void);
__u8 AppAuthority(void);
void GT02_Init(void);
void writePage(__u8 *data,__u8 *pPage) ;
void readPage(__u8 *data,__u8 *pPage) ;







#endif

