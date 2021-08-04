
set(GLFW_BUILD_DOCS OFF)

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
