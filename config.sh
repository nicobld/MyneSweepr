#!/bin/bash

export CC=g++

export MAKEFLAGS='-j'
export CPPFLAGS="-I../../libs/include"
export CFLAGS="-O3 -std=c++14 -Wall -Wextra"
export LDLIBS="-lpthread"