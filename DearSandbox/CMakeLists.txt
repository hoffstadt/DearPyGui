cmake_minimum_required (VERSION 3.16)

add_executable (DearSandbox)

set_property(TARGET DearSandbox PROPERTY CXX_STANDARD 17)

target_sources(DearSandbox
	PRIVATE
		"$<$<PLATFORM_ID:Windows>:main.cpp>"
		"$<$<PLATFORM_ID:Linux>:main_linux.cpp>"
		)

target_include_directories(DearSandbox

	PRIVATE

		"../Dependencies/cpython/Include/"
		"../Dependencies/json/include/"
		"../Dependencies/imgui/"
		"Vendor/CLIUtils/"
		"../DearPyGui/src/"
		"../DearPyGui/include/"
		"$<$<PLATFORM_ID:Linux>:/usr/include/python3.8/>"

)

if (WIN32)

	if(CONFIG_32BIT)
		target_link_directories(DearSandbox PRIVATE "../Dependencies/cpython/PCbuild/win32/")
	else()
		target_link_directories(DearSandbox PRIVATE "../Dependencies/cpython/PCbuild/amd64/")
	endif()

	target_link_libraries(DearSandbox

			PUBLIC

			dearpyguiemb
			nlohmann_json::nlohmann_json
			$<$<CONFIG:Debug>:python38_d>
			$<$<CONFIG:Release>:python38>

			"$<$<PLATFORM_ID:Linux>:-lcrypt -lpthread -ldl  -lutil -lm>"
			)
else()
	target_link_libraries(DearSandbox

			PUBLIC
			"-lcrypt -lpthread -ldl  -lutil -lm"
			dearpyguiemb
			)
endif()

target_compile_definitions(DearSandbox

	PUBLIC

		$<$<CONFIG:Release>:MV_RELEASE>
)

add_custom_command(TARGET DearSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_SOURCE_DIR}/DearSandbox/dearpygui_config.json $<TARGET_FILE_DIR:DearSandbox>)

if (WIN32)
	if(CONFIG_32BIT)
		add_custom_command(TARGET DearSandbox PRE_BUILD
						   COMMAND ${CMAKE_COMMAND} -E copy_directory
							   ${CMAKE_SOURCE_DIR}/Output/python/Debug32/ $<TARGET_FILE_DIR:DearSandbox>/Dependencies)

		add_custom_command(TARGET DearSandbox PRE_BUILD
						   COMMAND ${CMAKE_COMMAND} -E copy_if_different
							   ${CMAKE_SOURCE_DIR}/Output/python/Debug32/python3$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)

		add_custom_command(TARGET DearSandbox PRE_BUILD
						   COMMAND ${CMAKE_COMMAND} -E copy_if_different
							   ${CMAKE_SOURCE_DIR}/Output/python/Debug32/python38$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)
	else()
		add_custom_command(TARGET DearSandbox PRE_BUILD
						   COMMAND ${CMAKE_COMMAND} -E copy_directory
							   ${CMAKE_SOURCE_DIR}/Output/python/Debug/ $<TARGET_FILE_DIR:DearSandbox>/Dependencies)

		add_custom_command(TARGET DearSandbox PRE_BUILD
						   COMMAND ${CMAKE_COMMAND} -E copy_if_different
							   ${CMAKE_SOURCE_DIR}/Output/python/Debug/python3$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)

		add_custom_command(TARGET DearSandbox PRE_BUILD
						   COMMAND ${CMAKE_COMMAND} -E copy_if_different
							   ${CMAKE_SOURCE_DIR}/Output/python/Debug/python38$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)
	endif()
endif()