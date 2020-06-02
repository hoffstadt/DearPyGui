cmake_minimum_required (VERSION 3.16)

add_executable (MarvelSandbox)

target_compile_features(MarvelSandbox

	PUBLIC
		cxx_std_17
)

target_sources(MarvelSandbox

	PRIVATE

		"${CMAKE_CURRENT_SOURCE_DIR}/EntryPoint.cpp"
		
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/mvApp.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/mvPythonInterface.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/mvAppItem.cpp"

		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/AppItems/mvInputText.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/AppItems/mvTab.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/AppItems/mvMenu.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/AppItems/mvSpacing.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/AppItems/mvSameLine.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/AppItems/mvRadioButton.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/AppItems/mvButton.cpp"

		"${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Windows/mvWindowsWindow.cpp"
		
		# imgui
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/misc/cpp/imgui_stdlib.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/imgui.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/imgui_demo.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/imgui_draw.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/imgui_widgets.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/examples/imgui_impl_win32.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/examples/imgui_impl_dx11.cpp"
)

target_include_directories(MarvelSandbox

	PRIVATE

		"${CMAKE_CURRENT_SOURCE_DIR}/src/"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/imgui/examples/"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/cpython/Include/"
)

target_link_directories(MarvelSandbox

	PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/cpython/PCbuild/amd64/"
)

target_link_libraries(MarvelSandbox

	PRIVATE

		d3d11
		python3
)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Resources/python/Debug/ $<TARGET_FILE_DIR:MarvelSandbox>)
