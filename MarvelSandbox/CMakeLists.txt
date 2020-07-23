cmake_minimum_required (VERSION 3.16)

add_executable (MarvelSandbox)

set_property(TARGET MarvelSandbox PROPERTY CXX_STANDARD 17)

target_sources(MarvelSandbox PRIVATE "main.cpp")

target_include_directories(MarvelSandbox

	PRIVATE

		"../Dependencies/cpython/Include/"
		"../Dependencies/json/include/"
		"../Dependencies/imgui/"
		"Vendor/CLIUtils/"
		"../Marvel/src/"
		"../Marvel/include/"
)

target_link_directories(MarvelSandbox

	PUBLIC
		"../Dependencies/cpython/PCbuild/amd64/"
)

target_link_libraries(MarvelSandbox

	PUBLIC

		marvelembedded
		nlohmann_json::nlohmann_json
		$<$<CONFIG:Debug>:python38_d>
		$<$<CONFIG:Release>:python38>
)

target_compile_definitions(MarvelSandbox

	PUBLIC

		$<$<CONFIG:Release>:MV_RELEASE>
)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Output/python/Debug/ $<TARGET_FILE_DIR:MarvelSandbox>/Dependencies)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${CMAKE_SOURCE_DIR}/Output/python/Debug/python3$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:MarvelSandbox>)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${CMAKE_SOURCE_DIR}/Output/python/Debug/python38$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:MarvelSandbox>)
