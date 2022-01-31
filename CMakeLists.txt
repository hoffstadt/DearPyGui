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

if(WIN32)
else() # Linux
	add_definitions(-DCUSTOM_IMGUIFILEDIALOG_CONFIG="ImGuiFileDialogConfigUnix.h")
endif()

# various settings
add_definitions(
    -D_CRT_SECURE_NO_WARNINGS 
    -D_USE_MATH_DEFINES)

set(MV_PY_VERSION ${MV_PY_VERSION})
set(MVDPG_VERSION ${MVDPG_VERSION})
add_definitions(-DMV_DPG_MAJOR_VERSION=1)
add_definitions(-DMV_DPG_MINOR_VERSION=0)
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
    add_definitions(-DMV_LOG)
endif()

# runs python tests
set(MV_TESTS_ONLY ${MV_TESTS_ONLY})
if(MV_TESTS_ONLY)
    add_definitions(-DMV_TESTS_ONLY)
endif()

add_subdirectory("Dependencies")

# if this is not a distribution build
# build development environment
if(NOT MVDIST_ONLY)
    add_subdirectory ("DearSandbox")
endif()

add_subdirectory ("DearPyGui")
