#####################################################################
#																	 
#	Created by u'nSP IDE		22:58:36	06/26/09
#
#####################################################################




APPDIR	= C:\PROGRA~1\GENERA~1\UNSPID~1.1

OUTDIR	= .\Debug

CC	= $(APPDIR)\toolchain\udocc

AS	= $(APPDIR)\toolchain\xasm16

LD	= $(APPDIR)\toolchain\xlink16

AR	= $(APPDIR)\toolchain\xlib16

RESC	= $(APPDIR)\toolchain\resc

RM	= $(APPDIR)\toolchain\rm -f

INCLUDES	= -I"E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware" -I"C:/Program Files/Generalplus/unSPIDE 3.0.1/library/include" -I"C:/Program Files/Generalplus/unSPIDE 3.0.1/library/include/sys" -I"./Component/Include/FS"

BODY	= -body GPL162003_CS0Flash -nobdy -bfile "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System.bdy"

BINFILE	= "$(OUTDIR)\System.S37"

ARYFILE	= "$(OUTDIR)\System.ary"

SBMFILE	= "$(OUTDIR)\System.sbm"

OPT	= -S -gstabs -Wall -DUSBRP -mglobal-var-iram -mISA=2.0

ASFLAGS	= -t4 -d -be

CASFLAGS	= -t4 -be -wpop

CFLAGS	= $(OPT) -B$(APPDIR)\toolchain\ $(INCLUDES) 

BINTYPE	= -as

LDFLAGS	= -forspace -initdata

EXTRAFLAGS	= 


OBJFILES	= \
	"$(OUTDIR)\Resource.obj" \
	"$(OUTDIR)\app_exception.obj" \
	"$(OUTDIR)\audio.obj" \
	"$(OUTDIR)\containers.obj" \
	"$(OUTDIR)\d2d_copy.obj" \
	"$(OUTDIR)\device.obj" \
	"$(OUTDIR)\files.obj" \
	"$(OUTDIR)\lists.obj" \
	"$(OUTDIR)\macro.obj" \
	"$(OUTDIR)\mainLoop.obj" \
	"$(OUTDIR)\parsing.obj" \
	"$(OUTDIR)\pkg_ops.obj" \
	"$(OUTDIR)\startup.obj" \
	"$(OUTDIR)\sys_counters.obj" \
	"$(OUTDIR)\timeline.obj" \
	"$(OUTDIR)\util.obj" \
	"$(OUTDIR)\SD.obj" \
	"$(OUTDIR)\FS_user_nos.obj" \
	"$(OUTDIR)\nls_ascii.obj" \
	"$(OUTDIR)\DrvStruct.obj" \
	"$(OUTDIR)\nls_codepage.obj" \
	"$(OUTDIR)\MemPackCopy.obj" \
	"$(OUTDIR)\USB.obj" \
	"$(OUTDIR)\FS_DS.obj" \
	"$(OUTDIR)\SCSI_Table.obj" \
	"$(OUTDIR)\SystemIntoUSB.obj" \
	"$(OUTDIR)\IOKeyScan.obj" \
	"$(OUTDIR)\startup_Data.obj" \
	"$(OUTDIR)\isr.obj" \
	"$(OUTDIR)\ServiceLoop.obj" \
	"$(OUTDIR)\SysWaitMode.obj" \
	"$(OUTDIR)\SysOn2OffHook.obj" \
	"$(OUTDIR)\SysIntoHighSpeed.obj" \
	"$(OUTDIR)\SysIntorWaitMode.obj" \
	"$(OUTDIR)\SysOff2OnHook.obj" \
	"$(OUTDIR)\SysIntoHaltMode.obj" \
	"$(OUTDIR)\SysException.obj" \
	"$(OUTDIR)\ExtMemOper.obj" \
	"$(OUTDIR)\ucBS_Hook.obj" \
	"$(OUTDIR)\main.obj" \
	"$(OUTDIR)\BodyInit.obj" \
	"$(OUTDIR)\libgloss.obj" \
	"$(OUTDIR)\USBD.obj" \
	"$(OUTDIR)\USBD_Drv.obj" \
	"$(OUTDIR)\USB_ID.obj" \
	"$(OUTDIR)\USB_Flash_reprog.obj" \
	"$(OUTDIR)\Flash_reprog_Drv.obj" \
	"$(OUTDIR)\Flash_reprog_boot.obj" \
	"$(OUTDIR)\SD_reprog.obj" \
	"$(OUTDIR)\SD_reprog_finish.obj" \
	"$(OUTDIR)\USBD_Export.obj" \
	"$(OUTDIR)\Space.obj" 

"$(OUTDIR)\Resource.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Resource.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Resource.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Resource.asm" 

"$(OUTDIR)\app_exception.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\app_exception.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/app_exception.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/app_exception.c" 

"$(OUTDIR)\app_exception.obj": "$(OUTDIR)\app_exception.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\app_exception.obj" "$(OUTDIR)\app_exception.asm" 

"$(OUTDIR)\audio.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\audio.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/audio.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/audio.c" 

"$(OUTDIR)\audio.obj": "$(OUTDIR)\audio.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\audio.obj" "$(OUTDIR)\audio.asm" 

"$(OUTDIR)\containers.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\containers.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/containers.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/containers.c" 

"$(OUTDIR)\containers.obj": "$(OUTDIR)\containers.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\containers.obj" "$(OUTDIR)\containers.asm" 

"$(OUTDIR)\d2d_copy.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\d2d_copy.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/d2d_copy.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/d2d_copy.c" 

"$(OUTDIR)\d2d_copy.obj": "$(OUTDIR)\d2d_copy.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\d2d_copy.obj" "$(OUTDIR)\d2d_copy.asm" 

"$(OUTDIR)\device.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\device.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/device.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/device.c" 

"$(OUTDIR)\device.obj": "$(OUTDIR)\device.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\device.obj" "$(OUTDIR)\device.asm" 

"$(OUTDIR)\files.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\files.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/files.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/files.c" 

"$(OUTDIR)\files.obj": "$(OUTDIR)\files.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\files.obj" "$(OUTDIR)\files.asm" 

"$(OUTDIR)\lists.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\lists.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/lists.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/lists.c" 

"$(OUTDIR)\lists.obj": "$(OUTDIR)\lists.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\lists.obj" "$(OUTDIR)\lists.asm" 

"$(OUTDIR)\macro.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\macro.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/macro.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/macro.c" 

"$(OUTDIR)\macro.obj": "$(OUTDIR)\macro.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\macro.obj" "$(OUTDIR)\macro.asm" 

"$(OUTDIR)\mainLoop.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\mainLoop.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/mainLoop.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/mainLoop.c" 

"$(OUTDIR)\mainLoop.obj": "$(OUTDIR)\mainLoop.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\mainLoop.obj" "$(OUTDIR)\mainLoop.asm" 

"$(OUTDIR)\parsing.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\parsing.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/parsing.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/parsing.c" 

"$(OUTDIR)\parsing.obj": "$(OUTDIR)\parsing.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\parsing.obj" "$(OUTDIR)\parsing.asm" 

"$(OUTDIR)\pkg_ops.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\pkg_ops.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/pkg_ops.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/pkg_ops.c" 

"$(OUTDIR)\pkg_ops.obj": "$(OUTDIR)\pkg_ops.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\pkg_ops.obj" "$(OUTDIR)\pkg_ops.asm" 

"$(OUTDIR)\startup.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\startup.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/startup.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/startup.c" 

"$(OUTDIR)\startup.obj": "$(OUTDIR)\startup.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\startup.obj" "$(OUTDIR)\startup.asm" 

"$(OUTDIR)\sys_counters.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\sys_counters.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/sys_counters.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/sys_counters.c" 

"$(OUTDIR)\sys_counters.obj": "$(OUTDIR)\sys_counters.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\sys_counters.obj" "$(OUTDIR)\sys_counters.asm" 

"$(OUTDIR)\timeline.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\timeline.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/timeline.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/timeline.c" 

"$(OUTDIR)\timeline.obj": "$(OUTDIR)\timeline.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\timeline.obj" "$(OUTDIR)\timeline.asm" 

"$(OUTDIR)\util.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Application\TalkingBook\util.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/util.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Application/TalkingBook/util.c" 

"$(OUTDIR)\util.obj": "$(OUTDIR)\util.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\util.obj" "$(OUTDIR)\util.asm" 

"$(OUTDIR)\SD.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Component\FS\SD.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Component/FS/SD.c" 

"$(OUTDIR)\SD.obj": "$(OUTDIR)\SD.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SD.obj" "$(OUTDIR)\SD.asm" 

"$(OUTDIR)\FS_user_nos.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Component\FS\FS_user_nos.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FS_user_nos.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Component/FS/FS_user_nos.c" 

"$(OUTDIR)\FS_user_nos.obj": "$(OUTDIR)\FS_user_nos.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\FS_user_nos.obj" "$(OUTDIR)\FS_user_nos.asm" 

"$(OUTDIR)\nls_ascii.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Component\FS\nls_ascii.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/nls_ascii.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Component/FS/nls_ascii.c" 

"$(OUTDIR)\nls_ascii.obj": "$(OUTDIR)\nls_ascii.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\nls_ascii.obj" "$(OUTDIR)\nls_ascii.asm" 

"$(OUTDIR)\DrvStruct.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Component\FS\DrvStruct.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/DrvStruct.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Component/FS/DrvStruct.c" 

"$(OUTDIR)\DrvStruct.obj": "$(OUTDIR)\DrvStruct.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\DrvStruct.obj" "$(OUTDIR)\DrvStruct.asm" 

"$(OUTDIR)\nls_codepage.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Component\FS\nls_codepage.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/nls_codepage.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Component/FS/nls_codepage.c" 

"$(OUTDIR)\nls_codepage.obj": "$(OUTDIR)\nls_codepage.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\nls_codepage.obj" "$(OUTDIR)\nls_codepage.asm" 

"$(OUTDIR)\MemPackCopy.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Component\FS\MemPackCopy.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\MemPackCopy.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Component\FS\MemPackCopy.asm" 

"$(OUTDIR)\USB.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Component\FS\USB.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/USB.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Component/FS/USB.c" 

"$(OUTDIR)\USB.obj": "$(OUTDIR)\USB.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USB.obj" "$(OUTDIR)\USB.asm" 

"$(OUTDIR)\FS_DS.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Component\Sound\Customer_Code\FS_DS.c" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\math.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\sys\reent.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\machine\ieeefp.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\_ansi.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\machine/_types.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\assert.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\limits.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FS_DS.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Component/Sound/Customer_Code/FS_DS.c" 

"$(OUTDIR)\FS_DS.obj": "$(OUTDIR)\FS_DS.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\FS_DS.obj" "$(OUTDIR)\FS_DS.asm" 

"$(OUTDIR)\SCSI_Table.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Driver\USB\SCSI_Table.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SCSI_Table.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Driver\USB\SCSI_Table.asm" 

"$(OUTDIR)\SystemIntoUSB.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Driver\USB\SystemIntoUSB.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SystemIntoUSB.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Driver/USB/SystemIntoUSB.c" 

"$(OUTDIR)\SystemIntoUSB.obj": "$(OUTDIR)\SystemIntoUSB.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SystemIntoUSB.obj" "$(OUTDIR)\SystemIntoUSB.asm" 

"$(OUTDIR)\IOKeyScan.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Driver\IOKey\IOKeyScan.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\IOKeyScan.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Driver\IOKey\IOKeyScan.asm" 

"$(OUTDIR)\startup_Data.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\startup_Data.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\startup_Data.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\startup_Data.asm" 

"$(OUTDIR)\isr.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\isr.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\isr.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\isr.asm" 

"$(OUTDIR)\ServiceLoop.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\ServiceLoop.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/ServiceLoop.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/System/ServiceLoop.c" 

"$(OUTDIR)\ServiceLoop.obj": "$(OUTDIR)\ServiceLoop.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\ServiceLoop.obj" "$(OUTDIR)\ServiceLoop.asm" 

"$(OUTDIR)\SysWaitMode.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\WaitMode\SysWaitMode.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SysWaitMode.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/System/WaitMode/SysWaitMode.c" 

"$(OUTDIR)\SysWaitMode.obj": "$(OUTDIR)\SysWaitMode.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysWaitMode.obj" "$(OUTDIR)\SysWaitMode.asm" 

"$(OUTDIR)\SysOn2OffHook.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\WaitMode\SysOn2OffHook.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysOn2OffHook.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\WaitMode\SysOn2OffHook.asm" 

"$(OUTDIR)\SysIntoHighSpeed.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\WaitMode\SysIntoHighSpeed.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntoHighSpeed.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\WaitMode\SysIntoHighSpeed.asm" 

"$(OUTDIR)\SysIntorWaitMode.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\WaitMode\SysIntorWaitMode.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntorWaitMode.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\WaitMode\SysIntorWaitMode.asm" 

"$(OUTDIR)\SysOff2OnHook.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\WaitMode\SysOff2OnHook.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysOff2OnHook.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\WaitMode\SysOff2OnHook.asm" 

"$(OUTDIR)\SysIntoHaltMode.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\WaitMode\SysIntoHaltMode.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntoHaltMode.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\WaitMode\SysIntoHaltMode.asm" 

"$(OUTDIR)\SysException.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\SysException.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SysException.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/System/SysException.c" 

"$(OUTDIR)\SysException.obj": "$(OUTDIR)\SysException.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysException.obj" "$(OUTDIR)\SysException.asm" 

"$(OUTDIR)\ExtMemOper.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\ExtMem\ExtMemOper.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\ExtMemOper.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\ExtMem\ExtMemOper.asm" 

"$(OUTDIR)\ucBS_Hook.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\System\Buddy\ucBS_Hook.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/ucBS_Hook.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/System/Buddy/ucBS_Hook.c" 

"$(OUTDIR)\ucBS_Hook.obj": "$(OUTDIR)\ucBS_Hook.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\ucBS_Hook.obj" "$(OUTDIR)\ucBS_Hook.asm" 

"$(OUTDIR)\main.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\main.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/main.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/main.c" 

"$(OUTDIR)\main.obj": "$(OUTDIR)\main.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\main.obj" "$(OUTDIR)\main.asm" 

"$(OUTDIR)\BodyInit.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\BodyInit.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/BodyInit.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/BodyInit.c" 

"$(OUTDIR)\BodyInit.obj": "$(OUTDIR)\BodyInit.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\BodyInit.obj" "$(OUTDIR)\BodyInit.asm" 

"$(OUTDIR)\libgloss.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\libgloss.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/libgloss.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/libgloss.c" 

"$(OUTDIR)\libgloss.obj": "$(OUTDIR)\libgloss.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\libgloss.obj" "$(OUTDIR)\libgloss.asm" 

"$(OUTDIR)\USBD.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\USBD.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/USBD.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Reprog/USBD.c" 

"$(OUTDIR)\USBD.obj": "$(OUTDIR)\USBD.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USBD.obj" "$(OUTDIR)\USBD.asm" 

"$(OUTDIR)\USBD_Drv.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\USBD_Drv.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USBD_Drv.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\USBD_Drv.asm" 

"$(OUTDIR)\USB_ID.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\USB_ID.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USB_ID.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\USB_ID.asm" 

"$(OUTDIR)\USB_Flash_reprog.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\USB_Flash_reprog.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/USB_Flash_reprog.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Reprog/USB_Flash_reprog.c" 

"$(OUTDIR)\USB_Flash_reprog.obj": "$(OUTDIR)\USB_Flash_reprog.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USB_Flash_reprog.obj" "$(OUTDIR)\USB_Flash_reprog.asm" 

"$(OUTDIR)\Flash_reprog_Drv.obj": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\Flash_reprog_Drv.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Flash_reprog_Drv.obj" "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\Flash_reprog_Drv.asm" 

"$(OUTDIR)\Flash_reprog_boot.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\Flash_reprog_boot.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/Flash_reprog_boot.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Reprog/Flash_reprog_boot.c" 

"$(OUTDIR)\Flash_reprog_boot.obj": "$(OUTDIR)\Flash_reprog_boot.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Flash_reprog_boot.obj" "$(OUTDIR)\Flash_reprog_boot.asm" 

"$(OUTDIR)\SD_reprog.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\SD_reprog.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD_reprog.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Reprog/SD_reprog.c" 

"$(OUTDIR)\SD_reprog.obj": "$(OUTDIR)\SD_reprog.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SD_reprog.obj" "$(OUTDIR)\SD_reprog.asm" 

"$(OUTDIR)\SD_reprog_finish.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\SD_reprog_finish.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD_reprog_finish.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Reprog/SD_reprog_finish.c" 

"$(OUTDIR)\SD_reprog_finish.obj": "$(OUTDIR)\SD_reprog_finish.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SD_reprog_finish.obj" "$(OUTDIR)\SD_reprog_finish.asm" 

"$(OUTDIR)\USBD_Export.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\USBD_Export.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/USBD_Export.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Reprog/USBD_Export.c" 

"$(OUTDIR)\USBD_Export.obj": "$(OUTDIR)\USBD_Export.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USBD_Export.obj" "$(OUTDIR)\USBD_Export.asm" 

"$(OUTDIR)\Space.asm": "E:\Documents and Settings\Cliff\My Documents\svn\software\device\trunk\firmware\Reprog\Space.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/Space.asm" "E:/Documents and Settings/Cliff/My Documents/svn/software/device/trunk/firmware/Reprog/Space.c" 

"$(OUTDIR)\Space.obj": "$(OUTDIR)\Space.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Space.obj" "$(OUTDIR)\Space.asm" 


.SUFFIXES : .c .asm .obj .s37 .tsk .res

all :	 BEFOREBUILD "$(OUTDIR)" $(BINFILE)

BEFOREBUILD :
	@echo Old links and maps for comparison
	copy System.lik System-prev.lik
	copy Debug\System.map Debug\System-prev.map

"$(OUTDIR)" :
	if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

$(BINFILE) : $(OBJFILES) 
	$(LD) $(BINTYPE) $(ARYFILE) $(BINFILE) $(LDFLAGS) $(BODY) $(EXTRAFLAGS)
	@echo Build System.bin
	"tools\srec2bin" -q debug\System.S37 debug\System.bin

compile :	 $(OBJFILES)

clean :
	$(RM) "$(OUTDIR)\Resource.obj" 
	$(RM) "$(OUTDIR)\Resource.lst" 
	$(RM) "$(OUTDIR)\app_exception.obj" 
	$(RM) "$(OUTDIR)\app_exception.lst" 
	$(RM) "$(OUTDIR)\app_exception.asm" 
	$(RM) "$(OUTDIR)\audio.obj" 
	$(RM) "$(OUTDIR)\audio.lst" 
	$(RM) "$(OUTDIR)\audio.asm" 
	$(RM) "$(OUTDIR)\containers.obj" 
	$(RM) "$(OUTDIR)\containers.lst" 
	$(RM) "$(OUTDIR)\containers.asm" 
	$(RM) "$(OUTDIR)\d2d_copy.obj" 
	$(RM) "$(OUTDIR)\d2d_copy.lst" 
	$(RM) "$(OUTDIR)\d2d_copy.asm" 
	$(RM) "$(OUTDIR)\device.obj" 
	$(RM) "$(OUTDIR)\device.lst" 
	$(RM) "$(OUTDIR)\device.asm" 
	$(RM) "$(OUTDIR)\files.obj" 
	$(RM) "$(OUTDIR)\files.lst" 
	$(RM) "$(OUTDIR)\files.asm" 
	$(RM) "$(OUTDIR)\lists.obj" 
	$(RM) "$(OUTDIR)\lists.lst" 
	$(RM) "$(OUTDIR)\lists.asm" 
	$(RM) "$(OUTDIR)\macro.obj" 
	$(RM) "$(OUTDIR)\macro.lst" 
	$(RM) "$(OUTDIR)\macro.asm" 
	$(RM) "$(OUTDIR)\mainLoop.obj" 
	$(RM) "$(OUTDIR)\mainLoop.lst" 
	$(RM) "$(OUTDIR)\mainLoop.asm" 
	$(RM) "$(OUTDIR)\parsing.obj" 
	$(RM) "$(OUTDIR)\parsing.lst" 
	$(RM) "$(OUTDIR)\parsing.asm" 
	$(RM) "$(OUTDIR)\pkg_ops.obj" 
	$(RM) "$(OUTDIR)\pkg_ops.lst" 
	$(RM) "$(OUTDIR)\pkg_ops.asm" 
	$(RM) "$(OUTDIR)\startup.obj" 
	$(RM) "$(OUTDIR)\startup.lst" 
	$(RM) "$(OUTDIR)\startup.asm" 
	$(RM) "$(OUTDIR)\sys_counters.obj" 
	$(RM) "$(OUTDIR)\sys_counters.lst" 
	$(RM) "$(OUTDIR)\sys_counters.asm" 
	$(RM) "$(OUTDIR)\timeline.obj" 
	$(RM) "$(OUTDIR)\timeline.lst" 
	$(RM) "$(OUTDIR)\timeline.asm" 
	$(RM) "$(OUTDIR)\util.obj" 
	$(RM) "$(OUTDIR)\util.lst" 
	$(RM) "$(OUTDIR)\util.asm" 
	$(RM) "$(OUTDIR)\SD.obj" 
	$(RM) "$(OUTDIR)\SD.lst" 
	$(RM) "$(OUTDIR)\SD.asm" 
	$(RM) "$(OUTDIR)\FS_user_nos.obj" 
	$(RM) "$(OUTDIR)\FS_user_nos.lst" 
	$(RM) "$(OUTDIR)\FS_user_nos.asm" 
	$(RM) "$(OUTDIR)\nls_ascii.obj" 
	$(RM) "$(OUTDIR)\nls_ascii.lst" 
	$(RM) "$(OUTDIR)\nls_ascii.asm" 
	$(RM) "$(OUTDIR)\DrvStruct.obj" 
	$(RM) "$(OUTDIR)\DrvStruct.lst" 
	$(RM) "$(OUTDIR)\DrvStruct.asm" 
	$(RM) "$(OUTDIR)\nls_codepage.obj" 
	$(RM) "$(OUTDIR)\nls_codepage.lst" 
	$(RM) "$(OUTDIR)\nls_codepage.asm" 
	$(RM) "$(OUTDIR)\MemPackCopy.obj" 
	$(RM) "$(OUTDIR)\MemPackCopy.lst" 
	$(RM) "$(OUTDIR)\USB.obj" 
	$(RM) "$(OUTDIR)\USB.lst" 
	$(RM) "$(OUTDIR)\USB.asm" 
	$(RM) "$(OUTDIR)\FS_DS.obj" 
	$(RM) "$(OUTDIR)\FS_DS.lst" 
	$(RM) "$(OUTDIR)\FS_DS.asm" 
	$(RM) "$(OUTDIR)\SCSI_Table.obj" 
	$(RM) "$(OUTDIR)\SCSI_Table.lst" 
	$(RM) "$(OUTDIR)\SystemIntoUSB.obj" 
	$(RM) "$(OUTDIR)\SystemIntoUSB.lst" 
	$(RM) "$(OUTDIR)\SystemIntoUSB.asm" 
	$(RM) "$(OUTDIR)\IOKeyScan.obj" 
	$(RM) "$(OUTDIR)\IOKeyScan.lst" 
	$(RM) "$(OUTDIR)\startup_Data.obj" 
	$(RM) "$(OUTDIR)\startup_Data.lst" 
	$(RM) "$(OUTDIR)\isr.obj" 
	$(RM) "$(OUTDIR)\isr.lst" 
	$(RM) "$(OUTDIR)\ServiceLoop.obj" 
	$(RM) "$(OUTDIR)\ServiceLoop.lst" 
	$(RM) "$(OUTDIR)\ServiceLoop.asm" 
	$(RM) "$(OUTDIR)\SysWaitMode.obj" 
	$(RM) "$(OUTDIR)\SysWaitMode.lst" 
	$(RM) "$(OUTDIR)\SysWaitMode.asm" 
	$(RM) "$(OUTDIR)\SysOn2OffHook.obj" 
	$(RM) "$(OUTDIR)\SysOn2OffHook.lst" 
	$(RM) "$(OUTDIR)\SysIntoHighSpeed.obj" 
	$(RM) "$(OUTDIR)\SysIntoHighSpeed.lst" 
	$(RM) "$(OUTDIR)\SysIntorWaitMode.obj" 
	$(RM) "$(OUTDIR)\SysIntorWaitMode.lst" 
	$(RM) "$(OUTDIR)\SysOff2OnHook.obj" 
	$(RM) "$(OUTDIR)\SysOff2OnHook.lst" 
	$(RM) "$(OUTDIR)\SysIntoHaltMode.obj" 
	$(RM) "$(OUTDIR)\SysIntoHaltMode.lst" 
	$(RM) "$(OUTDIR)\SysException.obj" 
	$(RM) "$(OUTDIR)\SysException.lst" 
	$(RM) "$(OUTDIR)\SysException.asm" 
	$(RM) "$(OUTDIR)\ExtMemOper.obj" 
	$(RM) "$(OUTDIR)\ExtMemOper.lst" 
	$(RM) "$(OUTDIR)\ucBS_Hook.obj" 
	$(RM) "$(OUTDIR)\ucBS_Hook.lst" 
	$(RM) "$(OUTDIR)\ucBS_Hook.asm" 
	$(RM) "$(OUTDIR)\main.obj" 
	$(RM) "$(OUTDIR)\main.lst" 
	$(RM) "$(OUTDIR)\main.asm" 
	$(RM) "$(OUTDIR)\BodyInit.obj" 
	$(RM) "$(OUTDIR)\BodyInit.lst" 
	$(RM) "$(OUTDIR)\BodyInit.asm" 
	$(RM) "$(OUTDIR)\libgloss.obj" 
	$(RM) "$(OUTDIR)\libgloss.lst" 
	$(RM) "$(OUTDIR)\libgloss.asm" 
	$(RM) "$(OUTDIR)\USBD.obj" 
	$(RM) "$(OUTDIR)\USBD.lst" 
	$(RM) "$(OUTDIR)\USBD.asm" 
	$(RM) "$(OUTDIR)\USBD_Drv.obj" 
	$(RM) "$(OUTDIR)\USBD_Drv.lst" 
	$(RM) "$(OUTDIR)\USB_ID.obj" 
	$(RM) "$(OUTDIR)\USB_ID.lst" 
	$(RM) "$(OUTDIR)\USB_Flash_reprog.obj" 
	$(RM) "$(OUTDIR)\USB_Flash_reprog.lst" 
	$(RM) "$(OUTDIR)\USB_Flash_reprog.asm" 
	$(RM) "$(OUTDIR)\Flash_reprog_Drv.obj" 
	$(RM) "$(OUTDIR)\Flash_reprog_Drv.lst" 
	$(RM) "$(OUTDIR)\Flash_reprog_boot.obj" 
	$(RM) "$(OUTDIR)\Flash_reprog_boot.lst" 
	$(RM) "$(OUTDIR)\Flash_reprog_boot.asm" 
	$(RM) "$(OUTDIR)\SD_reprog.obj" 
	$(RM) "$(OUTDIR)\SD_reprog.lst" 
	$(RM) "$(OUTDIR)\SD_reprog.asm" 
	$(RM) "$(OUTDIR)\SD_reprog_finish.obj" 
	$(RM) "$(OUTDIR)\SD_reprog_finish.lst" 
	$(RM) "$(OUTDIR)\SD_reprog_finish.asm" 
	$(RM) "$(OUTDIR)\USBD_Export.obj" 
	$(RM) "$(OUTDIR)\USBD_Export.lst" 
	$(RM) "$(OUTDIR)\USBD_Export.asm" 
	$(RM) "$(OUTDIR)\Space.obj" 
	$(RM) "$(OUTDIR)\Space.lst" 
	$(RM) "$(OUTDIR)\Space.asm" 
	$(RM) $(BINFILE) $(SBMFILE) 

.c.asm:
	$(CC) $(CFLAGS) $(INCLUDES) -o "$(OUTDIR)/$@" $<

.asm.obj:
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)/$@" $<

