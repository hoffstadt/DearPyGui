cmake_minimum_required (VERSION 3.13)

project ("DearPyGui")

if(WIN32)
    add_definitions(-DIMGUI_USER_CONFIG="mvImGuiConfig.h")
    add_definitions(-DMV_PLATFORM="windows")
elseif(APPLE)
    add_definitions(-DIMGUI_USER_CONFIG="mvImGuiConfig.h")
    add_definitions(-DMV_PLATFORM="apple")
else() # Linux
	add_definitions(-DIMGUI_USER_CONFIG="mvImGuiLinuxConfig.h")
    add_definitions(-DMV_PLATFORM="linux")
endif()

# various settings
add_definitions(
    -D_CRT_SECURE_NO_WARNINGS 
    -D_USE_MATH_DEFINES)

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
else()
    add_subdirectory ("ImguiTesting")
    add_definitions(-DMV_LOG)
endif()

# generate temp files
set(MV_GEN_FILES_ONLY ${MV_GEN_FILES_ONLY})
if(MV_GEN_FILES_ONLY)
    add_definitions(-DMV_GEN_FILES_ONLY)
endif()

# runs python tests
set(MV_TESTS_ONLY ${MV_TESTS_ONLY})
if(MV_TESTS_ONLY)
    add_definitions(-DMV_TESTS_ONLY)
endif()

# version number of Dear PyGui, usually set
set(MVDPG_VERSION ${MVDPG_VERSION})

add_subdirectory("Dependencies")

# if this is not a distribution build
# build development environment
if(NOT MVDIST_ONLY)
    add_subdirectory ("DearSandbox")
endif()

add_subdirectory ("DearPyGui")
