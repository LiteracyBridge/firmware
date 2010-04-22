#####################################################################
#																	 
#	Created by u'nSP IDE		23:55:09	04/21/10
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

INCLUDES	= -I"C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem" -I"C:/Program Files/Generalplus/unSPIDE 3.0.1/library/include" -I"C:/Program Files/Generalplus/unSPIDE 3.0.1/library/include/sys" -I"./Component/Include/FS" -I"../"

BODY	= -body GPL162003_CS0Flash -nobdy -bfile "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\himem.bdy"

BINFILE	= "$(OUTDIR)\himem.S37"

ARYFILE	= "$(OUTDIR)\himem.ary"

SBMFILE	= "$(OUTDIR)\himem.sbm"

OPT	= -S -gstabs -Wall -mglobal-var-iram -mISA=2.0

ASFLAGS	= -t4 -d -be

CASFLAGS	= -t4 -be -wpop

CFLAGS	= $(OPT) -B$(APPDIR)\toolchain\ $(INCLUDES) 

BINTYPE	= -as

LDFLAGS	= -forspace -initdata

EXTRAFLAGS	= 


OBJFILES	= \
	"$(OUTDIR)\Resource.obj" \
	"$(OUTDIR)\SD.obj" \
	"$(OUTDIR)\FS_user_nos.obj" \
	"$(OUTDIR)\nls_ascii.obj" \
	"$(OUTDIR)\DrvStruct.obj" \
	"$(OUTDIR)\nls_codepage.obj" \
	"$(OUTDIR)\nordriver.obj" \
	"$(OUTDIR)\startup_Data.obj" \
	"$(OUTDIR)\ServiceLoop.obj" \
	"$(OUTDIR)\SysWaitMode.obj" \
	"$(OUTDIR)\SysOn2OffHook.obj" \
	"$(OUTDIR)\SysIntoHighSpeed.obj" \
	"$(OUTDIR)\SysIntorWaitMode.obj" \
	"$(OUTDIR)\SysOff2OnHook.obj" \
	"$(OUTDIR)\SysIntoHaltMode.obj" \
	"$(OUTDIR)\SysException.obj" \
	"$(OUTDIR)\ExtMemOper.obj" \
	"$(OUTDIR)\System.obj" \
	"$(OUTDIR)\isr.obj" \
	"$(OUTDIR)\main.obj" \
	"$(OUTDIR)\BodyInit.obj" \
	"$(OUTDIR)\libgloss.obj" \
	"$(OUTDIR)\SD_reprog_finish.obj" \
	"$(OUTDIR)\USB_Flash_reprog.obj" 

"$(OUTDIR)\Resource.obj": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\Resource.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Resource.obj" "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\Resource.asm" 

"$(OUTDIR)\SD.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\Component\FS\SD.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/Component/FS/SD.c" 

"$(OUTDIR)\SD.obj": "$(OUTDIR)\SD.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SD.obj" "$(OUTDIR)\SD.asm" 

"$(OUTDIR)\FS_user_nos.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\Component\FS\FS_user_nos.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FS_user_nos.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/Component/FS/FS_user_nos.c" 

"$(OUTDIR)\FS_user_nos.obj": "$(OUTDIR)\FS_user_nos.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\FS_user_nos.obj" "$(OUTDIR)\FS_user_nos.asm" 

"$(OUTDIR)\nls_ascii.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\Component\FS\nls_ascii.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/nls_ascii.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/Component/FS/nls_ascii.c" 

"$(OUTDIR)\nls_ascii.obj": "$(OUTDIR)\nls_ascii.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\nls_ascii.obj" "$(OUTDIR)\nls_ascii.asm" 

"$(OUTDIR)\DrvStruct.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\Component\FS\DrvStruct.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/DrvStruct.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/Component/FS/DrvStruct.c" 

"$(OUTDIR)\DrvStruct.obj": "$(OUTDIR)\DrvStruct.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\DrvStruct.obj" "$(OUTDIR)\DrvStruct.asm" 

"$(OUTDIR)\nls_codepage.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\Component\FS\nls_codepage.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/nls_codepage.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/Component/FS/nls_codepage.c" 

"$(OUTDIR)\nls_codepage.obj": "$(OUTDIR)\nls_codepage.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\nls_codepage.obj" "$(OUTDIR)\nls_codepage.asm" 

"$(OUTDIR)\nordriver.obj": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\Driver\NOR\nordriver.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\nordriver.obj" "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\Driver\NOR\nordriver.asm" 

"$(OUTDIR)\startup_Data.obj": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\startup_Data.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\startup_Data.obj" "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\startup_Data.asm" 

"$(OUTDIR)\ServiceLoop.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\ServiceLoop.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/ServiceLoop.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/System/ServiceLoop.c" 

"$(OUTDIR)\ServiceLoop.obj": "$(OUTDIR)\ServiceLoop.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\ServiceLoop.obj" "$(OUTDIR)\ServiceLoop.asm" 

"$(OUTDIR)\SysWaitMode.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\WaitMode\SysWaitMode.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SysWaitMode.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/System/WaitMode/SysWaitMode.c" 

"$(OUTDIR)\SysWaitMode.obj": "$(OUTDIR)\SysWaitMode.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysWaitMode.obj" "$(OUTDIR)\SysWaitMode.asm" 

"$(OUTDIR)\SysOn2OffHook.obj": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\WaitMode\SysOn2OffHook.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysOn2OffHook.obj" "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\WaitMode\SysOn2OffHook.asm" 

"$(OUTDIR)\SysIntoHighSpeed.obj": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\WaitMode\SysIntoHighSpeed.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntoHighSpeed.obj" "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\WaitMode\SysIntoHighSpeed.asm" 

"$(OUTDIR)\SysIntorWaitMode.obj": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\WaitMode\SysIntorWaitMode.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntorWaitMode.obj" "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\WaitMode\SysIntorWaitMode.asm" 

"$(OUTDIR)\SysOff2OnHook.obj": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\WaitMode\SysOff2OnHook.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysOff2OnHook.obj" "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\WaitMode\SysOff2OnHook.asm" 

"$(OUTDIR)\SysIntoHaltMode.obj": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\WaitMode\SysIntoHaltMode.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntoHaltMode.obj" "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\WaitMode\SysIntoHaltMode.asm" 

"$(OUTDIR)\SysException.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\SysException.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SysException.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/System/SysException.c" 

"$(OUTDIR)\SysException.obj": "$(OUTDIR)\SysException.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysException.obj" "$(OUTDIR)\SysException.asm" 

"$(OUTDIR)\ExtMemOper.obj": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\ExtMem\ExtMemOper.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\ExtMemOper.obj" "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\ExtMem\ExtMemOper.asm" 

"$(OUTDIR)\System.obj": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\System.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\System.obj" "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\System.asm" 

"$(OUTDIR)\isr.obj": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\isr.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\isr.obj" "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\System\isr.asm" 

"$(OUTDIR)\main.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\main.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/main.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/main.c" 

"$(OUTDIR)\main.obj": "$(OUTDIR)\main.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\main.obj" "$(OUTDIR)\main.asm" 

"$(OUTDIR)\BodyInit.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\BodyInit.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/BodyInit.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/BodyInit.c" 

"$(OUTDIR)\BodyInit.obj": "$(OUTDIR)\BodyInit.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\BodyInit.obj" "$(OUTDIR)\BodyInit.asm" 

"$(OUTDIR)\libgloss.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\libgloss.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/libgloss.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/libgloss.c" 

"$(OUTDIR)\libgloss.obj": "$(OUTDIR)\libgloss.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\libgloss.obj" "$(OUTDIR)\libgloss.asm" 

"$(OUTDIR)\SD_reprog_finish.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\reprog1\SD_reprog_finish.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD_reprog_finish.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/reprog1/SD_reprog_finish.c" 

"$(OUTDIR)\SD_reprog_finish.obj": "$(OUTDIR)\SD_reprog_finish.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SD_reprog_finish.obj" "$(OUTDIR)\SD_reprog_finish.asm" 

"$(OUTDIR)\USB_Flash_reprog.asm": "C:\Documents and Settings\Administrator\My Documents\talkingbook\software\device\trunk\Himem\reprog1\USB_Flash_reprog.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/USB_Flash_reprog.asm" "C:/Documents and Settings/Administrator/My Documents/talkingbook/software/device/trunk/Himem/reprog1/USB_Flash_reprog.c" 

"$(OUTDIR)\USB_Flash_reprog.obj": "$(OUTDIR)\USB_Flash_reprog.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USB_Flash_reprog.obj" "$(OUTDIR)\USB_Flash_reprog.asm" 


.SUFFIXES : .c .asm .obj .s37 .tsk .res

all :	 BEFOREBUILD "$(OUTDIR)" $(BINFILE)

BEFOREBUILD :

"$(OUTDIR)" :
	if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

$(BINFILE) : $(OBJFILES) 
	$(LD) $(BINTYPE) $(ARYFILE) $(BINFILE) $(LDFLAGS) $(BODY) $(EXTRAFLAGS)
	"tools\srec2bin" -q debug\himem.S37 debug\himem.bin
	"tools\dd" if=debug\himem.bin of=debug\himem.blk bs=32k count=4 skip=40

compile :	 $(OBJFILES)

clean :
	$(RM) "$(OUTDIR)\Resource.obj" 
	$(RM) "$(OUTDIR)\Resource.lst" 
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
	$(RM) "$(OUTDIR)\nordriver.obj" 
	$(RM) "$(OUTDIR)\nordriver.lst" 
	$(RM) "$(OUTDIR)\startup_Data.obj" 
	$(RM) "$(OUTDIR)\startup_Data.lst" 
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
	$(RM) "$(OUTDIR)\System.obj" 
	$(RM) "$(OUTDIR)\System.lst" 
	$(RM) "$(OUTDIR)\isr.obj" 
	$(RM) "$(OUTDIR)\isr.lst" 
	$(RM) "$(OUTDIR)\main.obj" 
	$(RM) "$(OUTDIR)\main.lst" 
	$(RM) "$(OUTDIR)\main.asm" 
	$(RM) "$(OUTDIR)\BodyInit.obj" 
	$(RM) "$(OUTDIR)\BodyInit.lst" 
	$(RM) "$(OUTDIR)\BodyInit.asm" 
	$(RM) "$(OUTDIR)\libgloss.obj" 
	$(RM) "$(OUTDIR)\libgloss.lst" 
	$(RM) "$(OUTDIR)\libgloss.asm" 
	$(RM) "$(OUTDIR)\SD_reprog_finish.obj" 
	$(RM) "$(OUTDIR)\SD_reprog_finish.lst" 
	$(RM) "$(OUTDIR)\SD_reprog_finish.asm" 
	$(RM) "$(OUTDIR)\USB_Flash_reprog.obj" 
	$(RM) "$(OUTDIR)\USB_Flash_reprog.lst" 
	$(RM) "$(OUTDIR)\USB_Flash_reprog.asm" 
	$(RM) $(BINFILE) $(SBMFILE) 

.c.asm:
	$(CC) $(CFLAGS) $(INCLUDES) -o "$(OUTDIR)/$@" $<

.asm.obj:
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)/$@" $<

