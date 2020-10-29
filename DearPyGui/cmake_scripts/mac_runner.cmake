
add_definitions(-DAPPLE)
add_definitions(-DUNIX)
set_target_properties(core PROPERTIES SUFFIX ".so")
set_target_properties(core PROPERTIES PREFIX "")
set_target_properties(core PROPERTIES CXX_STANDARD 17)
	
SET_PROPERTY(TARGET core APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc -Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall")

target_link_libraries(core

	PRIVATE

		glfw
		"-undefined dynamic_lookup"
		"-framework Metal"
		"-framework MetalKit"
		"-framework Cocoa"
		"-framework CoreVideo"
		"-framework IOKit"
		"-framework QuartzCore"
)

if(MVPY_VERSION EQUAL 36)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"/Users/appveyor/.localpython3.6.10"
			"/Users/appveyor/.localpython3.6.10/include/python3.6m"
	)

	target_link_directories(core 
		PRIVATE 
			"/Users/appveyor/.localpython3.6.10/lib"
	)

elseif(MVPY_VERSION EQUAL 37)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"/Users/appveyor/.localpython3.7.7"
			"/Users/appveyor/.localpython3.7.7/include/python3.7m"
	)

	target_link_directories(core 
		PRIVATE 
			"/Users/appveyor/.localpython3.7.7/lib"
	)

elseif(MVPY_VERSION EQUAL 38)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"/Users/appveyor/.localpython3.8.3"
			"/Users/appveyor/.localpython3.8.3/include/python3.8"
	)

	target_link_directories(core 
		PRIVATE 
			"/Users/appveyor/.localpython3.8.3/lib"
	)

elseif(MVPY_VERSION EQUAL 39)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"/Users/appveyor/.localpython3.9.0b1"
			"/Users/appveyor/.localpython3.9.0b1/include/python3.9"
	)

	target_link_directories(core 
		PRIVATE 
			"/Users/appveyor/.localpython3.9.0/lib"
	)
endif()