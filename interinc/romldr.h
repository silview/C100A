/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                               Melis ROM Loader
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : romldr.h
* By      : Sunny
* Version : v1.0
* Date    : 2011-3-27
* Descript: melis system ROM loader structures.
* Update  : date                auther      ver     notes
*           2011-3-27 11:15:29  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __ROMLDR_H__
#define __ROMLDR_H__

//IOCTL commmands for melis system loader
#define ROMLDR_IOC_BASE                0x100
#define ROMLDR_IOC_GET_MAGIC_INDEX     (ROMLDR_IOC_BASE + 1)
#define ROMLDR_IOC_GET_SECTION_NUM     (ROMLDR_IOC_BASE + 2)
#define ROMLDR_IOC_GET_SECTION_HDR     (ROMLDR_IOC_BASE + 3)
#define ROMLDR_IOC_GET_SECTION_DATA    (ROMLDR_IOC_BASE + 4)
#define ROMLDR_IOC_LAST                (ROMLDR_IOC_BASE + 50)
#define ROMLDR_IOC_CMD(cmd)            ((ROMLDR_IOC_BASE <= cmd) && \
                                       (ROMLDR_IOC_LAST >= cmd))

//the section header structure.
typedef struct __SECTION_ROM_HDR
{
    __u32   Size;                       //byte size of this section
    __u32   VAddr;                      //virtual address of this section
    __u32   Type;                       //type of this section
    __u32   Flags;                      //flags of this section
} __section_rom_hdr_t;

#endif  //__ROMLDR_H__
