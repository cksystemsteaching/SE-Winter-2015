#!/bin/bash

# To test self-compilation

clang -w -m32 -D'main(a, b)=main(int argc, char **argv)' -o selfie selfie.c
touch out
./selfie -c < selfie.c
mv out selfie.mips1
touch out
./selfie -m 32 selfie.mips1 < selfie.c
mv out selfie.mips2

echo '                                                 '
echo '*** ---------- *** ---------- *** ----------- ***'
diff -s selfie.mips1 selfie.mips2
echo '*** ---------- *** ---------- *** ----------- ***'
echo '                                                 '
