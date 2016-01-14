
clang -w -m32 -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie

./selfie -c kernel.c -o kernel.mips
./selfie -l kernel.mips -k 32
#clean	
	rm -rf selfie *.mips  *~ 

