/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2016-2020, Rounder, China
*											 All Rights Reserved
*
* File    	: silview_i2c.c
* By      	: rounder.huang
* Func		: i2c read write
* Version	: v1.0
* ============================================================================================================
* 2016-01-21 10:04:52  rounder.huang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/


#include "silview_i2c.h"

#define DRV_GT_ADD			0x68  //加密芯片7位地址



static __u8 Version[4] = {0x32, 0x30,0x84,0x71};  //加密芯片版本号

#define VERSION0  			0xB8

#define CMD_InitUsid   	0xAA
#define CMD_AuthDev		0x55
#define CMD_InitPage		0xA5
#define CMD_InitKey		0x5A
#define CMD_ReadMem		0x0F
#define CMD_WriteMem		0xF0
#define CMD_GenKey			0x33
#define	CMD_CmdClr			0x00

#define MEMBUF				0xC0
#define MACBUF				0xE0

#define SYSCTRL				0xB4
#define ESAC				0xB3
#define	MCMD				0xB0
#define	TAr					0xB1
#define	TAd					0xB2

#define PRT_CTRL			0xAE
#define DISABLE_INIT_PAGE	0xAF
#define	UIDPRO				0xAD
#define	KEYPRO				0xAC
#define PAGE_0PRO			0xA8
#define PAGE_1PRO			0xA9
#define PAGE_2PRO			0xAA
#define PAGE_3PRO			0xAB

#define	WDGCNT				0xA0
#define	WDGRSTCTRL			0xA3

#define USID				0x90
#define	KEY					0x80

#define	PAGE0				0x00
#define	PAGE1				0x20
#define	PAGE2				0x40
#define	PAGE3				0x60

#define WDOG_EN				(1U<<0)
#define SHA_RD_BYPASS		(1U<<2)
#define RST_EN_N			(1U<<4)
#define RST_POLARITY		(1U<<5)





static ES_FILE* fiic = NULL;

static  __u32 Seed;

static __u8 CHAData[8];
static __u8 MACData[32];
static __u8 KEYData[8] = {0x52,0x4F,0x55,0x4E,0x44,0x45,0x52,0x08};
static __u8 USIDData[8] = {0x53,0x69,0x6c,0x76,0x69,0x65,0x78,0x08};



static __u8 PageBuffer0[32] = {0};
static __u8 PageBuffer1[32] = {0};
static __u8 PageBuffer2[32] = {0};
static __u8 PageBuffer3[32] = {0};




static __u8 pCMDBuf[2];
static __u8 pPageBuf[2];
static __u8 pAddrBuf[2];
static __u8 pRegBuf[2];
static __u8 FlagData[8];
static __u8 ErrorFlag;

;

/* Private macro define --------------------------------------------------*/
#define RoundNum        64 
#define BlockWidthNum   16 
#define ConstNum        8 

/* Variables declaration --------------------------------------------------*/
/* Variables declaration --------------------------------------------------*/
static __u32 M[BlockWidthNum]; 
static __u32 W[RoundNum];
static __u8 NonceBuff[64];
static __u8 Message_Digest[32]; 
static __u32 MAC[8];



/**
  * SHA算法中输入的64 bytes消息块的最后8bytes常数
  */

const __u8 ConstData[8] = {0x80,0x00,0x00,0x00,0x00,0x00,0x01,0xb8};	  




const __u32 H[ConstNum] = 
{
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
}; 


const __u32 K[RoundNum] = 
{ 
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};  


/**
  ***************************************************************************
  * @Prototype : static void GetMessage(void)；
  * @Brief	   : 用来获取64字节消息块，并将其转化成16个字数据
  * @Parameter : None
  * @Return	   : None
  * @Caller	   : 见具体情况
  ***************************************************************************
  */ 	

static void GetMessage(void)
{    
	__u8 tcnt;

	for(tcnt=0; tcnt<14; tcnt++)
	{
			M[15-tcnt]  = NonceBuff[tcnt * 4];
			M[15-tcnt] |= NonceBuff[tcnt * 4 + 1] << 8;
			M[15-tcnt] |= NonceBuff[tcnt * 4 + 2] << 16;
			M[15-tcnt] |= NonceBuff[tcnt * 4 + 3] << 24;	
	}
	M[1] = 0x80000000;
	M[0] = 0x000001b8;	
}


/**
  ***************************************************************************
  * @Prototype : static uint32_t ShiftNum3(uint32_t,int32_t,int32_t,int32_t)；
  * @Brief	   : 见具体算法介绍文档
  * @Parameter : None
  * @Return	   : None
  * @Caller	   : 见具体情况
  ***************************************************************************
  */ 	
   
static __u32 ShiftNum3(__u32 input, __s32 OpOne, __s32 OpTwo, __s32 Opthree) 
{   
    __u32 Ans = 0 ;      
    
		Ans ^= ((input >> OpOne)|(input<<(32-OpOne)));     
    Ans ^= ((input >> OpTwo)|(input<<(32-OpTwo)));     
    
		if(Opthree > 0)         
				Ans ^= ((input >> Opthree)|(input<<(32-Opthree)));     
    else     
    {           
				Opthree = 0 - Opthree;            
				Ans ^= (input >> Opthree);     
    } 
	    
    return Ans; 
}  

/**
  ***************************************************************************
  * @Prototype : static void SetW()；
  * @Brief	   : 将16个字数据按规定扩展成64个字数据，将参加下面的轮运算
  * @Parameter : None
  * @Return	   : None
  * @Caller	   : 见具体情况
  ***************************************************************************
  */ 	

static void SetW()
{   
    __s32 index = 0; 
    __u32 s0 = 0, s1 = 0;   

		for(index = 0; index <= 15; index++) 
		{
			W[index] = M[index];	
		}
					
    for(index = 16; index < RoundNum; index++)   
    {       
        s0 = 0;         
        s1 = 0;         
        s0 = ShiftNum3(W[index-2],17,19,-10);         
        s1 = ShiftNum3(W[index-15],7,18,-3);         
        W[index] = (s0 + W[index-7] + s1 + W[index-16]);     
    } 
} 



/**
  ***************************************************************************
  * @Prototype : static void Round(void)；
  * @Brief	   : 进行64轮轮运算得到32字节的消息认证码MAC
  * @Parameter : None
  * @Return	   : None
  * @Caller	   : 见具体情况
  ***************************************************************************
  */ 	
 
static void Round(void) 
{   
		__u32 A,B,C,D,E,F,G,I;
		__s32 index = 0;     
		__u32 t1 = 0, t2= 0, s0 = 0, s1 = 0;
		__s32 i, j;     
								 
				
		A = H[0];     
		B = H[1];     
		C = H[2];     
		D = H[3];     
		E = H[4];     
		F = H[5];     
		G = H[6];     
		I = H[7];          
				
		for(index = 0; index < RoundNum; index++)   
		{         
				s0 = 0;         
				s1 = 0;         
				s0 = ShiftNum3(A, 2, 13, 22);         
				t2 = s0 + ((A&B)^(A&C)^(B&C));         
				s1 = ShiftNum3(E, 6, 11, 25);         
				t1 = I + s1 + ((E&F)^((~E)&G)) + K[index] + W[index];                  
				I  = G;         
				G  = F;         
				F  = E;         
				E  = D + t1;         
				D  = C;         
				C  = B;         
				B  = A;         
				A  = t1 + t2; 		    		
		} 
				 
		MAC[0] = H[0]+A;     
		MAC[1] = H[1]+B;     
		MAC[2] = H[2]+C;     
		MAC[3] = H[3]+D;     
		MAC[4] = H[4]+E;     
		MAC[5] = H[5]+F;     
		MAC[6] = H[6]+G;     
		MAC[7] = H[7]+I;

		for(i = 0; i < ConstNum; i++)       
		{
				for(j=0; j<4; j++)
				{
						Message_Digest[4*i +j] = (MAC[7-i] >> 8*j) & 0xff ;	
				}		    
		} 
} 


/**
  ***************************************************************************
  * @Prototype : extern void SHAHandle(void)；
  * @Brief	   : 用完整的SHA256算法来计算消息认证码MAC，以进行后面的身份认证
  * @Parameter : None
  * @Return	   : None
  * @Caller	   : 见具体情况
  ***************************************************************************
  */ 	

void SHAHandle(void) 
{ 
    GetMessage();         
    SetW();         
    Round();
}  





__s32 silview_i2c_init()
{
	__s32 ret;

	__msg("huang####   i2c  init  \n");
		
	if (fiic == NULL) {
		fiic = eLIBs_fopen("b:\\BUS\\twi2", "r+");
				
		if (NULL == fiic) {
			__msg("huang#####  open iic failed  \n");
		}
		else {
			__msg("huang####    open iic  success   \n");
		}
	} else {
		__msg("huang########   iic  had  opened   \n");
	}

	ret = eLIBs_fioctrl(fiic,TWI_SET_SCL_CLOCK,0,(void *) TWI_SCLK_100KHZ);

	if (ret == EPDK_OK) {
		__msg("huang####  set 100k  iic speed  success \n");
	} else {
		__msg("huang####  set 100k  iic speed  fail \n");
	}

	return EPDK_OK;
	
}

__s32 silview_i2c_deinit()
{
	if (fiic) {
		eLIBs_fclose(fiic);
	}

	fiic = NULL;

	return EPDK_OK;
}

__s32 silview_i2c_read(__u8 sAddr, __u8 Reg, __u8 *data,  __u16 n_byte)
{
	__twi_dev_para_ex_t piic;
	__s32 ret = 0;
	__u8 add1 = Reg;
	eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));

	piic.slave_addr = (sAddr);
	piic.slave_addr_flag = 0;
	piic.byte_addr = &add1;
	piic.byte_addr_width = 1;
	piic.data = data;
	piic.byte_count = n_byte;

	ret = eLIBs_fioctrl(fiic,TWI_READ_SPEC_RS,0,(void *)&piic);

	if (ret == EPDK_FAIL) {
		__msg("huang###  iic  read fail \n");
		return EPDK_FAIL;
	}

	return EPDK_OK;
}

__s32 silview_i2c_write(__u8 sAddr, __u8 Reg, __u8 *data,  __u16 n_byte)
{
	__twi_dev_para_ex_t piic;
	__s32 ret = 0;
	__u8 add1 = Reg;
	eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));

	piic.slave_addr = (sAddr);
	piic.slave_addr_flag = 0;
	piic.byte_addr = &add1;
	piic.byte_addr_width = 1;
	piic.data = data;
	piic.byte_count = n_byte;

	ret = eLIBs_fioctrl(fiic,TWI_WRITE_SPEC_RS,0,(void *)&piic);

	if (ret == EPDK_FAIL) {
		__msg("huang###  iic  write fail \n");
		return EPDK_FAIL;
	}

	return EPDK_OK;
}


/**
  ***************************************************************************
  * @Prototype : extern void SRand(uint32_t init)；
  * @Brief	   : 产生随机数源种子
  * @Parameter : init：输入的源种子数据
  * @Return	   : None
  * @Caller	   : extern  void GT02_Init(void);
  ***************************************************************************
  */ 

void SRand(__u32 init)
{
	Seed = init;
}


/**
  ***************************************************************************
  * @Prototype : static uint32_t Rand(void)；
  * @Brief	   : 产生一个32位的整型随机数
  * @Parameter : None
  * @Return	   : 返回一个32位的整型随机数
  * @Caller	   : extern void GenCHA(void)；
  ***************************************************************************
  */ 

static __u32 Rand(void)
{
	Seed = Seed * 1103515245L + 12345;
	return (__u32) (Seed / 65536L) % 32768L;
}

/**
  ***************************************************************************
  * @Prototype : extern void ClrCMD(void)；
  * @Brief	   : 命令寄存器清除
  * @Parameter : None
  * @Return	   : None
  * @Caller	   : 见各大命令的功能实现函数。
  ***************************************************************************
  */ 
void ClrCMD(void)
{
	pCMDBuf[0] = CMD_CmdClr;		
	silview_i2c_write(DRV_GT_ADD,MCMD,pCMDBuf,1);
}

/**
  ***************************************************************************
  * @Prototype : extern void AuthDevice(uint8_t *buffer, uint8_t *pPage)；
  * @Brief	   : 主机认证函数
  * @Parameter : buffer:指向参与MAC计算的随机数  pPage:指定参与MAC计算的PAGE
  * @Return	   : None
  * @Caller	   : 见具体情况
  ***************************************************************************
  */ 

void AuthDevice(__u8 *buffer, __u8 *pPage)
{
  __u8 i;
	ClrCMD();

	pCMDBuf[0]  = CMD_AuthDev;


	silview_i2c_write(DRV_GT_ADD,MEMBUF,buffer,8);
	
	
	silview_i2c_write(DRV_GT_ADD,TAr,pPage,1);
	silview_i2c_write(DRV_GT_ADD,MCMD,pCMDBuf,1);


	esKRNL_TimeDly(1);		
	
}

void GenCHA(void)
{	
	__u8 tcnt;
	__u32 RandTemp, RandTemp1;

	RandTemp = Rand();
	RandTemp1= Rand();
	for(tcnt=0; tcnt<4; tcnt++)
	{
		CHAData[tcnt]	=(__u8)((RandTemp >> (8*tcnt))&0xFF);
		CHAData[tcnt+4]	=(__u8)((RandTemp1>> (8*tcnt))&0xFF);
	}
}

/**
  ***************************************************************************
  * @Prototype : extern void FillSHABuf(uint8_t *page)；
  * @Brief	   : 将密钥KEYData、随机数CHAData、用户数据USIDData、
  *				 常数据ConstData、PAGE数据组成一个消息块作为SHA256算法的输入。
  * @Parameter : page：指定将参与MAC计算PAGE。
  * @Return	   : None
  * @Caller	   : extern void SOftComputeMac(uint8_t *pPage)；
  ***************************************************************************
  */ 

void FillSHABuf(__u8 *page)
{
	__u8 tcnt;
	__u8 buffer[32];

	switch(page[0])
	{
		case PAGE0:
			 for(tcnt=0;tcnt<32;tcnt++)
			 {
				 buffer[tcnt] = PageBuffer0[tcnt];
			 }
		 	 break;

		case PAGE1:
			 for(tcnt=0;tcnt<32;tcnt++)
			 {
				 buffer[tcnt] = PageBuffer1[tcnt];
			 }
			 break;

		case PAGE2:
			 for(tcnt=0;tcnt<32;tcnt++)
			 {
				 buffer[tcnt] = PageBuffer2[tcnt];
			 }
			 break;

		case PAGE3:
			 for(tcnt=0;tcnt<32;tcnt++)
			 {
				 buffer[tcnt] = PageBuffer3[tcnt];
			 }
			 break;

		default:
			 break;
	}	
	
	for(tcnt=0; tcnt<64; tcnt++)
	{	 
		if(tcnt <= 7)             
			NonceBuff[tcnt] = KEYData[tcnt];         
		else if(tcnt >= 8 && tcnt <= 39)          
			NonceBuff[tcnt] = buffer[tcnt-8];           
		else if(tcnt >=40 && tcnt <= 47)        
			NonceBuff[tcnt] = USIDData[tcnt-40];         
		else if(tcnt >=48 && tcnt <= 55)         
			NonceBuff[tcnt] = CHAData[tcnt-48];           
		else if(tcnt >=56 && tcnt <= 63)         
			NonceBuff[tcnt] = ConstData[tcnt-56];               
   	} 
}


/**
  ***************************************************************************
  * @Prototype : extern void SOftComputeMac(uint8_t *pPage)；
  * @Brief	   : 软件计算MAC
  * @Parameter : pPage：指定参与MAC计算的PAGE
  * @Return	   : None
  * @Caller	   : 见具体情况
  ***************************************************************************
  */ 

void SOftComputeMac(__u8 *pPage)
{
	__u8 tcnt;
	
	FillSHABuf(pPage);
	SHAHandle();	
	for(tcnt=0; tcnt<32; tcnt++)  
	{
		MACData[tcnt] = Message_Digest[tcnt];
	}	
}




/**
  ***************************************************************************
  * @Prototype : extern void GT02_Init(void)；
  * @Brief	   : 对读PAGE时的输出模式进行配置
  * @Parameter : None
  * @Return	   : None
  * @Caller	   : 在main开头调用
  ***************************************************************************
  */ 

void GT02_Init(void)
{	
	__u8 tcnt;

	for(tcnt=0; tcnt<8; tcnt++)
	{
		CHAData[tcnt]  = 0x00;	
	}

	for(tcnt=0; tcnt<32; tcnt++)
	{
		MACData[tcnt] = 0x00 ;		
	}
	
	/* 产生随机数源种子 */ 
	SRand(0x0230A29F);	

	/* 指定读PAGE区数据时，不与MAC异或而直接输出 */

}



/**
  ***************************************************************************
  * @Prototype : extern uint8_t Buffercmp(uint8_t*, uint8_t*, uint16_t)；
  * @Brief	   : 进行数据比较
  * @Parameter : pBuffer1/pBuffer2: 指向待比较数据. BufferLength: 指定数据长度
  * @Return	   : 返回 1 = 相等  0 = 不相等
  * @Caller	   : 见具体情况
  ***************************************************************************
  */ 

__u8 Buffercmp(__u8 *pBuffer1, __u8 *pBuffer2, __u8 BufferLength)
{
	while(BufferLength--)
	{
		if(*pBuffer1 != *pBuffer2)
		{
			return 0;
		}
		
		pBuffer1++;
		pBuffer2++;
	}

	return 1;
}

/**
  ***************************************************************************
  * @Prototype : extern uint8_t AppAuthority(void)；
  * @Brief	   : 应用认证，认证通过才能执行用户指定的代码
  * @Parameter : None
  * @Return	   : None
  * @Caller	   : 见具体情况
  ***************************************************************************
  */ 	

__u8 AppAuthority(void)
{
	__u8	AuthFlag = 0;
	__u8    data[32] = {0};
	__u8 i;

	GenCHA();
	
	/* 芯片计算MAC */
	pPageBuf[0]=PAGE0;
	AuthDevice(CHAData,pPageBuf);
	
	esKRNL_TimeDly(1);	
	silview_i2c_read(DRV_GT_ADD,MEMBUF,data , 32);
	esKRNL_TimeDly(1);	
	/* 软件计算MAC */
	SOftComputeMac(pPageBuf);
	
	AuthFlag = Buffercmp(MACData, data, 32);

	return AuthFlag;
}

void writePage(__u8 *data,__u8 *pPage) 
{
	__u8 i;
	__u8 status[1];
	
	ClrCMD();
	pCMDBuf[0] = CMD_WriteMem;
	pPageBuf[0]=PAGE0;
	for(i=0 ; i<8; i++) {
		CHAData[i] = data[i];
	}
	SOftComputeMac(pPageBuf);
	
	silview_i2c_write(DRV_GT_ADD,MEMBUF,data,32);
	//silview_i2c_write(DRV_GT_ADD,MEMBUF,CHAData,8);
	silview_i2c_write(DRV_GT_ADD,MACBUF,MACData,32);

	silview_i2c_write(DRV_GT_ADD,TAr,pPageBuf,1);
	silview_i2c_write(DRV_GT_ADD,TAd,pPage,1);
	silview_i2c_write(DRV_GT_ADD,MCMD,pCMDBuf,1);
	//for(i=0 ; i<100; i++) ;
	//silview_i2c_read(DRV_GT_ADD,0xB3,status , 1);
	//__msg("huang####  statucs = 0x%x\n",status[0]);
	
}


void readPage(__u8 *data,__u8 *pPage) 
{
	__u8 i;
	__u8 status[1];

	ClrCMD();
	pCMDBuf[0] = CMD_ReadMem;
	pPageBuf[0]=PAGE0;
	for(i=0 ; i<8; i++) {
		CHAData[i] = data[i];
	}
	SOftComputeMac(pPageBuf);
	//__msg("huang#####   data =  \n");
	silview_i2c_write(DRV_GT_ADD,MEMBUF,CHAData,8);
	silview_i2c_write(DRV_GT_ADD,MACBUF,MACData,32);
	silview_i2c_write(DRV_GT_ADD,TAr,pPageBuf,1);
	silview_i2c_write(DRV_GT_ADD,TAd,pPage,1);
	silview_i2c_write(DRV_GT_ADD,MCMD,pCMDBuf,1);
	for(i=0 ; i<100; i++) ;
	silview_i2c_read(DRV_GT_ADD,MEMBUF,data , 32);

	//silview_i2c_read(DRV_GT_ADD,0xB3,status , 1);
	//__msg("huang####  statucs = 0x%x\n",status[0]);

}


/**
  ***************************************************************************
  * @Prototype : extern uint8_t GT02_Check(void)；
  * @Brief	   : 检测是否是ZL1GT02芯片
  * @Parameter : None
  * @Return	   : 返回 1 = 表示是此芯片  0 = 不是此芯片或没有找到
  * @Caller	   : 见具体情况
  ***************************************************************************
  */ 

__u8 GT02_Check(void)
{
	__u8 IsEqual;	
	__u8 data[4] = {0x0};
	__u8 i;


	silview_i2c_read(DRV_GT_ADD, VERSION0,data, 4);

	IsEqual = Buffercmp(Version, data, 4);
	
	if(IsEqual)
	{	
		return 1;
	}
	else
	{
		return 0;
	}		
	
}

