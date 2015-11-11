default: a4

selfcompilation: 
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
	./selfie -c selfie.c -o selfie1.mips -m 32 -c selfie.c -o selfie2.mips
	diff -s selfie1.mips selfie2.mips

a4:
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie
	./selfie -c testForkAndWait.c -o out
	./selfie -l out -a 64 1
	./selfie -l out -a 64 100
	./selfie -l out -a 64 10000
