cmake_minimum_required (VERSION 3.13)

include("cmake/dpg_sources.cmake")

# For distribution
if(MVDIST_ONLY)

	add_library(_dearpygui SHARED)

	target_sources(_dearpygui PRIVATE ${MARVEL_SOURCES})

	target_compile_definitions(_dearpygui

		PRIVATE
			$<$<CONFIG:Release>:MV_RELEASE>
	)

	if(WIN32)
		include("cmake/windows_runner.cmake")
	elseif(APPLE)
		include("cmake/mac_runner.cmake")
	else() # Linux
		include("cmake/linux_runner.cmake")
	endif()

# For development
else()
	include("cmake/embedded.cmake")
endif()
