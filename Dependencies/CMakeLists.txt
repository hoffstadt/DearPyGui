
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)

if(WIN32)
    add_subdirectory("freetype")
elseif(APPLE)
    add_subdirectory("freetype")
    find_package("GLFW")
else() #Linux
    find_package("GLFW")
endif()
