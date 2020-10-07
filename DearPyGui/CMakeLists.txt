cmake_minimum_required (VERSION 3.13)

if(MVDIST_ONLY)
	include("distributed.cmake")
else()
	include("embedded.cmake")
endif()
