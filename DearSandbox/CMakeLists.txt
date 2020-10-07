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
						COMMAND ${CMAKE_COMMAND} -E copy_directory
							${CMAKE_SOURCE_DIR}/Output/python/Debug/ $<TARGET_FILE_DIR:DearSandbox>/Dependencies)

	add_custom_command(TARGET DearSandbox PRE_BUILD
						COMMAND ${CMAKE_COMMAND} -E copy_if_different
							${CMAKE_SOURCE_DIR}/Output/python/Debug/python3$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)


	add_custom_command(TARGET DearSandbox PRE_BUILD
						COMMAND ${CMAKE_COMMAND} -E copy_if_different
							${CMAKE_SOURCE_DIR}/Output/python/Debug/python38$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)

elseif(APPLE)

	set_property(TARGET DearSandbox APPEND_STRING PROPERTY COMPILE_FLAGS "-Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -arch x86_64 -g")

	target_link_directories(DearSandbox PRIVATE /Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/config-3.8-darwin)

	target_link_libraries(DearSandbox PUBLIC coreemb "python3.8")


else() # Linux

	target_link_directories(DearSandbox PRIVATE ../Dependencies/cpython/debug)

	target_link_libraries(DearSandbox

		PUBLIC
			"-lcrypt -lpthread -ldl -lutil -lm"
			coreemb
			python3.8d
	)
endif()