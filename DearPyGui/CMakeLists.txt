cmake_minimum_required (VERSION 3.16)

FIND_PACKAGE(Python3 COMPONENTS Development)

add_library(dearpygui SHARED)
set_target_properties(dearpygui PROPERTIES SUFFIX ".so")
SET_TARGET_PROPERTIES(dearpygui PROPERTIES PREFIX "")
#set_target_properties(dearpygui PROPERTIES OUTPUT_NAME "dearpygui$<$<CONFIG:Debug>:_d>")
set_target_properties(dearpygui PROPERTIES CXX_STANDARD 17)

set(MARVEL_SOURCES

	"src/Core/mvApp.cpp"
	"src/Core/mvAppLog.cpp"
	"src/Core/mvInput.cpp"
	"src/Core/mvWindow.cpp"
	"src/Core/mvTextEditor.cpp"
	"src/Core/mvDataStorage.cpp"
	"src/Core/mvTextureStorage.cpp"
	"src/Core/mvPythonTranslator.cpp"
	"src/Core/mvPythonParser.cpp"
	"$<$<PLATFORM_ID:Windows>:src/Core/mvMarvel.cpp>"
	"$<$<PLATFORM_ID:Darwin>:src/Core/mvMarvel.mm>"

	"src/Core/StandardWindows/mvStandardWindow.cpp"
	"src/Core/StandardWindows/mvAppEditor.cpp"
	"src/Core/StandardWindows/mvDocWindow.cpp"
	"src/Core/StandardWindows/mvAboutWindow.cpp"
	"src/Core/StandardWindows/mvMetricsWindow.cpp"
	"src/Core/StandardWindows/mvSourceWindow.cpp"
	"src/Core/StandardWindows/mvDebugWindow.cpp"

	"src/Core/AppItems/mvAppItem.cpp"
	"src/Core/AppItems/mvDrawing.cpp"
	"src/Core/AppItems/mvTable.cpp"

	#"$<$<PLATFORM_ID:Windows>:src/Platform/Windows/mvWindowsWindow.cpp>"
	"$<$<PLATFORM_ID:Windows>:src/Platform/Windows/mvUtilities.cpp>"

	"$<$<PLATFORM_ID:Darwin>:src/Platform/Apple/mvAppleWindow.mm>"
	"$<$<PLATFORM_ID:Darwin>:src/Platform/Apple/mvAppleUtilities.cpp>"

	# implot
	"src/Vendor/implot/implot.cpp"
	"src/Vendor/implot/implot_demo.cpp"

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

)

set(MARVEL_INCLUDE_DIR

	"include/"
	"src/"
	"../Dependencies/glfw/include/"
	"../Dependencies/imgui/"
	"../Dependencies/imgui/examples/"
	#"../Dependencies/cpython/Include/"
	"src/Vendor/implot/"
	"src/Vendor/stb/"
)



target_sources(dearpygui PRIVATE ${MARVEL_SOURCES} )
#target_include_directories(dearpygui PRIVATE ${MARVEL_INCLUDE_DIR} "../Output/Python.framework/Versions/3.8/include/python3.8")
target_include_directories(dearpygui PRIVATE ${MARVEL_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR})
target_link_directories(dearpygui
		PRIVATE
		${PYTHON_LIBRARIES})
SET_PROPERTY(TARGET dearpygui APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc -Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall")

target_link_libraries(dearpygui

	PRIVATE

		glfw
		"-undefined dynamic_lookup"
		#python3.8
		#Python3::Python
		#"-framework Python"
		"-framework Metal"
		"-framework MetalKit"
		"-framework Cocoa"
		"-framework CoreVideo"
		"-framework IOKit"
		"-framework QuartzCore"
)

target_compile_definitions(dearpygui

	PRIVATE

		$<$<CONFIG:Release>:MV_RELEASE>
)
