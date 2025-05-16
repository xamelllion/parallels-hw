#!/bin/bash

DIRS="src tests"
INCLUDE_DIR="include"

files=$(find $DIRS -type f \( -name "*.c" -o -name "*.h" \))
headers=$(find "$INCLUDE_DIR" -type f \( -name "*.h" \))

clang-format --Werror --dry-run -i $files $headers
