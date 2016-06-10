"tools\srec2bin" -q debug\System.S37 debug\System.bin
dd if=himem.bin of=himem.blk bs=32k count=2 skip=40