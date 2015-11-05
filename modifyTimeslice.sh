#!/bin/bash
sed -i -E "s/int instr_cycles = .*;/int instr_cycles = $1;/g" selfie.c
make
sed -i -E "s/int instr_cycles = .*;/int instr_cycles = $2;/g" selfie.c
make
sed -i -E "s/int instr_cycles = .*;/int instr_cycles = $3;/g" selfie.c
make
