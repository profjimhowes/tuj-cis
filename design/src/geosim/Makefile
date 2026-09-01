CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -O2
LDFLAGS = -lm

SRC     = src/mesh.c src/planet.c src/field_ops.c

.PHONY: test clean

test: build/test_mesh build/test_planet build/test_field_ops
	./build/test_mesh
	./build/test_planet
	./build/test_field_ops

build/test_mesh: tests/test_mesh.c $(SRC) src/mesh.h | build
	$(CC) $(CFLAGS) -Isrc -o $@ tests/test_mesh.c src/mesh.c $(LDFLAGS)

build/test_planet: tests/test_planet.c $(SRC) src/mesh.h src/planet.h | build
	$(CC) $(CFLAGS) -Isrc -o $@ tests/test_planet.c src/mesh.c src/planet.c $(LDFLAGS)

build/test_field_ops: tests/test_field_ops.c $(SRC) src/mesh.h src/field_ops.h | build
	$(CC) $(CFLAGS) -Isrc -o $@ tests/test_field_ops.c src/mesh.c src/field_ops.c $(LDFLAGS)

build:
	mkdir -p build

clean:
	rm -rf build
