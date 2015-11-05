sh runClean.sh
gcc -o selfie selfie.c
touch out
./selfie -c < lockTest.c
mv out lockTest.mips
./selfie -m 32 lockTest.mips

