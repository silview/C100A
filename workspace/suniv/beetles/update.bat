::/*
::*********************************************************************************************************
::*                                                    MELIS
::*                                    the Easy Portable/Player Develop Kits
::*                                               Update system files
::*
::*                                    (c) Copyright 2011-2014, Sunny China
::*                                             All Rights Reserved
::*
::* File    : update.bat
::* By      : Sunny
::* Version : v1.0
::* Date    : 2011-4-19
::* Descript: update system drivers and modules.
::* Update  : date                auther      ver     notes
::*           2011-4-19 13:05:05  Sunny       1.0     Create this file.
::*********************************************************************************************************
::*/

    @ECHO OFF
    
    ::----------------------------------------------------------------
    ::update boot0 + boot1
    ::----------------------------------------------------------------
    @copy ..\eGon\storage_media\spinor\boot0.bin     boot0.bin
    @copy ..\eGon\storage_media\spinor\boot1.bin     boot1.bin

 		::----------------------------------------------------------------
    ::update apps in ramfs
    ::----------------------------------------------------------------
    @copy .\..\ramfs\drv\twi.drv        .\ramfs\drv\twi.drv
    ::@copy .\..\ramfs\drv\rtc.drv        .\ramfs\drv\rtc.drv
    @copy .\..\ramfs\drv\power2.drv      .\ramfs\drv\power.drv
    @copy .\..\ramfs\drv\spinor.drv     .\ramfs\drv\spinor.drv
    @copy .\..\ramfs\drv\spi.drv        .\ramfs\drv\spi.drv
    @copy .\..\ramfs\drv\power_cfg.ini  .\ramfs\power_cfg.ini
    @copy .\..\ramfs\mod\slib.mod       .\ramfs\mod\slib.mod
    @copy .\..\ramfs\drv\ir.drv         .\rootfs\drv\ir.drv
    ::@copy .\..\ramfs\drv\rocker.drv      .\rootfs\drv\key.drv
    @copy .\ramfs.ini                  .\ramfs\ramfs_ini.tmp

    ::----------------------------------------------------------------
    :: update rootfs
    ::----------------------------------------------------------------
    @copy .\..\rootfs\pwm_cfg.ini        .\rootfs\pwm_cfg.ini
    @copy .\..\rootfs\standby.bin       .\rootfs\standby.bin
    ::@copy .\..\rootfs\drv\atv.drv      	.\rootfs\drv\atv.drv
    @copy .\..\rootfs\drv\pwm.drv      	.\rootfs\drv\pwm.drv
    ::@copy .\..\rootfs\drv\csi.drv         .\rootfs\drv\csi.drv
    ::@copy .\..\rootfs\drv\tvd.drv         .\rootfs\drv\tvd.drv
    ::@copy .\..\rootfs\drv\di.drv         .\rootfs\drv\di.drv
    @copy .\..\rootfs\epos.img          .\rootfs\epos.img
    @copy .\..\rootfs\drv\audio.drv     .\rootfs\drv\audio.drv
    @copy .\..\rootfs\drv\key.drv       .\rootfs\drv\key.drv
    ::@copy .\..\rootfs\drv\audio.drv     .\ramfs\drv\audio.drv
    @copy .\..\rootfs\drv\display.drv   .\rootfs\drv\display.drv
    ::@copy .\..\rootfs\drv\fm.drv        .\rootfs\drv\fm.drv
    @copy .\..\rootfs\drv\sdmmc.drv     .\rootfs\drv\sdmmc.drv
    @copy .\..\rootfs\drv\uart.drv      .\rootfs\drv\uart.drv
    @copy .\..\rootfs\drv\usbd_msc.drv  .\rootfs\drv\usbd_msc.drv
    @copy .\..\rootfs\drv\usb_host0.drv .\rootfs\drv\usb_host0.drv
    @copy .\..\rootfs\drv\monitor.drv        .\rootfs\drv\monitor.drv
    ::@copy .\..\rootfs\drv\tv.drv        .\rootfs\drv\tv.drv
    ::@copy .\..\rootfs\drv\tcon.drv        .\rootfs\drv\tcon.drv
    ::@copy .\..\rootfs\drv\mcu.drv 	.\rootfs\drv\mcu.drv
    @copy .\rootfs.ini                  .\rootfs\rootfs_ini.tmp

    
    ::----------------------------------------------------------------
    :: update rootfs modules
    ::----------------------------------------------------------------
    @copy .\..\rootfs\mod\orange.mod    .\rootfs\mod\orange.mod
    @copy .\..\rootfs\mod\cedar.mod     .\rootfs\mod\cedar.mod
    @copy .\..\rootfs\mod\charset.mod   .\rootfs\mod\charset.mod
    @copy .\..\rootfs\mod\charset.bin   .\rootfs\mod\charset.bin
    @copy .\..\rootfs\mod\ginkgo.mod 	.\rootfs\mod\ginkgo.mod
    @copy .\..\rootfs\mod\update.mod 	.\rootfs\mod\update.mod
    
    ::----------------------------------------------------------------
    :: update cedar plugs
    ::----------------------------------------------------------------
    ::update cedar modules
    @xcopy .\..\rootfs\mod\cedar        .\rootfs\mod\cedar  /s/i/y
    
    ::----------------------------------------------------------------
    :: update willow plugs
    ::----------------------------------------------------------------
    ::update willow modules
    @xcopy .\..\rootfs\mod\willow       .\rootfs\mod\willow /s/i/y
