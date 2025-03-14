qemu-system-x86_64 \
    -m 1024M \
    -kernel /home/ctf/bzImage \
    -initrd /home/ctf/initramfs.cpio.gz \
    -nographic \
    -monitor none \
    -no-reboot \
    -cpu kvm64,+smep,+smap \
    -smp 4 \
    -append "console=ttyS0 kaslr kpti=1 quiet panic=1"
