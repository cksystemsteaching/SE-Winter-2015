##-----------------##
##Christian Kawalar##
##12.2015          ##
##-----------------##

all: clean clang kernel p1 run

.PHONY: clean
clean:
	rm -rf *.mips32 selfie

clang:
	clang -w -m32 -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie

kernel:
	./selfie -c kernel.c -o kernel.mips32

p1:
	./selfie -c p1.c -o p1.mips32

run:
	./selfie -l kernel.mips32 -m 12

debug:
	clang -w -m32 -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie -g && gdb selfie

selftest: 
	clang -w -m32 -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie -g && ./selfie -c selfie.c -o selfie.mips32
