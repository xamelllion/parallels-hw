# PCT - Parallel Convolution Tool

![License](https://img.shields.io/badge/license-MIT-blue)

## Overview

PCT - image processing tool that applies convolution filters to images using various parallelization techniques. It supports multiple processing modes and filter types for flexible image manipulation.

## Features

- Multiple parallelization modes:
  - Sequential (single-threaded)
  - Pixel-level parallelism
  - Row-level parallelism
  - Column-level parallelism
  - Grid-based parallelism
- Supported filters:
  - Identity (no change)
  - Blur (smoothing)
  - Motion blur
  - Edge detection
  - Sharpening

## Installation

### Prerequisites

- C compiler
- CMake (version 3.10 or higher)
- Ninja or Make build system

### Building

```bash
# Configure with Clang in Release mode with Ninja
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Release -GNinja -Bbuild

# Build the project
cmake --build build
```

For debugging purposes, use `-DCMAKE_BUILD_TYPE=Debug`:

```bash
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -GNinja -Bbuild
```

## Usage

```bash
./build/src/pct [OPTIONS]
```

### Options

| Short | Long            | Description                          | Values                      |
|-------|-----------------|--------------------------------------|-----------------------------|
| `-h`  | `--help`        | Show help message                    |                             |
| `-i`  | `--input`       | Input image path                     | Any valid image file        |
| `-o`  | `--output`      | Output image path                    | Desired output path         |
| `-f`  | `--filter`      | Filter to apply                      | `id`, `bl`, `mb`, `ed`, `sr`|
| `-m`  | `--mode`        | Processing mode                      | `seq`, `pixel`, `row`, `column`, `grid`|
| `-t`  | `--threads`     | Number of threads to use             | Positive integer            |

### Filter Types

| Code | Filter Name      | Description                          |
|------|------------------|--------------------------------------|
| `id` | Identity         | No change to image                   |
| `bl` | Blur             | Image smoothing                      |
| `mb` | Motion Blur      | Directional blur effect              |
| `ed` | Edge Detection   | Highlights image edges               |
| `sr` | Sharpen          | Enhances image details               |

### Processing Modes

| Mode    | Description                              |
|---------|------------------------------------------|
| `seq`   | Sequential single-threaded processing    |
| `pixel` | Parallel processing per-pixel            |
| `row`   | Parallel processing per-row              |
| `column`| Parallel processing per-column           |
| `grid`  | Parallel processing using 2D grid        |

## Examples

1. Apply blur filter with row-level parallelization using 4 threads:
   ```bash
   ./build/src/pct -i input.jpg -o output.jpg -f bl -m row -t 4
   ```

2. Detect edges using grid mode with automatic thread count:
   ```bash
   ./build/src/pct -i photo.png -o edges.png -f ed -m grid
   ```

## Testing

Run the test suite with:
```bash
./scripts/run-tests.sh
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
