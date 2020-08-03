cmake_minimum_required (VERSION 3.16)

if(WIN32)

	add_library(dearpygui SHARED)
	set_target_properties(dearpygui PROPERTIES SUFFIX ".pyd")
	set_target_properties(dearpygui PROPERTIES OUTPUT_NAME "dearpygui$<$<CONFIG:Debug>:_d>")
	set_target_properties(dearpygui PROPERTIES CXX_STANDARD 17)

	add_library(dearpyguiemb STATIC)
	set_target_properties(dearpyguiemb PROPERTIES CXX_STANDARD 17)

elseif(APPLE)

	find_package(Python3 COMPONENTS Development)
	add_library(dearpygui SHARED)
	set_target_properties(dearpygui PROPERTIES SUFFIX ".so")
	set_target_properties(dearpygui PROPERTIES PREFIX "")
	set_target_properties(dearpygui PROPERTIES CXX_STANDARD 17)

else()

	add_library(dearpygui SHARED)
	set_target_properties(dearpygui PROPERTIES PREFIX "")
	set_target_properties(dearpygui PROPERTIES CXX_STANDARD 17)

	add_library(dearpyguiemb STATIC)
	set_target_properties(dearpyguiemb PROPERTIES CXX_STANDARD 17)

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

	"$<$<PLATFORM_ID:Darwin>:src/Platform/Apple/mvAppleWindow.mm>"
	"$<$<PLATFORM_ID:Darwin>:src/Platform/Apple/mvAppleUtilities.cpp>"

	"$<$<PLATFORM_ID:Linux>:src/Platform/Linux/mvUtilities.cpp>"
	"$<$<PLATFORM_ID:Linux>:src/Platform/Linux/mvLinuxWindow.cpp>"

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
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/examples/libs/gl3w/GL/gl3w.c>"
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/examples/imgui_impl_glfw.cpp>"
	"$<$<PLATFORM_ID:Linux>:../Dependencies/imgui/examples/imgui_impl_opengl3.cpp>"

)

set(MARVEL_INCLUDE_DIR

	"include/"
	"src/"
	"../Dependencies/glfw/include/"
	"../Dependencies/glfw/deps/"
	"../Dependencies/imgui/"
	"../Dependencies/imgui/examples/"
	"../Dependencies/imgui/examples/libs/gl3w"
	"../Dependencies/cpython/Include/"
	"src/Vendor/implot/"
	"src/Vendor/stb/"

	"/usr/include/python3.8/"

	#new
	"../Dependencies/cpython/debug/"
)




if(WIN32)

	target_sources(dearpygui PRIVATE ${MARVEL_SOURCES} )
	target_sources(dearpyguiemb PRIVATE ${MARVEL_SOURCES} )

	target_include_directories(dearpygui PRIVATE ${MARVEL_INCLUDE_DIR} )
	target_include_directories(dearpyguiemb PRIVATE ${MARVEL_INCLUDE_DIR} )

	if(CONFIG_32BIT)
		target_link_directories(dearpygui PRIVATE "../Dependencies/cpython/PCbuild/win32/")
		target_link_directories(dearpyguiemb PRIVATE "../Dependencies/cpython/PCbuild/win32/")
	else()
		target_link_directories(dearpygui PRIVATE "../Dependencies/cpython/PCbuild/amd64/")
		target_link_directories(dearpyguiemb PRIVATE "../Dependencies/cpython/PCbuild/amd64/")
	endif()

	target_link_libraries(dearpygui

		PUBLIC

			$<$<PLATFORM_ID:Windows>:d3d11>
			$<$<CONFIG:Debug>:python38_d>
			$<$<CONFIG:Release>:python38>
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

elseif(APPLE)

	target_sources(dearpygui PRIVATE ${MARVEL_SOURCES} )
	target_include_directories(dearpygui PRIVATE ${MARVEL_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR})
	target_link_directories(dearpygui PRIVATE ${PYTHON_LIBRARIES})
	SET_PROPERTY(TARGET dearpygui APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc -Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall")

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
else()
	target_sources(dearpygui PRIVATE ${MARVEL_SOURCES} )
	target_sources(dearpyguiemb PRIVATE ${MARVEL_SOURCES} )

	target_include_directories(dearpygui PRIVATE ${MARVEL_INCLUDE_DIR} )
	target_include_directories(dearpyguiemb PRIVATE ${MARVEL_INCLUDE_DIR})

	set_property(TARGET dearpygui APPEND_STRING PROPERTY COMPILE_FLAGS "-fPIC -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall")
	set_property(TARGET dearpyguiemb APPEND_STRING PROPERTY COMPILE_FLAGS "-fPIC -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall")
	add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)

	target_link_libraries(dearpygui
			PUBLIC
			"-L/usr/lib  -lcrypt -lpthread -ldl  -lutil -lm -lm"
			GL
			glfw
			python3.8
			)

	target_link_libraries(dearpyguiemb
			PUBLIC
			"-L/usr/lib  -lcrypt -lpthread -ldl  -lutil -lm -lm"
			GL
			glfw
			python3.8
			)
endif()



target_compile_definitions(dearpygui

	PRIVATE

		$<$<CONFIG:Release>:MV_RELEASE>
)
