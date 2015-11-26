#!/bin/sh
cd Game
make
cd ..
mkdir -p ../Dev/Runtime
cp built/Debug/libGameServer.so ../Dev/Runtime
