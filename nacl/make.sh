#!/bin/sh

set -eu

builddir=`pwd`/../build

nacl-gcc -g -Wall hellow.c \
    -static -nostdlib -Wl,-T,elf_i386.x \
    '-Wl,-(' \
    -lgcc \
    $builddir/csu/crt1.o \
    $builddir/csu/crti.o \
    $builddir/csu/crtn.o \
    $builddir/libc.a \
    '-Wl,-)' \
    -o hellow 
