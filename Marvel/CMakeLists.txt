cmake_minimum_required (VERSION 3.16)

add_library(Marvel STATIC)

set_property(TARGET Marvel PROPERTY CXX_STANDARD 17)

target_sources(Marvel

	PRIVATE

		"MarvelApplication.cpp"
		"src/Core/mvApp.cpp"
		"src/Core/mvWindow.cpp"
		"src/Core/mvTextEditor.cpp"

		"src/Core/StandardWindows/mvStandardWindow.cpp"
		"src/Core/StandardWindows/mvAppEditor.cpp"
		"src/Core/StandardWindows/mvAppLog.cpp"
		"src/Core/StandardWindows/mvDocWindow.cpp"
		"src/Core/StandardWindows/mvAboutWindow.cpp"
		"src/Core/StandardWindows/mvMetricsWindow.cpp"
		"src/Core/StandardWindows/mvSourceWindow.cpp"
		
		"src/Core/Concurrency/mvThreadPool.cpp"

		"src/Core/AppItems/mvAppItem.cpp"
		"src/Core/AppItems/mvDrawing.cpp"

		"src/Core/PythonInterfaces/mvPythonTranslator.cpp"
		"src/Core/PythonInterfaces/mvPythonModule.cpp"
		"src/Core/PythonInterfaces/mvAppInterface.cpp"
		"src/Core/PythonInterfaces/mvLoggerInterface.cpp"
		"src/Core/PythonInterfaces/mvInputInterface.cpp"
		"src/Core/PythonInterfaces/mvPlotInterface.cpp"
		"src/Core/PythonInterfaces/mvDrawingInterface.cpp"
		"src/Core/PythonInterfaces/mvWidgetAddingInterface.cpp"
		"src/Core/PythonInterfaces/mvInterfaceRegistry.cpp"
		"src/Core/PythonInterfaces/mvModuleConstants.cpp"
		"src/Core/PythonInterfaces/mvModuleInitializer.cpp"

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

target_include_directories(Marvel

	PUBLIC

		"src/"
		"include/"
		"../Dependencies/imgui/"
		"../Dependencies/imgui/examples/"
		"../Dependencies/cpython/Include/"
		"src/Vendor/implot/"
		"src/Vendor/stb/"
		"src/Vendor/CLIUtils/"
)

target_link_directories(Marvel

	PUBLIC
		"../Dependencies/cpython/PCbuild/amd64/"
)

target_link_libraries(Marvel

	PUBLIC

		d3d11
		$<$<CONFIG:Debug>:python3_d>
		$<$<CONFIG:Release>:python3>
)

target_compile_definitions(Marvel

	PUBLIC

		$<$<CONFIG:Release>:MV_RELEASE>
)
