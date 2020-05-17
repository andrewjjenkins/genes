
all: build

cmake-build: build/genes/Makefile

build/genes/Makefile:
	mkdir -p build
	cmake -Wno-dev -B build/genes

build_release/genes/Makefile:
	mkdir -p build_release
	cmake -Wno-dev -B build_release/genes -DCMAKE_BUILD_TYPE=Release

build: build/genes/Makefile
	make -C build/genes -j16

cmd: build/genes/Makefile
	make -C build/genes -j16 GenesCmds

fmt: build/genes/Makefile
	make -C build/genes clang-format

benchmark: build_release/genes/Makefile
	make -C build_release/genes -j16 GenesBenchmark
	build_release/genes/test/GenesBenchmark --benchmark_repetitions=5 --benchmark_color=true

build-clean: 
	rm -rf build/ build-release/

clean: build/genes/Makefile
	make -C build/genes clean
	make -C build_release/genes clean

.PHONY: all cmake-build build cmd fmt benchmark build-clean clean test
