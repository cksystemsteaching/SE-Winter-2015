#!/bin/bash

# Compilation of selfie

gcc -o selfie selfie.c
touch out
./selfie -c < selfie.c
mv out selfie.mips1
