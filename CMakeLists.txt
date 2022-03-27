cmake_minimum_required(VERSION 3.8)
project(nada_sub)
set(CMAKE_CXX_STANDARD 17)

# Build Config
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(NADA_CXX_FLAGS ${CMAKE_CXX_FLAGS} -O2 -s -m64 -march=athlon64-sse3 -mtune=generic)
    add_subdirectory(libs)
else () # Debug build
    message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "-g -w")
    add_subdirectory(libs)
    set(NADA_CXX_FLAGS -g -Werror -Wall -Wextra)
endif ()
add_subdirectory(libs/libnada)
add_subdirectory(libs/enet)

find_package(SFML 2 REQUIRED graphics window system) # audio network

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/)
file(GLOB_RECURSE QUELLTEXT src/*.hpp src/*cpp)
add_executable(nada_sub ${QUELLTEXT})

if (UNIX)
    set(OS_BIBLIOTHEKEN OpenGL GL pthread stdc++fs)
elseif (WIN32)
    find_package(OpenGL REQUIRED)
    set(OS_BIBLIOTHEKEN ${OPENGL_LIBRARIES} stdc++fs ws2_32 winmm)
endif()

target_link_libraries(nada_sub PRIVATE
        nada
        enet
        bibliotheken
        sfml-graphics sfml-window sfml-system
        ${OS_BIBLIOTHEKEN}
        )

target_compile_options(nada_sub PRIVATE ${NADA_CXX_FLAGS})
