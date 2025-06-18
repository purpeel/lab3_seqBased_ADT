cmake_minimum_required(VERSION 3.10)
project(lab3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/inc/tmplInc)
include_directories(${CMAKE_SOURCE_DIR}/inc/ui)

file(GLOB SOURCES 
    "src/tmpl/*.cpp"
    "src/tmpl/*.tpp"
    "src/console/*.cpp"
    "src/ui/*.cpp"
)

add_executable(lab3_main 
    src/console/main.cpp
    ${SOURCES}
)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(tests)