cmake_minimum_required (VERSION 3.16)

project ("DearPyGui")

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(CONFIG_32BIT OFF)

add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DMV_SANDBOX_VERSION="0.1 alpha 20")

# Include sub-projects.
add_subdirectory ("ImguiTesting")
add_subdirectory ("DearPyGui")
add_subdirectory ("DearSandbox")
add_subdirectory ("Dependencies/json")
