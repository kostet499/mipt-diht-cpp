GCC=arm-linux-gnueabi-gcc

all:
	${GCC} -static -g main.c asm.S

run: all
	qemu-arm a.out
