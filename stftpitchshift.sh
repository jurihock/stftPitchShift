#!/bin/sh

mkdir -p build-release

pushd build-release >/dev/null 2>&1
cmake -DCMAKE_BUILD_TYPE=Release .. || exit 1
cmake --build . || exit 1
popd >/dev/null 2>&1

run() {
    echo $@ && /usr/bin/time -h -p $@
}

run build-release/stftpitchshift -i voice.wav -o voice.cpp.wav $@
run python stftpitchshift.py -i voice.wav -o voice.python.wav $@
