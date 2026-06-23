# GBemu

A simple multi-platform gameboy emulator. Desktop layer written with raylib. Planned STM32 port in the future.

## Structure

The project uses unity build approach (one big translation unit).  

The ```platform/``` directory contains the platform specific code as well as the entry points (e.g. ```desktop_gbemu.c``` is the desktop entry point and uses raylib).  

Tests and their runners are in the ```tests/``` directory.  

Platform non-specific code (basically the gameboy code) is in ```gameboy/```.  

Any 3rd party dependencies are vendored in ```external/```.

## Build Dependencies

To build the project, a C compiler is required:  

Linux: ```gcc```

Windows: ```mingw``` (basically gcc for windows)  

## Build

Run ```./run.sh``` on Linux.

Run ```run.bat``` on Windows.
