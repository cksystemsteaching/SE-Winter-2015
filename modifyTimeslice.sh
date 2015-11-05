#!/bin/bash

CC=clang
CC_OPT="-w -m32 -D'main(a, b)=main(int argc, char **argv)'"
EXEC=selfie
MEM_SIZE=32
TEST_INPUT=test.c
TEST_BINARY=myprog

emulate() {
	clang -w -m32 -D'main(a, b)=main(int argc, char **argv)' selfie.c -o selfie
	touch myprog
	./selfie -c test.c -o myprog
	./selfie -l myprog -m 32
}

sed -i -E "s/int instr_cycles = .*;/int instr_cycles = $1;/g" selfie.c
emulate
echo "-----------------------------------"
sed -i -E "s/int instr_cycles = .*;/int instr_cycles = $2;/g" selfie.c
emulate
echo "-----------------------------------"
sed -i -E "s/int instr_cycles = .*;/int instr_cycles = $3;/g" selfie.c
emulate
