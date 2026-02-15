#!/bin/bash

BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cmake -B "$BUILD_DIR"
if [ $? -ne 0 ]; then
    echo "Configuration failed"
    exit 1
fi

echo "Building Debug..."
cmake --build "$BUILD_DIR" --config Debug
if [ $? -ne 0 ]; then
    echo "Build failed"
    exit 1
fi

echo "Building Release..."
cmake --build "$BUILD_DIR" --config Release
if [ $? -ne 0 ]; then
    echo "Build failed"
    exit 1
fi

echo "Done!"
