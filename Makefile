
all: release

debug:
	mkdir -p build-debug
	+(cd build-debug && cmake ../src -DCMAKE_BUILD_TYPE=debug && make)

release:
	mkdir -p build-release
	+(cd build-release && cmake ../src && make)

clean:
	rm -rf build-debug build-release

