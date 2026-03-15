#!/bin/bash

# Flag -j is used to enable parallel building, which can significantly speed up the build process on multi-core systems.
cmake --build build -j
cd build
./main