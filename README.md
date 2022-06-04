# software rasterizer
this is a project of software rasterizer built on sdl library

## setup
on linux:
* install `g++` and `make`
* install `sdl2` and `sdl2_ttf` libraries from your package manager (`yum`, `apt`, `dnf` etc.)
* `cd` into project root and run `make`
* to run the program execute `bin/program`

on windows:
* install `vcpkg`
* run `vcpkg install sdl2:x64-windows` and `vcpkg install sdl2_ttf:x64-windows`
* open visual studio, select new -> project from source
* create project in the root of this directory
* add `include` to additional #include directories for the project