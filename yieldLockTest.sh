gcc -o selfie selfie.c
touch yieldLockTest.mips
./selfie -c yieldLockTest.c -o yieldLockTest.mips
./selfie -l yieldLockTest.mips -m 32 yieldLockTest.mips
