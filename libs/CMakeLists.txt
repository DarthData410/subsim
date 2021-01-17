cmake_minimum_required(VERSION 3.8)

project(libs)

# Windows spezifisch
if (WIN32)
    # TODO doc
    # mingw64/mingw-w64-x86_64-mesa
    #opengl32 glew32 freeglut glu32
    #osmesa
    #find_package(GLEW REQUIRED)
    #set(MSYS2_LIBS SDL2 osmesa)
endif()

#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#find_package(OpenGL REQUIRED)
#find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)

# Ogre3D einrichten
set(OGRE_VER "1.12.10")
set(OGRE_BUILD_COMPONENT_OVERLAY TRUE)
set(OGRE_BUILD_COMPONENT_OVERLAY_IMGUI TRUE)
set(OGRE_BUILD_COMPONENT_PYTHON FALSE)
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
        #${IMGUI_VER}/imgui.cpp
        #${IMGUI_VER}/imgui_widgets.cpp
        #${IMGUI_VER}/imgui_draw.cpp
        #${IMGUI_VER}/examples/imgui_impl_sdl.cpp
        #${IMGUI_VER}/examples/imgui_impl_opengl3.cpp
        implot/implot.cpp
        implot/implot_items.cpp
        )

target_compile_definitions(libs PUBLIC
        #IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL_opengl.h>
        #GL_GLEXT_PROTOTYPES=1
        #SDL_MAIN_HANDLED=1
        )

target_include_directories(libs SYSTEM PUBLIC
        #${SDL2_INCLUDE_DIRS}
        #${SDL2_IMAGE_INCLUDE_DIRS}
        #${OpenGL_INCLUDE_DIRS}
        ${PROJECT_BINARY_DIR}/libs/implot
        ${PROJECT_BINARY_DIR}/libs/ogre3d/imgui-1.79
        ${PROJECT_BINARY_DIR}/libs/ogre3d/include
        ${CMAKE_CURRENT_LIST_DIR}/ogre-${OGRE_VER}/OgreMain/include
        #${CMAKE_CURRENT_LIST_DIR}/${IMGUI_VER}
        )

target_link_libraries(libs
        ${MSYS2_LIBS}
        OgreBites
        OgreRTShaderSystem
        #${SDL2_LIBRARIES}
        #${SDL2_IMAGE_LIBRARIES}
        #${OPENGL_LIBRARIES}
        )
