#src
add_executable(gpxSpeedEditor
main.cpp
app/app.cpp
app/gpxadapter.cpp
app/gpxspeedmodifier.cpp
app/toolkit.cpp
)
target_link_libraries(gpxSpeedEditor PRIVATE
imgui
implot
pugixml::pugixml
OpenGL::GL
glfw)
target_include_directories(gpxSpeedEditor PRIVATE
${CMAKE_SOURCE_DIR}/libs)

if(WIN32)
    set_target_properties(gpxSpeedEditor
    PROPERTIES
        LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
        LINK_FLAGS_RELEASE "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
        LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
        LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
    )
endif(WIN32)
if(UNIX)
    target_link_libraries(gpxSpeedEditor PRIVATE
    dl)
endif(UNIX)