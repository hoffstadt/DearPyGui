
set(MARVEL_SOURCES

	"$<$<PLATFORM_ID:Windows>:src/Core/mvMarvel.cpp>"
	"$<$<PLATFORM_ID:Linux>:src/Core/mvMarvel.cpp>"
	"$<$<PLATFORM_ID:Darwin>:src/Core/mvMarvel.mm>"

	"$<$<PLATFORM_ID:Windows>:src/Platform/Windows/mvWindowsWindow.cpp>"
	"$<$<PLATFORM_ID:Windows>:src/Platform/Windows/mvUtilities.cpp>"

	"$<$<PLATFORM_ID:Darwin>:src/Platform/Apple/mvAppleWindow.mm>"
	"$<$<PLATFORM_ID:Darwin>:src/Platform/Apple/mvAppleUtilities.mm>"

	"$<$<PLATFORM_ID:Linux>:src/Platform/Linux/mvUtilities.cpp>"
	"$<$<PLATFORM_ID:Linux>:src/Platform/Linux/mvLinuxWindow.cpp>"

	"src/Core/mvApp.cpp"
	"src/Core/mvAppLog.cpp"
	"src/Core/mvInput.cpp"
	"src/Core/mvWindow.cpp"
	"src/Core/mvTextEditor.cpp"
	"src/Core/mvDataStorage.cpp"
	"src/Core/mvTextureStorage.cpp"
	"src/Core/mvValueStorage.cpp"

	"src/Core/mvPythonTranslator.cpp"
	"src/Core/mvPythonParser.cpp"
	"src/Core/mvPythonExceptions.cpp"

	"src/Core/PythonCommands/mvInterfaceCore.cpp"
	"src/Core/PythonCommands/mvPlotInterface.cpp"
	"src/Core/PythonCommands/mvDrawingInterface.cpp"
	"src/Core/PythonCommands/mvTableInterface.cpp"
	"src/Core/PythonCommands/mvThemeInterface.cpp"
	"src/Core/PythonCommands/mvInputInterface.cpp"
	"src/Core/PythonCommands/mvBasicWidgetInterface.cpp"
	"src/Core/PythonCommands/mvSliderInterface.cpp"
	"src/Core/PythonCommands/mvInputsInterface.cpp"
	"src/Core/PythonCommands/mvDragInterface.cpp"
	"src/Core/PythonCommands/mvContainerInterface.cpp"
	"src/Core/PythonCommands/mvAppItemInterface.cpp"
	"src/Core/PythonCommands/mvAppInterface.cpp"

	"src/Core/StandardWindows/mvStandardWindow.cpp"
	"src/Core/StandardWindows/mvDocWindow.cpp"
	"src/Core/StandardWindows/mvAboutWindow.cpp"
	"src/Core/StandardWindows/mvMetricsWindow.cpp"
	"src/Core/StandardWindows/mvSourceWindow.cpp"
	"src/Core/StandardWindows/mvDebugWindow.cpp"
	"src/Core/StandardWindows/mvFileDialog.cpp"
	"src/Core/StandardWindows/mvStyleWindow.cpp"

	"src/Core/AppItems/mvAppItem.cpp"
	"src/Core/AppItems/mvDrawing.cpp"
	"src/Core/AppItems/mvTable.cpp"
	"src/Core/AppItems/mvLogger.cpp"

	"src/Core/PlotAppItems/mvPlot.cpp"
	"src/Core/PlotAppItems/mvAreaSeries.cpp"

	# implot
	"src/Vendor/implot/implot.cpp"
	"src/Vendor/implot/implot_items.cpp"
	"src/Vendor/implot/implot_demo.cpp"

	# ImGuiFileDialog
	"../Dependencies/ImGuiFileDialog/ImGuiFileDialog/ImGuiFileDialog.cpp"

	# imgui
	"../Dependencies/imgui/misc/cpp/imgui_stdlib.cpp"
	"../Dependencies/imgui/imgui.cpp"
	"../Dependencies/imgui/imgui_demo.cpp"
	"../Dependencies/imgui/imgui_draw.cpp"
	"../Dependencies/imgui/imgui_widgets.cpp"
	"$<$<PLATFORM_ID:Windows>:../Dependencies/imgui/examples/imgui_impl_win32.cpp>"
	"$<$<PLATFORM_ID:Windows>:../Dependencies/imgui/examples/imgui_impl_dx11.cpp>"
	"$<$<PLATFORM_ID:Darwin>:../Dependencies/imgui/examples/imgui_impl_metal.mm>"
	"$<$<PLATFORM_ID:Darwin>:../Dependencies/imgui/examples/imgui_impl_glfw.cpp>"
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/examples/libs/gl3w/GL/gl3w.c>"
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/examples/imgui_impl_glfw.cpp>"
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/examples/imgui_impl_opengl3.cpp>"
)

set(MARVEL_INCLUDE_DIR

	"include/"
	"src/"
	"$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/DearPyGui/src/Vendor/dirent/>"
	"../Dependencies/ImGuiFileDialog/ImGuiFileDialog/"
	"../Dependencies/glfw/include/"
	"../Dependencies/glfw/deps/"
	"../Dependencies/imgui/"
	"../Dependencies/imgui/examples/"
	"../Dependencies/imgui/examples/libs/gl3w"
	"../Dependencies/cpython/"
	"../Dependencies/cpython/Include/"
	"src/Vendor/implot/"
	"src/Vendor/stb/"
	"$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/Dependencies/cpython/PC/>"

	#"/usr/include/python3.7/"
	#"/usr/include/python3.8/"

	# may can delete this
	"../Dependencies/cpython/debug/"
)