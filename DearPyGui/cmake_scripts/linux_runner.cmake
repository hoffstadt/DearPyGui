
add_definitions(-DLINUX)
add_definitions(-DUNIX)
set_target_properties(core PROPERTIES PREFIX "")
set_target_properties(core PROPERTIES CXX_STANDARD 17)

set_property(TARGET core APPEND_STRING PROPERTY COMPILE_FLAGS "-fPIC -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall")
	
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)

if(MVPY_VERSION EQUAL 36)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"/home/appveyor/.localpython3.6.12"
			"/home/appveyor/.localpython3.6.12/include/python3.6m"
	)

	target_link_directories(core 
		PRIVATE 
			"/usr/lib"
			"/home/appveyor/.localpython3.6.12/lib"
	)

	target_link_libraries(core 
		PRIVATE 
			"-fPIC -lcrypt -lpthread -ldl  -lutil -lm"
			GL
			glfw
			python3.6m
	)

elseif(MVPY_VERSION EQUAL 37)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"/home/appveyor/.localpython3.7.9"
			"/home/appveyor/.localpython3.7.9/include/python3.7m"
	)

	target_link_directories(core 
		PRIVATE 
			"/usr/lib"
			"/home/appveyor/.localpython3.7.9/lib"
	)

	target_link_libraries(core 
		PRIVATE 
			"-fPIC -lcrypt -lpthread -ldl  -lutil -lm"
			GL
			glfw
			python3.7m
	)

elseif(MVPY_VERSION EQUAL 38)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"/home/appveyor/.localpython3.8.5"
			"/home/appveyor/.localpython3.8.5/include/python3.8"
	)

	target_link_directories(core 
		PRIVATE 
			"/usr/lib"
			"/home/appveyor/.localpython3.8.5/lib"
	)

	target_link_libraries(core 
		PRIVATE 
			"-fPIC -lcrypt -lpthread -ldl  -lutil -lm"
			GL
			glfw
			python3.8
	)
endif()