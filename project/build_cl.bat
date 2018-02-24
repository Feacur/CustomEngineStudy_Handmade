@echo off

set target_name=elementary

set build_folder=build
if not exist %build_folder% mkdir %build_folder%
pushd %build_folder%

set intermediate_folder=intermediate
if not exist %intermediate_folder% mkdir %intermediate_folder%

rem -Zo: Enhance Optimized Debugging
rem -Z7: Debug Information Format

rem -Od: Disable (Debug)
rem -Ob: Inline Function Expansion
rem -Og: Global Optimizations
rem -Oy: Frame-Pointer Omission
rem -Os: Favor Small Code
rem -Ot: Favor Fast Code
rem -Oi: Generate Intrinsic Functions
rem -GF: Eliminate Duplicate Strings
rem -Gs: Control Stack Checking Calls
rem -Gy: Enable Function-Level Linking
rem -O1: Minimize Size: -Og -Os -Oy -Ob2 -Gs -GF -Gy
rem -O2: Maximize Speed: -Og -Oi -Ot -Oy -Ob2 -Gs -GF -Gy
rem -fp:fast: Specify Floating-Point Behavior (fast)
rem -fp:except-: Specify Floating-Point Behavior (disabled exceptions)

rem -W4: Warning Level
rem -WX: Treat Linker Warnings as Errors: 
rem -wd4201: nameless struct/union
rem -wd4189: local variable is initialized but not referenced
rem -wd4100: unreferenced formal parameter

rem -nologo: Suppress Startup Banner
rem -Gm-: Enable Minimal Rebuild (disabled)
rem -MT: Use Run-Time Library
rem -GR-: Enable Run-Time Type Information (disabled)
rem -EHa-: Exception Handling Model: all (disabled)
rem -EHsc: Exception Handling Model: C++
rem -FC: Full Path of Source Code File in Diagnostics
rem -LD: Creates a DLL
rem -Fmabc: Name Map File "abc"
rem -Foabc: Name Object File "abc"

rem -DLL: Build a DLL
rem -PDB:abc: Use Program Database "abc"
rem -opt:ref: Optimizations (eliminates functions and data that are never referenced)
rem -incremental:no: Link Incrementally (disabled)
rem -Feabc: Name EXE or DLL File "abc"

set DllName=%target_name%
set DllIntermediate=%intermediate_folder%\%DllName%_dll

set ExeName=%target_name%
set ExeIntermediate=%intermediate_folder%\%ExeName%_exe

set DebugFlags=-Zo -Z7
set OptimizeFlags=-O2 -fp:fast -fp:except-
set WarningFlags=-W4 -WX -wd4201 -wd4189 -wd4100
set AdditionalFlags=-I..\code -nologo -Gm- -MT -GR- -EHa- -FC
set CompilerFlags=-std:c++14 %AdditionalFlags% %OptimizeFlags% %DebugFlags% %WarningFlags%
set DllCompilerIntermediate=-Fm.\%DllIntermediate% -Fo.\%DllIntermediate%
set DllCompilerFlags=%CompilerFlags% -LD %DllCompilerIntermediate%
set ExeCompilerIntermediate=-Fm.\%ExeIntermediate% -Fo.\%ExeIntermediate%
set ExeCompilerFlags=%CompilerFlags%     %ExeCompilerIntermediate%

set LinkerFlags=-link -opt:ref -incremental:no
set ExeLibraries=user32.lib gdi32.lib
set DllLinkerFlags=%LinkerFlags% -PDB:.\%DllName%_dll_%random%.pdb -DLL -IMPLIB:.\%DllIntermediate%.lib
set ExeLinkerFlags=%LinkerFlags% -PDB:.\%ExeName%_exe_%random%.pdb %ExeLibraries%

rem console, use "int main(...) { }
set ExeLinkerFlags=%ExeLinkerFlags% -SUBSYSTEM:CONSOLE

rem 32-bit, use "int CALLBACK WinMain(...) { }
rem set ExeLinkerFlags=%ExeLinkerFlags% -SUBSYSTEM:WINDOWS,5.01

rem 64-bit, use "int CALLBACK WinMain(...) { }
rem set ExeLinkerFlags=%ExeLinkerFlags% -SUBSYSTEM:WINDOWS,5.02

if exist *.pdb del /q *.pdb

echo    - time: %time%
cl "..\code\game.cpp"  -Fe.\%target_name% %DllCompilerFlags% %DllLinkerFlags%
echo    - time: %time%
cl "..\code\win32.cpp" -Fe.\%target_name% %ExeCompilerFlags% %ExeLinkerFlags%
echo    - time: %time%

popd
