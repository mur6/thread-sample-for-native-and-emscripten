#!/bin/bash

HERE=$(dirname $(realpath $0))

clean() {
    echo "DIR=$HERE"
    echo "Cleaning..."
    rm -rf $HERE/build
}

build() {
    echo "DIR=$HERE"
    echo "Building..."
    mkdir -p $HERE/build
    clang++ $HERE/src/cpp/main.cpp -O1 -std=c++20 -o $HERE/build/main
}

run() {
    echo "DIR=$HERE"
    echo "Running..."
    $HERE/build/main
}

clean
build
run
