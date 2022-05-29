dd if=bin/kernel.bin of=../bochs/hd60M.img bs=512 count=200 seek=9 conv=notrunc conv=sync
dd if=bin/mbr.bin of=../bochs/hd60M.img bs=512 count=1 conv=notrunc conv=sync
dd if=bin/loader.bin of=../bochs/hd60M.img bs=512 count=3 seek=2 conv=notrunc conv=sync
dd if=bin/kernel.bin of=../bochs/hd60M.img bs=512 count=200 seek=9 conv=notrunc conv=sync
dd if=src/lib/command/cat of=../bochs/hd60M.img bs=512 count=20 seek=300 conv=notrunc conv=sync
