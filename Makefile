
all: release

prefix ?= /usr/local

debug:
	mkdir -p build-debug
	+(cd build-debug && cmake ../src -DCMAKE_INSTALL_PREFIX=$(prefix) -DCMAKE_BUILD_TYPE=debug && make)

release:
	mkdir -p build-release
	+(cd build-release && cmake ../src -DCMAKE_INSTALL_PREFIX=$(prefix) && make)

install: release
	+(cd build-release && cmake ../src -DCMAKE_INSTALL_PREFIX=$(prefix) && make install)
clean:
	rm -rf build-debug build-release

