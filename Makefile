#####################################################################
#																	 
#	Created by u'nSP IDE		10:42:07	04/23/10
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

INCLUDES	= -I"C:/LiteracyBridge/SVN/Himem" -I"C:/Program Files/Generalplus/unSPIDE 3.0.1/library/include" -I"C:/Program Files/Generalplus/unSPIDE 3.0.1/library/include/sys" -I"./Component/Include/FS" -I"../" -I"../firmware"

BODY	= -body GPL162003_CS0Flash -nobdy -bfile "C:\LiteracyBridge\SVN\Himem\himem.bdy"

BINFILE	= "$(OUTDIR)\himem.S37"

ARYFILE	= "$(OUTDIR)\himem.ary"

SBMFILE	= "$(OUTDIR)\himem.sbm"

OPT	= -S -gstabs -Wall -DUSBRP -mglobal-var-iram -mISA=2.0

ASFLAGS	= -t4 -d -be

CASFLAGS	= -t4 -be -wpop

CFLAGS	= $(OPT) -B$(APPDIR)\toolchain\ $(INCLUDES) 

BINTYPE	= -as

LDFLAGS	= -forspace -initdata

EXTRAFLAGS	= 


OBJFILES	= \
	"$(OUTDIR)\Resource.obj" \
	"$(OUTDIR)\SD.obj" \
	"$(OUTDIR)\nls_codepage.obj" \
	"$(OUTDIR)\nls_ascii.obj" \
	"$(OUTDIR)\DrvStruct.obj" \
	"$(OUTDIR)\FS_user_nos.obj" \
	"$(OUTDIR)\nordriver.obj" \
	"$(OUTDIR)\startup_Data.obj" \
	"$(OUTDIR)\SysOn2OffHook.obj" \
	"$(OUTDIR)\SysIntoHighSpeed.obj" \
	"$(OUTDIR)\SysIntorWaitMode.obj" \
	"$(OUTDIR)\SysOff2OnHook.obj" \
	"$(OUTDIR)\SysIntoHaltMode.obj" \
	"$(OUTDIR)\SysWaitMode.obj" \
	"$(OUTDIR)\SysException.obj" \
	"$(OUTDIR)\ExtMemOper.obj" \
	"$(OUTDIR)\System.obj" \
	"$(OUTDIR)\isr.obj" \
	"$(OUTDIR)\main.obj" \
	"$(OUTDIR)\BodyInit.obj" \
	"$(OUTDIR)\libgloss.obj" \
	"$(OUTDIR)\SD_reprog_finish.obj" \
	"$(OUTDIR)\USB_Flash_reprog.obj" \
	"$(OUTDIR)\USBD_Drv.obj" \
	"$(OUTDIR)\USB_ID.obj" \
	"$(OUTDIR)\Flash_reprog_boot.obj" \
	"$(OUTDIR)\SCSI_Table.obj" \
	"$(OUTDIR)\Flash_reprog_Drv.obj" \
	"$(OUTDIR)\Space.obj" 

"$(OUTDIR)\Resource.obj": "C:\LiteracyBridge\SVN\Himem\Resource.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Resource.obj" "C:\LiteracyBridge\SVN\Himem\Resource.asm" 

"$(OUTDIR)\SD.asm": "C:\LiteracyBridge\SVN\firmware\Component\FS\SD.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD.asm" "C:/LiteracyBridge/SVN/firmware/Component/FS/SD.c" 

"$(OUTDIR)\SD.obj": "$(OUTDIR)\SD.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SD.obj" "$(OUTDIR)\SD.asm" 

"$(OUTDIR)\nls_codepage.asm": "C:\LiteracyBridge\SVN\firmware\Component\FS\nls_codepage.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/nls_codepage.asm" "C:/LiteracyBridge/SVN/firmware/Component/FS/nls_codepage.c" 

"$(OUTDIR)\nls_codepage.obj": "$(OUTDIR)\nls_codepage.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\nls_codepage.obj" "$(OUTDIR)\nls_codepage.asm" 

"$(OUTDIR)\nls_ascii.asm": "C:\LiteracyBridge\SVN\firmware\Component\FS\nls_ascii.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/nls_ascii.asm" "C:/LiteracyBridge/SVN/firmware/Component/FS/nls_ascii.c" 

"$(OUTDIR)\nls_ascii.obj": "$(OUTDIR)\nls_ascii.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\nls_ascii.obj" "$(OUTDIR)\nls_ascii.asm" 

"$(OUTDIR)\DrvStruct.asm": "C:\LiteracyBridge\SVN\Himem\Component\FS\DrvStruct.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/DrvStruct.asm" "C:/LiteracyBridge/SVN/Himem/Component/FS/DrvStruct.c" 

"$(OUTDIR)\DrvStruct.obj": "$(OUTDIR)\DrvStruct.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\DrvStruct.obj" "$(OUTDIR)\DrvStruct.asm" 

"$(OUTDIR)\FS_user_nos.asm": "C:\LiteracyBridge\SVN\Himem\Component\FS\FS_user_nos.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FS_user_nos.asm" "C:/LiteracyBridge/SVN/Himem/Component/FS/FS_user_nos.c" 

"$(OUTDIR)\FS_user_nos.obj": "$(OUTDIR)\FS_user_nos.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\FS_user_nos.obj" "$(OUTDIR)\FS_user_nos.asm" 

"$(OUTDIR)\nordriver.obj": "C:\LiteracyBridge\SVN\Himem\Driver\NOR\nordriver.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\nordriver.obj" "C:\LiteracyBridge\SVN\Himem\Driver\NOR\nordriver.asm" 

"$(OUTDIR)\startup_Data.obj": "C:\LiteracyBridge\SVN\Himem\System\startup_Data.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\startup_Data.obj" "C:\LiteracyBridge\SVN\Himem\System\startup_Data.asm" 

"$(OUTDIR)\SysOn2OffHook.obj": "C:\LiteracyBridge\SVN\Himem\System\WaitMode\SysOn2OffHook.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysOn2OffHook.obj" "C:\LiteracyBridge\SVN\Himem\System\WaitMode\SysOn2OffHook.asm" 

"$(OUTDIR)\SysIntoHighSpeed.obj": "C:\LiteracyBridge\SVN\Himem\System\WaitMode\SysIntoHighSpeed.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntoHighSpeed.obj" "C:\LiteracyBridge\SVN\Himem\System\WaitMode\SysIntoHighSpeed.asm" 

"$(OUTDIR)\SysIntorWaitMode.obj": "C:\LiteracyBridge\SVN\Himem\System\WaitMode\SysIntorWaitMode.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntorWaitMode.obj" "C:\LiteracyBridge\SVN\Himem\System\WaitMode\SysIntorWaitMode.asm" 

"$(OUTDIR)\SysOff2OnHook.obj": "C:\LiteracyBridge\SVN\Himem\System\WaitMode\SysOff2OnHook.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysOff2OnHook.obj" "C:\LiteracyBridge\SVN\Himem\System\WaitMode\SysOff2OnHook.asm" 

"$(OUTDIR)\SysIntoHaltMode.obj": "C:\LiteracyBridge\SVN\Himem\System\WaitMode\SysIntoHaltMode.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysIntoHaltMode.obj" "C:\LiteracyBridge\SVN\Himem\System\WaitMode\SysIntoHaltMode.asm" 

"$(OUTDIR)\SysWaitMode.asm": "C:\LiteracyBridge\SVN\firmware\System\WaitMode\SysWaitMode.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SysWaitMode.asm" "C:/LiteracyBridge/SVN/firmware/System/WaitMode/SysWaitMode.c" 

"$(OUTDIR)\SysWaitMode.obj": "$(OUTDIR)\SysWaitMode.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysWaitMode.obj" "$(OUTDIR)\SysWaitMode.asm" 

"$(OUTDIR)\SysException.asm": "C:\LiteracyBridge\SVN\Himem\System\SysException.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SysException.asm" "C:/LiteracyBridge/SVN/Himem/System/SysException.c" 

"$(OUTDIR)\SysException.obj": "$(OUTDIR)\SysException.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SysException.obj" "$(OUTDIR)\SysException.asm" 

"$(OUTDIR)\ExtMemOper.obj": "C:\LiteracyBridge\SVN\Himem\System\ExtMem\ExtMemOper.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\ExtMemOper.obj" "C:\LiteracyBridge\SVN\Himem\System\ExtMem\ExtMemOper.asm" 

"$(OUTDIR)\System.obj": "C:\LiteracyBridge\SVN\Himem\System\System.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\System.obj" "C:\LiteracyBridge\SVN\Himem\System\System.asm" 

"$(OUTDIR)\isr.obj": "C:\LiteracyBridge\SVN\Himem\System\isr.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\isr.obj" "C:\LiteracyBridge\SVN\Himem\System\isr.asm" 

"$(OUTDIR)\main.asm": "C:\LiteracyBridge\SVN\Himem\main.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/main.asm" "C:/LiteracyBridge/SVN/Himem/main.c" 

"$(OUTDIR)\main.obj": "$(OUTDIR)\main.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\main.obj" "$(OUTDIR)\main.asm" 

"$(OUTDIR)\BodyInit.asm": "C:\LiteracyBridge\SVN\Himem\BodyInit.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/BodyInit.asm" "C:/LiteracyBridge/SVN/Himem/BodyInit.c" 

"$(OUTDIR)\BodyInit.obj": "$(OUTDIR)\BodyInit.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\BodyInit.obj" "$(OUTDIR)\BodyInit.asm" 

"$(OUTDIR)\libgloss.asm": "C:\LiteracyBridge\SVN\Himem\libgloss.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/libgloss.asm" "C:/LiteracyBridge/SVN/Himem/libgloss.c" 

"$(OUTDIR)\libgloss.obj": "$(OUTDIR)\libgloss.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\libgloss.obj" "$(OUTDIR)\libgloss.asm" 

"$(OUTDIR)\SD_reprog_finish.asm": "C:\LiteracyBridge\SVN\firmware\Reprog\SD_reprog_finish.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD_reprog_finish.asm" "C:/LiteracyBridge/SVN/firmware/Reprog/SD_reprog_finish.c" 

"$(OUTDIR)\SD_reprog_finish.obj": "$(OUTDIR)\SD_reprog_finish.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SD_reprog_finish.obj" "$(OUTDIR)\SD_reprog_finish.asm" 

"$(OUTDIR)\USB_Flash_reprog.asm": "C:\LiteracyBridge\SVN\firmware\Reprog\USB_Flash_reprog.c" "C:\LiteracyBridge\SVN\firmware\Reprog\USB_Flash_reprog.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/USB_Flash_reprog.asm" "C:/LiteracyBridge/SVN/firmware/Reprog/USB_Flash_reprog.c" 

"$(OUTDIR)\USB_Flash_reprog.obj": "$(OUTDIR)\USB_Flash_reprog.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USB_Flash_reprog.obj" "$(OUTDIR)\USB_Flash_reprog.asm" 

"$(OUTDIR)\USBD_Drv.obj": "C:\LiteracyBridge\SVN\firmware\Reprog\USBD_Drv.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USBD_Drv.obj" "C:\LiteracyBridge\SVN\firmware\Reprog\USBD_Drv.asm" 

"$(OUTDIR)\USB_ID.obj": "C:\LiteracyBridge\SVN\firmware\Reprog\USB_ID.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\USB_ID.obj" "C:\LiteracyBridge\SVN\firmware\Reprog\USB_ID.asm" 

"$(OUTDIR)\Flash_reprog_boot.asm": "C:\LiteracyBridge\SVN\firmware\Reprog\Flash_reprog_boot.c" "C:\LiteracyBridge\SVN\firmware\Reprog\GPL162002.h" "C:\LiteracyBridge\SVN\firmware\Reprog\USB_Flash_reprog.h" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/Flash_reprog_boot.asm" "C:/LiteracyBridge/SVN/firmware/Reprog/Flash_reprog_boot.c" 

"$(OUTDIR)\Flash_reprog_boot.obj": "$(OUTDIR)\Flash_reprog_boot.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Flash_reprog_boot.obj" "$(OUTDIR)\Flash_reprog_boot.asm" 

"$(OUTDIR)\SCSI_Table.obj": "C:\LiteracyBridge\SVN\firmware\Driver\USB\SCSI_Table.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\SCSI_Table.obj" "C:\LiteracyBridge\SVN\firmware\Driver\USB\SCSI_Table.asm" 

"$(OUTDIR)\Flash_reprog_Drv.obj": "C:\LiteracyBridge\SVN\firmware\Reprog\Flash_reprog_Drv.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Flash_reprog_Drv.obj" "C:\LiteracyBridge\SVN\firmware\Reprog\Flash_reprog_Drv.asm" 

"$(OUTDIR)\Space.asm": "C:\LiteracyBridge\SVN\Himem\Application\Space.c" 
	set PATH="$(APPDIR)\toolchain\";%PATH% & \
	$(CC) $(CFLAGS) -o "$(OUTDIR)/Space.asm" "C:/LiteracyBridge/SVN/Himem/Application/Space.c" 

"$(OUTDIR)\Space.obj": "$(OUTDIR)\Space.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o "$(OUTDIR)\Space.obj" "$(OUTDIR)\Space.asm" 


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
	$(RM) "$(OUTDIR)\nls_codepage.obj" 
	$(RM) "$(OUTDIR)\nls_codepage.lst" 
	$(RM) "$(OUTDIR)\nls_codepage.asm" 
	$(RM) "$(OUTDIR)\nls_ascii.obj" 
	$(RM) "$(OUTDIR)\nls_ascii.lst" 
	$(RM) "$(OUTDIR)\nls_ascii.asm" 
	$(RM) "$(OUTDIR)\DrvStruct.obj" 
	$(RM) "$(OUTDIR)\DrvStruct.lst" 
	$(RM) "$(OUTDIR)\DrvStruct.asm" 
	$(RM) "$(OUTDIR)\FS_user_nos.obj" 
	$(RM) "$(OUTDIR)\FS_user_nos.lst" 
	$(RM) "$(OUTDIR)\FS_user_nos.asm" 
	$(RM) "$(OUTDIR)\nordriver.obj" 
	$(RM) "$(OUTDIR)\nordriver.lst" 
	$(RM) "$(OUTDIR)\startup_Data.obj" 
	$(RM) "$(OUTDIR)\startup_Data.lst" 
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
	$(RM) "$(OUTDIR)\SysWaitMode.obj" 
	$(RM) "$(OUTDIR)\SysWaitMode.lst" 
	$(RM) "$(OUTDIR)\SysWaitMode.asm" 
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
	$(RM) "$(OUTDIR)\USBD_Drv.obj" 
	$(RM) "$(OUTDIR)\USBD_Drv.lst" 
	$(RM) "$(OUTDIR)\USB_ID.obj" 
	$(RM) "$(OUTDIR)\USB_ID.lst" 
	$(RM) "$(OUTDIR)\Flash_reprog_boot.obj" 
	$(RM) "$(OUTDIR)\Flash_reprog_boot.lst" 
	$(RM) "$(OUTDIR)\Flash_reprog_boot.asm" 
	$(RM) "$(OUTDIR)\SCSI_Table.obj" 
	$(RM) "$(OUTDIR)\SCSI_Table.lst" 
	$(RM) "$(OUTDIR)\Flash_reprog_Drv.obj" 
	$(RM) "$(OUTDIR)\Flash_reprog_Drv.lst" 
	$(RM) "$(OUTDIR)\Space.obj" 
	$(RM) "$(OUTDIR)\Space.lst" 
	$(RM) "$(OUTDIR)\Space.asm" 
	$(RM) $(BINFILE) $(SBMFILE) 

.c.asm:
	$(CC) $(CFLAGS) $(INCLUDES) -o "$(OUTDIR)/$@" $<

.asm.obj:
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)/$@" $<

