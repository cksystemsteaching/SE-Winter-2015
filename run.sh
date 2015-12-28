#compile	
	clang -w -m32 -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie
	touch out
	./selfie -c < testFile.c
	mv out testFile.mips
	touch out
	./selfie -c < selfie.c
	mv out selfie.mips

#run 
		./selfie -c testFile.c -m 32 
		#./selfie -o testFile.mips
		#./selfie -m 32 testFile.mips 
#clean	
	rm -rf selfie selfie.mips testFile.mips *~ 

