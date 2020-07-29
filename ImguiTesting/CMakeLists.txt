cmake_minimum_required (VERSION 3.16)

add_executable (PureImgui)


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

		glfw

)
