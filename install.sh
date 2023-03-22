#!/bin/bash

# Run cmake to generate build files
cmake -S . -B build

# Change to the build directory
cd build

# Run make to compile the project
make

# Run the tests
make test
