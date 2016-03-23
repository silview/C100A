/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									          ir keyboard driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : drv_irkey_i.h
* By      : jerry
* Version : V1.00
*********************************************************************************************************
*/
#ifndef	_DRV_KEY_I_H_
#define	_DRV_KEY_I_H_

#include "epdk.h"
#include "lradc.h"


#define LRADC_KEY_DEV_NAME  "KEY"
#define KEY_CODE_TABLE_SIZE   9

#define LRADC_DATA_ADDR         ((volatile __u32 *)0xf1c2340c)
#define LRADC_DATA_VALUE        (*(LRADC_DATA_ADDR))

typedef struct tag_key_drv
{
    __u32            mid;
    __u32            used;
    __u32            status;
    __input_dev_t    *pKeyc_dev; /* lradc key device handle */
    
}__key_drv_t;

typedef struct tag_key_dev
{
    __u32 used;
}__key_dev_t;


/* KEY Message */
typedef struct tag_key_msg
{
    __u32 key_value;  //key value.
    __u32 flag;       //1:keydown; 0:keyup.		
}__key_msg_t;


extern __s32 DEV_KEYC_Ioctrl(__hdle hKey, __u32 cmd, __s32 aux, void *pbuffer);

extern __s32 drv_keyc_event(__input_dev_t *dev, __u32 type, __u32 code, __s32 value);

extern __s32 DRV_KEYC_MInit(void);
extern __s32 DRV_KEYC_MExit(void);
extern __mp* DRV_KEYC_MOpen(__u32 mid, __u32 mode);
extern __s32 DRV_KEYC_MClose(__mp *mp);
extern __u32 DRV_KEYC_MRead(void *pdata, __u32 size, __u32 n, __mp *mp);
extern __u32 DRV_KEYC_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
extern __s32 DRV_KEYC_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);



#endif	/* _DRV_IRKEY0_I_H_ */
