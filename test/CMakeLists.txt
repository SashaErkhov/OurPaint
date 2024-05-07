cmake_minimum_required(VERSION 3.14)
SET(CMAKE_MAKE_PROGRAM "D:/program/Msys2/usr/bin/make.exe")
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER D:/program/Msys2/mingw64/bin/gcc.exe)
SET(CMAKE_CXX_COMPILER D:/program/gcc/bin/g++.exe)
project(ourpaintest)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
set(SOURCES for_test.cpp ../paint.cpp ../objects.cpp ../BMPpainter.cpp ../BMPfile/code/BMPfile.cpp)
include_directories(.. , ../additions , ../BMPfile/code)
add_executable(
  ourpaintest
  test.cpp
  ${SOURCES}
)
target_link_libraries(
  ourpaintest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ourpaintest)
