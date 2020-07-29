cmake_minimum_required (VERSION 3.16)

add_executable (PureImgui)

if(WIN32)
	target_sources(PureImgui

		PRIVATE
			"main.cpp"
			"src/mvWindowsWindow.cpp"

			# implot
			"../DearPyGui/src/Vendor/implot/implot.cpp"
			"../DearPyGui/src/Vendor/implot/implot_demo.cpp"

			# imgui
			"../Dependencies/imgui/imgui.cpp"
			"../Dependencies/imgui/imgui_demo.cpp"
			"../Dependencies/imgui/imgui_draw.cpp"
			"../Dependencies/imgui/imgui_widgets.cpp"
			"../Dependencies/imgui/examples/imgui_impl_win32.cpp"
			"../Dependencies/imgui/examples/imgui_impl_dx11.cpp")
endif()

if(APPLE)
	target_sources(PureImgui

		PRIVATE
			"main.mm"
			"src/mvAppleWindow.mm"


			# implot
			"../DearPyGui/src/Vendor/implot/implot.cpp"
			"../DearPyGui/src/Vendor/implot/implot_demo.cpp"

			# imgui
			"../Dependencies/imgui/imgui.cpp"
			"../Dependencies/imgui/imgui_demo.cpp"
			"../Dependencies/imgui/imgui_draw.cpp"
			"../Dependencies/imgui/imgui_widgets.cpp"
			"../Dependencies/imgui/examples/imgui_impl_metal.mm"
			"../Dependencies/imgui/examples/imgui_impl_glfw.cpp"
			)

	SET_PROPERTY(TARGET PureImgui APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")

	set(CMAKE_EXE_LINKER_FLAGS "\
		-framework Metal \
		-framework MetalKit \
		-framework Cocoa \
		-framework IOKit \
		-framework CoreVideo \
		-framework QuartzCore")
endif()

target_include_directories(PureImgui

	PRIVATE

		"src/"
		"../Dependencies/glfw/include/"
		"../Dependencies/imgui/"
		"../Dependencies/imgui/examples/"
		"../DearPyGui/src/Vendor/implot/"
		"../DearPyGui/src/Vendor/stb/"

)

target_link_libraries(PureImgui

	PRIVATE

		$<$<PLATFORM_ID:Darwin>:glfw>
		$<$<PLATFORM_ID:Windows>:d3d11>
)
