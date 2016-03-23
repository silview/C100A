/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                             audio driver Module
*
*                                    (c) Copyright 2009-2010, kevin China
*                                             All Rights Reserved
*
* File    : drv_ir_remote_i.h
* By      : victor
* Version : V1.0
* Date    : 2010-12-31
*********************************************************************************************************
*/

#ifndef	_DRV_IR_REMOTE_I_H_
#define	_DRV_IR_REMOTE_I_H_

#include "epdk.h"
#include "ir_remote.h"


#define IR_KEY_DEV_NAME  "IR_KEY"


typedef struct tag_ir_key_dev
{
    __u32 used;
}__ir_key_dev_t;

typedef struct tag_ir_key_drv
{
    __u32            mid;
    __u32            used;
    __u32            status;
    __input_dev_t    *pIrKey_dev; /* ir key device handle for register input subsystem */
    
    __ir_key_dev_t     ir_key_dev;/* ir key device */
    
}__ir_key_drv_t;


/* ir KEY Message */
typedef struct tag_ir_key_msg
{
    __u32 key_value;  //key value.
    __u32 flag;       //1:keydown; 0:keyup.		
}__ir_key_msg_t;


extern __s32 DEV_IR_KEY_Ioctrl(__hdle hKey, __u32 cmd, __s32 aux, void *pbuffer);

extern __s32 drv_ir_key_event(__input_dev_t *dev, __u32 type, __u32 code, __s32 value);

extern __s32 DRV_IR_KEY_MInit(void);
extern __s32 DRV_IR_KEY_MExit(void);
extern __mp* DRV_IR_KEY_MOpen(__u32 mid, __u32 mode);
extern __s32 DRV_IR_KEY_MClose(__mp *mp);
extern __u32 DRV_IR_KEY_MRead(void *pdata, __u32 size, __u32 n, __mp *mp);
extern __u32 DRV_IR_KEY_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
extern __s32 DRV_IR_KEY_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);



#endif	/* _DRV_IRKEY0_I_H_ */
