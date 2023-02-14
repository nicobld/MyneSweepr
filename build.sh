#!/bin/sh
set -e
. ./config.sh

make -C src/client ${MAKEFLAGS}
make -C src/server ${MAKEFLAGS}

mkdir -p bin
cp src/client/client_MyneSweepr bin/
cp src/server/server_MyneSweepr bin/

echo "build finished"