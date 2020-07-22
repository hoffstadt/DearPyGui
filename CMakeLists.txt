cmake_minimum_required (VERSION 3.16)

project ("MarvelSandbox")

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DMV_MAIN_DIR="${CMAKE_CURRENT_SOURCE_DIR}/MarvelSandbox/")
add_definitions(-DMV_SANDBOX_VERSION="0.1 alpha 13")

# Include sub-projects.
add_subdirectory ("ImguiTesting")
add_subdirectory ("marvel")
add_subdirectory ("MarvelSandbox")
add_subdirectory ("Dependencies/json")
