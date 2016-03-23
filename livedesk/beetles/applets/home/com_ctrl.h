#ifndef _COM_CTRL_H_
#define _COM_CTRL_H_

//#include "app_home_i.h"
#include "apps.h"
#include "beetles_app.h"


//#define DEBUG_COM_CTRL

// 波特率
#define COM_BAUD_RATE 38400
#define COM_BAUD_RATE_BLUETOOTH 115200

// 数据接收状态，接收包头、接收包头外的数据
#define RECEIVE_CHECK_HEADER 0
#define RECEIVE_PACK_DATA    1

// 数据包开头与结束定义
#define PACK_FRAME_HEADER    0xFF //    帧开头
#define PACK_FRAME_END       0xFE //    帧结束
#define PACK_FRAME_MAX_DATA  0xFD //    帧内最大数据

// 接收缓存状态，当接收缓存为空时，设定为0；非空时设定为接收到的数据包长度
#define RX_BUF_STATUS_EMPTY   0

// 数据包最大、最小长度限定
#define RECEIVE_PACK_LEN_MIN    5
#define RECEIVE_PACK_LEN_MAX    64

// 数据包接收超时时间，单位 100ms
#define RECEIVE_PACK_TIME_MAX    3

/***********************************************************************************/
#define COM_ST_IDLE  0 // 发送空闲
#define COM_ST_TX    1 // 发送中
#define COM_ST_WAP   2 // 等待应答

/*
0x00：命令执行成功
0x01：命令执行失败
0x02：功能码不存在
0x03：数据写入失败
0x04：数据防写
0x05：参数不存在
0x80：数据校验错误
*/
#define  EXC_SUCCEED    	0x00
#define  EXC_FAILURE    	0x01
#define  EXC_NO_FUN_C   	0x02
#define  EXC_WR_FAILURE 	0x03
#define  EXC_WR_INHIBIT 	0x04
#define  EXC_NO_PARAMETER 	0x05
#define  EXC_CRC_ERROR  	0x80


#define REG_ADDR_OVERLOAD          0xA8



// 数据包内各段在数据包的偏移量
#define TX_HEAHER_OFFSET    0  //  
#define TX_FUNCTION_OFFSET  1  //  

#define RX_HEAHER_OFFSET      0 //  
#define RX_EXC_RESULT_OFFSET  1 //  
#define RX_FUNCTION_OFFSET    2 //  

// 通信功能码
#define COM_FUNC_WR_CTR_CMD   	0x10  //   写控制指令， 向下控写入控制运行状态指令
#define COM_FUNC_RD_CTR_CMD   	0x11  //   读控制指令，读取下控最后收到的控制指令
#define COM_FUNC_WR_ONE       		0x20  //   写一个参数，写入下控指定序号的参数
#define COM_FUNC_RD_ONE       		0x21  //   读一起参数，读取下控指定序号的参数
#define COM_FUNC_WR_SOME      	0x40  //   写若干个参数，写若干个参数到下控
#define COM_FUNC_RD_SOME      	0x41  //   读若干个参数，从下控读若干个参数
#define COM_FUNC_IDLE				0xFF

// 开发人员专用
#define COM_FUNC_RD_ONE_DEVELOPER       0x46
#define COM_FUNC_WR_ONE_DEVELOPER       0x47

/*
	参数位置
*/
#define REG_ADDR_DIRECTION_SET    0  // 转动方向设定R/W
#define REG_ADDR_DIRECTION_NOW    1  // 实际转动方向R
#define REG_ADDR_ROTATE_SPD_SET   2  // 转速设定，R/W
#define REG_ADDR_ROTATE_SPD_NOW   3  
#define REG_ADDR_RESISTANCE_SET   4	 // 阻力设定
#define REG_ADDR_RESISTANCE_NOW   5
#define REG_ADDR_POWER_SET        6  // 功率设定
#define REG_ADDR_POWER_NOW        7

#define REG_ADDR_MAC_SELECT		0x0 //选择机种
#define REG_ADDR_ERR				0x23 //椭圆机出错信息
#define REG_ADDR_WR_DATA			0x4B //椭圆机读写最大段数、最大COUNTER数
#define REG_ADDR_NORMAL_DATA     0x4C //读椭圆机常态数据包序号

#define REG_ADDR_RD_INC_ADC		0x16 //读当前的ADC值
#define REG_ADDR_RD_INC_STATUS   0x17 //读椭圆机扬声状态
#define REG_ADDR_RD_MIN_ADC_INC	0x18 //读最低扬声ADC值
#define REG_ADDR_RD_MAX_ADC_INC	0x19
#define REG_ADDR_WR_INC			0x15	//设置该段扬声
#define REG_ADDR_WR_MAX_ADC		0x19
#define REG_ADDR_WR_MIN_ADC		0x18
#define	REG_ADDR_WR_MAX_INC		0x1A	//设置最大扬声
#define REG_ADDR_WR_INC_CMD		0x1B //写从扬声命令
#define REG_ADDR_WR_BUFFER		0x1D //写响声次数
#define REG_ADDR_WR_BUFFER_TIME	0X1F //写响声时间
#define REG_ADDR_WR_LEVEL			0x20 //设定当前level段数
#define	REG_ADDR_WD_STRIDE		0x21 //写STRIDE AD值
#define	REG_ADDR_RD_COUNTER		0x22 //读取当前实际COUNTER数

#define	REG_ADDR_RD_ERR			0x23 //椭圆机出错信息
#define	REG_ADDR_COMMAD			0x24 //拉线器命令
#define	REG_ADDR_STATUS			0x25 //拉线器状态
#define REG_ADDR_PWM				0x29 //pwm 分频数

#define REG_ADDR_FAN				0x32	//风扇开关

#define	REG_ADDR_LEVEL_1			0x50 //读写该段段数
#define	REG_ADDR_LEVEL_2			0x51
#define	REG_ADDR_LEVEL_3			0x52
#define	REG_ADDR_LEVEL_4			0x53
#define	REG_ADDR_LEVEL_5			0x54
#define	REG_ADDR_LEVEL_6			0x55
#define	REG_ADDR_LEVEL_7			0x56
#define	REG_ADDR_LEVEL_8			0x57
#define	REG_ADDR_LEVEL_9			0x58
#define	REG_ADDR_LEVEL_10			0x59
#define	REG_ADDR_LEVEL_11			0x5A
#define	REG_ADDR_LEVEL_12			0x5B
#define	REG_ADDR_LEVEL_13			0x5C
#define	REG_ADDR_LEVEL_14			0x5D
#define	REG_ADDR_LEVEL_15			0x5E
#define	REG_ADDR_LEVEL_16			0x5F
#define	REG_ADDR_LEVEL_17			0x60
#define	REG_ADDR_LEVEL_18			0x61
#define	REG_ADDR_LEVEL_19			0x62
#define	REG_ADDR_LEVEL_20			0x63


//add  for controllor 20150430
//#define REG_ADDR_PROGRAMME_IDENTIFIER   140
#define REG_ADDR_PROGRAMME_VERSION      141
#define REG_ADDR_PROGRAMME_YEAR         142
#define REG_ADDR_PROGRAMME_MONTH_DAY    143

//add  end





#define REG_ADDR_MOTOR_STATUS    10  // 控制器状态
#define REG_ADDR_RELAY_STATUS    11  // ---
#define REG_ADDR_EXCEPTION_MES	 13  // 异常信息
#define REG_ADDR_STATUS_CHANGE   18  //  马达运行状态改变标志

#define REG_ADDR_CURRENT_SET     20  // 设定电流
#define REG_ADDR_CURRENT_NOW     21	 // 实际电流

#define REG_ADDR_RUN_MODE        26  // 运行模式，有四种
#define REG_ADDR_RUN_CMD         27  // 控制命令
#define REG_ADDR_CURRENT_ADC     28
#define REG_ADDR_MAIN_VOLT_ADC   29
#define REG_ADDR_MAIN_VOLTAGE    30
#define REG_ADDR_H_BRIDGE_PWM    31
#define REG_ADDR_NO_CURRENT_ADC  32

#define REG_ADDR_POWER_ON_1MS_TICK  40 // 上电计时，单位1ms
#define REG_ADDR_POWER_ON_10S_TICK  41 //  上电计时，单位10s

#define REG_ADDR_MAIN_WHILE_TIME      45
#define REG_ADDR_MAIN_WHILE_MAX_TIME  46


#define REG_ADDR_ADC_REF_VOLTAGE   100
#define REG_ADDR_ROTATE_SPD_P      101
#define REG_ADDR_ROTATE_SPD_I      102
#define REG_ADDR_CURRENT_P         103
#define REG_ADDR_CURRENT_I         104

// 运行模式
#define RUN_MODE_FREE_SPEED    0
#define RUN_MODE_CONST_SPEED   1
#define RUN_MODE_RESISTANCE    2
#define RUN_MODE_POWER         3
#define RUN_MODE_POSITION_LOCK 4
#define RUN_MODE_OC_DEBUG	   0x0A
#define RUN_MODE_RPM_DEBUG	   0x0B
#define RUN_MODE_PLACE_DEBUG   0x0C

// 运行命令
#define RUN_CMD_STOP       0
#define RUN_CMD_RUN        1
#define RUN_CMD_EMER_STOP  2
#define RUN_CMD_LOCK       3
#define RUN_CMD_SLEEP      4

//异常信息
#define ERROR_NO          	0x00 	// 无异常
#define ERROR_OUTPUT_OC   	0x01    //输出过电流
#define ERROR_MOTOR_OL    	0x02    //马达过载
#define ERROR_BRAKE_OL    	0x03    //制动电阻过载
#define ERROR_LOW_VOLTAGE 	0x04    //主电源低压
#define ERROR_OVER_VOLTAGE 	0x05    //主电源过压
#define ERROR_BRAKE_LOSS  	0x06    //制动电路失效
#define ERROR_SPD_SENSE   	0x07    //速度检测异常
#define ERROR_NO_CURRENT  	0x08    //马达未连接或驱动异常
#define ERROR_OUTPUT_OL   	0x09    //输出过载
#define ERROR_SYS_OL      	0x0A    //系统过载
#define ERROR_MCU         	0x0B    //主控异常
#define ERROR_SAFEKEY_OFF 	0x20    //安全开关断开
#define ERROR_COM_TIMEOUT 	0x21    //通信超时


// 马达状态
#define MOTOR_STATUS_IDLE	0  //马达处于空闲状态
#define MOTOR_STATUS_RUN    1  // 马达处于运行状态
#define MOTOR_STATUS_STOP   2  // 马达处于停止状态，无法运行
#define MOTOR_STATUS_LOCK   3  // 上控已经发送停止指令，转向空闲状态


// 运行方向
#define RUN_DIR_FORWARD		0  // 正向
#define RUN_DIR_REVERSE		1  // 反向

// 阻力最大值
#define RESISTANCE_MAX 2000

typedef struct tag_communicate_para {
	__u8	com_func; //功能码

	// 写控制指令
	__u8	crtl_com; 			//控制指令, 偏移地址

	//__u16   direction_set;  		// 设置运行方向
	//__u16   direction_now;			// 实际运行方向
	//__u16   rotate_spd_set;			// 设置运行速度
	//__u16   rotate_spd_now;			// 实际运行速度
	//__s16   rotate_spd_now;			// 实际运行速度
	__u16   level_set;			// 设置运行阻力
	//short	level_now;			// 实际运行阻力
	__u16   inc_set;
	__u16   max_inc;
	__u16   minInc_ADC;
	__u16   maxInc_ADC;
	__u16   curInc_ADC;
	__u16	power_set;				// 设置运行功率
	short	power_now;				// 实际运行功率
	__u16	motor_status;			// 马达状态
	__u16	overload_status;       // 痉挛状态
	__u16	relay_status;			// 
	__u16   status_change;
	__u16  fan;     //风扇控制
	__u16  adcValue;

	__u16	curRpm; //当前rpm
//add by Tong for controllor  20150430
	__u16   version;//版本
	__u16   year;//年
	__u16   date;//月日
//add end
	__u16   error_code;              // 出错编号
	__u32	run_mode;  				 // 运行模式

	__krnl_event_t  *sem;     	// 通讯信号量控制, 互斥事件
	__bool  flag;				// 通讯标致位, 0则表示没有被占用，1则表示被占用

	__u16 MaxCounter;   		//椭圆机最大可设段数
	__u16 counter[20];        //20段每段的值
	__u16 curCounter;			//当前counter值
	__u16 HrFlag;
	__u16 HandHr;			//有线心跳
	__u16 WirHr;			//无线心跳
	__u16 INCCmd;          //从扬声命令
	__u16 bufferCount;   //buffer响声次数
	__u16 bufferTime;		//buffer响声时间
	__u16 LevelCmd;
	__u16 pwm;
	//__bool keyTonFlag;
	__u8 INCstatus;      //扬声状态
	__bool E2;

	__u8 key_value; //按键值
}com_data_para_t;

void com_send_thread(com_data_para_t *p_cmd_data);
void com_rece_thread(com_data_para_t *p_cmd_data);

//void com_rece_thread(void *arg);
__s32 open_uart0();
__s32 open_uart2();

int  bluetooth_sendbuf(char *pucData);
int  bluetooth_writebuf(__u8 *pucData,__u8 len);
__u8  bluetooth_write_tx(__u32 *num,int len);


void SetRunMode(com_data_para_t *p_cmd_data, __u16 mode);
void SetRunCtr(com_data_para_t *p_cmd_data, __u8 ctr_cmd);
void SetRunRpm(com_data_para_t *p_cmd_data, __u16 rpm);
void GetRunRPM(com_data_para_t *p_cmd_data);

void GetVersion(com_data_para_t *p_cmd_data);
void GetYEAR(com_data_para_t *p_cmd_data);
void GetDay(com_data_para_t *p_cmd_data);

void getNormalData(com_data_para_t *p_cmd_data);
void SeclectMac(com_data_para_t *p_cmd_data, __u16 mode);
void SetCounter(com_data_para_t *p_cmd_data, __u16 *counter,__u8 maxCounter);
void GetRunLevel(com_data_para_t *p_cmd_data);
void SetRunLevel(com_data_para_t *p_cmd_data, __u16 level);
void GetINCstatus(com_data_para_t *p_cmd_data);
void calbrationINC(com_data_para_t *p_cmd_data);
void InitINC(com_data_para_t *p_cmd_data);


//__s32 init_buffer();
void open_buffer(com_data_para_t *p_cmd_data,__u16 count,__bool keyTonFlag);

void buffer_time(com_data_para_t *p_cmd_data,__u16 sec);



void SetRunINC(com_data_para_t *p_cmd_data, __u16 inc);
void SetRunMAXINC(com_data_para_t *p_cmd_data, __u16 inc);

void SetFanOnOff(com_data_para_t *p_cmd_data, __u16 falg);
void GetRunIncAdc(com_data_para_t *p_cmd_data);
void GetRunIncMINAdc(com_data_para_t *p_cmd_data);
__s32 read_BTData(__u8 *ResultBuf);
__s32 write_BTData(__u8 cmd,__u8 len,__u16 *SrcData) ;

void Send_BT_status(__u8 status) ;

void SetMaxADC(com_data_para_t *p_cmd_data, __u16 adcValue);
void SetMinADC(com_data_para_t *p_cmd_data, __u16 adcValue);
void SetCMDLevel(com_data_para_t *p_cmd_data,__u16 cmd);
void SetPWM(com_data_para_t *p_cmd_data,__u16 pwm);
void GetCounter(com_data_para_t *p_cmd_data);
void SetRunSTRDIE(com_data_para_t *p_cmd_data, __u16 inc);














#endif
