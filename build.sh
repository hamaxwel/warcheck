#!/bin/bash
echo "Building InvasionEngine..."

mkdir -p build
cd build

cmake ..
if [ $? -ne 0 ]; then
    echo "CMake failed!"
    exit 1
fi

make
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build successful!"
echo "Running game..."
cd ..
./build/InvasionEngine 