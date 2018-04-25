#!/bin/bash -ex

set_compilers() {
	local COMPNAME=$(echo "${COMPILER}" | cut -d- -f1)
	local COMPVER=$(echo "${COMPILER}" | cut -d- -f2)

	case "$COMPNAME" in
		clang)
			export CC="clang-$COMPVER"
			export CXX="clang++-$COMPVER"
			;;
		*)
			export CC="gcc-$COMPVER"
			export CXX="g++-$COMPVER"
			;;
	esac
}

# --------------------------------------------------------------

set_compilers "${COMPILER}"

cmake --version
ninja --version
${CC} --version
${CXX} --version

# --------------------------------------------------------------
# Build
mkdir build && cd build
cmake -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_C_COMPILER=${CC} \
	-DPAMMAP_BUILD_EXAMPLES=OFF \
	-DPAMMAP_ENABLE_TESTS=ON ..
cmake --build  . --target all -- -j ${CORES:-1}

# Test
./pammap/core/tests/test_pammap_core
