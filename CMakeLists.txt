cmake_minimum_required(VERSION 4.2.0)

project(case-study)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
# include header only unit-test-framework
FetchContent_Declare(Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.13.10
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(apps)
