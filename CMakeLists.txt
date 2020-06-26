cmake_minimum_required (VERSION 3.16)

project ("MarvelSandbox")

add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DMV_MAIN_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

# Include sub-projects.
add_subdirectory ("PureImgui")
add_subdirectory ("Marvel")
add_subdirectory ("MarvelSandbox")