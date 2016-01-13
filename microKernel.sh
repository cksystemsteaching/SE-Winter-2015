sh runClean.sh
gcc -o selfie selfie.c

./selfie -c kernel.c -o kernel.mips
./selfie -l kernel.mips -k 32
