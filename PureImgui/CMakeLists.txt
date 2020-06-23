cmake_minimum_required (VERSION 3.16)

add_executable (PureImgui)

target_sources(PureImgui

	PRIVATE

		"main.cpp"
		"src/mvWindowsWindow.cpp"
		
		# implot
		"../MarvelSandbox/src/Vendor/implot/implot.cpp"
		"../MarvelSandbox/src/Vendor/implot/implot_demo.cpp"

		# imgui
		"../Dependencies/imgui/imgui.cpp"
		"../Dependencies/imgui/imgui_demo.cpp"
		"../Dependencies/imgui/imgui_draw.cpp"
		"../Dependencies/imgui/imgui_widgets.cpp"
		"../Dependencies/imgui/examples/imgui_impl_win32.cpp"
		"../Dependencies/imgui/examples/imgui_impl_dx11.cpp"

)

target_include_directories(PureImgui

	PRIVATE

		"src/"
		"../Dependencies/imgui/"
		"../Dependencies/imgui/examples/"
		"../MarvelSandbox/src/Vendor/implot/"
		"../MarvelSandbox/src/Vendor/stb/"

)

target_link_libraries(PureImgui

	PRIVATE

		d3d11

)
