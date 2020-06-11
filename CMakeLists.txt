cmake_minimum_required (VERSION 3.16)

project ("MarvelSandbox")

add_definitions(-D_USE_MATH_DEFINES)

# Include sub-projects.
add_subdirectory ("PureImgui")
add_subdirectory ("MarvelSandbox")
