#!/bin/bash

run() {
  echo $@ && time $@
}

mkdir -p build-release

pushd build-release >/dev/null 2>&1
cmake -DCMAKE_BUILD_TYPE=Release .. || exit 1
cmake --build . || exit 1
popd >/dev/null 2>&1

example=examples/voice

run build-release/stftpitchshift -i $example.wav -o $example.cpp.wav $@
run python stftpitchshift.py -i $example.wav -o $example.python.wav $@
