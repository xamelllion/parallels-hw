#!/bin/bash

SRC_DIR="src"
INCLUDE_DIR="include"

if [ ! -d "$SRC_DIR" ]; then
    echo "Error: Directory '$SRC_DIR' not found."
    exit 1
fi


files=$(find "$SRC_DIR" -type f \( -name "*.c" -o -name "*.h" \))
headers=$(find "$INCLUDE_DIR" -type f \( -name "*.h" \))

clang-format --Werror --verbose -i $files $headers

echo "Files formatted successfully."
