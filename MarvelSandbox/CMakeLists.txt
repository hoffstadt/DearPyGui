cmake_minimum_required (VERSION 3.16)

add_executable (MarvelSandbox)

target_sources(MarvelSandbox

	PRIVATE

		"EntryPoint.cpp"
		"src/Core/mvApp.cpp"
		"src/Core/mvAppLog.cpp"
		"src/Core/mvWindow.cpp"
		"src/Core/mvTextEditor.cpp"

		"src/Core/StandardWindows/mvDocWindow.cpp"
		"src/Core/StandardWindows/mvAboutWindow.cpp"
		"src/Core/StandardWindows/mvMetricsWindow.cpp"
		"src/Core/StandardWindows/mvSourceWindow.cpp"
		
		"src/Core/Concurrency/mvThreadPool.cpp"

		"src/Core/PythonUtilities/mvPythonTranslator.cpp"
		"src/Core/PythonUtilities/mvPythonModule.cpp"

		"src/Core/AppItems/mvAppItem.cpp"
		"src/Core/AppItems/mvDrawing.cpp"

		"src/Core/PythonInterfaces/mvAppInterface.cpp"
		"src/Core/PythonInterfaces/mvLoggerInterface.cpp"
		"src/Core/PythonInterfaces/mvInputInterface.cpp"
		"src/Core/PythonInterfaces/mvPlotInterface.cpp"
		"src/Core/PythonInterfaces/mvDrawingInterface.cpp"
		"src/Core/PythonInterfaces/mvWidgetAddingInterface.cpp"
		"src/Core/PythonInterfaces/mvInterfaceRegistry.cpp"
		"src/Core/PythonInterfaces/mvModuleConstants.cpp"

		"src/Platform/Windows/mvWindowsWindow.cpp"
		"src/Platform/Windows/mvUtilities.cpp"

		# implot
		"src/Vendor/implot/implot.cpp"
		"src/Vendor/implot/implot_demo.cpp"

		# imgui
		"../Dependencies/imgui/misc/cpp/imgui_stdlib.cpp"
		"../Dependencies/imgui/imgui.cpp"
		"../Dependencies/imgui/imgui_demo.cpp"
		"../Dependencies/imgui/imgui_draw.cpp"
		"../Dependencies/imgui/imgui_widgets.cpp"
		"../Dependencies/imgui/examples/imgui_impl_win32.cpp"
		"../Dependencies/imgui/examples/imgui_impl_dx11.cpp")

target_include_directories(MarvelSandbox

	PRIVATE

		"src/"
		"../Dependencies/imgui/"
		"../Dependencies/imgui/examples/"
		"../Dependencies/cpython/Include/"
		"src/Vendor/implot"
		"src/Vendor/stb"
)

target_link_directories(MarvelSandbox

	PRIVATE
		"../Dependencies/cpython/PCbuild/amd64/"
)

target_link_libraries(MarvelSandbox

	PRIVATE

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
