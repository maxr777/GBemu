# GBemu

A simple multi-platform gameboy emulator. Desktop layer written with raylib. Planned STM32 (ESP32?) port in the future.

## Current state

- [x] CPU fully implemented (passes all opcode tests: `./run.sh --cpu`)
- [x] ROM loading (passes my own tests)
- [x] Memory map read and writes (passes my own tests)
- [x] Interrupts (passes all mooneye tests)
- [ ] WIP: Timers
- [ ] PPU
- [ ] Joystick input
- [ ] Save files (SRAM persistence)
- [ ] Save states (save whole state of the emulator)
- [ ] MBCs (current: MBC0, MBC1; planned: MBC3, MBC5)
- [ ] Audio
- [ ] STM32 breadboard
- [ ] STM32 port
- [ ] STM32 PCB
- [ ] 3d printed case

I'm still not sure if I want to go with STM32 or ESP32.  

One stretch goal would be to get rid of raylib and implement platform video, audio, and input interface myself.

## Structure

The project uses unity build approach (one big translation unit) inspired by [Handmade Hero](https://guide.handmadehero.org/).

The ```platform/``` directory contains platform specific code as well as the entry points (e.g. ```desktop_gbemu.c``` is the desktop entry point and uses raylib).  

Tests and their runners are in the ```tests/``` directory. For more on tests, see `tests/README.md`.  

Platform non-specific code (basically the gameboy code) is in ```gameboy/```.  

Any 3rd party dependencies are vendored in ```external/```.

## Build Dependencies

To build the project, a C compiler is required:  

Linux: ```gcc```

Windows: ```mingw```

Note that first build also compiles raylib, so it will be relatively slow.
Everything after that just reuses ```libraylib.a```.

## Build

Run ```./run.sh``` on Linux.

Run ```run.bat``` on Windows.

```./run.sh --cpu``` to run the CPU tests (they take long, so they're not run by default).
