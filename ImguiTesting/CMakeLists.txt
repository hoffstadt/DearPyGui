cmake_minimum_required (VERSION 3.16)

add_executable (PureImgui)


target_sources(PureImgui

	PRIVATE

		"$<$<PLATFORM_ID:Windows>:main.cpp>"
		"$<$<PLATFORM_ID:Darwin>:main.mm>"
		"$<$<PLATFORM_ID:Darwin>:src/mvAppleWindow.mm>"
		"$<$<PLATFORM_ID:Windows>:src/mvWindowsWindow.cpp>"


		# implot
		"../DearPyGui/src/Vendor/implot/implot.cpp"
		"../DearPyGui/src/Vendor/implot/implot_demo.cpp"

		# imgui
		"../Dependencies/imgui/imgui.cpp"
		"../Dependencies/imgui/imgui_demo.cpp"
		"../Dependencies/imgui/imgui_draw.cpp"
		"../Dependencies/imgui/imgui_widgets.cpp"
		"$<$<PLATFORM_ID:Windows>:../Dependencies/imgui/examples/imgui_impl_win32.cpp>"
		"$<$<PLATFORM_ID:Windows>:../Dependencies/imgui/examples/imgui_impl_dx11.cpp>"
		"$<$<PLATFORM_ID:Darwin>:../Dependencies/imgui/examples/imgui_impl_metal.mm>"
		"$<$<PLATFORM_ID:Darwin>:../Dependencies/imgui/examples/imgui_impl_glfw.cpp>"
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
