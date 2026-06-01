cmake_minimum_required(VERSION 3.10.0)
project(ShitEngine VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)

set(IMGUI_SOURCES
   src/dearimgui/imgui_demo.cpp
   src/dearimgui/imgui_draw.cpp
   src/dearimgui/imgui_tables.cpp
   src/dearimgui/imgui_widgets.cpp
   src/dearimgui/imgui.cpp
   include/dearimgui/backends/imgui_impl_sdl3.cpp
   include/dearimgui/backends/imgui_impl_opengl3.cpp
)
# lib
add_library(sh_engine
   ${IMGUI_SOURCES}
   # engine
   src/engine/sh_engine.cpp
   src/engine/graphics.cpp
   src/engine/utiles.cpp
   src/glad.c
)
set_target_properties(sh_engine PROPERTIES
   ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/shitengine/lib
)
target_include_directories(sh_engine PRIVATE
   include
   include/engine
   include/dearimgui
   src
)
target_link_libraries(sh_engine PRIVATE SDL3::SDL3)

# Copy engine headers
add_custom_command(
   TARGET sh_engine POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_CURRENT_SOURCE_DIR}/include/engine"
      "${CMAKE_CURRENT_BINARY_DIR}/shitengine/include/engine"
   COMMENT "Copying engine headers to build directory"
)