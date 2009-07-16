v1.1

- fixed JUMP_TIME int/long problem
- fixed bugs with readBuffer
- added support for paused events
- added rewind config option for sound inserts
- stopped insert sound from unpausing
- fixed backward jumps in playLongInt (abs() was bad/int)

v1.2
- broke up one source file into many and added prototypes
- voltage logging
- macro facility

v1.22
- latest USB host and device library code included (not applied yet)
- new logException with string logging
- attempting to allow instant reset
- file system date/time reflect cycle number and powerup time

v1.23
- fixed memory corruption problem (ensured audio stopped before r/w memory card)
- announces subject before and after new recording to help user remember where it is
- USB mode from startup if pressing '*' key

v1.24
- fixed problem with deleting wrong recording when trying to deleting brand new recording
- recording now inserts itself in list position
- removed control files from simple content - just uses one template file
- grouped complex content into folders
- logs deletes and records

v1.30
- reprogramming with reprog.exe and by memory card in /System/Firmware/Update
- keystroke capture option (LOG_KEYS:1 in config.txt) with macro format for playback
- logging with time of each package played for user experience analysis in pilots

v1.31
- bad release: bad USB-Device Mode (main.c was old and other small problems)

v1.32
- VERSION retrievable by reprog.exe
- need to track down these problems:
    - once found 128 '\0's in OTHER.txt
    - often found device hanging on long recordings (tested 1200 & 1800 sec)
      with a 0byte file; LEDs were off -- might be hung on close()
- changed RED LED to go out after closing recorded file

v1.33
- fixed problem with recordings freezing up the device when device powered on for 9+ hours
- added code to change clock rate in config.txt (12MHz to 96MHz possible)
- set default clock rate, add isr processing for USB host interrupts
- try to become a usb client after 5(?) seconds of inactivity
- added code to support USB host, Inbox folder on SD card
