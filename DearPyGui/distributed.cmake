cmake_minimum_required (VERSION 3.13)

include("dpg_sources.cmake")

add_library(core SHARED)

target_sources(core PRIVATE ${MARVEL_SOURCES} )
target_include_directories(core PRIVATE ${MARVEL_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})

target_compile_definitions(core

	PRIVATE
		$<$<CONFIG:Release>:MV_RELEASE>
)

if(WIN32)

	add_definitions(-DWIN32)
	set_target_properties(core PROPERTIES SUFFIX ".pyd")
	set_target_properties(core PROPERTIES OUTPUT_NAME "core$<$<CONFIG:Debug>:_d>")
	set_target_properties(core PROPERTIES CXX_STANDARD 17)

	target_link_directories(core PRIVATE "../Dependencies/cpython/PCbuild/amd64/")

	if(MV37DIST)
		target_link_libraries(core PUBLIC $<$<PLATFORM_ID:Windows>:d3d11> $<$<CONFIG:Debug>:python37_d> $<$<CONFIG:Release>:python37>)
	else()
		target_link_libraries(core PUBLIC $<$<PLATFORM_ID:Windows>:d3d11> $<$<CONFIG:Debug>:python38_d> $<$<CONFIG:Release>:python38>)
	endif()

elseif(APPLE)

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

else() # Linux

	add_definitions(-DLINUX)
	add_definitions(-DUNIX)
	set_target_properties(core PROPERTIES PREFIX "")
	set_target_properties(core PROPERTIES CXX_STANDARD 17)

	set_property(TARGET core APPEND_STRING PROPERTY COMPILE_FLAGS "-fPIC -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall")
	
	add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)

	if(MV37DIST)
		target_link_directories(core PRIVATE "/home/runner/work/DearPyGui/DearPyGui/Dependencies/cpython/debug/lib")
		target_link_libraries(core PUBLIC "-L/usr/lib  -lcrypt -lpthread -ldl  -lutil -lm -lm" GL glfw python3.7m)
	else()
		target_link_directories(core PRIVATE "/home/runner/work/DearPyGui/DearPyGui/Dependencies/cpython/debug/lib")
		target_link_libraries(core PUBLIC "-L/usr/lib  -lcrypt -lpthread -ldl  -lutil -lm -lm" GL glfw python3.8)
	endif()

endif()
