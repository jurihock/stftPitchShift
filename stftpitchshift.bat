@echo off

mkdir build-release

pushd build-release
cmake -A x64 .. || exit /b
cmake --build . --config Release || exit /b
popd

set example=examples\voice

build-release\Release\stftpitchshift.exe -i %example%.wav -o %example%.cpp.wav %*
python stftpitchshift.py -i %example%.wav -o %example%.python.wav %*

python compare.py %example%.python.wav %example%.cpp.wav
