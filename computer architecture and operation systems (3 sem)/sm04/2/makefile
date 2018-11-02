GCC=arm-linux-gnueabi-gcc

all:
	${GCC} -static -g main.c asm.s

run: all
	qemu-arm a.out
