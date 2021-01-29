cmake_minimum_required(VERSION 3.8)
project(libs)
set(CMAKE_CXX_STANDARD 17)

# Windows spezifisch
if (WIN32)
    #ws2_32 wsock32
    set(MSYS2_LIBS)
endif()

add_subdirectory(enet enet) # ENet 1.3.17 getestet

# Ogre3D einrichten
set(OGRE_VER "1.12.10")
set(OGRE_BUILD_COMPONENT_OVERLAY TRUE)
set(OGRE_BUILD_COMPONENT_OVERLAY_IMGUI TRUE)
set(OGRE_BUILD_COMPONENT_PYTHON FALSE)
set(OGRE_BUILD_COMPONENT_JAVA FALSE)
set(OGRE_BUILD_COMPONENT_CSHARP FALSE)
set(OGRE_BUILD_SAMPLES FALSE)
set(OGRE_NODELESS_POSITIONING FALSE)
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/ogre-${OGRE_VER}/CMakeLists.txt")
    message(STATUS "Ogre3D gefunden")
else()
    message(STATUS "Ogre3D wird entpackt...")
    execute_process(
            COMMAND ${UNZIP_CMD} ogre-${OGRE_VER}.zip
            WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
endif()
add_subdirectory(ogre-${OGRE_VER} ogre3d)

# Bibliotheken Bauen
add_library(libs STATIC
        implot/implot.cpp
        implot/implot_items.cpp
        )

target_compile_definitions(libs PUBLIC
        #IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL_opengl.h>
        #GL_GLEXT_PROTOTYPES=1
        #SDL_MAIN_HANDLED=1
        )

target_include_directories(libs SYSTEM PUBLIC
        ${PROJECT_BINARY_DIR}/libs/ogre3d/imgui-1.79
        ${PROJECT_BINARY_DIR}/libs/ogre3d/include
        ${CMAKE_CURRENT_LIST_DIR}/ogre-${OGRE_VER}/OgreMain/include
        ${CMAKE_CURRENT_LIST_DIR}/implot
        ${CMAKE_CURRENT_LIST_DIR}/enet/include
        )

target_link_libraries(libs
        enet
        OgreBites
        OgreRTShaderSystem
        ${MSYS2_LIBS}
        )
