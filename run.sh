#!/bin/bash

cmake --build build -j
cd build
./main $1