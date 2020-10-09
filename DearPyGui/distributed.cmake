cmake_minimum_required (VERSION 3.13)

include("dpg_sources.cmake")

add_library(core SHARED)

target_sources(core PRIVATE ${MARVEL_SOURCES} )

target_compile_definitions(core

	PRIVATE
		$<$<CONFIG:Release>:MV_RELEASE>
)

if(WIN32)

	if(MVPY_VERSION EQUAL 36)
		target_include_directories(core 
			PRIVATE 
				${MARVEL_INCLUDE_DIR}
				"C:/Python36-x64"
				"C:/Python36-x64/include"
		)

		target_link_directories(core 
			PRIVATE 
				"C:/Python36-x64"
				"C:/Python36-x64/libs"
				"C:/Python36-x64/DLLs"
		)

	elseif(MVPY_VERSION EQUAL 37)
		target_include_directories(core 
			PRIVATE 
				${MARVEL_INCLUDE_DIR}
				"C:/Python37-x64"
				"C:/Python37-x64/include"
		)

		target_link_directories(core 
			PRIVATE 
				"C:/Python37-x64"
				"C:/Python37-x64/libs"
				"C:/Python37-x64/DLLs"
		)

	elseif(MVPY_VERSION EQUAL 38)

		target_include_directories(core 
			PRIVATE 
				${MARVEL_INCLUDE_DIR}
				"C:/Python38-x64"
				"C:/Python38-x64/include"
		)

		target_link_directories(core 
			PRIVATE 
				"C:/Python38-x64"
				"C:/Python38-x64/libs"
				"C:/Python38-x64/DLLs"
			)
	endif()

	add_definitions(-DWIN32)
	set_target_properties(core PROPERTIES SUFFIX ".pyd")
	set_target_properties(core PROPERTIES CXX_STANDARD 17)

	if(MVPY_VERSION EQUAL 37)
		target_link_libraries(core PUBLIC d3d11 python37)
	elseif(MVPY_VERSION EQUAL 38)
		target_link_libraries(core PUBLIC d3d11 python38)
	endif()

elseif(APPLE)


	target_include_directories(core PRIVATE ${MARVEL_INCLUDE_DIR})
	add_definitions(-DAPPLE)
	add_definitions(-DUNIX)
	find_package(Python3 COMPONENTS Interpreter Development)
	set_target_properties(core PROPERTIES SUFFIX ".so")
	set_target_properties(core PROPERTIES PREFIX "")
	set_target_properties(core PROPERTIES CXX_STANDARD 17)

	target_link_directories(core PRIVATE ${PYTHON_LIBRARIES})
	
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
	endif()

else() # Linux

	#target_include_directories(core PRIVATE ${MARVEL_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})
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
endif()
