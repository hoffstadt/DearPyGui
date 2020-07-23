cmake_minimum_required (VERSION 3.16)

add_executable (DearSandbox)

set_property(TARGET DearSandbox PROPERTY CXX_STANDARD 17)

target_sources(DearSandbox PRIVATE "main.cpp")

target_include_directories(DearSandbox

	PRIVATE

		"../Dependencies/cpython/Include/"
		"../Dependencies/json/include/"
		"../Dependencies/imgui/"
		"Vendor/CLIUtils/"
		"../DearPyGui/src/"
		"../DearPyGui/include/"
)

target_link_directories(DearSandbox

	PUBLIC
		"../Dependencies/cpython/PCbuild/amd64/"
)

target_link_libraries(DearSandbox

	PUBLIC

		dearpyguiemb
		nlohmann_json::nlohmann_json
		$<$<CONFIG:Debug>:python38_d>
		$<$<CONFIG:Release>:python38>
)

target_compile_definitions(DearSandbox

	PUBLIC

		$<$<CONFIG:Release>:MV_RELEASE>
)

add_custom_command(TARGET DearSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Output/python/Debug/ $<TARGET_FILE_DIR:DearSandbox>/Dependencies)

add_custom_command(TARGET DearSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${CMAKE_SOURCE_DIR}/Output/python/Debug/python3$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)

add_custom_command(TARGET DearSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${CMAKE_SOURCE_DIR}/Output/python/Debug/python38$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)
