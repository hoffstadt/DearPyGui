cmake_minimum_required (VERSION 3.13)

include("cmake_scripts/dpg_sources.cmake")

# For distribution
if(MVDIST_ONLY)

	add_library(core SHARED)

	target_sources(core PRIVATE ${MARVEL_SOURCES} )

	target_compile_definitions(core

		PRIVATE
			$<$<CONFIG:Release>:MV_RELEASE>
	)

	if(WIN32)
		include("cmake_scripts/windows_runner.cmake")
	elseif(APPLE)
		include("cmake_scripts/mac_runner.cmake")
	else() # Linux
		include("cmake_scripts/linux_runner.cmake")
	endif()

# For developing
else()
	include("cmake_scripts/embedded.cmake")
endif()
