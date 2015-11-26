#!/bin/sh
cd Game
make ConfigurationName="Release"
cd ..
mkdir -p ../Dev/Runtime
cp built/Release/libGameServer.so ../Dev/Runtime
