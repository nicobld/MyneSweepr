#!/bin/sh
set -e
. ./config.sh

make -C src/server clean ${MAKEFLAGS}
make -C src/client clean ${MAKEFLAGS}

rm -rf bin/