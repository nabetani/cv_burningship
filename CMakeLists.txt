cmake_minimum_required(VERSION 3.18)

project(Main)

set(CMAKE_CXX_STANDARD 17)

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

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(C_CXX_FLAGS "-Wall -O2 -Xpreprocessor -fopenmp" )
else()
    set(C_CXX_FLAGS "-Wall -O2 -fopenmp" )
endif()

set(CXX_FLAGS "${C_CXX_FLAGS}" )
set(C_FLAGS "${C_CXX_FLAGS}" )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

set_property(TARGET Main PROPERTY CXX_STANDARD 17) 

set_target_properties(Main
    PROPERTIES
        C_STANDARD 11
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
target_include_directories(Main PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    /usr/local/Cellar/libomp/11.0.0/include/
)

target_link_directories(Main PUBLIC
    /usr/local/Cellar/libomp/11.0.0/lib/
)

target_link_libraries(Main 
    ${OpenCV_LIBS}
    omp)
