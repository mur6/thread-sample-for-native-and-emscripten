#!/bin/bash

HERE=$(dirname $(realpath $0))
BUILD_DIR=$HERE/web/dist

_clean() {
    target_dir=$1
    echo "Removing $target_dir"
    rm -rf "$target_dir"
}

clean_em() {
    _clean $BUILD_DIR
}

build_em() {
    PROG_NAME=$1
    echo "DIR=$HERE"
    echo "PROG_NAME=$PROG_NAME"
    echo "Building..."
    mkdir -p "$BUILD_DIR"
    echo "BUILD_DIR=$BUILD_DIR"
    emcc "$HERE/src/$PROG_NAME.cpp" \
        -std=c++23 \
        -s MODULARIZE=1 \
        -s EXPORT_ES6=1 \
        -g \
        -pthread \
        -sPTHREAD_POOL_SIZE=1 \
        -fwasm-exceptions \
        --preload-file assets@/assets \
        --use-preload-plugins \
        --bind \
        -o "$BUILD_DIR/$PROG_NAME".html
}

run_em() {
    echo "DIR=$HERE"
    echo "Running..."
    # emrun --browser chrome $HERE/web/main.js
    python3 $HERE/web/server.py --directory $HERE/web
}

case $1 in
    clean)
        clean_em
        ;;
    build)
        # assert $2 is in (super_simple or em_filesystem or em_thread_and_event_loop), and call build_em with $2
        if [ "$2" != "super_simple" ] && [ "$2" != "em_filesystem" ] && [ "$2" != "em_thread_and_event_loop" ]; then
            echo "Usage: $0 build {super_simple|em_filesystem|em_thread_and_event_loop}"
            exit 1
        fi
        build_em $2
        ;;
    run)
        run_em
        ;;
    *)
        echo "Usage: $0 {clean|build|run}"
        exit 1
        ;;
esac
