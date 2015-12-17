default: test

selfcompilation: 
	clang -v -w -m32 -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
	./selfie -c selfie.c -o selfie1.mips -m 32 -c selfie.c -o selfie2.mips
	diff -s selfie1.mips selfie2.mips
	
test:
	 clang -v -w -m32 -Wl,--unresolved-symbols=ignore-in-object-files -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
	 ./selfie -c selfie.c -o selfie1.mips
	 ./selfie -c dummy.c -m 32
