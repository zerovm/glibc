#!/bin/bash

set -eu

if ! which nacl-gcc >/dev/null; then
  echo Error: nacl-gcc not found on PATH
  exit 1
fi

# Since we are taking Linux glibc as a starting point we need the
# Linux kernel headers.
# TODO(mseaborn): Remove this dependency.
rm -rf kernel-headers
mkdir kernel-headers
cp -a /usr/include/asm /usr/include/asm-generic /usr/include/linux \
    kernel-headers/

source_dir=`pwd`

mkdir -p build
cd build
$source_dir/configure \
    --prefix=/usr \
    --host=i486-linux-gnu \
    CC="`which nacl-gcc` -L$source_dir/nacl/dyn-link " \
    CFLAGS="-march=i486 -pipe -fstrict-aliasing -O2 -mno-tls-direct-seg-refs -g" \
    --with-headers=$source_dir/kernel-headers \
    --enable-kernel=2.2.0
