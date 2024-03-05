cmake_minimum_required (VERSION 3.13)

add_executable (DearSandbox)

set_target_properties(DearSandbox
  PROPERTIES
  CXX_STANDARD 17
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}$<$<CONFIG:Release>:/cmake-build-release/>$<$<CONFIG:Debug>:/cmake-build-debug/>DearSandbox/"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}$<$<CONFIG:Release>:/cmake-build-release/>$<$<CONFIG:Debug>:/cmake-build-debug/>DearSandbox/"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}$<$<CONFIG:Release>:/cmake-build-release/>$<$<CONFIG:Debug>:/cmake-build-debug/>DearSandbox/"
  )

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

		"$<$<PLATFORM_ID:Linux>:${CMAKE_SOURCE_DIR}/thirdparty/cpython/build/debug/include/python3.9d/>"
		"$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/thirdparty/cpython/PC/>"
		"../thirdparty/imgui/"
		"../thirdparty/imgui/backends/"
		"../thirdparty/cpython/"
		"../thirdparty/cpython/build/debug/"
		"../thirdparty/cpython/Include/"
		"../thirdparty/implot/"
		"../src/"
)

target_compile_definitions(DearSandbox

	PUBLIC

		$<$<CONFIG:Debug>:MV_DEBUG>
		$<$<CONFIG:Release>:MV_RELEASE>
)

if (WIN32)

	target_link_directories(DearSandbox PRIVATE "../thirdparty/cpython/PCbuild/amd64/")
	target_link_libraries(DearSandbox PUBLIC coreemb $<$<CONFIG:Debug>:python39_d> $<$<CONFIG:Release>:python39>)

	add_custom_command(TARGET DearSandbox PRE_BUILD
						COMMAND ${CMAKE_COMMAND} -E copy_if_different
							${CMAKE_SOURCE_DIR}/thirdparty/cpython/PCbuild/amd64/python3$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)


	add_custom_command(TARGET DearSandbox PRE_BUILD
						COMMAND ${CMAKE_COMMAND} -E copy_if_different
							${CMAKE_SOURCE_DIR}/thirdparty/cpython/PCbuild/amd64/python39$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:DearSandbox>)

elseif(APPLE)

	set_property(TARGET DearSandbox APPEND_STRING PROPERTY COMPILE_FLAGS "-fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -arch x86_64 -g")

    target_link_directories(DearSandbox PRIVATE ../thirdparty/cpython/build/debug/lib)

	target_link_libraries(DearSandbox PUBLIC coreemb -ldl "-framework CoreFoundation" "python3.9d")

	file(GLOB PYTHON_LIBS_PATH "../thirdparty/cpython/build/debug/lib/python*")

	add_compile_definitions(PYTHON_LIBS_PATH="${PYTHON_LIBS_PATH}")

else() # Linux

	target_link_directories(DearSandbox PRIVATE ../thirdparty/cpython/build/debug)

	target_link_libraries(DearSandbox

		PUBLIC
			"-lcrypt -lpthread -ldl -lutil -lm"
			coreemb
			python3.9d
	)
endif()