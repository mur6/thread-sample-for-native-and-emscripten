#!/bin/bash

BUILD_DIR="build/em"
PTHREAD_POOL_SIZE=2

clean() {
    rm -rf $BUILD_DIR
}

build() {
    mkdir -p $BUILD_DIR
    emcc src/test2/main.cpp -pthread \
        --bind \
        --pre-js src/test2/pre.js \
        -s EXIT_RUNTIME=0 \
        -s ASYNCIFY=1 \
        -s 'ASYNCIFY_IMPORTS=["asyncCalculation"]' \
        -s DISABLE_EXCEPTION_CATCHING=0 \
        -s PTHREAD_POOL_SIZE=$PTHREAD_POOL_SIZE \
        -o $BUILD_DIR/async_test.html
}
# このエラーは、__cxa_init_primary_exceptionというシンボルが見つからないために発生しています。これは、C++の例外処理に関連するシンボルです。EmscriptenでC++の例外処理を有効にする必要があります。


run() {
    emrun --browser chrome $BUILD_DIR/async_test.html
}

run_server() {
    cp $BUILD_DIR/*.js $BUILD_DIR/*.wasm src/test2/html/
    python3 server.py
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
elif [ "$1" == "run" ]; then
    run
elif [ "$1" == "run_server" ]; then
    run_server
elif [ "$1" == "all" ]; then
    all
else
    # help
    echo "Usage: $0 [clean|build|run|run_server|all]"
fi

