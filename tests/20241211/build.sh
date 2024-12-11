#!/bin/bash

HERE=$(dirname $(realpath $0))

clean() {
    echo "DIR=$HERE"
    echo "Cleaning..."
    rm -rf $HERE/build
    rm -rf $HERE/web
}

build_native() {
    echo "DIR=$HERE"
    echo "Building..."
    mkdir -p $HERE/build
    clang++ $HERE/src/cpp/main.cpp -O1 -std=c++20 -o $HERE/build/main
}

build_em() {
    echo "DIR=$HERE"
    echo "Building..."
    mkdir -p $HERE/web
    emcc $HERE/src/cpp/main.cpp -std=c++20 -o $HERE/web/main.js
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
