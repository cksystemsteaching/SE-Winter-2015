sh runClean.sh
gcc -o selfie selfie.c
touch out
./selfie -c < yieldTest1.c
mv out yieldTest.mips
./selfie -m 32 yieldTest.mips

