cmake_minimum_required(VERSION 3.10)
project(lab3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# Enable Qt MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/inc/tmplInc)
include_directories(${CMAKE_SOURCE_DIR}/inc/ui)

# Common sources
file(GLOB COMMON_SOURCES 
    "src/tmpl/*.cpp"
    "src/tmpl/*.tpp"
)

# Qt UI sources
set(UI_SOURCES
    src/ui/main.cpp
    src/ui/mainwindow.cpp
    inc/ui/mainwindow.hpp
    src/ui/mainwindow.ui
)

# Qt application
add_executable(lab3_qt 
    ${UI_SOURCES}
    ${COMMON_SOURCES}
)

target_link_libraries(lab3_qt Qt5::Core Qt5::Widgets)

# Tests
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(tests)