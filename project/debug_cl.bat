@echo off

set target_name=elementary
set build_folder="build"
if not exist %build_folder% mkdir %build_folder%

pushd %build_folder%

start devenv %target_name%.exe "..\code\win32.cpp"

popd
