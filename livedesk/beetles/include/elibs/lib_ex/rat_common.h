#ifndef __RAT_COMMON_H__
#define __RAT_COMMON_H__

#define RAT_MAX_FULL_PATH_LEN (768+1)

typedef	__hdle	HRAT;
typedef	HRAT HRATNPL;

/*!
* \brief
*     �����ý������
*/
typedef enum
{
    RAT_MEDIA_TYPE_ALL = 0,     //����ý������
	RAT_MEDIA_TYPE_PIC,         //����ͼƬ�ļ�
	RAT_MEDIA_TYPE_AUDIO,       //������Ƶ�ļ�
	RAT_MEDIA_TYPE_VIDEO,       //������Ƶ�ļ�	
	RAT_MEDIA_TYPE_EBOOK,       //���е������ļ�	
	RAT_MEDIA_TYPE_FIRMWARE,    //���й̼��ļ�
	RAT_MEDIA_TYPE_UNKNOWN,     //����δ֪�ļ�����
	RAT_MEDIA_TYPE_FOLDER,		//�ļ���
	RAT_MEDIA_TYPE_USB_DEVICE,	//USB�豸
	RAT_MEDIA_TYPE_SD_DEVICE,	//SD�豸
	RAT_MEDIA_TYPE_FAVOR_PIC,         //????????
	RAT_MEDIA_TYPE_FAVOR_AUDIO,       //????????
	RAT_MEDIA_TYPE_FAVOR_VIDEO,       //????????	
	RAT_MEDIA_TYPE_FAVOR_EBOOK,       //?????????	
	RAT_MEDIA_TYPE_MAX,			//
}rat_media_type_t;

/* play mode */                  
typedef enum __rat_play_mode_e
{
	RAT_PLAY_MODE_ONLY_ONCE = 0, 
	RAT_PLAY_MODE_ROTATE_ONE   ,
	RAT_PLAY_MODE_ROTATE_ALL   ,
	RAT_PLAY_MODE_SEQUENCE     ,
	RAT_PLAY_MODE_RANDOM       ,
	RAT_PLAY_MODE_MAX
}rat_play_mode_e;


#endif//__RAT_COMMON_H__