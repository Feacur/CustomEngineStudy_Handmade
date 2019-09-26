@echo off

call ..\\init_environment_gcc.bat

set target_name=elementary

set build_folder=..\..\build
if not exist %build_folder% mkdir %build_folder%

set source_folder=%cd%
pushd %build_folder%

set intermediate_folder=intermediate
if not exist %intermediate_folder% mkdir %intermediate_folder%

set intermediate=%intermediate_folder%\%target_name%

if exist *.pdb del /q *.pdb

rem //
rem // >> COMPILER
rem //
set compiler=

set compiler=%compiler% -I%source_folder%\..

set compiler=%compiler% -std=c++14

rem //
rem // >> COMPILER: optimization
rem //
set optimization=-O3

rem -fno-math-errno
rem -funsafe-math-optimizations
rem -ffinite-math-only
rem -fno-rounding-math
rem -fno-signaling-nans
rem -fcx-limited-range

set optimization=%optimization% -ffast-math
set optimization=%optimization% -msse2

rem -O0: Disable (Debug)
rem -O1: Optimize
rem -O2: Optimize even more
rem -O3: Optimize yet more for speed
rem -Os: Optimize yet more for size
rem -Og: Optimize with respect to debugging experience

set compiler=%compiler% %optimization%

rem //
rem // >> COMPILER: debug, slow compile times
rem //
set debug=-ggdb

rem set compiler=%compiler% %debug%

rem //
rem // >> COMPILER: warnings
rem //
set warnings=

set warnings=%warnings% -Wall
set warnings=%warnings% -Wextra
set warnings=%warnings% -Werror
set warnings=%warnings% -Wno-unused-variable
set warnings=%warnings% -Wno-unused-parameter
set warnings=%warnings% -Wno-missing-field-initializers
set warnings=%warnings% -Wno-psabi

set compiler=%compiler% %warnings%

rem //
rem // >> COMPILER: build options
rem //

rem //
rem // >> COMPILER: intermediate
rem //

rem //
rem // >> COMPILER: diagnostics
rem //

rem //
rem // >> LINKER
rem //

rem -l abc: Search the library named "abc" when linking
rem -Wl,option: Pass option as an option to the linker

rem //
rem // >> LINKER: libs
rem //

set libs=-l user32 -l gdi32
rem -Wl,--library=user32,--library=gdi32

rem //
rem // >> LINKER: intermediate
rem //

set map=-Wl,--Map=.\%intermediate%_exe.map

rem //
rem // >> LINKER: subsystem
rem //

rem console, use "int main(...) { }
set subsystem=-mconsole

rem 32-bit, 64-bit, use "int CALLBACK WinMain(...) { }
rem set subsystem=-mwindows

rem console, use "int main(...) { }
rem set subsystemL=-Wl,--subsystem,console

rem 32-bit, use "int CALLBACK WinMain(...) { }
rem set subsystemL=-Wl,--subsystem,windows:5.01

rem 64-bit, use "int CALLBACK WinMain(...) { }
rem set subsystemL=-Wl,--subsystem,windows:5.02

rem //
rem // >> GCC
rem //
rem -shared: Produce a shared object which can then be linked with other objects to form an executable
set compiler=-o %target_name%.exe %compiler%

echo ---- EXE  ---- %time%
gcc "%source_folder%\main.cpp" %compiler% %libs% %subsystem%
echo ---- DONE ---- %time%

popd
