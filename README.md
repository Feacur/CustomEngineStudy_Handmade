# Summary
Study on C/C++, custom engines, games and stuff.

# How to work with this stuff
### See sub-projects inside "code/demo" folder
* "code/demo/conway_life"
* "code/demo/tetris"
* "code/demo/naive_silly_ecs"
* "code/demo/study_raymarch"
* "code/demo/study_raytrace"

### My IDE of choice is Visual Studio Code
* See ".vscode/tasks.json" for some useful commands

### See "/project" folder for build scripts
* I'm using unity build right now ("code/game.cpp", "code/win32.cpp")
* Definitely works with Microsoft's cl compiler
* Occasionally I test code with MinGW-w64 gcc compiler

Build scripts put stuff into "/build" folder

# Current state
* Application: ".exe" platform + hot reloadable ".dll" game
* Rendering 2d: plain C and SIMD
* Custom math library
* Simple BMP reader
* Keyboard and pointer input
* Somewhat const correct in places

# To do
* Memory management
* Assets management
* "Entity Component System" framework
* Physics, 2d for starters
* Auto array
* Finilize input recording
