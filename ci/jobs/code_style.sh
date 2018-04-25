#!/bin/bash -ex

find_run_clang_tidy() {
	local CLANG_TIDY_VERSION=$(clang-tidy --version | awk '/LLVM version/ {print $3}')
	local MAJOR=$(echo "${CLANG_TIDY_VERSION}" | cut -d. -f1)
	local MINOR=$(echo "${CLANG_TIDY_VERSION}" | cut -d. -f2)
	local SUFFIX="${MAJOR}.${MINOR}"

	for path in /usr/lib/llvm-${SUFFIX}/share/clang /usr/lib/llvm-${SUFFIX}/bin; do
		for name in run-clang-tidy.py run-clang-tidy-${SUFFIX}.py; do
			if [ -x "$path/$name" ]; then
				echo "$path/$name"
				return 0
			fi
		done
	done
	return 1
}

# --------------------------------------------------------------

clang-tidy --version
find_run_clang_tidy

# --------------------------------------------------------------
# Initialise build directory
# This exports the compilation commands for clang-tidy
mkdir build
pushd build
cmake -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_C_COMPILER=${CC} \
	-DPAMMAP_BUILD_EXAMPLES=OFF \
	-DPAMMAP_ENABLE_TESTS=ON ..
popd

# Run clang-tidy
./scripts/run-clang-tidy.py -run-clang-tidy "$(find_run_clang_tidy)" \
	-j ${CORES:-1} -check-output
