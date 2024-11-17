#libs/imgui

add_library(imgui
imgui.cpp
imgui_demo.cpp
imgui_draw.cpp
imgui_tables.cpp
imgui_widgets.cpp
backends/imgui_impl_glfw.cpp
backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
${CMAKE_CURRENT_LIST_DIR} # for extenstions and implot to be able to use #include "implot.h"
${CMAKE_CURRENT_LIST_DIR}/backends # for main.cpp to include implementatinos straight up
${CMAKE_CURRENT_LIST_DIR}/..
)
target_link_libraries(imgui OpenGL::GL glfw)