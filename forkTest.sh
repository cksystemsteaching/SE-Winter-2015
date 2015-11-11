sh runClean.sh
gcc -o selfie selfie.c
touch forkTest.mips
./selfie -c forkTest.c -o forkTest.mips
./selfie -l forkTest.mips -m 32 forkTest.mips
