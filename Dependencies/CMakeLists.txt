
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)

if(WIN32)
    add_subdirectory("freetype")
elseif(APPLE)
    add_definitions("-DCMAKE_DISABLE_FIND_PACKAGE_HarfBuzz=TRUE")
    add_definitions("-DCMAKE_DISABLE_FIND_PACKAGE_BZip2=TRUE")
    add_definitions("-DCMAKE_DISABLE_FIND_PACKAGE_PNG=TRUE")
    add_definitions("-DCMAKE_DISABLE_FIND_PACKAGE_ZLIB=TRUE")
    add_subdirectory("freetype")
    add_subdirectory ("glfw")
else() #Linux
	add_subdirectory ("glfw")
endif()
