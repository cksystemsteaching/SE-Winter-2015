sh runClean.sh
gcc -o selfie selfie.c
touch out
./selfie -c < yieldTest.c
mv out yieldTest.mips
./selfie -m 32 yieldTest.mips
