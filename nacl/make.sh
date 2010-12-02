#!/bin/sh

set -eu

srcdir=nacl
builddir=build

nacl-gcc -g -Wall $srcdir/hellow.c \
    -static -nostdlib -Wl,-T,$srcdir/dyn-link/ldscripts/elf_nacl.x.static \
    '-Wl,-(' \
    -lgcc \
    $builddir/csu/crt1.o \
    $builddir/csu/crti.o \
    $builddir/csu/crtn.o \
    $builddir/libc.a \
    '-Wl,-)' \
    -o $builddir/hellow-static

nacl-gcc -g -Wall $srcdir/hellow.c \
    -nostdlib -L$srcdir/dyn-link \
    -lgcc \
    $builddir/csu/crt1.o \
    $builddir/csu/crti.o \
    $builddir/csu/crtn.o \
    $builddir/libc.so $builddir/libc_nonshared.a $builddir/elf/ld.so \
    -o $builddir/hellow-dynamic

nacl-gcc -g -Wall $srcdir/hellow.c \
    -nostdlib -L$srcdir/dyn-link \
    -lgcc \
    $builddir/csu/crt1.o \
    $builddir/csu/crti.o \
    $builddir/csu/crtn.o \
    $builddir/nptl/libpthread.so \
    $builddir/libc.so $builddir/libc_nonshared.a $builddir/elf/ld.so \
    -o $builddir/hellow-dynamic-pthread
