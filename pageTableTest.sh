sh runClean.sh
gcc -o selfie selfie.c
./selfie -c forkTest.c -o forkTest.mips
./selfie -l forkTest.mips -m 2 forkTest.mips
