#!/bin/bash

DIRS="src tests"
INCLUDE_DIR="include"

files=$(find $DIRS -type f \( -name "*.c" -o -name "*.h" \))
headers=$(find "$INCLUDE_DIR" -type f \( -name "*.h" \))

clang-format --Werror --verbose -i $files $headers

echo "Files formatted successfully."
