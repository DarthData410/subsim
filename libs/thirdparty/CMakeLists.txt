cmake_minimum_required(VERSION 3.8)
project(thirdparty)
set(CMAKE_CXX_STANDARD 17)

set(THIRDPARTY_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/selbaward
                         ${CMAKE_CURRENT_LIST_DIR}/simplex_noise/include)
set(THIRDPARTY_INCLUDE_DIRS ${THIRDPARTY_INCLUDE_DIRS} PARENT_SCOPE)

add_library(thirdparty SHARED
        selbaward/selbaward/Ring.cpp
        simplex_noise/src/SimplexNoise.cpp
        )

target_include_directories(thirdparty PRIVATE ${THIRDPARTY_INCLUDE_DIRS})
