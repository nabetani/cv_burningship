cmake_minimum_required(VERSION 2.8)
project(MyOpenCVProject)

find_library(OpenMP_LIBRARY
    NAMES omp
)
message(STATUS "OpenMP_LIBRARY: ${OpenMP_LIBRARY}")

# Create Main project
set(SOURCES
    main.cpp
)

set(HEADERS
)

add_executable(Main
    ${SOURCES}
    ${HEADERS}
)


find_package(OpenCV REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(C_CXX_FLAGS "-Wall -O2 -Xpreprocessor -fopenmp" )
else()
    set(C_CXX_FLAGS "-Wall -O2 -fopenmp" )
endif()

set(CXX_FLAGS "-std=c++17 ${C_CXX_FLAGS}" )
set(C_FLAGS "-std=c11 ${C_CXX_FLAGS}" )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

target_link_directories( Main PUBLIC
    /usr/local/lib/
    /usr/local/Cellar/libomp/11.0.0/lib/
)

target_include_directories(Main PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    /usr/local/Cellar/libomp/11.0.0/include/
    )

    target_link_libraries(Main 
        ${OpenCV_LIBS}
        omp)
