cmake_minimum_required (VERSION 3.13)

add_library(coreemb STATIC)
set_target_properties(coreemb PROPERTIES CXX_STANDARD 17)

target_sources(coreemb PRIVATE ${MARVEL_SOURCES} )
target_include_directories(coreemb PRIVATE ${MARVEL_INCLUDE_DIR})

target_compile_definitions(coreemb
	PUBLIC
		$<$<CONFIG:Release>:MV_RELEASE>
)

if(WIN32)

	add_definitions(-DWIN32)
	target_link_directories(coreemb PRIVATE "../Dependencies/cpython/PCbuild/amd64/")
	target_link_libraries(coreemb PUBLIC $<$<PLATFORM_ID:Windows>:d3d11> $<$<CONFIG:Debug>:python38_d> $<$<CONFIG:Release>:python38>)

elseif(APPLE)

	add_definitions(-DAPPLE)
	add_definitions(-DUNIX)
	#find_package(Python3 COMPONENTS Interpreter Development)

	#target_link_directories(coreemb PRIVATE /Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/)

	target_link_directories(coreemb PRIVATE ../Dependencies/cpython/debug)
	
	SET_PROPERTY(TARGET coreemb APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc -Wunused-function -Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -arch x86_64")
	target_link_libraries(coreemb

		PUBLIC

			glfw
			"-framework Metal"
			"-framework MetalKit"
			"-framework Cocoa"
			"-framework CoreVideo"
			"-framework IOKit"
			"-framework QuartzCore"
			"-framework CoreFoundation"
	)

else() # Linux

	add_definitions(-DLINUX)
	add_definitions(-DUNIX)
	add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)

	set_property(TARGET coreemb APPEND_STRING PROPERTY COMPILE_FLAGS "-fPIC -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall")
	target_link_libraries(coreemb PRIVATE "-lcrypt -lpthread -ldl -lutil -lm" GL glfw python3.8d)
	target_link_directories(coreemb PRIVATE ../Dependencies/cpython/debug)

endif()
