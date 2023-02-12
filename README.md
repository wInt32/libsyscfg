# libsyscfg - A unified system configuration library

### WARNING: This project is in early development, expect issues. 

Libsyscfg makes configuration easy, as it has a single API for developers to use.

## API

There are only 3 API functions you really need:
- ```syscfg_list()```
- ```syscfg_get()```
- ```syscfg_set()```

They are documented in the syscfg.h header.

## Modules

Modules are a key feature of libsyscfg. The idea is to have multiple different implementations for the API, but link to a single library.  
NOTE: due to the nature of modules, static linking is not officialy supported.

## Building
It should be as simple as running `make`, the resulting binaries are in `bin`, headers in `include` and shared libraries in `lib`.  
To do something actually useful, change `SYSCFG_MODULE_PATH` and `SYSCFG_M_SIMPLE_CFG_PATH` to a path of your liking (e.g. `/lib/syscfg/modules/simple.so` and `/etc/cfg/`)

## Issues
If you find an issue, you can debug it with gdb - just add CFLAGS=-g before `make` and it'll build binaries with debug symbols.