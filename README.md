### GBemu

A simple multi-platform gameboy emulator. Desktop layer written with raylib. Planned STM32 port in the future.

### Structure

The project uses unity build approach (one big translation unit). The desktop entry point is in ```desktop_gbemu.c```, whereas the STM32 is in ```stm_gbemu.c```

### Dependencies

Raylib and gcc

### Build

Just run ```./run.sh```
