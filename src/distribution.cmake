
find_package (Python ${MV_PY_VERSION} EXACT COMPONENTS Development)
if(NOT Python_Development_FOUND)
	message(FATAL_ERROR "The python3 development library from your distribution repo need to be installed first!")
endif()

add_library(_dearpygui SHARED)

set_target_properties(_dearpygui
  PROPERTIES
  CXX_STANDARD 17
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/cmake-build-local/DearPyGui/"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/cmake-build-local/DearPyGui/"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/cmake-build-local/DearPyGui/"
  )

target_sources(_dearpygui PRIVATE ${MARVEL_SOURCES})

target_include_directories(_dearpygui 
		PRIVATE 
			${Python_INCLUDE_DIRS}
			${MARVEL_INCLUDE_DIR}
	)

target_link_directories(_dearpygui 
		PRIVATE 
			"$<$<PLATFORM_ID:Linux>:/usr/lib>"
			${Python_LIBRARY_DIRS}
	)

target_compile_definitions(_dearpygui

	PRIVATE
		$<$<CONFIG:Release>:MV_RELEASE>
)

add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)

if(WIN32)

	set_target_properties(_dearpygui PROPERTIES SUFFIX ".pyd")
	set_target_properties(_dearpygui PROPERTIES PREFIX "")
	target_link_libraries(_dearpygui PUBLIC d3d11 dxgi dwmapi ${Python_LIBRARIES} freetype)

elseif(APPLE)

	add_definitions(-DAPPLE)
	set_target_properties(_dearpygui PROPERTIES SUFFIX ".so")
	set_target_properties(_dearpygui PROPERTIES PREFIX "")
	SET_PROPERTY(TARGET _dearpygui APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc -fno-common -dynamic -DNDEBUG -fwrapv -O3")
	target_link_libraries(_dearpygui

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
	set_target_properties(_dearpygui PROPERTIES PREFIX "")
	set_property(TARGET _dearpygui APPEND_STRING PROPERTY COMPILE_FLAGS "-fPIC -fwrapv -O3")
	add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)
	target_link_libraries(_dearpygui 
		PRIVATE 
			"-fPIC -lcrypt -lpthread -ldl  -lutil -lm"
			GL
			glfw
	)

endif()
