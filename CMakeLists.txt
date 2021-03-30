cmake_minimum_required (VERSION 3.13)

project ("DearPyGui")

# various settings
set(GLFW_BUILD_DOCS OFF)
add_definitions(-D_USE_MATH_DEFINES -DIMGUI_USER_CONFIG="mvImGuiConfig.h")

# can be set to 36, 37, 38, 39, or 0
#   * 36 = Python 3.6
#   * 37 = Python 3.7
#   * 38 = Python 3.8
#   * 39 = Python 3.9
#   *  0 = Embedded Python
set(MVPY_VERSION ${MVPY_VERSION})
if(MVDPG_VERSION)
    add_definitions(-DMV_SANDBOX_VERSION="${MVDPG_VERSION}")
else()
    add_definitions(-DMV_SANDBOX_VERSION="master")
endif()

# when set to "ON", the C++ version will be
# build instead of the Python version
set(MV_CPP ${MV_CPP})
if(MV_CPP)
    add_definitions(-DMV_CPP)
    add_subdirectory ("DearSandboxCpp")
endif()

# when set to "ON", the embedded version
# will be the only version build (no sandbox builds)
set(MVDIST_ONLY ${MVDIST_ONLY})

if(MVDIST_ONLY)
    add_definitions(-DMVDIST_ONLY)
endif()

if(NOT MVDIST_ONLY)
    add_subdirectory ("ImguiTesting")
    add_definitions(-DMV_LOG)
endif()

# version number of Dear PyGui, usually set
# by AppVeyor
set(MVDPG_VERSION ${MVDPG_VERSION})

# if this is not a distribution build
# build development environment
if(NOT MVDIST_ONLY AND NOT MV_CPP)
    add_subdirectory("Dependencies/spdlog")
    add_subdirectory ("DearSandbox")
endif()

if(WIN32)
    # do nothing
else() # Apple and Linux
	add_subdirectory ("Dependencies/glfw")
endif()

add_subdirectory("Dependencies/freetype")
add_subdirectory ("DearPyGui")
