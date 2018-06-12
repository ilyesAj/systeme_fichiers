# systeme_fichiers
projet systeme
execution et test :
creation du disque virtuel :dd if=/dev/zero of=vdisk bs=1M count=100
compilation :gcc -o blk blocks.c
execution ./blk

