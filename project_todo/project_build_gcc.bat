@echo off

chcp 1252
rem code page 1250: Windows Central Europe
rem code page 1251: Windows Cyrillic
rem code page 1252: Windows Western

rem read batch file arguments
set compilation_unit=%1
set target_name=%2
set kind=%3
set architecture=%4
set configuration=%5
set defines=%6
set links=%7

if [%compilation_unit%] == [] (
	echo provide compilation unit [*.cpp]
	exit /b 0
)

if [%target_name%] == [] (
	echo provide target name
	exit /b 0
)

if [%kind%] == [] (
	echo provide kind [Console, SharedLib, WindowedApp]
	exit /b 0
)

if [%architecture%] == [] (
	set architecture="x64"
)

if [%configuration%] == [] (
	set configuration="Shipping"
)

if [%defines%] == [] (
	set defines=""
)

if [%links%] == [] (
	set links=""
)

rem clean batch file arguments
set compilation_unit=%compilation_unit:"=%
set target_name=%target_name:"=%
set kind=%kind:"=%
set architecture=%architecture:"=%
set configuration=%configuration:"=%
set defines=%defines:"=%
set links=%links:"=%

rem declare names
set extra_name=%target_name%_extra

rem declare paths
set sourcedir=%~dp0\code
set targetdir=%~dp0\bin\custom
set objdir=%~dp0\bin-int\custom

rem //
rem // >> COMPILER
rem //
set compiler=-std:c++17
set compiler=%compiler% -I%sourcedir%
set compiler=%compiler% %defines%

if %kind% == WindowedApp (
	set compiler=%compiler% -DWIN_MAIN
)

if %configuration% == Shipping (
	set compiler=%compiler% -DSHIPPING
) else if %configuration% == Development (
	set compiler=%compiler% -DDEVELOPMENT
) else if %configuration% == Debug (
	set compiler=%compiler% -DDEBUG
)

set compiler=%compiler% -o.\%target_name%

rem //
rem // >> COMPILER: optimization
rem //
set optimization=
if %configuration% == Shipping (
	set optimization=%optimization% -O3
) else if %configuration% == Development (
	set optimization=%optimization% -O3
) else if %configuration% == Debug (
	set optimization=%optimization% -O0
)

set optimization=%optimization% -ffast-math
set optimization=%optimization% -msse2

set compiler=%compiler% %optimization%

rem //
rem // >> COMPILER: debug, slow compile times
rem //
set debug=-ggdb

rem set compiler=%compiler% %debug%

rem //
rem // >> COMPILER: warnings
rem //
set warnings=-Wall -Wextra -Werror

set warnings=%warnings% -Wno-unused-variable
set warnings=%warnings% -Wno-unused-parameter
set warnings=%warnings% -Wno-missing-field-initializers

set compiler=%compiler% %warnings%

rem //
rem // >> COMPILER: build options
rem //
set build_options=-nologo
if %configuration% == Shipping (
	set debug=%debug% -MT
) else if %configuration% == Development (
	set debug=%debug% -MT
) else if %configuration% == Debug (
	set debug=%debug% -MTd
)

rem Enable Minimal Rebuild (disabled)
set build_options=%build_options% -Gm-
rem Enable Run-Time Type Information (disabled)
set build_options=%build_options% -GR-
rem Exception Handling Model: all (disabled)
set build_options=%build_options% -EHa-
rem Full Path of Source Code File in Diagnostics
set build_options=%build_options% -FC
rem Build with Multiple Processes
set build_options=%build_options% -MP
rem Calling Convention
set build_options=%build_options% -Gd
rem Specify Source File Type
set build_options=%build_options% -TP
rem Remove unreferenced COMDAT
set build_options=%build_options% -Zc:inline
rem Buffer Security Check
set build_options=%build_options% -GS

rem -nologo: Suppress Startup Banner
rem -c:   Compile Without Linking
rem -MT:  Use Run-Time Library (multithread, static); defines _MT
rem -MTd: Use Run-Time Library (multithread, static, debug); defines _MT, _DEBUG

set compiler=%compiler% %build_options%

rem //
rem // >> COMPILER: intermediate
rem //

set obj=-Fo%objdir%\%extra_name%
set map=-Wl,--Map=%objdir%\%extra_name%

rem //
rem // >> COMPILER: diagnostics
rem //
set diagnostics=-ftime-report -ftime-report-details

rem -ftime-report:             Makes the compiler print some statistics about the time consumed by each pass when it finishes.
rem -ftime-report-details: Record the time consumed by infrastructure parts separately for each pass.

rem set compiler=%compiler% %diagnostics%

rem //
rem // >> LINKER
rem //
set linker=-link

if %kind% == SharedLib (
	set linker=%linker% -Wl,--out-implib,%objdir%\%extra_name%.lib
)

if %configuration% == Shipping (
	set linker=%linker%
) else if %configuration% == Development (
	set linker=%linker%
) else if %configuration% == Debug (
	set linker=%linker%
)

rem //
rem // >> LINKER: libs
rem //
set libs=

set libs=%libs% %links%

rem //
rem // >> LINKER: subsystem
rem //

if %kind% == ConsoleApp (
	rem use int main(...) { }
	if %architecture% == x64 (
		set subsystem=-Wl,--subsystem,console:5.02
	) else if %architecture% == x86 (
		set subsystem=-Wl,--subsystem,console:5.01
	) else (
		echo unexpected architecture %architecture%
		exit /b 0
	)
) else if %kind% == WindowedApp (
	rem use int CALLBACK WinMain(...) { }
	if %architecture% == x64 (
		set subsystem=-Wl,--subsystem,windows:5.02
	) else if %architecture% == x86 (
		set subsystem=-Wl,--subsystem,windows:5.01
	) else (
		echo unexpected architecture %architecture%
		exit /b 0
	)
)

rem //
rem // >> CL
rem //
set compiler=%compiler% %obj% %map%

if %kind% == SharedLib (
	set compiler=%compiler% -shared
)

rem prepare folders, clean up, build
if not exist %targetdir% mkdir %targetdir%
if not exist %objdir% mkdir %objdir%

pushd %targetdir%
if exist %extra_name%* del /q %extra_name%*
popd

pushd %objdir%
if exist %extra_name%* del /q %extra_name%*
popd

if %architecture% == x64 (
	set PATH=%PATH%;C:\Program Files\mingw64\bin
) else if %architecture% == x86 (
	set PATH=%PATH%;C:\Program Files (x86)\mingw32\bin
) else (
	echo unexpected architecture %architecture%
	exit /b 0
)

echo ---- BUILD ---- %time%
pushd %targetdir%
gcc "%compilation_unit%" %compiler% %linker% %libs% %subsystem%
popd
echo ---- DONE ---- %time%
