@echo off

mkdir build-release

pushd build-release
cmake -A x64 .. || exit /B 1
cmake --build . --config Release || exit /B 1
popd

set example=examples\voice

build-release\Release\stftpitchshift.exe -i %example%.wav -o %example%.cpp.wav %*
python3 stftpitchshift.py -i %example%.wav -o %example%.python.wav %*
