#/*
#*********************************************************************************************************
#*                                                    MELIS
#*                                    the Easy Portable/Player Develop Kits
#*                                               makefile Module
#*
#*                                    (c) Copyright 2006-2010, kevin.z China
#*                                             All Rights Reserved
#*
#* File    : makefile
#* By      : kevin.z
#* Version : v1.0
#* Date    : 2010-9-3 9:39
#* Descript:
#* Update  : date                auther      ver     notes
#*********************************************************************************************************
#*/



#�������Ŀ���ļ�
all:
	make -C eLibs
	make -C eMod
	make -C livedesk\beetles	
	
    
# ɾ�����ɵ��м��ļ�
clean:
	make -C eLibs       clean
	make -C eMod       clean
	make -C livedesk\beetles         clean