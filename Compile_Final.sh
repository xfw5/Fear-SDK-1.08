#!/bin/sh
cd Game
make ConfigurationName="Final"
cd ..
mkdir -p ../Dev/Runtime
cp built/Final/libGameServer.so ../Dev/Runtime
