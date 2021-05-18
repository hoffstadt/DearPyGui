cmake_minimum_required (VERSION 3.13)

add_library(coreemb STATIC)
set_target_properties(coreemb PROPERTIES CXX_STANDARD 17)

target_include_directories(coreemb PRIVATE ${MARVEL_INCLUDE_DIR})

target_compile_definitions(coreemb
	PUBLIC
		$<$<CONFIG:Debug>:MV_DEBUG>
		$<$<CONFIG:Release>:MV_RELEASE>
)

# Add sources
target_sources(coreemb PRIVATE ${MARVEL_SOURCES})

###############################################################################
# Windows Specifics
###############################################################################
if(WIN32)

	add_definitions(-DWIN32)

	# tell cmake where to find the python3x dlls
	target_link_directories(coreemb PRIVATE "../Dependencies/cpython/PCbuild/amd64/")

	# Add libraries to link to
	target_link_libraries(coreemb PUBLIC d3d11 freetype $<$<CONFIG:Debug>:python39_d> $<$<CONFIG:Release>:python39>)
	
###############################################################################
# Apple Specifics
###############################################################################
elseif(APPLE)

	add_definitions(-DAPPLE)
	add_definitions(-DUNIX)

	# tell cmake where to find the python3x dlls
	target_link_directories(coreemb PRIVATE "../Dependencies/cpython/build/debug")
	
	SET_PROPERTY(TARGET coreemb APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc -Wunused-function -Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -arch x86_64")
	
	target_link_libraries(coreemb

		PUBLIC

			glfw
			freetype
			"-framework Metal"
			"-framework MetalKit"
			"-framework Cocoa"
			"-framework CoreVideo"
			"-framework IOKit"
			"-framework QuartzCore"
			"-framework CoreFoundation"
	)

###############################################################################
# Linux Specifics
###############################################################################
else() # Linux

	add_definitions(-DLINUX)
	add_definitions(-DUNIX)
	add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)

	# tell cmake where to find the python3x dlls
	target_link_directories(coreemb PRIVATE "../Dependencies/cpython/build/debug")

	set_property(TARGET coreemb APPEND_STRING PROPERTY COMPILE_FLAGS "-fPIC -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall")
	
	# Add libraries to link to
	target_link_libraries(coreemb PRIVATE "-lcrypt -lpthread -ldl -lutil -lm" GL glfw python3.9d freetype)

endif()
