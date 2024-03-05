cmake_minimum_required (VERSION 3.13)


set(MARVEL_SOURCES

        # core
        "mvContext.cpp"
        "mvMath.cpp"
        "mvProfiler.cpp"
        "dearpygui.cpp"

        # platform
        "$<$<PLATFORM_ID:Windows>:mvViewport_win32.cpp>"
        "$<$<PLATFORM_ID:Windows>:mvUtilities_win32.cpp>"
        "$<$<PLATFORM_ID:Windows>:mvGraphics_win32.cpp>"

        "$<$<PLATFORM_ID:Darwin>:mvViewport_apple.mm>"
        "$<$<PLATFORM_ID:Darwin>:mvUtilities_apple.mm>"
        "$<$<PLATFORM_ID:Darwin>:mvGraphics_apple.mm>"

        "$<$<PLATFORM_ID:Linux>:mvUtilities_linux.cpp>"
        "$<$<PLATFORM_ID:Linux>:mvViewport_linux.cpp>"
        "$<$<PLATFORM_ID:Linux>:mvGraphics_linux.cpp>"

        # python
        "mvPyUtils.cpp"
        "mvCustomTypes.cpp"

        # ui
        "mvBasicWidgets.cpp"
        "mvTables.cpp"
        "mvThemes.cpp"
        "mvNodes.cpp"
        "mvDrawings.cpp"
        "mvGlobalHandlers.cpp"
        "mvItemHandlers.cpp"
        "mvValues.cpp"
        "mvTextureItems.cpp"
        "mvFontItems.cpp"
        "mvColors.cpp"
        "mvPlotting.cpp"
        "mvContainers.cpp"

        "mvCallbackRegistry.cpp"
        "mvLoadingIndicatorCustom.cpp"
        "mvFontManager.cpp"
        "mvToolManager.cpp"
        "mvToolWindow.cpp"
        "mvAboutWindow.cpp"
        "mvDocWindow.cpp"
        "mvMetricsWindow.cpp"
        "mvStackWindow.cpp"
        "mvStyleWindow.cpp"
        "mvDebugWindow.cpp"
        "mvLayoutWindow.cpp"
        "mvAppItemState.cpp"
        "mvAppItem.cpp"
        "mvItemRegistry.cpp"

        "mvDatePicker.cpp"
        "mvTimePicker.cpp"
        "mvSlider3D.cpp"
        "mvLoadingIndicator.cpp"
        "mvFileDialog.cpp"
        "mvFileExtension.cpp"

        # imnodes
        "../thirdparty/imnodes/imnodes.cpp"

        # implot
        "../thirdparty/implot/implot.cpp"
        "../thirdparty/implot/implot_items.cpp"
        "../thirdparty/implot/implot_demo.cpp"

        # ImGuiFileDialog
        "../thirdparty/ImGuiFileDialog/ImGuiFileDialog.cpp"

        # imgui
        "../thirdparty/imgui/misc/cpp/imgui_stdlib.cpp"
        "../thirdparty/imgui/imgui.cpp"
        "../thirdparty/imgui/imgui_demo.cpp"
        "../thirdparty/imgui/imgui_draw.cpp"
        "../thirdparty/imgui/imgui_widgets.cpp"
        "../thirdparty/imgui/imgui_tables.cpp"
        "$<$<PLATFORM_ID:Windows>:../thirdparty/imgui/misc/freetype/imgui_freetype.cpp>"
        "$<$<PLATFORM_ID:Windows>:../thirdparty/imgui/backends/imgui_impl_win32.cpp>"
        "$<$<PLATFORM_ID:Windows>:../thirdparty/imgui/backends/imgui_impl_dx11.cpp>"
        "$<$<PLATFORM_ID:Darwin>:../thirdparty/imgui/backends/imgui_impl_metal.mm>"
        "$<$<PLATFORM_ID:Darwin>:../thirdparty/imgui/backends/imgui_impl_glfw.cpp>"
        "$<$<PLATFORM_ID:Linux>:../thirdparty/imgui/backends/imgui_impl_glfw.cpp>"
        "$<$<PLATFORM_ID:Linux>:../thirdparty/imgui/backends/imgui_impl_opengl3.cpp>"

        # gl3w
        "$<$<PLATFORM_ID:Linux>:../thirdparty/gl3w/GL/gl3w.c>"
        )

set(MARVEL_INCLUDE_DIR
        "."
        "../thirdparty/implot"
        "../thirdparty/stb"
        "../thirdparty/imnodes"
        "../thirdparty/ImGuiFileDialog"
        "../thirdparty/glfw/include/"
        "../thirdparty/glfw/deps/"
        "../thirdparty/imgui/"
        "../thirdparty/gl3w/"
        "../thirdparty/imgui/misc/freetype/"
        "../thirdparty/imgui/backends/"
        "../thirdparty/cpython/"
        "../thirdparty/cpython/Include/"
        "../thirdparty/cpython/build/debug/"
        "$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/thirdparty/cpython/PC/>"
        "$<IF:$<CONFIG:Release>,${CMAKE_SOURCE_DIR}/thirdparty/cpython/build/release/,${CMAKE_SOURCE_DIR}/thirdparty/cpython/build/debug/>"
        )


if(MVDIST_ONLY)
    include("distribution.cmake")
else()
    include("embedded.cmake")
endif()