
set(MARVEL_PY_SOURCES

	"$<$<PLATFORM_ID:Windows>:src/core/mvMarvel.cpp>"
	"$<$<PLATFORM_ID:Linux>:src/core/mvMarvel.cpp>"
	"$<$<PLATFORM_ID:Darwin>:src/core/mvMarvel.mm>"

	"src/core/Registries/mvDataStorage.cpp"

	"src/core/AppItems/composite/mvDebugWindow.cpp"
	"src/core/AppItems/composite/mvDocWindow.cpp"

	"src/core/PythonUtilities/mvPyObject.cpp"
	"src/core/PythonUtilities/mvPythonTranslator.cpp"
	"src/core/PythonUtilities/mvPythonParser.cpp"
	"src/core/PythonUtilities/mvPythonExceptions.cpp"
	"src/core/PythonUtilities/mvGlobalIntepreterLock.cpp"

	"src/core/PythonCommands/mvPlotInterface.cpp"
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
)

set(MARVEL_CPP_SOURCES

	"src/core/CppUtilities/mvCppTranslator.cpp"
)

set(MARVEL_SOURCES

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
	"src/core/mvProfiler.cpp"

	"src/core/Theming/mvAppItemStyleManager.cpp"
	"src/core/Theming/mvThemeManager.cpp"

	"src/core/DrawCommands/mvDrawList.cpp"
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
	
	"src/core/Registries/mvTextureStorage.cpp"

	"src/core/AppItems/mvAppItemState.cpp"
	"src/core/AppItems/mvAppItem.cpp"
	"src/core/AppItems/mvTypeBases.cpp"
	"src/core/AppItems/mvItemRegistry.cpp"

	"src/core/AppItems/basic/mvMenuItem.cpp"
	"src/core/AppItems/basic/mvInputText.cpp"
	"src/core/AppItems/basic/mvText.cpp"
	"src/core/AppItems/basic/mvSliderItems.cpp"
	"src/core/AppItems/basic/mvImage.cpp"
	"src/core/AppItems/basic/mvImageButton.cpp"
	"src/core/AppItems/basic/mvColorItems.cpp"
	"src/core/AppItems/basic/mvRadioButton.cpp"
	"src/core/AppItems/basic/mvListbox.cpp"
	"src/core/AppItems/basic/mvCombo.cpp"
	"src/core/AppItems/basic/mvCheckbox.cpp"
	"src/core/AppItems/basic/mvButton.cpp"
	"src/core/AppItems/basic/mvSelectable.cpp"
	"src/core/AppItems/basic/mvTabButton.cpp"
	"src/core/AppItems/basic/mvDragItems.cpp"
	"src/core/AppItems/basic/mvInputItems.cpp"
	"src/core/AppItems/basic/mvSliderItems.cpp"

	"src/core/AppItems/misc/mvProgressBar.cpp"

	"src/core/AppItems/containers/mvCollapsingHeader.cpp"
	"src/core/AppItems/containers/mvGroup.cpp"
	"src/core/AppItems/containers/mvChild.cpp"
	"src/core/AppItems/containers/mvTooltip.cpp"
	"src/core/AppItems/containers/mvTab.cpp"
	"src/core/AppItems/containers/mvTabBar.cpp"
	"src/core/AppItems/containers/mvMenu.cpp"
	"src/core/AppItems/containers/mvMenuBar.cpp"
	"src/core/AppItems/containers/mvTreeNode.cpp"
	"src/core/AppItems/containers/mvColumns.cpp"
	"src/core/AppItems/containers/mvWindowAppItem.cpp"

	"src/core/AppItems/custom/mvDatePicker.cpp"
	"src/core/AppItems/custom/mvTimePicker.cpp"
	"src/core/AppItems/custom/mvDrawing.cpp"
	"src/core/AppItems/custom/mvTextEditor.cpp"
	
	"src/core/AppItems/composite/mvFileDialog.cpp"
	"src/core/AppItems/composite/mvAboutWindow.cpp"
	"src/core/AppItems/composite/mvTable.cpp"
	"src/core/AppItems/composite/mvLogger.cpp"
	"src/core/AppItems/composite/mvStyleWindow.cpp"
	"src/core/AppItems/composite/mvMetricsWindow.cpp"
	"src/core/AppItems/composite/mvAboutWindow.cpp"

	"src/core/AppItems/nodes/mvNodeEditor.cpp"
	"src/core/AppItems/nodes/mvNode.cpp"
	"src/core/AppItems/nodes/mvNodeAttribute.cpp"
	
	"src/core/AppItems/plots/mvPlot.cpp"
	"src/core/AppItems/plots/mvAreaSeries.cpp"

	# imnodes
	"vendor/imnodes/imnodes.cpp"

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
	"../Dependencies/imgui/imgui_tables.cpp"
	"$<$<PLATFORM_ID:Windows>:../Dependencies/imgui/backends/imgui_impl_win32.cpp>"
	"$<$<PLATFORM_ID:Windows>:../Dependencies/imgui/backends/imgui_impl_dx11.cpp>"
	"$<$<PLATFORM_ID:Darwin>:../Dependencies/imgui/backends/imgui_impl_metal.mm>"
	"$<$<PLATFORM_ID:Darwin>:../Dependencies/imgui/backends/imgui_impl_glfw.cpp>"
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/examples/libs/gl3w/GL/gl3w.c>"
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/backends/imgui_impl_glfw.cpp>"
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/backends/imgui_impl_opengl3.cpp>"
)

set(MARVEL_INCLUDE_DIR

	"vendor/implot"
	"vendor/stb"
	"vendor/imnodes"
	"src/"
	"src/core"
	"src/core/AppItems"
	"src/core/DrawCommands"
	"src/core/PythonCommands"
	"src/core/CppUtilities"
	"src/core/PythonUtilities"
	"src/core/Registries"
	"src/core/Theming"
	"$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/DearPyGui/vendor/dirent/>"
	"../Dependencies/ImGuiFileDialog/ImGuiFileDialog/"
	"../Dependencies/glfw/include/"
	"../Dependencies/glfw/deps/"
	"../Dependencies/imgui/"
	"../Dependencies/imgui/backends/"
	"../Dependencies/imgui/examples/libs/gl3w"
	"../Dependencies/cpython/"
	"../Dependencies/cpython/Include/"
	"$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/Dependencies/cpython/PC/>"
	# may can delete this
	"../Dependencies/cpython/debug/"
)
