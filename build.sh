#!/usr/bin/env bash

cat README
echo "continue with ENTER/RETURN"
read
./selfie -c init.c -o init
./selfie -c kernel.c -k 64 init
