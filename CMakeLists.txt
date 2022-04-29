cmake_minimum_required(VERSION 3.8)
project(nada_sub)
set(CMAKE_CXX_STANDARD 17)

# Build Config
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-O2 -s -m64 -march=athlon64-sse3 -mtune=generic") # evtl. -fimplicit-constexpr -ffastmath
    add_subdirectory(libs)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wpedantic")
else () # Debug build
    set(CMAKE_CXX_FLAGS "-g")
    add_subdirectory(libs)
    set(CMAKE_CXX_FLAGS "-g -Werror -Wall -Wextra -Wpedantic")
endif ()
add_subdirectory(libs/libnada)
add_subdirectory(libs/enet)
find_package(SFML 2 REQUIRED graphics window system) # audio network

if (UNIX)
    set(OS_BIBLIOTHEKEN OpenGL GL pthread stdc++fs)
elseif (WIN32)
    find_package(OpenGL REQUIRED)
    set(OS_BIBLIOTHEKEN ${OPENGL_LIBRARIES} stdc++fs ws2_32 winmm)
endif()
set(NADA_LINK_LIBRARIES nada bibliotheken sfml-graphics sfml-window sfml-system ${OS_BIBLIOTHEKEN} enet)

message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/)
file(GLOB_RECURSE QUELLTEXT src/*.hpp src/*cpp)
add_executable(nada_sub ${QUELLTEXT})
target_link_libraries(nada_sub PRIVATE ${NADA_LINK_LIBRARIES})
#target_compile_options(nada_sub PRIVATE ${CMAKE_CXX_FLAGS})

file(GLOB_RECURSE TESTS test/*.hpp)
add_executable(nada_sub_test ${QUELLTEXT} ${TESTS})
target_compile_definitions(nada_sub_test PRIVATE UNITTEST=1)
target_link_libraries(nada_sub_test PRIVATE ${NADA_LINK_LIBRARIES} gcov)
target_compile_options(nada_sub_test PRIVATE -coverage)
