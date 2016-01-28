gcc -w -m32 -Wl,--unresolved-symbols=ignore-in-object-files -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie

./selfie -c kernel.c -o kernel.mips
./selfie -c test.c -o test.mips

./selfie -l kernel.mips -k 32
#clean	
	rm -rf selfie *.mips  *~ 
