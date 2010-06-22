v1.41 (MOFA/VSO customer firmware with bug fixes)
 - now incorporates PCB test procedure and starts when config or serial num is missing
 - removed voltage handling feature for now until more stable
 - removed reset button option (concerned causing bug, but may have been fine)
 - new d2d copy is more robust and plays bips during copy
 - fixed bug in audio recording after USB connection is made
 - seemed to fix the shutdown bug from bad voltage readings
 - no longer repeats category name and instructions after "empty" message
 - minimum volume is now 1 - which means cannot mute, but might reduce confusion when silent
 - fixed date/time stamp problem in v1.40-release (reset RTC and month/day)
 - fixed problem with behavior when config.txt does not exist


V1.40 (MAJOR UPDATE)
- system reset with SELECT + PLAY (via int fiq)
- firmware reprogramming compat with new hw flash
- firmware update files now *.img (put version in filename)
- Sleep and Halt code for new hardware (SCH 3.4)
- New DEFINE in talkingbook.h: TB_CAN_WAKE for old/new hw builds
- Power down amp when not playing 
- New "Trim" feature allows user to cut the beginning and ending of a recording. 
- new logRTC() fct
- Added ability to comment config items with '//' at beginning of line only
- Moved 'bip' and 'ding' to NOR flash instead of microSD

v1.37
- record starts faster
- empty categories bounce back to list name

v1.35
- early/unfinished work with package types and "application types"
- config paths now have one backslash not two
- config CLOCK_RATE now specified in MHz (12-96)
- fixed bug in v1.34 that didn't use specified addresses in system.lik
- improved copy UI with better audio instructions in English and Dagaare
- ProcessInbox() can now copy in any new file, not just user audio and firmware
- new setOperationalMode() for WAIT/HALT/SLEEP; SLEEP triggered by 'Z' action
- voltage logging now works well when audio is stopped
- when logging duration of played audio msg, now shows last volume level
- blinks green light when inactive for 30 seconds

v1.34
- fixed button response problem caused by auto check to become USB client
- changed INBOX to allow check and copy of system firmware updates -- see config.txt
  So now new user content and firmware updates can be dropped into \inbox
  TODO: allow replacement of \system audio files and control txt files
- only log voltage when stopped or paused (not sure yet if this helps erratic measurements)
- small code refactoring and cleanup; also moved some #defines to confix.txt

v1.33
- fixed problem with recordings freezing up the device when device powered on for 9+ hours
- added code to change clock rate in config.txt (12MHz to 96MHz possible)
- set default clock rate, add isr processing for USB host interrupts
- try to become a usb client after 5(?) seconds of inactivity
- added code to support USB host, Inbox folder on SD card
- initial d2d copy from host to Inbox of client and Inbox processing (Inbox.c)
- added copyOutbox() for usb copy between devices
- added pause functionality to record feature
- plays most recently copied package and its category name

v1.32
- VERSION retrievable by reprog.exe
- need to track down these problems:
    - once found 128 '\0's in OTHER.txt
    - often found device hanging on long recordings (tested 1200 & 1800 sec)
      with a 0byte file; LEDs were off -- might be hung on close()
- changed RED LED to go out after closing recorded file

v1.31
- bad release: bad USB-Device Mode (main.c was old and other small problems)

v1.30
- reprogramming with reprog.exe and by memory card in /System/Firmware/Update
- keystroke capture option (LOG_KEYS:1 in config.txt) with macro format for playback
- logging with time of each package played for user experience analysis in pilots

v1.24
- fixed problem with deleting wrong recording when trying to deleting brand new recording
- recording now inserts itself in list position
- removed control files from simple content - just uses one template file
- grouped complex content into folders
- logs deletes and records

v1.23
- fixed memory corruption problem (ensured audio stopped before r/w memory card)
- announces subject before and after new recording to help user remember where it is
- USB mode from startup if pressing '*' key

v1.22
- latest USB host and device library code included (not applied yet)
- new logException with string logging
- attempting to allow instant reset
- file system date/time reflect cycle number and powerup time

v1.2
- broke up one source file into many and added prototypes
- voltage logging
- macro facility

v1.1
- fixed JUMP_TIME int/long problem
- fixed bugs with readBuffer
- added support for paused events
- added rewind config option for sound inserts
- stopped insert sound from unpausing
- fixed backward jumps in playLongInt (abs() was bad/int)
