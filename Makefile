CSOURCES=src/libsyscfg.c src/config.h include/syscfg.h
CFLAGS += -Iinclude
CC ?= gcc
LD ?= ld
RM ?= rm

all: lib/libsyscfg.so lib/modules/simple.so bin/test

build/libsyscfg.o: $(CSOURCES)
	$(CC) -o build/libsyscfg.o src/libsyscfg.c $(CFLAGS) -c

lib/libsyscfg.so: build/libsyscfg.o
	$(LD) -o lib/libsyscfg.so build/libsyscfg.o -shared

lib/modules/simple.so: build/modules/simple.o
	$(LD) -o lib/modules/simple.so build/modules/simple.o -shared

build/modules/simple.o: src/modules/simple.c
	$(CC) -o build/modules/simple.o src/modules/simple.c $(CFLAGS) -c

bin/test: src/test.c lib/libsyscfg.so lib/modules/simple.so
	$(CC) -o bin/test src/test.c $(CFLAGS) -Llib -lsyscfg

test: bin/test
	@sh -c "LD_LIBRARY_PATH=lib ./bin/test"


clean:
	rm -r build
	rm -r bin
	rm -r lib
	mkdir build/modules -p
	mkdir bin
	mkdir lib/modules -p