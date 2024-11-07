#!/bin/bash

BUILD_DIR="build/em"
PTHREAD_POOL_SIZE=8

clean() {
    rm -rf $BUILD_DIR
}

build() {
    mkdir -p $BUILD_DIR
    emcc src/calculate_primes.cpp -O1 -std=c++17 -s USE_PTHREADS=1 \
      -s PTHREAD_POOL_SIZE=$PTHREAD_POOL_SIZE \
      -o $BUILD_DIR/calculate_primes.html
}

build_p() {
    mkdir -p $BUILD_DIR
    # emcc src/calculate_primes_parallel.cpp -std=c++17 \
    #   -s USE_PTHREADS=1 \
    #   -s PTHREAD_POOL_SIZE=4 \
    #   -o $BUILD_DIR/calculate_primes_parallel.html
    emcc src/calculate_primes_parallel.cpp -std=c++17 \
      -pthread \
      -s ASSERTIONS=1 \
      -s PTHREAD_POOL_SIZE=$PTHREAD_POOL_SIZE \
      --emrun \
      -o $BUILD_DIR/calculate_primes_parallel.html
}

run() {
    emrun --browser chrome $BUILD_DIR/calculate_primes.html
}

run_p() {
    emrun --browser chrome $BUILD_DIR/calculate_primes_parallel.html
}

all () {
    clean
    build
    build_p
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

