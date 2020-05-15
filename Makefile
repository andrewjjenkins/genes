
all: build

cmake-build:
	mkdir -p build
	cmake -B build/genes

build: cmake-build
	make -C build/genes

cmd: cmake-build
	make -C build/genes GenesCmds

fmt: cmake-build
	make -C build/genes clang-format

clean:
	rm -rf build/

.PHONY: all cmake-build build fmt clean test
