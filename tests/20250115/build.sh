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
    # if PROG_NAME == em_camera_histogram then DIST_DIR = $BUILD_DIR/em_camera_histogram else DIST_DIR = $BUILD_DIR/em_heavy_calculation
    if [ "$PROG_NAME" == "camera_hist" ]; then
        DIST_DIR=$HERE/web/camera_hist/dist
    elif [ "$PROG_NAME" == "save_as_png" ]; then
        DIST_DIR=$HERE/web/save_as_png/dist
    else
        echo "Invalid PROG_NAME=$PROG_NAME"
        exit 1
    fi
    mkdir -p "$DIST_DIR"
    echo "DIST_DIR=$DIST_DIR"
    # emcc src/em_camera_histogram.cpp -o camera_histogram.js -s WASM=1 -s "EXPORTED_FUNCTIONS=['_malloc','_free']" -s "EXPORTED_RUNTIME_METHODS=['ccall','cwrap']" -O3
    # emcc -o output.js capture_red_image_wasm.cpp lodepng.cpp -s USE_PTHREADS=0 -s MODULARIZE=1 -s EXPORT_NAME='Module'
    if [ "$PROG_NAME" == "camera_hist" ]; then
        EXPORTED_FUNCTIONS="_malloc,_free"
    elif [ "$PROG_NAME" == "save_as_png" ]; then
        EXPORTED_FUNCTIONS="_malloc,_free,_main,_captureImageFromCamera"
    else
        echo "Invalid PROG_NAME=$PROG_NAME"
        exit 1
    fi
    # ['_malloc','_free', '_main', '_captureImageFromCamera']
    emcc "$HERE/src/$PROG_NAME.cpp" \
        "$HERE/src/lodepng.cpp" \
        -std=c++23 \
        -s MODULARIZE=1 \
        -s EXPORT_ES6=1 \
        -g \
        -pthread \
        -sPTHREAD_POOL_SIZE=1 \
        -fwasm-exceptions \
        -sENVIRONMENT=web,worker \
        -s "EXPORTED_FUNCTIONS=$EXPORTED_FUNCTIONS" \
        -s "EXPORTED_RUNTIME_METHODS=['ccall','cwrap']" \
        --bind \
        -o "$DIST_DIR/my_em".js
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
        # assert $2 is in (em_filesystem or em_heavy_calculation), and call build_em with $2
        if [ "$2" != "camera_hist" ] && [ "$2" != "save_as_png" ]; then
            echo "Usage: $0 build {camera_hist|save_as_png}"
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
