﻿
set(MARVEL_SOURCES

	# core
	"src/core/mvContext.cpp"
	"src/core/mvMath.cpp"
	"src/core/mvProfiler.cpp"

	# modules
	"$<$<PLATFORM_ID:Windows>:src/modules/dearpygui.cpp>"
	"$<$<PLATFORM_ID:Linux>:src/modules/dearpygui.cpp>"
	"$<$<PLATFORM_ID:Darwin>:src/modules/dearpygui.mm>"

	# platform
	"$<$<PLATFORM_ID:Windows>:src/platform/Windows/mvViewport.cpp>"
	"$<$<PLATFORM_ID:Windows>:src/platform/Windows/mvUtilities.cpp>"
	"$<$<PLATFORM_ID:Windows>:src/platform/Windows/mvGraphics.cpp>"

	"$<$<PLATFORM_ID:Darwin>:src/platform/Apple/mvViewport.mm>"
	"$<$<PLATFORM_ID:Darwin>:src/platform/Apple/mvUtilities.mm>"
	"$<$<PLATFORM_ID:Darwin>:src/platform/Apple/mvGraphics.mm>"

	"$<$<PLATFORM_ID:Linux>:src/platform/Linux/mvUtilities.cpp>"
	"$<$<PLATFORM_ID:Linux>:src/platform/Linux/mvViewport.cpp>"
	"$<$<PLATFORM_ID:Linux>:src/platform/Linux/mvGraphics.cpp>"

	# python
	"src/python/utilities/mvPyObject.cpp"
	"src/python/utilities/mvPythonTranslator.cpp"
	"src/python/utilities/mvPythonParser.cpp"
	"src/python/utilities/mvPythonExceptions.cpp"
	"src/python/utilities/mvGlobalIntepreterLock.cpp"
	"src/python/utilities/mvPythonTypeChecker.cpp"
	"src/python/types/mvBuffer.cpp"
	"src/python/types/mvVec4Type.cpp"
	"src/python/types/mvMat4Type.cpp"

	# ui
	"src/ui/AppItems/mvBasicWidgets.cpp"
	"src/ui/AppItems/mvTables.cpp"
	"src/ui/AppItems/mvThemes.cpp"
	"src/ui/AppItems/mvNodes.cpp"
	"src/ui/AppItems/mvDrawings.cpp"
	"src/ui/AppItems/mvGlobalHandlers.cpp"
	"src/ui/AppItems/mvItemHandlers.cpp"
	"src/ui/AppItems/mvValues.cpp"
	"src/ui/AppItems/mvTextureItems.cpp"
	"src/ui/AppItems/mvFontItems.cpp"
	"src/ui/AppItems/mvColors.cpp"
	"src/ui/AppItems/mvPlotting.cpp"
	"src/ui/AppItems/mvContainers.cpp"

	"src/ui/mvCallbackRegistry.cpp"
	"src/ui/CustomImGuiWidgets/mvKnobCustom.cpp"
	"src/ui/CustomImGuiWidgets/mvLoadingIndicatorCustom.cpp"
	"src/ui/Theming/mvFontManager.cpp"
	"src/ui/Tooling/mvToolManager.cpp"
	"src/ui/Tooling/mvToolWindow.cpp"
	"src/ui/Tooling/mvAboutWindow.cpp"
	"src/ui/Tooling/mvDocWindow.cpp"
	"src/ui/Tooling/mvMetricsWindow.cpp"
	"src/ui/Tooling/mvStyleWindow.cpp"
	"src/ui/Tooling/mvDebugWindow.cpp"
	"src/ui/Tooling/mvLayoutWindow.cpp"
	"src/ui/AppItems/mvAppItemState.cpp"
	"src/ui/AppItems/mvAppItem.cpp"
	"src/ui/AppItems/mvItemRegistry.cpp"
	"src/ui/AppItems/custom/mvDatePicker.cpp"
	"src/ui/AppItems/custom/mvTimePicker.cpp"
	"src/ui/AppItems/custom/mvSlider3D.cpp"
	"src/ui/AppItems/custom/mvKnob.cpp"
	"src/ui/AppItems/custom/mvLoadingIndicator.cpp"
	"src/ui/AppItems/composite/mvFileDialog.cpp"
	"src/ui/AppItems/composite/mvFileExtension.cpp"

	# imnodes
	"vendor/imnodes/imnodes.cpp"

	# implot
	"vendor/implot/implot.cpp"
	"vendor/implot/implot_items.cpp"
	"vendor/implot/implot_demo.cpp"

	# ImGuiFileDialog
	"vendor/ImGuiFileDialog/ImGuiFileDialog.cpp"

	# imgui
	"../Dependencies/imgui/misc/cpp/imgui_stdlib.cpp"
	"../Dependencies/imgui/imgui.cpp"
	"../Dependencies/imgui/imgui_demo.cpp"
	"../Dependencies/imgui/imgui_draw.cpp"
	"../Dependencies/imgui/imgui_widgets.cpp"
	"../Dependencies/imgui/imgui_tables.cpp"
	"$<$<PLATFORM_ID:Windows>:../Dependencies/imgui/misc/freetype/imgui_freetype.cpp>"
	"$<$<PLATFORM_ID:Windows>:../Dependencies/imgui/backends/imgui_impl_win32.cpp>"
	"$<$<PLATFORM_ID:Windows>:../Dependencies/imgui/backends/imgui_impl_dx11.cpp>"
	"$<$<PLATFORM_ID:Darwin>:../Dependencies/imgui/backends/imgui_impl_metal.mm>"
	"$<$<PLATFORM_ID:Darwin>:../Dependencies/imgui/backends/imgui_impl_glfw.cpp>"
	"$<$<PLATFORM_ID:Darwin>:../Dependencies/imgui/misc/freetype/imgui_freetype.cpp>"
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/backends/imgui_impl_glfw.cpp>"
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/backends/imgui_impl_opengl3.cpp>"
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/examples/libs/gl3w/GL/gl3w.c>"
)

set(MARVEL_INCLUDE_DIR

	"vendor"
	"vendor/implot"
	"vendor/stb"
	"vendor/imnodes"
	"vendor/ImGuiFileDialog"
	"src/"
	"src/core"
	"src/renderer"
	"src/ui"
	"src/modules"
	"src/python/types"
	"src/python/utilities"
	"src/ui/CustomImGuiWidgets"
	"src/ui/AppItems"
	"src/ui/Theming"
	"src/ui/Tooling"
	"../Dependencies/glfw/include/"
	"../Dependencies/glfw/deps/"
	"../Dependencies/imgui/"
	"../Dependencies/gl3w/"
	"../Dependencies/imgui/misc/freetype/"
	"../Dependencies/imgui/backends/"
	"../Dependencies/imgui/examples/libs/gl3w"
	"../Dependencies/cpython/"
	"../Dependencies/cpython/Include/"
	"../Dependencies/cpython/build/debug/"
	"$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/Dependencies/cpython/PC/>"
	"$<IF:$<CONFIG:Release>,${CMAKE_SOURCE_DIR}/Dependencies/cpython/build/release/,${CMAKE_SOURCE_DIR}/Dependencies/cpython/build/debug/>"
)
