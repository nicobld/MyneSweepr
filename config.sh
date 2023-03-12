#!/bin/bash

export CC=g++

export MAKEFLAGS='-j'
export CPPFLAGS="-Wall -Wextra"
export CFLAGS="-I../../libs/include -O3 -std=c++14"
export LIBS="-lpthread"