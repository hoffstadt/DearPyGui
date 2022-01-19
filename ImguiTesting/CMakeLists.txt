cmake_minimum_required (VERSION 3.13)

add_executable (PureImgui)

target_sources(PureImgui

	PRIVATE

		"$<$<PLATFORM_ID:Linux>:main.cpp>"
		"$<$<PLATFORM_ID:Windows>:main.cpp>"
		"$<$<PLATFORM_ID:Darwin>:main.mm>"
		"$<$<PLATFORM_ID:Darwin>:src/mvAppleWindow.mm>"
		"$<$<PLATFORM_ID:Windows>:src/mvWindowsWindow.cpp>"
		"$<$<PLATFORM_ID:Linux>:src/mvLinuxWindow.cpp>"

		# imnodes
		"../DearPyGui/vendor/imnodes/imnodes.cpp"

		# implot
		"../DearPyGui/vendor/implot/implot.cpp"
		"../DearPyGui/vendor/implot/implot_items.cpp"
		"../DearPyGui/vendor/implot/implot_demo.cpp"

		# imgui
		"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/examples/libs/gl3w/GL/gl3w.c>"
)



if(APPLE)

	SET_PROPERTY(TARGET PureImgui APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")

	set(CMAKE_EXE_LINKER_FLAGS "\
		-framework Metal \
		-framework MetalKit \
		-framework Cocoa \
		-framework IOKit \
		-framework CoreVideo \
		-framework QuartzCore")

elseif(UNIX)

	SET_PROPERTY(TARGET PureImgui APPEND_STRING PROPERTY COMPILE_FLAGS "-g -Wall -Wformat")
	add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)

endif()

target_include_directories(PureImgui

	PRIVATE

		"src/"
		"../DearPyGui/vendor/ImGuiFileDialog/"
		"../Dependencies/glfw/include/"
		"../Dependencies/glfw/deps/"
		"../Dependencies/gl3w/"
		"../DearPyGui/vendor/implot/"
		"../DearPyGui/vendor/stb/"
		"../DearPyGui/vendor/imnodes/"
)

target_link_libraries(PureImgui

	PRIVATE
		${IMGUI_LIBRARIES}		
		freetype
		$<$<PLATFORM_ID:Linux>:GL>
		$<$<PLATFORM_ID:Linux>:glfw>
		$<$<PLATFORM_ID:Darwin>:glfw>
		$<$<PLATFORM_ID:Windows>:d3d11>
)
