@echo off
chcp 1252

set target_name=elementary
set build_folder="build"

pushd %build_folder%

%target_name%

popd
