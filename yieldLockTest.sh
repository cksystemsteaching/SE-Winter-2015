gcc -o selfie selfie.c
touch yieldTest1.mips
./selfie -c yieldTest1.c -o yieldTest1.mips
./selfie -l yieldTest1.mips -m 32 yieldTest1.mips
