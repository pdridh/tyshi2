dev:
	cmake -S . -B build/linux_build -D CMAKE_BUILD_TYPE=Debug
	cmake --build build/linux_build
	build/linux_build/app

release:
	cmake -S . -B build/linux_build -D CMAKE_BUILD_TYPE=Release
	cmake --build build/linux_build

win-dev:
	cmake -S . -B build/win_build -D CMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=CMake/mingw-toolchain.cmake
	cmake --build build/win_build

win-release:
	cmake -S . -B build/win_build -D CMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=CMake/mingw-toolchain.cmake
	cmake --build build/win_build

clean:
	rm -rf build