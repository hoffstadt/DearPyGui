cmake_minimum_required (VERSION 3.16)

add_executable (PureImgui)

target_sources(PureImgui

	PRIVATE

		"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
		
		"${CMAKE_CURRENT_SOURCE_DIR}/src/mvWindowsWindow.cpp"
		
		# imgui
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/imgui.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/imgui_demo.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/imgui_draw.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/imgui_widgets.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/examples/imgui_impl_win32.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/examples/imgui_impl_dx11.cpp"

)

target_include_directories(PureImgui

	PRIVATE

		"${CMAKE_CURRENT_SOURCE_DIR}/src/"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/examples/"

)

target_link_libraries(PureImgui

	PRIVATE

		d3d11

)
