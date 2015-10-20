clang -w -g -m32 -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
touch out
./selfie < selfie.c
mv out selfie.mips1
touch out
./selfie < test.c
mv out myprog
./selfie -m 32 myprog
