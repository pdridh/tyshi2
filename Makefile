dev:
	cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Debug
	cmake --build build
	build/app

release:
	cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Release
	cmake --build build/

clean:
	rm -rf build