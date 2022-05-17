cmake_minimum_required(VERSION 3.8)
project(nada)
set(CMAKE_CXX_STANDARD 17)

set(LIBNADA_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)
set(LIBNADA_INCLUDE_DIRS ${LIBNADA_INCLUDE_DIRS} PARENT_SCOPE)

add_library(nada SHARED
        src/misc.cpp
        src/random.cpp
        )

target_include_directories(nada
        PRIVATE ${LIBNADA_INCLUDE_DIRS}
        )
