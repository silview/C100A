call update.bat
call set_path.bat
@xcopy .\rootfs\apps       %ROOTFS%:\rootfs\apps /s/i/y
