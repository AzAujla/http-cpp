#!/bin/bash
rm -rf build
mkdir build && cd build
BUILD_TYPE=${1:-Debug}
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE
make
