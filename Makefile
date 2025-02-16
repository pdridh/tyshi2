BUILD_DIR = build
BUILD_WIN_DIR = ${BUILD_DIR}/win_build
BUILD_LINUX_DIR = ${BUILD_DIR}/linux_build


BIN_DIR = bin
BIN_WIN_DIR = ${BIN_DIR}/win_bin
BIN_LINUX_DIR = ${BIN_DIR}/linux_bin

MINGW_TOOLCHAIN_FILE = CMake/mingw-toolchain.cmake


all: dev

dev:
	cmake -S . -B ${BUILD_LINUX_DIR} -D CMAKE_BUILD_TYPE=Debug
	cmake --build ${BUILD_LINUX_DIR} --target install
	${BUILD_LINUX_DIR}/app

release:
	cmake -S . -B ${BUILD_LINUX_DIR} -D CMAKE_BUILD_TYPE=Release
	cmake --build ${BUILD_LINUX_DIR} --target install

win-dev:
	cmake -S . -B ${BUILD_WIN_DIR} -D CMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=${MINGW_TOOLCHAIN_FILE}
	cmake --build ${BUILD_WIN_DIR} --target install
	${BIN_WIN_DIR}/app.exe

win-release:
	cmake -S . -B ${BUILD_WIN_DIR} -D CMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=${MINGW_TOOLCHAIN_FILE}
	cmake --build ${BUILD_WIN_DIR} --target install

clean: clean-build clean-bin

clean-build:
	rm -rf ${BUILD_DIR}

clean-bin:
	rm -rf ${BIN_DIR}

.PHONY : dev release win-dev win-release clean clean-build clean-bin 


