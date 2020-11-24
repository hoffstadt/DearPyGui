
set(MARVEL_SOURCES

	"$<$<PLATFORM_ID:Windows>:src/core/mvMarvel.cpp>"
	"$<$<PLATFORM_ID:Linux>:src/core/mvMarvel.cpp>"
	"$<$<PLATFORM_ID:Darwin>:src/core/mvMarvel.mm>"

	"$<$<PLATFORM_ID:Windows>:src/platform/Windows/mvWindowsWindow.cpp>"
	"$<$<PLATFORM_ID:Windows>:src/platform/Windows/mvUtilities.cpp>"

	"$<$<PLATFORM_ID:Darwin>:src/platform/Apple/mvAppleWindow.mm>"
	"$<$<PLATFORM_ID:Darwin>:src/platform/Apple/mvAppleUtilities.mm>"

	"$<$<PLATFORM_ID:Linux>:src/platform/Linux/mvUtilities.cpp>"
	"$<$<PLATFORM_ID:Linux>:src/platform/Linux/mvLinuxWindow.cpp>"

	"src/core/mvApp.cpp"
	"src/core/mvAppLog.cpp"
	"src/core/mvInput.cpp"
	"src/core/mvWindow.cpp"
	"src/core/mvTextEditor.cpp"
	"src/core/mvProfiler.cpp"

	"src/core/Theming/mvAppItemStyleManager.cpp"

	"src/core/DrawCommands/mvDrawImageCmd.cpp"
	"src/core/DrawCommands/mvDrawLineCmd.cpp"
	"src/core/DrawCommands/mvDrawArrowCmd.cpp"
	"src/core/DrawCommands/mvDrawTriangleCmd.cpp"
	"src/core/DrawCommands/mvDrawCircleCmd.cpp"
	"src/core/DrawCommands/mvDrawTextCmd.cpp"
	"src/core/DrawCommands/mvDrawRectCmd.cpp"
	"src/core/DrawCommands/mvDrawQuadCmd.cpp"
	"src/core/DrawCommands/mvDrawPolylineCmd.cpp"
	"src/core/DrawCommands/mvDrawPolygonCmd.cpp"
	"src/core/DrawCommands/mvDrawBezierCurveCmd.cpp"

	"src/core/Registries/mvItemRegistry.cpp"
	"src/core/Registries/mvDataStorage.cpp"
	"src/core/Registries/mvTextureStorage.cpp"
	"src/core/Registries/mvValueStorage.cpp"
	"src/core/Registries/mvDrawList.cpp"

	"src/core/PythonUtilities/mvPyObject.cpp"
	"src/core/PythonUtilities/mvPythonTranslator.cpp"
	"src/core/PythonUtilities/mvPythonParser.cpp"
	"src/core/PythonUtilities/mvPythonExceptions.cpp"

	"src/core/PythonCommands/mvInterfaceCore.cpp"
	"src/core/PythonCommands/mvPlotInterface.cpp"
	"src/core/PythonCommands/mvDrawingInterface.cpp"
	"src/core/PythonCommands/mvTableInterface.cpp"
	"src/core/PythonCommands/mvThemeInterface.cpp"
	"src/core/PythonCommands/mvInputInterface.cpp"
	"src/core/PythonCommands/mvBasicWidgetInterface.cpp"
	"src/core/PythonCommands/mvSliderInterface.cpp"
	"src/core/PythonCommands/mvInputsInterface.cpp"
	"src/core/PythonCommands/mvDragInterface.cpp"
	"src/core/PythonCommands/mvContainerInterface.cpp"
	"src/core/PythonCommands/mvAppItemInterface.cpp"
	"src/core/PythonCommands/mvAppInterface.cpp"

	"src/core/AppItems/mvInputText.cpp"
	"src/core/AppItems/mvTypeBases.cpp"
	"src/core/AppItems/mvTimePicker.cpp"
	"src/core/AppItems/mvTooltip.cpp"
	"src/core/AppItems/mvText.cpp"
	"src/core/AppItems/mvTab.cpp"
	"src/core/AppItems/mvImage.cpp"
	"src/core/AppItems/mvContainerItems.cpp"
	"src/core/AppItems/mvFileDialog.cpp"
	"src/core/AppItems/mvDatePicker.cpp"
	"src/core/AppItems/mvColumns.cpp"
	"src/core/AppItems/mvColorItems.cpp"
	"src/core/AppItems/mvBasicItems.cpp"
	"src/core/AppItems/mvDebugWindow.cpp"
	"src/core/AppItems/mvDocWindow.cpp"
	"src/core/AppItems/mvAppItem.cpp"
	"src/core/AppItems/mvDrawing.cpp"
	"src/core/AppItems/mvTable.cpp"
	"src/core/AppItems/mvLogger.cpp"
	"src/core/AppItems/mvStyleWindow.cpp"
	"src/core/AppItems/mvWindowAppItem.cpp"

	"src/core/PlotAppItems/mvPlot.cpp"
	"src/core/PlotAppItems/mvAreaSeries.cpp"

	# implot
	"vendor/implot/implot.cpp"
	"vendor/implot/implot_items.cpp"
	"vendor/implot/implot_demo.cpp"

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

	"src/"
	"src/core"
	"$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/DearPyGui/Vendor/dirent/>"
	"../Dependencies/ImGuiFileDialog/ImGuiFileDialog/"
	"../Dependencies/glfw/include/"
	"../Dependencies/glfw/deps/"
	"../Dependencies/imgui/"
	"../Dependencies/imgui/examples/"
	"../Dependencies/imgui/examples/libs/gl3w"
	"../Dependencies/cpython/"
	"../Dependencies/cpython/Include/"
	"vendor/implot/"
	"vendor/stb/"
	"$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/Dependencies/cpython/PC/>"

	# may can delete this
	"../Dependencies/cpython/debug/"
)
