#!/bin/bash

HERE=$(dirname $(realpath $0))

_clean() {
    target_dir=$1
    echo "Removing $target_dir"
    rm -rf "$target_dir"
}

clean_em() {
    target_dir=$HERE/web/dist
    _clean "$target_dir"
}

build_em() {
    echo "DIR=$HERE"
    echo "Building..."
    mkdir -p $HERE/web
    emcc $HERE/src/cpp/main.cpp -std=c++20 -o $HERE/web/main.js
}


build_native() {
    echo "DIR=$HERE"
    echo "Building..."
    mkdir -p $HERE/build
    clang++ $HERE/src/cpp/main.cpp -O1 -std=c++20 -o $HERE/build/main
}

run_native() {
    echo "DIR=$HERE"
    echo "Running..."
    $HERE/build/main
}

native_all() {
    clean
    build_native
    run_native
}

em_all() {
    echo "DIR=$HERE"
    echo "Running..."
    emrun --browser chrome $HERE/web/main.js
}
