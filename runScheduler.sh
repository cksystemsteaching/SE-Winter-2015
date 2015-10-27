gcc -o selfie selfie.c
touch out
./selfie -c < cStarTest.c
mv out cStarTest.mips
./selfie -m 32 cStarTest.mips
