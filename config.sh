#!/bin/bash

#export CC="zig c++ -iquote /usr/include/SDL2/" 
export CC="emcc -L${HOME}/SDL/build -sUSE_SDL=2 --preload-file img/textures.bmp -sPTHREAD_POOL_SIZE=1"
#export CC=gcc

export PREFIX="$(pwd)"

export MAKEFLAGS='-j'
export CPPFLAGS="-I${PREFIX}/libs/include"
export CFLAGS="-O0 -std=c++14 -Wall -Wextra"
# export LDLIBS="-lpthread"
