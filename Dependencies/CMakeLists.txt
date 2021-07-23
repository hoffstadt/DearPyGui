
set(GLFW_BUILD_DOCS OFF)


if(WIN32)
    add_definitions(-DIMGUI_USER_CONFIG="mvImGuiConfig.h")
elseif(APPLE)
    add_definitions(-DIMGUI_USER_CONFIG="mvImGuiConfig.h")
else() # Linux
	add_definitions(-DIMGUI_USER_CONFIG="mvImGuiLinuxConfig.h")
endif()

if(NOT MVDIST_ONLY)
    add_subdirectory("spdlog")
endif()

if(WIN32)
    add_subdirectory("freetype")
elseif(APPLE)
    add_subdirectory("freetype")
    add_subdirectory ("glfw")
else() #Linux
	add_subdirectory ("glfw")
endif()

