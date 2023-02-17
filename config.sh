#!/bin/sh

export CC=g++

export MAKEFLAGS='-j'
export CFLAGS="-I../../libs/include -O2 -s"
export CPPFLAGS="-Wall -Wextra"
export LIBS="-lpthread"