#!/bin/bash

BUILD_DIR="build/native"


clean() {
    rm -rf $BUILD_DIR
}

build() {
    mkdir -p $BUILD_DIR
    clang++ -O1 -std=c++17 -Wall -Wextra -pedantic -pthread \
        src/calculate_primes.cpp \
        -o $BUILD_DIR/calculate_primes
}

build_p() {
    mkdir -p $BUILD_DIR
    clang++ -O1 -std=c++17 -Wall -Wextra -pedantic -pthread \
        src/calculate_primes_parallel.cpp \
        -o $BUILD_DIR/calculate_primes_parallel
}

run() {
    ./$BUILD_DIR/calculate_primes
}

run_p() {
    ./$BUILD_DIR/calculate_primes_parallel
}

all () {
    clean
    build
    build_p
    run
    run_p
}

# switch by argument
if [ "$1" == "clean" ]; then
    clean
elif [ "$1" == "build" ]; then
    build
elif [ "$1" == "build_p" ]; then
    build_p
elif [ "$1" == "run" ]; then
    run
elif [ "$1" == "run_p" ]; then
    run_p
elif [ "$1" == "all" ]; then
    all
else
    # help
    echo "Usage: $0 [clean|build|run|build_p|run_p|all]"
fi

