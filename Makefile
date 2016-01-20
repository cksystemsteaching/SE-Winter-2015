default: test

test:
	 clang -v -w -m32 -Wl,--unresolved-symbols=ignore-in-object-files -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
	 ./selfie -c selfie.c -o selfie1.mips
	 ./selfie -c testThreadfork.c -m 32
	 
selfcompilation:
	 clang -v -w -m32 -Wl,--unresolved-symbols=ignore-in-object-files -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
	 ./selfie -c selfie.c -o selfie1.mips
	 ./selfie -l selfie1.mips -m 32
	 diff -s selfie1.mips selfie2.mips
	 
debug:
	clang -v -w -g -m32 -Wl,--unresolved-symbols=ignore-in-object-files -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
	 ./selfie -c selfie.c -o selfie1.mips
	 
