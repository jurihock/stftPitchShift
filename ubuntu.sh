#!/bin/bash

# fix distribution name in the changelog template
DISTRO=$(lsb_release --short --codename)
sed -i "s/unstable/${DISTRO}/g" debian/changelog.template

# execute the debian build script
source debian.sh

# print commands to be executed next
echo "debsign build-debian/stftpitchshift_${VERSION}_source.changes"
echo "dput stftpitchshift build-debian/stftpitchshift_${VERSION}_source.changes"
