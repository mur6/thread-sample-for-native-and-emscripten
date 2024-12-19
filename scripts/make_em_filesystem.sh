#!/bin/bash

EMSDK_DIR="/Users/taichi.muraki/workspace/TestLangs/emsdk"
# set GENERATED_DIR is here.
here=$(dirname $(realpath $0))
GENERATE_TARGET_DIR="$here"
echo "GENERATE_TARGET_DIR=$GENERATE_TARGET_DIR"
pushd "$EMSDK_DIR/upstream/emscripten" || exit 1

FILE_PACKAGE_CMD="./tools/file_packager"
DATA_FILE="$GENERATE_TARGET_DIR/filesystem.data"
JS_FILE="$GENERATE_TARGET_DIR/filesystem.js"
PRELOAD_DIR="/Users/taichi.muraki/workspace/Python/mur6/2024/st-tech/ilab-zozomat-sdk/data@data"
COMMAND="$FILE_PACKAGE_CMD $DATA_FILE --preload $PRELOAD_DIR --js-output=$JS_FILE"
echo "$COMMAND"

$COMMAND

popd || exit 1
