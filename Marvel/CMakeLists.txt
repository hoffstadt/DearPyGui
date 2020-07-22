cmake_minimum_required (VERSION 3.16)

add_library(marvel SHARED)
set_target_properties(marvel PROPERTIES SUFFIX ".pyd")
set_target_properties(marvel PROPERTIES OUTPUT_NAME "marvel$<$<CONFIG:Debug>:_d>")

set_property(TARGET marvel PROPERTY CXX_STANDARD 17)

target_sources(marvel

	PRIVATE

		"src/Core/mvApp.cpp"
		"src/Core/mvInput.cpp"
		"src/Core/mvWindow.cpp"
		"src/Core/mvTextEditor.cpp"
		"src/Core/mvDataStorage.cpp"
		"src/Core/mvTextureStorage.cpp"

		"src/Core/StandardWindows/mvStandardWindow.cpp"
		"src/Core/StandardWindows/mvAppEditor.cpp"
		"src/Core/StandardWindows/mvAppLog.cpp"
		"src/Core/StandardWindows/mvDocWindow.cpp"
		"src/Core/StandardWindows/mvAboutWindow.cpp"
		"src/Core/StandardWindows/mvMetricsWindow.cpp"
		"src/Core/StandardWindows/mvSourceWindow.cpp"
		"src/Core/StandardWindows/mvDebugWindow.cpp"

		"src/Core/AppItems/mvAppItem.cpp"
		"src/Core/AppItems/mvDrawing.cpp"
		"src/Core/AppItems/mvTable.cpp"

		"src/Core/PythonInterfaces/mvPythonTranslator.cpp"
		"src/Core/PythonInterfaces/mvPythonParser.cpp"
		"src/Core/PythonInterfaces/mvPythonModule.cpp"
		"src/Core/PythonInterfaces/mvMarvel.cpp"
		"src/Core/PythonInterfaces/mvInterfaceRegistry.cpp"
		#"src/Core/PythonInterfaces/mvMarvelConstants.cpp"

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

target_include_directories(marvel

	PUBLIC

		"src/"
		"../Dependencies/imgui/"
		"../Dependencies/imgui/examples/"
		"../Dependencies/cpython/Include/"
		"src/Vendor/implot/"
		"src/Vendor/stb/"
)

target_link_directories(marvel

	PUBLIC
		"../Dependencies/cpython/PCbuild/amd64/"
)

target_link_libraries(marvel

	PUBLIC

		d3d11
		$<$<CONFIG:Debug>:python38_d>
		$<$<CONFIG:Release>:python38>
)

target_compile_definitions(marvel

	PUBLIC

		$<$<CONFIG:Release>:MV_RELEASE>

	PRIVATE
		_EXPORTING
)

