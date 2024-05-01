include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)
enable_testing()
add_executable(test_exe test.cpp)
target_include_directories(test_exe PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(test_exe gtest_main ${GTEST_BOTH_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(test_exe)

target_compile_definitions(test_exe PRIVATE
    INPUT_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/sample-input-SQ24.txt"
    OUTPUT_FILE_PATH="${CMAKE_CURRENT_BINARY_DIR}/test_output.txt"
    EXPECTED_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/sample-output.txt"
    EXECUTABLE_FILE_PATH="${CMAKE_CURRENT_BINARY_DIR}/../${PROJECT_NAME}"
)
