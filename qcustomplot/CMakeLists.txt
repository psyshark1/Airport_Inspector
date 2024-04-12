cmake_minimum_required(VERSION 3.5)

project(qcustomplot)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

include_directories(qcustomplot)

set(QCUSTOMPLOT qcustomplot.h qcustomplot.cpp)

add_library(qcustomplot SHARED ${QCUSTOMPLOT})

target_compile_definitions(qcustomplot PRIVATE QCUSTOMPLOT_COMPILE_LIBRARY)

target_link_libraries(qcustomplot Qt6::Core)
