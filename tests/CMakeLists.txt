include_directories("${PROJECT_SOURCE_DIR}")
add_executable(test_library test.cpp)
target_link_libraries(test_library pammap)
add_test(NAME "Test pammap C++ library" COMMAND test_library)
