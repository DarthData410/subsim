# CMake Datei zur Erzeugung von ImGui als Library mit SFML im Backend.
cmake_minimum_required(VERSION 3.9)

project(bibliotheken)

# TODO cmake version von download_libraries.sh
#[[function(download_and_extract NAME VERSION URL)
    if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/${NAME})
        set(ZIP_FILE ${CMAKE_CURRENT_LIST_DIR}/${NAME}.zip)
        file(DOWNLOAD ${URL} ${ZIP_FILE} SHOW_PROGRESS)
        file(ARCHIVE_EXTRACT INPUT ${ZIP_FILE} DESTINATION ${CMAKE_CURRENT_LIST_DIR})
        file(RENAME ${CMAKE_CURRENT_LIST_DIR}/${NAME}-${VERSION} ${CMAKE_CURRENT_LIST_DIR}/${NAME})
    endif()
endfunction(download_and_extract)

download_and_extract(cereal 1.3.2 https://github.com/USCiLab/cereal/archive/refs/tags/v1.3.2.zip)
download_and_extract(doctest 2.4.7 https://github.com/doctest/doctest/archive/refs/tags/2.4.7.zip)
download_and_extract(imgui 1.87 https://github.com/ocornut/imgui/archive/refs/tags/v1.87.zip)
download_and_extract(imgui-sfml 2.5 https://github.com/eliasdaler/imgui-sfml/archive/refs/tags/v2.5.zip)
download_and_extract(implot 0.13 https://github.com/epezent/implot/archive/refs/tags/v0.13.zip)
download_and_extract(enet 1.3.17 https://github.com/lsalzman/enet/archive/refs/tags/v1.3.17.zip)]]

include_directories(${SFML_DIR}/include)
include_directories(enet/include)
include_directories(imgui)
include_directories(implot)
include_directories(imgui-sfml)
include_directories(libnada/include)

add_library(bibliotheken STATIC
        imgui/imgui.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/misc/cpp/imgui_stdlib.cpp
        imgui-sfml/imgui-SFML.cpp
        implot/implot.cpp
        implot/implot_items.cpp
        )

target_include_directories(bibliotheken SYSTEM PUBLIC
        ${SFML_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/imgui
        ${CMAKE_CURRENT_LIST_DIR}/imgui-sfml
        ${CMAKE_CURRENT_LIST_DIR}/implot
        ${CMAKE_CURRENT_LIST_DIR}/enet/include
        ${CMAKE_CURRENT_LIST_DIR}/doctest/doctest
        ${CMAKE_CURRENT_LIST_DIR}/cereal/include
        ${CMAKE_CURRENT_LIST_DIR}/libnada/include
        )

# Ueber Build Modus informieren
add_custom_command(TARGET bibliotheken PRE_BUILD COMMAND echo CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
