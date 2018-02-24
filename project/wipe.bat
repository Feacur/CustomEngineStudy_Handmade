@echo off
chcp 1252

set build_folder=build
if exist %build_folder% rmdir /s /q %build_folder%
