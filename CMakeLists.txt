cmake_minimum_required(VERSION 3.5)
project(project1 VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
set(SOURCES_FILES ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES_FILES})
