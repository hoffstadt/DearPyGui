
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

	"src/core/mvEvents.cpp"
	"src/core/mvApp.cpp"
	"src/core/mvAppLog.cpp"
	"src/core/mvInput.cpp"
	"src/core/mvWindow.cpp"
	"src/core/mvTextEditor.cpp"
	"src/core/mvProfiler.cpp"
	"src/core/mvThreadPoolManager.cpp"

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

	"src/core/Registries/mvCallbackRegistry.cpp"
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

	"src/core/AppItems/mvAppItemState.cpp"
	"src/core/AppItems/mvAppItem.cpp"
	"src/core/AppItems/mvTypeBases.cpp"

	"src/core/AppItems/basic/mvInputText.cpp"
	"src/core/AppItems/basic/mvText.cpp"
	"src/core/AppItems/basic/mvImage.cpp"
	"src/core/AppItems/basic/mvColorItems.cpp"
	"src/core/AppItems/basic/mvBasicItems.cpp"

	"src/core/AppItems/containers/mvTooltip.cpp"
	"src/core/AppItems/containers/mvTab.cpp"
	"src/core/AppItems/containers/mvContainerItems.cpp"
	"src/core/AppItems/containers/mvColumns.cpp"
	"src/core/AppItems/containers/mvWindowAppItem.cpp"

	"src/core/AppItems/custom/mvDatePicker.cpp"
	"src/core/AppItems/custom/mvTimePicker.cpp"
	"src/core/AppItems/custom/mvDrawing.cpp"
	
	"src/core/AppItems/composite/mvFileDialog.cpp"
	"src/core/AppItems/composite/mvDebugWindow.cpp"
	"src/core/AppItems/composite/mvDocWindow.cpp"
	"src/core/AppItems/composite/mvTable.cpp"
	"src/core/AppItems/composite/mvLogger.cpp"
	"src/core/AppItems/composite/mvStyleWindow.cpp"
	
	"src/core/AppItems/plots/mvPlot.cpp"
	"src/core/AppItems/plots/mvAreaSeries.cpp"

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
