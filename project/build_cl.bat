@echo off

set target_name=elementary

set build_folder=build
if not exist %build_folder% mkdir %build_folder%
pushd %build_folder%

set intermediate_folder=intermediate
if not exist %intermediate_folder% mkdir %intermediate_folder%

set intermediate=%intermediate_folder%\%target_name%

if exist *.pdb del /q *.pdb

rem //
rem // >> COMPILER
rem //
set compiler=-I..\code

set compiler=%compiler% -std:c++14
set compiler=%compiler% -Fe.\%target_name%

rem //
rem // >> COMPILER: optimization
rem //
set optimization=-O2

rem -Ob: Inline Function Expansion
rem -Og: Global Optimizations
rem -Oy: Frame-Pointer Omission
rem -Os: Favor Small Code
rem -Ot: Favor Fast Code
rem -Oi: Generate Intrinsic Functions
rem -GF: Eliminate Duplicate Strings
rem -Gs: Control Stack Checking Calls
rem -Gy: Enable Function-Level Linking

rem -Od: Disable (Debug)
rem -O1: Minimize Size: -Og -Os -Oy -Ob2 -Gs -GF -Gy
rem -O2: Maximize Speed: -Og -Oi -Ot -Oy -Ob2 -Gs -GF -Gy

rem Specify Floating-Point Behavior (fast)
set optimization=%optimization% -fp:fast
rem Specify Floating-Point Behavior (disabled exceptions)
set optimization=%optimization% -fp:except-

set compiler=%compiler% %optimization%

rem //
rem // >> COMPILER: debug, slow compile times
rem //
set debug=

rem -Z7: Produces an .obj file containing full symbolic debugging information
rem -Zi: Produces a program database
rem -ZI: PDB, edit and continue
rem -Zf (Faster PDB generation)

rem Enhance Optimized Debugging
set debug=%debug% -Zo

rem Debug Information Format
set debug=%debug% -Zi -Zf

rem set compiler=%compiler% %debug%

rem //
rem // >> COMPILER: warnings
rem //
set warnings=-W4

rem wd####: Suppresses the compiler warning

rem Treat Linker Warnings as Errors
set warnings=%warnings% -WX
rem nameless struct/union (suppress)
set warnings=%warnings% -wd4201
rem local variable is initialized but not referenced (suppress)
set warnings=%warnings% -wd4189
rem unreferenced formal parameter (suppress)
set warnings=%warnings% -wd4100

set compiler=%compiler% %warnings%

rem //
rem // >> COMPILER: build options
rem //
set build_options=-nologo

rem Enable Minimal Rebuild (disabled)
set build_options=%build_options% -Gm-
rem Use Run-Time Library
set build_options=%build_options% -MT
rem Enable Run-Time Type Information (disabled)
set build_options=%build_options% -GR-
rem Exception Handling Model: all (disabled)
set build_options=%build_options% -EHa-
rem Full Path of Source Code File in Diagnostics
set build_options=%build_options% -FC
rem Build with Multiple Processes
set build_options=%build_options% -MP

set compiler=%compiler% %build_options%

rem //
rem // >> COMPILER: intermediate
rem //

set dll_obj=-Fo.\%intermediate%_dll
set dll_map=-Fm.\%intermediate%_dll

set exe_obj=-Fo.\%intermediate%_exe
set exe_map=-Fm.\%intermediate%_exe

rem //
rem // >> COMPILER: diagnostics
rem //
set diagnostics=

rem build throughput
set diagnostics=%diagnostics% -Bt
rem code generation summary
set diagnostics=%diagnostics% -d2cgsummary

rem set compiler=%compiler% %diagnostics%

rem //
rem // >> LINKER
rem //
set linker=-link

rem Optimizations (eliminates functions and data that are never referenced)
set linker=%linker% -opt:ref
rem Link Incrementally (disabled)
set linker=%linker% -incremental:no

rem //
rem // >> LINKER: libs
rem //
set libs=user32.lib gdi32.lib

rem //
rem // >> LINKER: intermediate
rem //

set dll_pdb=-PDB:.\%target_name%_dll_%random%.pdb
set dll_imp=-IMPLIB:.\%intermediate%_dll.lib

set exe_pdb=-PDB:.\%target_name%_exe_%random%.pdb

rem //
rem // >> LINKER: subsystem
rem //

rem console, use "int main(...) { }
set subsystem=-SUBSYSTEM:CONSOLE

rem 32-bit, use "int CALLBACK WinMain(...) { }
rem set subsystem=-SUBSYSTEM:WINDOWS,5.01

rem 64-bit, use "int CALLBACK WinMain(...) { }
rem set subsystem=-SUBSYSTEM:WINDOWS,5.02

rem //
rem // >> CL
rem //
rem -LD: Creates a DLL
rem -DLL: Build a DLL
set dll_compiler=%compiler% %dll_obj%
set exe_compiler=%compiler% %exe_obj%

echo ---- DLL  ---- %time%
cl "..\code\game.cpp"  %dll_compiler% -LD %linker% -DLL %dll_imp%
echo ---- EXE  ---- %time%
cl "..\code\win32.cpp" %exe_compiler%     %linker% %libs% %subsystem%
echo ---- DONE ---- %time%

popd
