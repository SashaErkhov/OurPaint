cmake_minimum_required(VERSION 3.14)
project(ourpainttest)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
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
set(SOURCES ../painter/paint.cpp ../painter/objects.cpp ../painter/BMPpainter.cpp "../Matrix/Matrix.h" "../graph/simpleGraph.h" ../painter/requirements.cpp ../painter/requirements.h ../painter/enums.h)
include_directories(../additions)
include_directories(../BMPfile/code)
add_subdirectory(../BMPfile build)
add_executable(
  ourpainttest
  test.cpp
  ${SOURCES}
)
target_link_libraries(
  ourpainttest bitmap
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ourpainttest)
