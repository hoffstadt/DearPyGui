cmake_minimum_required (VERSION 3.16)

add_library(dearpygui SHARED)
set_target_properties(dearpygui PROPERTIES SUFFIX ".pyd")
set_target_properties(dearpygui PROPERTIES OUTPUT_NAME "dearpygui$<$<CONFIG:Debug>:_d>")
set_target_properties(dearpygui PROPERTIES CXX_STANDARD 17)

add_library(dearpyguiemb STATIC)
set_target_properties(dearpyguiemb PROPERTIES CXX_STANDARD 17)

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
	"src/Core/mvMarvel.cpp"

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

	"$<$<PLATFORM_ID:Windows>:src/Platform/Windows/mvWindowsWindow.cpp>"
	"$<$<PLATFORM_ID:Windows>:src/Platform/Windows/mvUtilities.cpp>"

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

)

set(MARVEL_INCLUDE_DIR

	"include/"
	"src/"
	"../Dependencies/imgui/"
	"../Dependencies/imgui/examples/"
	"../Dependencies/cpython/Include/"
	"src/Vendor/implot/"
	"src/Vendor/stb/"
)

target_sources(dearpygui PRIVATE ${MARVEL_SOURCES} )
target_sources(dearpyguiemb PRIVATE ${MARVEL_SOURCES} )

target_include_directories(dearpygui PRIVATE ${MARVEL_INCLUDE_DIR} )
target_include_directories(dearpyguiemb PRIVATE ${MARVEL_INCLUDE_DIR} )

if (WIN32)

	if(CONFIG_32BIT)
		target_link_directories(dearpygui PRIVATE "../Dependencies/cpython/PCbuild/win32/")
		target_link_directories(dearpyguiemb PRIVATE "../Dependencies/cpython/PCbuild/win32/")
	else()
		target_link_directories(dearpygui PRIVATE "../Dependencies/cpython/PCbuild/amd64/")
		target_link_directories(dearpyguiemb PRIVATE "../Dependencies/cpython/PCbuild/amd64/")
	endif()

endif()

target_link_libraries(dearpygui

	PUBLIC

		$<$<PLATFORM_ID:Windows>:d3d11>
		$<$<CONFIG:Debug>:python38_d>
		$<$<CONFIG:Release>:python38>
)

target_compile_definitions(dearpygui

	PRIVATE

		$<$<CONFIG:Release>:MV_RELEASE>
)

target_link_libraries(dearpyguiemb

	PUBLIC

		$<$<PLATFORM_ID:Windows>:d3d11>
		$<$<CONFIG:Debug>:python38_d>
		$<$<CONFIG:Release>:python38>
)

target_compile_definitions(dearpyguiemb

	PUBLIC

		$<$<CONFIG:Release>:MV_RELEASE>
)

