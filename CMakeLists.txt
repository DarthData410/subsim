cmake_minimum_required(VERSION 3.8)
project(nada_sub)
set(CMAKE_CXX_STANDARD 17)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/libs/reactphysics3d/CMakeLists.txt")
    message(STATUS "reactphysics3d gefunden")
else()
    message(STATUS "reactphysics3d wird entpackt")
    execute_process(
            COMMAND unzip reactphysics3d-0.8.0.zip
            WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/libs"
    )
endif()

add_subdirectory(libs/reactphysics3d reactphysics3d)

include_directories(SYSTEM libs/reactphysics3d/include)

add_executable(nada_sub src/main.cpp)

target_link_libraries(nada_sub reactphysics3d)