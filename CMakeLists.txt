cmake_minimum_required(VERSION 3.8)
project(nada_sub)
set(CMAKE_CXX_STANDARD 17)

# Build Config
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s -m64 -march=athlon64-sse3 -mtune=generic")
    if (UNIX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
    endif ()
    set(NADA_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    add_subdirectory(libs)
else () # Debug build
    message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "-g -w")
    add_subdirectory(libs)
    set(NADA_CXX_FLAGS -g -Werror -Wall -Wextra)
endif ()
add_subdirectory(libs/libnada)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/)
file(GLOB_RECURSE QUELLTEXT src/*.hpp src/*cpp)
add_executable(nada_sub ${QUELLTEXT})
target_include_directories(nada_sub PRIVATE
        ${LIBNADA_INCLUDE_DIRS}
        )
target_link_libraries(nada_sub
        nada
        libs
        )
target_compile_options(nada_sub PRIVATE ${NADA_CXX_FLAGS})
