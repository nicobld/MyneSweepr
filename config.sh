#!/bin/bash

export CC="zig c++ -iquote /usr/include/SDL2/" 
export CC="g++"

export PREFIX="$(pwd)"

export MAKEFLAGS='-j'
export CPPFLAGS="-I${PREFIX}/libs/include"
export CFLAGS="-O3 -std=c++14 -Wall -Wextra"
export LDLIBS="-lpthread"
