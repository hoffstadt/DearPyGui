
# various settings
set(GLFW_BUILD_DOCS OFF)

if(NOT MVDIST_ONLY)
    add_subdirectory("spdlog")
endif()

if(WIN32)
    add_subdirectory("freetype")
elseif(APPLE)
    add_subdirectory("freetype")
    add_subdirectory ("glfw")
else() #Linux
	add_subdirectory ("glfw")
endif()

###############################################################################
# imgui, implot, imnodes, and ImGuiFileDialog
###############################################################################

if(WIN32)
    add_definitions(-DIMGUI_USER_CONFIG="mvImGuiConfig.h")
elseif(APPLE)
    add_definitions(-DIMGUI_USER_CONFIG="mvImGuiConfig.h")
else() # Linux
	add_definitions(-DIMGUI_USER_CONFIG="mvImGuiLinuxConfig.h")
endif()

# imgui
add_library(imgui STATIC)

target_include_directories(imgui

    PUBLIC

    "glfw/include/"
	"glfw/deps/"
	"imgui/"
	"imgui/misc/freetype/"
	"imgui/backends/"
	"imgui/examples/libs/gl3w"
	"ImGuiFileDialog/ImGuiFileDialog/"
	"ImGuiFileDialog/"
	"../DearPyGui/src/core/"
	"../DearPyGui/vendor/imnodes/"
	"../DearPyGui/vendor/implot"
)

target_sources(imgui
	PRIVATE 
	
	# imnodes
	"../DearPyGui/vendor/imnodes/imnodes.cpp"

	# implot
	"../DearPyGui/vendor/implot/implot.cpp"
	"../DearPyGui/vendor/implot/implot_items.cpp"
	"../DearPyGui/vendor/implot/implot_demo.cpp"

	# ImGuiFileDialog
	"ImGuiFileDialog/ImGuiFileDialog/ImGuiFileDialog.cpp"

	# imgui
	"imgui/misc/cpp/imgui_stdlib.cpp"
	"imgui/imgui.cpp"
	"imgui/imgui_demo.cpp"
	"imgui/imgui_draw.cpp"
	"imgui/imgui_widgets.cpp"
	"imgui/imgui_tables.cpp"
	"$<$<PLATFORM_ID:Windows>:imgui/misc/freetype/imgui_freetype.cpp>"
	"$<$<PLATFORM_ID:Windows>:imgui/backends/imgui_impl_win32.cpp>"
	"$<$<PLATFORM_ID:Windows>:imgui/backends/imgui_impl_dx11.cpp>"
	"$<$<PLATFORM_ID:Darwin>:imgui/backends/imgui_impl_metal.mm>"
	"$<$<PLATFORM_ID:Darwin>:imgui/backends/imgui_impl_glfw.cpp>"
	"$<$<PLATFORM_ID:Darwin>:imgui/misc/freetype/imgui_freetype.cpp>"
	"$<$<PLATFORM_ID:Linux>:imgui/examples/libs/gl3w/GL/gl3w.c>"
	"$<$<PLATFORM_ID:Linux>:imgui/backends/imgui_impl_glfw.cpp>"
	"$<$<PLATFORM_ID:Linux>:imgui/backends/imgui_impl_opengl3.cpp>"
)

# Windows Specifics
if(WIN32)
	target_link_libraries(imgui PUBLIC d3d11 freetype)
	
# Apple Specifics
elseif(APPLE)

	SET_PROPERTY(TARGET imgui APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc -Wunused-function -Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -arch x86_64")
	
	target_link_libraries(imgui

		PUBLIC

			glfw
			freetype
			"-framework Metal"
			"-framework MetalKit"
			"-framework Cocoa"
			"-framework CoreVideo"
			"-framework IOKit"
			"-framework QuartzCore"
			"-framework CoreFoundation"
	)

# Linux Specifics
else() 
	set_property(TARGET imgui APPEND_STRING PROPERTY COMPILE_FLAGS "-fPIC -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O3 -Wall")
	target_link_libraries(imgui PUBLIC "-lcrypt -lpthread -ldl -lutil -lm" GL glfw)

endif()