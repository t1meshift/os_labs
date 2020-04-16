#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

cmake -Bbuild -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" .
cmake --build build --target "RUN_$1"