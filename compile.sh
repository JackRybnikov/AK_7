#1/bin/bash
set -e
make
cp hello.ko ~/repos/busybox/_install/
cd ~/repos/busybox/_install/
find . | cpio -o -H newc | gzip > ../rootfs.cpio.gz
cd ..
qemu-system-arm -kernel _install/boot/zImage -initrd rootfs.cpio.gz -machine virt -nographic -m 512 --append "root=/dev/ram0 rw console=ttyAMA0,115200 mem=512M low-level=7"
