
#ifndef  _DRV_ATV_H_
#define  _DRV_ATV_H_
/*
#define  ATV_SEARCH_CHN_MIN_FREQ         87000
#define  FM_SEARCH_CHN_MAX_FREQ        108000

#define  FM_SEARCH_JAP_MIN_FREQ         76000
#define  FM_SEARCH_JAP_MAX_FREQ         91000

#define  FM_SEARCH_STEP                   100
*/
#define  ATV_DRV_STATUS_RECEIVE         0x8000
#define  ATV_DRV_STATUS_SEND            0x8001

typedef enum  __DRV_ATV_CMD_SET
{
    DRV_ATV_CMD_NONE = 0,
    DRV_ATV_CMD_TVPOWERON,//tv power on 
    DRV_ATV_CMD_TVPOWEROFF,//tv power off
	DRV_ATV_CMD_FMPOWERON,//fm power on 
	DRV_ATV_CMD_FMPOWEROFF,//fm power off
	DRV_ATV_CMD_POWERDOWN,//power down the modual, turn off the modual's power
	DRV_ATV_CMD_TVSCANINIT,//scan init
	DRV_ATV_CMD_TVSCANEND,//scan end
	DRV_ATV_CMD_TVSCAN,//scan ing~
	DRV_ATV_CMD_TVSENDTUNER,//send freq to tuner~
	DRV_ATV_CMD_TVTUNER_STANDBY,//tuner standby~
	DRV_ATV_CMD_TVTUNER_RESTART,//tuner restart~
	DRV_ATV_CMD_TVTUNER_LOCK,
	DRV_ATV_CMD_TVTUNER_CHECK_SIGNAL,
	//DRV_ATV_CMD_TVGETSYSMODE, //cai++ for audio mode

}__drv_atv_cmd_set_t;

#endif   /*_DRV_ATV_H_*/

