cmake_minimum_required(VERSION 3.10.0)
project(emulator VERSION 0.1.0 LANGUAGES C CXX)

#raylib
add_subdirectory(include/raylib)

#raygui
set(RAYGUI_DIR ${CMAKE_SOURCE_DIR}/include/raygui/src)
include_directories(${RAYGUI_DIR})

#imgui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/include/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
    ${CMAKE_SOURCE_DIR}/include/ImGuiFileDialog/ImGuiFileDialog.cpp
    ${CMAKE_SOURCE_DIR}/include/ImGuiColorTextEdit/TextEditor.cpp

    #${IMGUI_DIR}/imgui_demo.cpp
)

#rlimgui
add_library(rlImGui
    include/rlImGui/rlImGui.cpp
    ${IMGUI_SOURCES}
)

target_include_directories(rlImGui PUBLIC
    include/rlImGui
    ${IMGUI_DIR}
    ${CMAKE_SOURCE_DIR}/include/ImGuiFileDialog
    ${CMAKE_SOURCE_DIR}/include/ImGuiColorTextEdit
)

target_link_libraries(rlImGui PUBLIC raylib)

file(GLOB SRC_FILES
    src/*.cpp
)

add_executable(main ${SRC_FILES})

target_include_directories(main PRIVATE
    ${RAYGUI_DIR}
)

target_link_libraries(main PRIVATE
    raylib
    rlImGui
)