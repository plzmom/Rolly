#!/bin/bash

cmake -S . -B build \
    -DBUILD_APP=ON \
    -DAPP_ENTRY_FILE=src/main.c

cmake --build build --config Release
