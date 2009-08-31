#####################################################################
#																	 
#	Created by u'nSP IDE		01:50:30	08/31/09
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

INCLUDES	= -I"C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware" -I"C:/Program Files/Generalplus/unSPIDE 3.0.1/library/include" -I"C:/Program Files/Generalplus/unSPIDE 3.0.1/library/include/sys" -I"./Component/Include/FS"

BODY	= -body GPL162003_CS0Flash -nobdy -bfile "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System.bdy"

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
	"$(OUTDIR)\Inbox.obj" \
	"$(OUTDIR)\lists.obj" \
	"$(OUTDIR)\macro.obj" \
	"$(OUTDIR)\mainLoop.obj" \
	"$(OUTDIR)\parsing.obj" \
	"$(OUTDIR)\pkg_ops.obj" \
	"$(OUTDIR)\startup.obj" \
	"$(OUTDIR)\sys_counters.obj" \
	"$(OUTDIR)\timeline.obj" \
	"$(OUTDIR)\util.obj" \
	"$(OUTDIR)\quiz.obj" \
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

"$(OUTDIR)\Resource.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Resource.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Resource.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Resource.asm" 

"$(OUTDIR)\app_exception.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\app_exception.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/app_exception.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/app_exception.c" 

"$(OUTDIR)\app_exception.obj": "$(OUTDIR)\app_exception.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\app_exception.obj" "$(OUTDIR)\app_exception.asm" 

"$(OUTDIR)\audio.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\audio.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\pkg_ops.h" ".\Application\TalkingBook\Include\device.h" ".\Application\TalkingBook\Include\containers.h" ".\Application\TalkingBook\Include\audio.h" ".\Application\TalkingBook\Include\util.h" ".\Application\TalkingBook\Include\lists.h" "Component\Include\FS\vfs.h" "Component\Include\component_head.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/audio.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/audio.c" 

"$(OUTDIR)\audio.obj": "$(OUTDIR)\audio.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\audio.obj" "$(OUTDIR)\audio.asm" 

"$(OUTDIR)\containers.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\containers.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\containers.h" ".\Application\TalkingBook\Include\util.h" ".\Application\TalkingBook\Include\lists.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/containers.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/containers.c" 

"$(OUTDIR)\containers.obj": "$(OUTDIR)\containers.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\containers.obj" "$(OUTDIR)\containers.asm" 

"$(OUTDIR)\d2d_copy.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\d2d_copy.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\util.h" ".\Application\TalkingBook\Include\device.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\audio.h" ".\Application\TalkingBook\Include\containers.h" ".\Application\TalkingBook\Include\d2d_copy.h" ".\Application\TalkingBook\Include\lists.h" "Component\Include\FS\vfs.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" "Component\Include\component_head.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/d2d_copy.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/d2d_copy.c" 

"$(OUTDIR)\d2d_copy.obj": "$(OUTDIR)\d2d_copy.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\d2d_copy.obj" "$(OUTDIR)\d2d_copy.asm" 

"$(OUTDIR)\device.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\device.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\macro.h" ".\Application\TalkingBook\Include\containers.h" ".\Application\TalkingBook\Include\Inbox.h" ".\Application\TalkingBook\Include\audio.h" ".\Application\TalkingBook\Include\device.h" ".\Application\TalkingBook\Include\util.h" ".\Application\TalkingBook\Include\lists.h" "Component\Include\FS\vfs.h" "Component\Include\component_head.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/device.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/device.c" 

"$(OUTDIR)\device.obj": "$(OUTDIR)\device.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\device.obj" "$(OUTDIR)\device.asm" 

"$(OUTDIR)\files.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\files.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\device.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\util.h" "Component\Include\component_head.h" "Component\Include\FS\vfs.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/files.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/files.c" 

"$(OUTDIR)\files.obj": "$(OUTDIR)\files.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\files.obj" "$(OUTDIR)\files.asm" 

"$(OUTDIR)\Inbox.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\Inbox.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\device.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\lists.h" ".\Application\TalkingBook\Include\containers.h" ".\Application\TalkingBook\Include\audio.h" ".\Application\TalkingBook\Include\pkg_ops.h" ".\Application\TalkingBook\Include\SD_reprog.h" ".\Application\TalkingBook\Include\Inbox.h" ".\Application\TalkingBook\Include\util.h" "Component\Include\component_head.h" "Component\Include\FS\vfs.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/Inbox.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/Inbox.c" 

"$(OUTDIR)\Inbox.obj": "$(OUTDIR)\Inbox.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Inbox.obj" "$(OUTDIR)\Inbox.asm" 

"$(OUTDIR)\lists.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\lists.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\audio.h" ".\Application\TalkingBook\Include\containers.h" ".\Application\TalkingBook\Include\lists.h" ".\Application\TalkingBook\Include\util.h" "Component\Include\FS\vfs.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/lists.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/lists.c" 

"$(OUTDIR)\lists.obj": "$(OUTDIR)\lists.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\lists.obj" "$(OUTDIR)\lists.asm" 

"$(OUTDIR)\macro.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\macro.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\device.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\macro.h" ".\Application\TalkingBook\Include\util.h" "Component\Include\component_head.h" "Component\Include\FS\vfs.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/macro.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/macro.c" 

"$(OUTDIR)\macro.obj": "$(OUTDIR)\macro.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\macro.obj" "$(OUTDIR)\macro.asm" 

"$(OUTDIR)\mainLoop.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\mainLoop.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\containers.h" ".\Application\TalkingBook\Include\timeline.h" ".\Application\TalkingBook\Include\device.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\startup.h" ".\Application\TalkingBook\Include\sys_counters.h" ".\Application\TalkingBook\Include\audio.h" ".\Application\TalkingBook\Include\pkg_ops.h" ".\Application\TalkingBook\Include\parsing.h" ".\Application\TalkingBook\Include\macro.h" ".\Application\TalkingBook\Include\d2d_copy.h" ".\Application\TalkingBook\Include\Inbox.h" ".\Application\TalkingBook\Include\mainLoop.h" ".\Application\TalkingBook\Include\util.h" ".\Application\TalkingBook\Include\lists.h" "Component\Include\component_head.h" "Component\Include\FS\vfs.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/mainLoop.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/mainLoop.c" 

"$(OUTDIR)\mainLoop.obj": "$(OUTDIR)\mainLoop.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\mainLoop.obj" "$(OUTDIR)\mainLoop.asm" 

"$(OUTDIR)\parsing.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\parsing.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\device.h" ".\Application\TalkingBook\Include\containers.h" ".\Application\TalkingBook\Include\parsing.h" ".\Application\TalkingBook\Include\util.h" ".\Application\TalkingBook\Include\lists.h" "Component\Include\FS\vfs.h" "Component\Include\component_head.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/parsing.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/parsing.c" 

"$(OUTDIR)\parsing.obj": "$(OUTDIR)\parsing.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\parsing.obj" "$(OUTDIR)\parsing.asm" 

"$(OUTDIR)\pkg_ops.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\pkg_ops.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\device.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\lists.h" ".\Application\TalkingBook\Include\containers.h" ".\Application\TalkingBook\Include\audio.h" ".\Application\TalkingBook\Include\pkg_ops.h" ".\Application\TalkingBook\Include\util.h" "Component\Include\component_head.h" "Component\Include\FS\vfs.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/pkg_ops.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/pkg_ops.c" 

"$(OUTDIR)\pkg_ops.obj": "$(OUTDIR)\pkg_ops.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\pkg_ops.obj" "$(OUTDIR)\pkg_ops.asm" 

"$(OUTDIR)\startup.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\startup.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\device.h" ".\Application\TalkingBook\Include\parsing.h" ".\Application\TalkingBook\Include\macro.h" ".\Application\TalkingBook\Include\sys_counters.h" ".\Application\TalkingBook\Include\Inbox.h" ".\Application\TalkingBook\Include\startup.h" ".\Application\TalkingBook\Include\util.h" ".\Application\TalkingBook\Include\containers.h" ".\Application\TalkingBook\Include\lists.h" "Component\Include\FS\vfs.h" "Component\Include\component_head.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/startup.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/startup.c" 

"$(OUTDIR)\startup.obj": "$(OUTDIR)\startup.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\startup.obj" "$(OUTDIR)\startup.asm" 

"$(OUTDIR)\sys_counters.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\sys_counters.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\files.h" ".\Application\TalkingBook\Include\sys_counters.h" ".\Application\TalkingBook\Include\util.h" "Component\Include\FS\vfs.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/sys_counters.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/sys_counters.c" 

"$(OUTDIR)\sys_counters.obj": "$(OUTDIR)\sys_counters.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\sys_counters.obj" "$(OUTDIR)\sys_counters.asm" 

"$(OUTDIR)\timeline.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\timeline.c" ".\Application\TalkingBook\Include\talkingbook.h" ".\Application\TalkingBook\Include\device.h" ".\Application\TalkingBook\Include\containers.h" ".\Application\TalkingBook\Include\timeline.h" ".\Application\TalkingBook\Include\util.h" ".\Application\TalkingBook\Include\lists.h" "Component\Include\component_head.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/timeline.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/timeline.c" 

"$(OUTDIR)\timeline.obj": "$(OUTDIR)\timeline.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\timeline.obj" "$(OUTDIR)\timeline.asm" 

"$(OUTDIR)\util.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\util.c" ".\Application\TalkingBook\Include\app_exception.h" ".\Application\TalkingBook\Include\util.h" ".\Component\Include\FS\typedef.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\string.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\stddef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/util.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/util.c" 

"$(OUTDIR)\util.obj": "$(OUTDIR)\util.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\util.obj" "$(OUTDIR)\util.asm" 

"$(OUTDIR)\quiz.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Application\TalkingBook\quiz.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/quiz.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Application/TalkingBook/quiz.c" 

"$(OUTDIR)\quiz.obj": "$(OUTDIR)\quiz.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\quiz.obj" "$(OUTDIR)\quiz.asm" 

"$(OUTDIR)\SD.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Component\FS\SD.c" ".\Component\Include\FS\typedef.h" ".\Component\Include\FS\DriverLayer.h" ".\Component\Include\FS\typedef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Component/FS/SD.c" 

"$(OUTDIR)\SD.obj": "$(OUTDIR)\SD.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SD.obj" "$(OUTDIR)\SD.asm" 

"$(OUTDIR)\FS_user_nos.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Component\FS\FS_user_nos.c" "System\include\system\GPL162002.h" "Component\Include\FS\typedef.h" "Application\TalkingBook\Include\sys_counters.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FS_user_nos.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Component/FS/FS_user_nos.c" 

"$(OUTDIR)\FS_user_nos.obj": "$(OUTDIR)\FS_user_nos.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\FS_user_nos.obj" "$(OUTDIR)\FS_user_nos.asm" 

"$(OUTDIR)\nls_ascii.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Component\FS\nls_ascii.c" ".\Component\Include\FS\vfs.h" ".\Component\Include\FS\Typedef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/nls_ascii.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Component/FS/nls_ascii.c" 

"$(OUTDIR)\nls_ascii.obj": "$(OUTDIR)\nls_ascii.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\nls_ascii.obj" "$(OUTDIR)\nls_ascii.asm" 

"$(OUTDIR)\DrvStruct.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Component\FS\DrvStruct.c" ".\Component\Include\FS\DriverLayer.h" ".\Component\Include\FS\typedef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/DrvStruct.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Component/FS/DrvStruct.c" 

"$(OUTDIR)\DrvStruct.obj": "$(OUTDIR)\DrvStruct.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\DrvStruct.obj" "$(OUTDIR)\DrvStruct.asm" 

"$(OUTDIR)\nls_codepage.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Component\FS\nls_codepage.c" ".\Component\Include\FS\vfs.h" ".\Component\Include\FS\Typedef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/nls_codepage.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Component/FS/nls_codepage.c" 

"$(OUTDIR)\nls_codepage.obj": "$(OUTDIR)\nls_codepage.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\nls_codepage.obj" "$(OUTDIR)\nls_codepage.asm" 

"$(OUTDIR)\MemPackCopy.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Component\FS\MemPackCopy.asm" "System\Include\System\GPL162002.inc" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\MemPackCopy.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Component\FS\MemPackCopy.asm" 

"$(OUTDIR)\USB.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Component\FS\USB.c" ".\Component\Include\FS\typedef.h" ".\Component\Include\FS\DriverLayer.h" ".\Component\Include\FS\typedef.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/USB.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Component/FS/USB.c" 

"$(OUTDIR)\USB.obj": "$(OUTDIR)\USB.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USB.obj" "$(OUTDIR)\USB.asm" 

"$(OUTDIR)\FS_DS.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Component\Sound\Customer_Code\FS_DS.c" "Component\Include\Component_head.h" "System\Include\System_head.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\math.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\sys\reent.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\machine\ieeefp.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\_ansi.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\machine/_types.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\assert.h" "C:\Program Files\Generalplus\unSPIDE 3.0.1\library\include\limits.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FS_DS.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Component/Sound/Customer_Code/FS_DS.c" 

"$(OUTDIR)\FS_DS.obj": "$(OUTDIR)\FS_DS.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\FS_DS.obj" "$(OUTDIR)\FS_DS.asm" 

"$(OUTDIR)\SCSI_Table.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Driver\USB\SCSI_Table.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SCSI_Table.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Driver\USB\SCSI_Table.asm" 

"$(OUTDIR)\SystemIntoUSB.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Driver\USB\SystemIntoUSB.c" "Reprog\USBD_Define.h" "Reprog\USBD.h" "Reprog\USB_Flash_reprog.h" "Component\FS\usb_host\USB_Host_Constant.h" "Component\FS\usb_host\USBHostMSDC.h" "Application\Talkingbook\Include\talkingbook.h" "Application\Talkingbook\Include\device.h" "Application\Talkingbook\Include\containers.h" "Application\TalkingBook\Include\device.h" "Reprog\USBD_Define.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SystemIntoUSB.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Driver/USB/SystemIntoUSB.c" 

"$(OUTDIR)\SystemIntoUSB.obj": "$(OUTDIR)\SystemIntoUSB.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SystemIntoUSB.obj" "$(OUTDIR)\SystemIntoUSB.asm" 

"$(OUTDIR)\IOKeyScan.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Driver\IOKey\IOKeyScan.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\IOKeyScan.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Driver\IOKey\IOKeyScan.asm" 

"$(OUTDIR)\startup_Data.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\startup_Data.asm" "system\include\system_head.inc" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\startup_Data.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\startup_Data.asm" 

"$(OUTDIR)\isr.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\isr.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\isr.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\isr.asm" 

"$(OUTDIR)\ServiceLoop.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\ServiceLoop.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/ServiceLoop.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/System/ServiceLoop.c" 

"$(OUTDIR)\ServiceLoop.obj": "$(OUTDIR)\ServiceLoop.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\ServiceLoop.obj" "$(OUTDIR)\ServiceLoop.asm" 

"$(OUTDIR)\SysWaitMode.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\WaitMode\SysWaitMode.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SysWaitMode.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/System/WaitMode/SysWaitMode.c" 

"$(OUTDIR)\SysWaitMode.obj": "$(OUTDIR)\SysWaitMode.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysWaitMode.obj" "$(OUTDIR)\SysWaitMode.asm" 

"$(OUTDIR)\SysOn2OffHook.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\WaitMode\SysOn2OffHook.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysOn2OffHook.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\WaitMode\SysOn2OffHook.asm" 

"$(OUTDIR)\SysIntoHighSpeed.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\WaitMode\SysIntoHighSpeed.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntoHighSpeed.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\WaitMode\SysIntoHighSpeed.asm" 

"$(OUTDIR)\SysIntorWaitMode.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\WaitMode\SysIntorWaitMode.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntorWaitMode.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\WaitMode\SysIntorWaitMode.asm" 

"$(OUTDIR)\SysOff2OnHook.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\WaitMode\SysOff2OnHook.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysOff2OnHook.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\WaitMode\SysOff2OnHook.asm" 

"$(OUTDIR)\SysIntoHaltMode.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\WaitMode\SysIntoHaltMode.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntoHaltMode.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\WaitMode\SysIntoHaltMode.asm" 

"$(OUTDIR)\SysException.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\SysException.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SysException.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/System/SysException.c" 

"$(OUTDIR)\SysException.obj": "$(OUTDIR)\SysException.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysException.obj" "$(OUTDIR)\SysException.asm" 

"$(OUTDIR)\ExtMemOper.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\ExtMem\ExtMemOper.asm" "system\include\system_head.inc" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\ExtMemOper.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\ExtMem\ExtMemOper.asm" 

"$(OUTDIR)\ucBS_Hook.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\System\Buddy\ucBS_Hook.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/ucBS_Hook.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/System/Buddy/ucBS_Hook.c" 

"$(OUTDIR)\ucBS_Hook.obj": "$(OUTDIR)\ucBS_Hook.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\ucBS_Hook.obj" "$(OUTDIR)\ucBS_Hook.asm" 

"$(OUTDIR)\main.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\main.c" ".\system\include\system_head.h" ".\driver\include\driver_head.h" ".\component\include\component_head.h" ".\Application\TalkingBook\Include\mainloop.h" "System\include\system\GPL162002.h" "System\include\system\GPL162001ISR.h" "System\include\system\SysException.h" "System\include\system\Sys_GV.h" "System\include\WaitMode\SysWaitMode.h" "System\include\Message\Message.h" "System\include\ExtMem\unExtMem.h" "System\include\buddy\ucBS.h" "system\include\system\unSPMACRO.h" "driver\include\SD\SDCDriver.h" "driver\include\NandDriver\Nand2_Oper.h" "driver\include\NandDriver\nanddriver.h" "Component\Include\FS\typedef.h" "Component\Include\FS\vfs.h" "Component\Include\speech\algorithm.h" "Component\Include\speech\sacm_API.h" "Component\Include\speech\MP3.h" "Component\Include\speech\SACM_GV.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/main.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/main.c" 

"$(OUTDIR)\main.obj": "$(OUTDIR)\main.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\main.obj" "$(OUTDIR)\main.asm" 

"$(OUTDIR)\BodyInit.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\BodyInit.c" ".\system\include\system_head.h" ".\driver\include\driver_head.h" ".\component\include\component_head.h" "System\include\system\GPL162002.h" "System\include\system\GPL162001ISR.h" "System\include\system\SysException.h" "System\include\system\Sys_GV.h" "System\include\WaitMode\SysWaitMode.h" "System\include\Message\Message.h" "System\include\ExtMem\unExtMem.h" "System\include\buddy\ucBS.h" "system\include\system\unSPMACRO.h" "driver\include\SD\SDCDriver.h" "driver\include\NandDriver\Nand2_Oper.h" "driver\include\NandDriver\nanddriver.h" "Component\Include\FS\typedef.h" "Component\Include\FS\vfs.h" "Component\Include\speech\algorithm.h" "Component\Include\speech\sacm_API.h" "Component\Include\speech\MP3.h" "Component\Include\speech\SACM_GV.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/BodyInit.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/BodyInit.c" 

"$(OUTDIR)\BodyInit.obj": "$(OUTDIR)\BodyInit.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\BodyInit.obj" "$(OUTDIR)\BodyInit.asm" 

"$(OUTDIR)\libgloss.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\libgloss.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/libgloss.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/libgloss.c" 

"$(OUTDIR)\libgloss.obj": "$(OUTDIR)\libgloss.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\libgloss.obj" "$(OUTDIR)\libgloss.asm" 

"$(OUTDIR)\USBD.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\USBD.c" ".\Reprog\GPL162002.h" ".\Reprog\USB_Flash_reprog.h" "reprog\USBD_Define.h" "reprog\USBD.h" "Driver\Include\USB\USBD_Define.h" "Driver\Include\USB\USBD.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/USBD.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Reprog/USBD.c" 

"$(OUTDIR)\USBD.obj": "$(OUTDIR)\USBD.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USBD.obj" "$(OUTDIR)\USBD.asm" 

"$(OUTDIR)\USBD_Drv.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\USBD_Drv.asm" "System\include\system\GPL162002.inc" "Driver\Include\USB\USBD.inc" "Driver\Include\USB\USBD_Define.inc" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USBD_Drv.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\USBD_Drv.asm" 

"$(OUTDIR)\USB_ID.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\USB_ID.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USB_ID.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\USB_ID.asm" 

"$(OUTDIR)\USB_Flash_reprog.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\USB_Flash_reprog.c" ".\Reprog\USB_Flash_reprog.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/USB_Flash_reprog.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Reprog/USB_Flash_reprog.c" 

"$(OUTDIR)\USB_Flash_reprog.obj": "$(OUTDIR)\USB_Flash_reprog.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USB_Flash_reprog.obj" "$(OUTDIR)\USB_Flash_reprog.asm" 

"$(OUTDIR)\Flash_reprog_Drv.obj": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\Flash_reprog_Drv.asm" "System\include\system\GPL162002.inc" "Reprog\USBD_Define.inc" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Flash_reprog_Drv.obj" "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\Flash_reprog_Drv.asm" 

"$(OUTDIR)\Flash_reprog_boot.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\Flash_reprog_boot.c" ".\Reprog\GPL162002.h" ".\Reprog\USB_Flash_reprog.h" "Reprog\USBD_Define.h" "Reprog\USBD.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/Flash_reprog_boot.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Reprog/Flash_reprog_boot.c" 

"$(OUTDIR)\Flash_reprog_boot.obj": "$(OUTDIR)\Flash_reprog_boot.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Flash_reprog_boot.obj" "$(OUTDIR)\Flash_reprog_boot.asm" 

"$(OUTDIR)\SD_reprog.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\SD_reprog.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD_reprog.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Reprog/SD_reprog.c" 

"$(OUTDIR)\SD_reprog.obj": "$(OUTDIR)\SD_reprog.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SD_reprog.obj" "$(OUTDIR)\SD_reprog.asm" 

"$(OUTDIR)\SD_reprog_finish.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\SD_reprog_finish.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD_reprog_finish.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Reprog/SD_reprog_finish.c" 

"$(OUTDIR)\SD_reprog_finish.obj": "$(OUTDIR)\SD_reprog_finish.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SD_reprog_finish.obj" "$(OUTDIR)\SD_reprog_finish.asm" 

"$(OUTDIR)\USBD_Export.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\USBD_Export.c" "System\Include\System\GPL162002.h" "Reprog\USBD_Define.h" "Reprog\USBD.h" "Driver\Include\USB\USBD_Define.h" "Driver\Include\USB\USBD.h" "Reprog\USB_Flash_reprog.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/USBD_Export.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Reprog/USBD_Export.c" 

"$(OUTDIR)\USBD_Export.obj": "$(OUTDIR)\USBD_Export.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USBD_Export.obj" "$(OUTDIR)\USBD_Export.asm" 

"$(OUTDIR)\Space.asm": "C:\Documents and Settings\Cliff Schmidt\My Documents\talkingbook\software\device\trunk\firmware\Reprog\Space.c" ".\Component\Include\FS\typedef.h" ".\Component\Include\FS\DriverLayer.h" ".\Component\Include\FS\typedef.h" "Reprog/USBD_Define.h" "Reprog/USBD.h" "System\include\system\GPL162002.h" "Component\Include\FS\typedef.h" "application\talkingbook\include\talkingbook.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/Space.asm" "C:/Documents and Settings/Cliff Schmidt/My Documents/talkingbook/software/device/trunk/firmware/Reprog/Space.c" 

"$(OUTDIR)\Space.obj": "$(OUTDIR)\Space.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Space.obj" "$(OUTDIR)\Space.asm" 


.SUFFIXES : .c .asm .obj .s37 .tsk .res

all :	 BEFOREBUILD "$(OUTDIR)" $(BINFILE)

BEFOREBUILD :
	@echo Old links and maps for comparison
	rem copy System.lik System-prev.lik
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
	$(RM) "$(OUTDIR)\Inbox.obj" 
	$(RM) "$(OUTDIR)\Inbox.lst" 
	$(RM) "$(OUTDIR)\Inbox.asm" 
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
	$(RM) "$(OUTDIR)\quiz.obj" 
	$(RM) "$(OUTDIR)\quiz.lst" 
	$(RM) "$(OUTDIR)\quiz.asm" 
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

