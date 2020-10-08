cmake_minimum_required (VERSION 3.13)

add_executable (DearSandbox)

set_property(TARGET DearSandbox PROPERTY CXX_STANDARD 17)

if(APPLE)
	find_package(Python3 COMPONENTS Interpreter Development)
endif()

target_sources(DearSandbox
	
	PRIVATE

		"$<$<PLATFORM_ID:Windows>:main.cpp>"
		"$<$<PLATFORM_ID:Darwin>:main_linux.cpp>"
		"$<$<PLATFORM_ID:Linux>:main_linux.cpp>"
)

target_include_directories(DearSandbox

	PRIVATE

		"$<$<PLATFORM_ID:Linux>:${CMAKE_SOURCE_DIR}/Dependencies/cpython/debug/include/python3.8d/>"
		"$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/Dependencies/cpython/PC/>"
		"../Dependencies/imgui/"
		"../DearPyGui/src/"
		"../DearPyGui/include/"
		"../Dependencies/cpython/"
		"../Dependencies/cpython/debug/"
		"../Dependencies/cpython/Include/"
)

target_compile_definitions(DearSandbox

	PUBLIC

		$<$<CONFIG:Release>:MV_RELEASE>
)

if (WIN32)

	target_link_directories(DearSandbox PRIVATE "../Dependencies/cpython/PCbuild/amd64/")
	target_link_libraries(DearSandbox PUBLIC coreemb $<$<CONFIG:Debug>:python38_d> $<$<CONFIG:Release>:python38>)

	add_custom_command(TARGET DearSandbox PRE_BUILD
						COMMAND ${CMAKE_COMMAND} -E copy_if_different
							${CMAKE_SOURCE_DIR}/Dependencies/cpython/PCbuild/amd64/python3$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)


	add_custom_command(TARGET DearSandbox PRE_BUILD
						COMMAND ${CMAKE_COMMAND} -E copy_if_different
							${CMAKE_SOURCE_DIR}/Dependencies/cpython/PCbuild/amd64/python38$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)

elseif(APPLE)

	set_property(TARGET DearSandbox APPEND_STRING PROPERTY COMPILE_FLAGS "-Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -arch x86_64 -g")

	target_link_directories(DearSandbox PRIVATE ../Dependencies/cpython/debug)

	add_custom_command(TARGET DearSandbox POST_BUILD
			COMMAND install_name_tool -id @loader_path/../../Dependencies/cpython/debug/libpython3.8d.dylib libpython3.8d.dylib
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Dependencies/cpython/debug
			COMMENT "Changing python lib id"
			)

	add_custom_command(TARGET DearSandbox POST_BUILD
			COMMAND install_name_tool -change /usr/local/libpython3.8d.dylib @loader_path/../../Dependencies/cpython/debug/libpython3.8d.dylib DearSandbox
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake-build-debug/DearSandbox
			COMMENT "Changing python lib load path")

	target_link_libraries(DearSandbox PUBLIC coreemb -ldl "-framework CoreFoundation" "python3.8d")


else() # Linux

	target_link_directories(DearSandbox PRIVATE ../Dependencies/cpython/debug)

	target_link_libraries(DearSandbox

		PUBLIC
			"-lcrypt -lpthread -ldl -lutil -lm"
			coreemb
			python3.8d
	)
endif()