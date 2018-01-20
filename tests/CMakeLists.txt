
file(DOWNLOAD
	https://github.com/catchorg/Catch2/releases/download/v1.12.0/catch.hpp
	${CMAKE_CURRENT_BINARY_DIR}/catch.hpp
	EXPECTED_HASH
	SHA256=505cb08da48a91eec41c3c1591d04d18776c8522b0d1e9e50c4133daa820c903
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/pammap/core")
add_executable(test_library
	test.cpp
	main.cpp
)
target_link_libraries(test_library pammap)
add_test(NAME "Test pammap C++ library" COMMAND test_library)
