default: a2

selfcompilation: 
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
	touch out
	./selfie < selfie.c
	mv out selfie.mips1
	touch out
	./selfie -m 32 selfie.mips1 < selfie.c
	diff -s out selfie.mips1
				 
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
