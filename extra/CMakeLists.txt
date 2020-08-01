# Basic cmake script to build the library

cmake_minimum_required(VERSION 3.14)
project(shader_editor)

include_directories("${IMGUI_INCLUDE_DIR}")
include_directories("${GLFW_INCLUDE_DIR}")
include_directories("./src/")

find_package(Boost)
include_directories("${Boost_INCLUDE_DIRS}")

file(GLOB LibSources ./src/shader_core/*.cpp ./src/shader_graph/*.cpp ./src/shader_editor/*.cpp)
add_library(shader_editor STATIC ${LibSources})

file(GLOB HeadersCore ./src/shader_core/*.h)
file(GLOB HeadersGraph ./src/shader_graph/*.h)
file(GLOB HeadersEditor ./src/shader_editor/*.h)

install(TARGETS shader_editor LIBRARY DESTINATION ./lib)
install(FILES ${HeadersCore} DESTINATION ./include/shader_core)
install(FILES ${HeadersGraph} DESTINATION ./include/shader_graph)
install(FILES ${HeadersEditor} DESTINATION ./include/shader_editor)
