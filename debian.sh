#!/bin/bash

BUILD=build-debian
VERSION=$(cat VERSION | xargs)

rm -rf ${BUILD}

cmake -S . -B ${BUILD} -DBUILD_SHARED_LIBS=ON -DDEB=ON || exit $?
cpack --config ${BUILD}/CPackSourceConfig.cmake || exit $?

tar -xzvf stftpitchshift_${VERSION}.tar.gz -C ${BUILD} || exit $?

pushd ${BUILD}/stftpitchshift_${VERSION} >/dev/null 2>&1
debuild -S --no-sign || exit $?
debuild -B --no-sign || exit $?
popd >/dev/null 2>&1
