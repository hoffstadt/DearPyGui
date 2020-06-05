cmake_minimum_required (VERSION 3.16)

add_executable (MarvelSandbox)

target_sources(MarvelSandbox

	PRIVATE

		"${CMAKE_CURRENT_SOURCE_DIR}/EntryPoint.cpp"
		
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/mvApp.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/mvPythonModule.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/mvAppItem.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/mvTheme.cpp"

		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/PythonInterfaces/mvAppInterface.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/PythonInterfaces/mvLoggerInterface.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/PythonInterfaces/mvInputInterface.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/PythonInterfaces/mvPlotInterface.cpp"

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

		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/MarvelGraph/MarvelGraph/include/"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/MarvelGraph/MarvelGraph/include/layers"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/MarvelGraph/MarvelGraph/include/events"
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/MarvelGraph/MarvelGraph/include/instrumentation"
)

target_link_directories(MarvelSandbox

	PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/cpython/PCbuild/amd64/"
)

target_link_libraries(MarvelSandbox

	PRIVATE

		marvelgraph
		d3d11
		$<$<CONFIG:Debug>:python3_d>
		$<$<CONFIG:Release>:python3>
)

target_compile_definitions(MarvelSandbox

	PUBLIC

		$<$<CONFIG:Release>:MV_RELEASE>
)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Resources/python/Debug/ $<TARGET_FILE_DIR:MarvelSandbox>)
