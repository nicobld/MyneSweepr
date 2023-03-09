#!/bin/sh

export CC=g++

export MAKEFLAGS='-j'
export CFLAGS="-I../../libs/include -g -O0 -std=c++14"
export CPPFLAGS="-Wall -Wextra"
export LIBS="-lpthread"