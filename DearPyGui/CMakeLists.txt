cmake_minimum_required (VERSION 3.16)

add_library(dearpygui SHARED)

if(NOT MVDIST_ONLY)
	add_library(dearpyguiemb STATIC)
	set_target_properties(dearpyguiemb PROPERTIES CXX_STANDARD 17)
endif()

if(WIN32)

	add_definitions(-DWIN32)
	set_target_properties(dearpygui PROPERTIES SUFFIX ".pyd")
	set_target_properties(dearpygui PROPERTIES OUTPUT_NAME "dearpygui$<$<CONFIG:Debug>:_d>")
	set_target_properties(dearpygui PROPERTIES CXX_STANDARD 17)

elseif(APPLE)

	add_definitions(-DAPPLE)
	add_definitions(-DUNIX)
	find_package(Python3 COMPONENTS Interpreter Development)
	set_target_properties(dearpygui PROPERTIES SUFFIX ".so")
	set_target_properties(dearpygui PROPERTIES PREFIX "")
	set_target_properties(dearpygui PROPERTIES CXX_STANDARD 17)

else() # Linux

	add_definitions(-DLINUX)
	add_definitions(-DUNIX)
	set_target_properties(dearpygui PROPERTIES PREFIX "")
	set_target_properties(dearpygui PROPERTIES CXX_STANDARD 17)

endif()

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
	"$<$<PLATFORM_ID:Linux>:src/Core/mvMarvel.cpp>"
	"$<$<PLATFORM_ID:Darwin>:src/Core/mvMarvel.mm>"

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

	"src/Core/PlotAppItems/mvPlot.cpp"
	"src/Core/PlotAppItems/mvAreaSeries.cpp"

	"$<$<PLATFORM_ID:Windows>:src/Platform/Windows/mvWindowsWindow.cpp>"
	"$<$<PLATFORM_ID:Windows>:src/Platform/Windows/mvUtilities.cpp>"

	"$<$<PLATFORM_ID:Darwin>:src/Platform/Apple/mvAppleWindow.mm>"
	"$<$<PLATFORM_ID:Darwin>:src/Platform/Apple/mvAppleUtilities.mm>"

	"$<$<PLATFORM_ID:Linux>:src/Platform/Linux/mvUtilities.cpp>"
	"$<$<PLATFORM_ID:Linux>:src/Platform/Linux/mvLinuxWindow.cpp>"

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

	"/usr/include/python3.7/"
	"/usr/include/python3.8/"

	# may can delete this
	"../Dependencies/cpython/debug/"
)

target_sources(dearpygui PRIVATE ${MARVEL_SOURCES} )
target_include_directories(dearpygui PRIVATE ${MARVEL_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})

if(NOT MVDIST_ONLY)
	target_sources(dearpyguiemb PRIVATE ${MARVEL_SOURCES} )
	target_include_directories(dearpyguiemb PRIVATE ${MARVEL_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})
endif()

if(WIN32)

	target_link_directories(dearpygui PRIVATE "../Dependencies/cpython/PCbuild/amd64/")
	if(NOT MVDIST_ONLY)
		target_link_directories(dearpyguiemb PRIVATE "../Dependencies/cpython/PCbuild/amd64/")
	endif()

	if(MV37DIST)
		target_link_libraries(dearpygui PUBLIC $<$<PLATFORM_ID:Windows>:d3d11> $<$<CONFIG:Debug>:python37_d> $<$<CONFIG:Release>:python37>)
	else()
		target_link_libraries(dearpygui PUBLIC $<$<PLATFORM_ID:Windows>:d3d11> $<$<CONFIG:Debug>:python38_d> $<$<CONFIG:Release>:python38>)
	endif()

	if(NOT MVDIST_ONLY)
		target_link_libraries(dearpyguiemb PUBLIC $<$<PLATFORM_ID:Windows>:d3d11> $<$<CONFIG:Debug>:python38_d> $<$<CONFIG:Release>:python38>)
	endif()



elseif(APPLE)

	target_link_directories(dearpygui PRIVATE ${PYTHON_LIBRARIES})
	
	SET_PROPERTY(TARGET dearpygui APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc -Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall")

	if(NOT MVDIST_ONLY)

		target_link_directories(dearpyguiemb PRIVATE /Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/)
		
		SET_PROPERTY(TARGET dearpyguiemb APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc -Wunused-function -Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -arch x86_64")
		target_link_libraries(dearpyguiemb

			PUBLIC

			glfw
			#"-framework Python"
			"-framework Metal"
			"-framework MetalKit"
			"-framework Cocoa"
			"-framework CoreVideo"
			"-framework IOKit"
			"-framework QuartzCore"
			)
	endif()

	target_link_libraries(dearpygui

		PRIVATE

			glfw
			"-undefined dynamic_lookup"
			"-framework Metal"
			"-framework MetalKit"
			"-framework Cocoa"
			"-framework CoreVideo"
			"-framework IOKit"
			"-framework QuartzCore"
	)

else() # Linux

	set_property(TARGET dearpygui APPEND_STRING PROPERTY COMPILE_FLAGS "-fPIC -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall")
	
	add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)

	

	if(NOT MVDIST_ONLY)
		set_property(TARGET dearpyguiemb APPEND_STRING PROPERTY COMPILE_FLAGS "-fPIC -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall")
		target_link_libraries(dearpyguiemb PUBLIC "-L/usr/lib  -lcrypt -lpthread -ldl  -lutil -lm -lm" GL glfw python3.8)
	endif()

	if(MV37DIST)
		target_link_directories(dearpygui PRIVATE "/home/runner/work/DearPyGui/DearPyGui/Dependencies/cpython/debug/lib")
		target_link_libraries(dearpygui PUBLIC "-L/usr/lib  -lcrypt -lpthread -ldl  -lutil -lm -lm" GL glfw python3.7m)
	else()
		target_link_directories(dearpygui PRIVATE "/home/runner/work/DearPyGui/DearPyGui/Dependencies/cpython/debug/lib")
		target_link_libraries(dearpygui PUBLIC "-L/usr/lib  -lcrypt -lpthread -ldl  -lutil -lm -lm" GL glfw python3.8)
	endif()


endif()

if(NOT MVDIST_ONLY)
	target_compile_definitions(dearpyguiemb

		PUBLIC

			$<$<CONFIG:Release>:MV_RELEASE>
	)
endif()

target_compile_definitions(dearpygui

	PRIVATE

		$<$<CONFIG:Release>:MV_RELEASE>
)
