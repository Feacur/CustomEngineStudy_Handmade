@echo off

set build_folder="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build"

pushd %build_folder%

rem 32-bit
call "vcvarsall.bat" x86

rem 64-bit
rem call "vcvarsall.bat" x64

popd
