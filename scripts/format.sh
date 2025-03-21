#!/bin/bash

SRC_DIR="src"

if [ ! -d "$SRC_DIR" ]; then
    echo "Error: Directory '$SRC_DIR' not found."
    exit 1
fi


files=$(find "$SRC_DIR" -type f \( -name "*.c" -o -name "*.h" \))
clang-format --Werror -i $files

echo "Files in '$SRC_DIR' formatted successfully."
