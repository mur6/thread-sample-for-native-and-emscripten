#!/bin/bash

BUILD_DIR="build/em"
PTHREAD_POOL_SIZE=8

clean() {
    rm -rf $BUILD_DIR
}

build() {
    mkdir -p $BUILD_DIR
    emcc src/test2/main.cpp -pthread \
        -s EXIT_RUNTIME=0 \
        -s PROXY_TO_PTHREAD=1 \
        -s PTHREAD_POOL_SIZE=$PTHREAD_POOL_SIZE \
        -o $BUILD_DIR/async_test.html
}

run() {
    emrun --browser chrome $BUILD_DIR/async_test.html
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

