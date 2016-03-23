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



#生成输出目标文件
all:
	make -C drv_display
	make -C drv_fm
	make -C drv_ir
	make -C drv_matrixkey	
	#make -C drv_power2_ladc
	make -C mod_standby
	make -C mod_orange
		    
# 删除生成的中间文件
clean:
	make -C drv_display clean
	make -C drv_fm clean
	make -C drv_ir clean
	make -C drv_matrixkey	clean
	#make -C drv_power2_ladc clean
	make -C mod_standby clean
	make -C mod_orange clean
	