#!/bin/bash
set -e
. ./config.sh

make -C server clean ${MAKEFLAGS} &
make -C client clean ${MAKEFLAGS} &

wait

rm -rf bin/