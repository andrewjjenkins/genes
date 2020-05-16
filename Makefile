
all: build

cmake-build: build/genes/Makefile

build/genes/Makefile:
	mkdir -p build
	cmake -Wno-dev -B build/genes

build: build/genes/Makefile
	make -C build/genes -j16

cmd: build/genes/Makefile
	make -C build/genes -j16 GenesCmds

fmt: build/genes/Makefile
	make -C build/genes clang-format

build-clean: 
	rm -rf build/

clean: build/genes/Makefile
	make -C build/genes clean

.PHONY: all cmake-build build cmd fmt build-clean clean test
