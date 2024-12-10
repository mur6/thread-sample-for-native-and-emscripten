#!/bin/bash

HERE=$(dirname $(realpath $0))

build() {
    emcc $HERE/main_2.cpp \
    -std=c++20 \
    -pthread \
    -s PTHREAD_POOL_SIZE=2 \
    -s USE_PTHREADS=1 \
    -s PROXY_TO_PTHREAD=1 \
    -g \
    --bind \
    -s EXIT_RUNTIME=0 \
    -s MODULARIZE=1 \
    -s EXPORT_ES6=1 \
    -s DISABLE_EXCEPTION_CATCHING=0 \
    -o $HERE/prime_finder.js
}

build
