cmake_minimum_required (VERSION 3.13)

project ("DearPyGui")

if(WIN32)
    add_definitions(-DIMGUI_USER_CONFIG="mvImGuiConfig.h")
elseif(APPLE)
    add_definitions(-DIMGUI_USER_CONFIG="mvImGuiConfig.h")
else() # Linux
	add_definitions(-DIMGUI_USER_CONFIG="mvImGuiLinuxConfig.h")
endif()

# various settings
add_definitions(
    -D_CRT_SECURE_NO_WARNINGS 
    -D_USE_MATH_DEFINES)

# can be set to 36, 37, 38, 39, or 0
#   * 36 = Python 3.6
#   * 37 = Python 3.7
#   * 38 = Python 3.8
#   * 39 = Python 3.9
#   * 40 = Python 3.7 Raspberry Pi
#   *  0 = Embedded Python
set(MVPY_VERSION ${MVPY_VERSION})
if(MVDPG_VERSION)
    add_definitions(-DMV_SANDBOX_VERSION="${MVDPG_VERSION}")
else()
    add_definitions(-DMV_SANDBOX_VERSION="master")
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

# runs python tests
set(MV_TESTS_ONLY ${MV_TESTS_ONLY})
if(MV_TESTS_ONLY)
    add_definitions(-DMV_TESTS_ONLY)
endif()

# version number of Dear PyGui, usually set
# by AppVeyor
set(MVDPG_VERSION ${MVDPG_VERSION})

add_subdirectory("Dependencies")

# if this is not a distribution build
# build development environment
if(NOT MVDIST_ONLY)
    add_subdirectory ("DearSandbox")
endif()

add_subdirectory ("DearPyGui")
