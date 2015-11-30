all:
	clang -w -m32 -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c

test:
	./selfie -c asg5testfile.c -o as5.mips32 && \
	./selfie -l as5.mips32 -m 32

