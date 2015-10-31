default: a2

selfcompilation: 
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
	touch selfie1.mips
	touch selfie2.mips
	./selfie -c selfie.c -o selfie1.mips -m 32 -c selfie.c -o selfie2.mips
	diff -s selfie1.mips selfie2.mips
				 
a1: 
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
	touch out
	./selfie < ourBinary.c
	./selfie -a 32 out
	
a2:
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
	touch out
	./selfie < yieldExample.c
	./selfie -a 32 out
