cmake_minimum_required (VERSION 3.13)

project ("DearPyGui")

# variables set by the command line
set(MV_CPP ON)
#set(MV_CPP ${MV_CPP})
set(MVDIST_ONLY ${MVDIST_ONLY})
set(MV37DIST ${MV37DIST})
set(MVPY_VERSION ${MVPY_VERSION})
set(MVDPG_VERSION ${MVDPG_VERSION})
set(GLFW_BUILD_DOCS OFF)

add_definitions(-D_USE_MATH_DEFINES)

if(MVDPG_VERSION)
    add_definitions(-DMV_SANDBOX_VERSION="${MVDPG_VERSION}")
else()
    add_definitions(-DMV_SANDBOX_VERSION="master")
endif()

if(MVDIST_ONLY)
    add_definitions(-DMVDIST_ONLY)
endif()

if(MV_CPP)
    add_definitions(-DMV_CPP)
    add_subdirectory ("DearSandboxCpp")
endif()

# include sub-projects.
add_subdirectory ("DearPyGui")

# if this is not a distribution build
# build development environment
if(NOT MVDIST_ONLY AND NOT MV_CPP)
    add_subdirectory ("DearSandbox")
endif()

if(NOT MVDIST_ONLY)
    add_subdirectory ("ImguiTesting")
endif()

if(WIN32)
    # do nothing
else() # Apple or Linux
	add_subdirectory ("Dependencies/glfw")
endif()
