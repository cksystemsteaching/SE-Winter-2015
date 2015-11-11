default: a3

selfcompilation: 
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
	./selfie -c selfie.c -o selfie1.mips -m 32 -c selfie.c -o selfie2.mips
	diff -s selfie1.mips selfie2.mips
				 
a1: 
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie
	./selfie -c ourBinary.c -o out
	./selfie -l out -m 32

a2:
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie
	./selfie -c yieldExample.c -o out
	./selfie -l out -a 64
	
a3:
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie
	./selfie -c mutualExclusion.c -o out
	./selfie -l out -a 64 3 1
	./selfie -l out -a 64 3 100
	./selfie -l out -a 64 3 1000
	
bonus:
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie
	./selfie -c multiThreadTest.c -o out
	./selfie -l out -t 64
