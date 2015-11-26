sh runClean.sh
gcc -o selfie selfie.c
./selfie -c pageTableTest.c -o pageTableTest.mips
./selfie -l pageTableTest.mips -m 2 pageTableTest.mips
