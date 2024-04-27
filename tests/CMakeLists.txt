include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)
enable_testing()
add_executable(test_exe test.cpp)
target_link_libraries(test_exe gtest_main)

include(GoogleTest)
gtest_discover_tests(test_exe)
