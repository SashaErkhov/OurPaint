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
set(SOURCES  ../painter/objects.cpp  ../painter/saveload/objectInFile.cpp ../painter/saveload/FileOurP.cpp)
add_executable(
  savergtest
  savergtests.cpp
  ${SOURCES}
)
add_executable(
        ourpainttest
        test.cpp
        ${SOURCES}
)
add_executable(
        graphgtest
        graphgtests.cpp
        ${SOURCES}
)
target_link_libraries(
  ourpainttest
  GTest::gtest_main
)
target_link_libraries(
        graphgtest
        GTest::gtest_main
)
target_link_libraries(
        savergtest
        GTest::gtest_main
)
target_link_libraries(
        ourpainttest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ourpainttest)
