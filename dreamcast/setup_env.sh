#!/bin/bash
set -e

# This script sets up the Dreamcast development environment.
# It assumes you have the necessary dependencies installed.

PROJECT_ROOT=$(pwd)
DREAMCAST_DIR=$PROJECT_ROOT/dreamcast
KOS_DIR=$DREAMCAST_DIR/KOS
KOS_CHAIN_DIR=$KOS_DIR/utils/kos-chain

echo "Setting up KallistiOS toolchain..."
cd $KOS_CHAIN_DIR
# Using stable profile by default
make build platform=dreamcast toolchain_profile=stable

echo "Setting up KOS environment..."
cd $KOS_DIR
cp doc/environ.sh.sample environ.sh
# Adjust environ.sh if necessary, but usually defaults are okay for /opt/toolchains/dc/kos

echo "Building KOS..."
# We need to source the environ script before building
# This part is tricky in a script, usually users do it in their shell
# For the purpose of this script, we'll try to automate it for the build
export KOS_BASE=$KOS_DIR
export KOS_ARCH="dreamcast"
export KOS_SUBARCH="none"
export KOS_CC_PREFIX="sh-elf"
export KOS_CC_BASE="/opt/toolchains/dc/sh-elf"
export KOS_AS="$KOS_CC_BASE/bin/$KOS_CC_PREFIX-as"
export KOS_CC="$KOS_CC_BASE/bin/$KOS_CC_PREFIX-gcc"
export KOS_CPP="$KOS_CC_BASE/bin/$KOS_CC_PREFIX-g++"
export KOS_LD="$KOS_CC_BASE/bin/$KOS_CC_PREFIX-ld"
export KOS_AR="$KOS_CC_BASE/bin/$KOS_CC_PREFIX-ar"
export KOS_OBJCOPY="$KOS_CC_BASE/bin/$KOS_CC_PREFIX-objcopy"
export KOS_STRIP="$KOS_CC_BASE/bin/$KOS_CC_PREFIX-strip"
export KOS_CFLAGS="-O2 -fomit-frame-pointer -ml -m4-single-only -ffunction-sections -fdata-sections"
export KOS_CPPFLAGS="-fno-operator-names -fno-exceptions -fno-rtti"
export KOS_LDFLAGS="-ml -m4-single-only -Wl,-Ttext=0x8c010000 -Wl,--gc-sections"
# (This is a simplification, real environ.sh has more)

make

echo "Building SDL2 for Dreamcast..."
cd $DREAMCAST_DIR/SDL
mkdir build
cd build
../configure --host=sh-elf --with-kos=$KOS_DIR --prefix=$KOS_DIR/addons
make
make install

echo "Building SDL2_mixer for Dreamcast..."
cd $DREAMCAST_DIR/SDL_mixer
mkdir build
cd build
../configure --host=sh-elf --with-sdl-prefix=$KOS_DIR/addons --disable-music-midi --disable-music-mod --disable-music-mp3 --prefix=$KOS_DIR/addons
make
make install

echo "Dreamcast environment setup complete."
