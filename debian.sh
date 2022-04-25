#!/bin/bash

BUILD=build-debian
VERSION=$(cat VERSION | xargs)

rm -rf ${BUILD}

cmake -S . -B ${BUILD} -DSHARED=ON -DDEB=ON
cpack --config ${BUILD}/CPackSourceConfig.cmake

tar -xzvf stftpitchshift_${VERSION}.tar.gz -C ${BUILD}

pushd ${BUILD}/stftpitchshift_${VERSION}
debuild -S --no-sign
debuild -B --no-sign
popd
