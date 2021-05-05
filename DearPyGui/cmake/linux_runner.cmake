
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
			"/home/appveyor/.localpython3.6.13"
			"/home/appveyor/.localpython3.6.13/include/python3.6m"
	)

	target_link_directories(core 
		PRIVATE 
			"/usr/lib"
			"/home/appveyor/.localpython3.6.13/lib"
	)

	target_link_libraries(core 
		PRIVATE 
			"-fPIC -lcrypt -lpthread -ldl  -lutil -lm"
			GL
			freetype
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
			freetype
			glfw
			python3.7m
	)

elseif(MVPY_VERSION EQUAL 38)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"/home/appveyor/.localpython3.8.9"
			"/home/appveyor/.localpython3.8.9/include/python3.8"
	)

	target_link_directories(core 
		PRIVATE 
			"/usr/lib"
			"/home/appveyor/.localpython3.8.9/lib"
	)

	target_link_libraries(core 
		PRIVATE 
			"-fPIC -lcrypt -lpthread -ldl  -lutil -lm"
			GL
			freetype
			glfw
			python3.8
	)

elseif(MVPY_VERSION EQUAL 39)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"/home/appveyor/.localpython3.9.4"
			"/home/appveyor/.localpython3.9.4/include/python3.9"
	)

	target_link_directories(core 
		PRIVATE 
			"/usr/lib"
			"/home/appveyor/.localpython3.9.4/lib"
	)

	target_link_libraries(core 
		PRIVATE 
			"-fPIC -lcrypt -lpthread -ldl  -lutil -lm"
			GL
			glfw
			freetype
			python3.9
	)

elseif(MVPY_VERSION EQUAL 0)
	find_package (Python COMPONENTS Development)
	if(NOT Python_Development_FOUND)
		message(FATAL_ERROR "The python3 development library from your distribution repo need to be installed first!")
	endif()
	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			${Python_INCLUDE_DIRS}
	)

	target_link_directories(core 
		PRIVATE 
			"/usr/lib"
			${Python_LIBRARY_DIRS}
	)

	target_link_libraries(core 
		PRIVATE 
			"-fPIC -lcrypt -lpthread -ldl  -lutil -lm"
			GL
			glfw
			freetype
			${Python_LIBRARIES}
	)
endif()