add_library(implot
implot.cpp
implot_demo.cpp
implot_items.cpp
)

target_link_libraries(implot PRIVATE
imgui)
target_include_directories(implot PUBLIC
${CMAKE_CURRENT_LIST_DIR}
${CMAKE_CURRENT_LIST_DIR}/..
)
