#!/bin/bash

echo "Creating release files..."
mkdir Release
cp gcc_run_example.sh ./Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
cd ..

echo "Creating debug files..."
mkdir Debug
cp gcc_run_example.sh ./Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cd ..


