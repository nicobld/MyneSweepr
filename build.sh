#!/bin/bash
set -e
. ./config.sh

make -C client ${MAKEFLAGS} &
make -C server ${MAKEFLAGS} &

wait

mkdir -p bin
cp client/client_MyneSweepr bin/
cp server/server_MyneSweepr bin/

echo "build finished"