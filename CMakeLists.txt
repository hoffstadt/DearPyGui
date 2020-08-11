cmake_minimum_required (VERSION 3.16)

project ("DearPyGui")

set(CONFIG_32BIT ${CONFIG_32BIT})
set(MVDIST_ONLY ${MVDIST_ONLY})

add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DMV_SANDBOX_VERSION="0.1.0b1")

# Include sub-projects.
add_subdirectory ("DearPyGui")

if(NOT MVDIST_ONLY)
    add_subdirectory ("DearSandbox")
    add_subdirectory ("ImguiTesting")
endif()

if(WIN32)
    # do nothing
else() # Apple or Linux
	add_subdirectory ("Dependencies/glfw")
endif()

