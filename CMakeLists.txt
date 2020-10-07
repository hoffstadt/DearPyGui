cmake_minimum_required (VERSION 3.13)

project ("DearPyGui")

# variables set by the command line
set(MVDIST_ONLY ${MVDIST_ONLY})
set(MV37DIST ${MV37DIST})

add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DMV_SANDBOX_VERSION="0.4.5")

if(MVDIST_ONLY)
    add_definitions(-DMVDIST_ONLY)
endif()

if(MV37DIST)
    add_definitions(-DMV_PYTHON_VERSION=37)
else()
    add_definitions(-DMV_PYTHON_VERSION=38)
endif()

# include sub-projects.
add_subdirectory ("DearPyGui")

# if this is not a distribution build
# build development environment
if(NOT MVDIST_ONLY)
    add_subdirectory ("DearSandbox")
    add_subdirectory ("ImguiTesting")
endif()

if(WIN32)
    # do nothing
else() # Apple or Linux
	add_subdirectory ("Dependencies/glfw")
endif()
