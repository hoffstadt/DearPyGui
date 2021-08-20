cmake_minimum_required (VERSION 3.13)

include("cmake/dpg_sources.cmake")

if(MVDIST_ONLY)
	include("cmake/distribution.cmake")
else()
	include("cmake/embedded.cmake")
endif()