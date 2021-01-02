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
		"../DearPyGui/vendor/implot/"
		"../DearPyGui/src/"
		"../DearPyGui/src/core/"
		"../DearPyGui/src/core/AppItems"
		"../DearPyGui/src/core/DrawCommands"
		"../DearPyGui/src/core/PythonCommands"
		"../DearPyGui/src/core/PythonUtilities"
		"../DearPyGui/src/core/Registries"
		"../DearPyGui/src/core/Theming"
		"../Dependencies/cpython/"
		"../Dependencies/cpython/debug/"
		"../Dependencies/cpython/Include/"
)

target_compile_definitions(DearSandbox

	PUBLIC

		$<$<CONFIG:Debug>:MV_DEBUG>
		$<$<CONFIG:Release>:MV_RELEASE>
)

if (WIN32)


	set_target_properties(DearSandbox
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<$<CONFIG:Release>:/DearSandbox/>$<$<CONFIG:Debug>:/DearSandbox/>"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<$<CONFIG:Release>:/DearSandbox/>$<$<CONFIG:Debug>:/DearSandbox/>"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<$<CONFIG:Release>:/DearSandbox/>$<$<CONFIG:Debug>:/DearSandbox/>"
	)

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

    target_link_directories(DearSandbox PRIVATE ../Dependencies/cpython/debug/lib)

	target_link_libraries(DearSandbox PUBLIC coreemb -ldl "-framework CoreFoundation" "python3.8d")

	file(GLOB PYTHON_LIBS_PATH "../Dependencies/cpython/debug/lib/python*")

	add_compile_definitions(PYTHON_LIBS_PATH="${PYTHON_LIBS_PATH}")

else() # Linux

	target_link_directories(DearSandbox PRIVATE ../Dependencies/cpython/debug)

	target_link_libraries(DearSandbox

		PUBLIC
			"-lcrypt -lpthread -ldl -lutil -lm"
			coreemb
			python3.8d
	)
endif()