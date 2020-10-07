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

		"$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/Dependencies/cpython/PC/>"
		"../Dependencies/imgui/"
		"../DearPyGui/src/"
		"../DearPyGui/include/"
		"../Dependencies/cpython/"
		"../Dependencies/cpython/Include/"
		if(MV37DIST)
			"$<$<PLATFORM_ID:Linux>:/usr/include/python3.7/>"
		else()
			"$<$<PLATFORM_ID:Linux>:/usr/include/python3.8/>"
		endif()
)

target_compile_definitions(DearSandbox

	PUBLIC

		$<$<CONFIG:Release>:MV_RELEASE>
)

if (WIN32)

	target_link_directories(DearSandbox PRIVATE "../Dependencies/cpython/PCbuild/amd64/")
	
	if(MV37DIST)
		target_link_libraries(DearSandbox PUBLIC coreemb $<$<CONFIG:Debug>:python37_d> $<$<CONFIG:Release>:python37>)
	else()
		target_link_libraries(DearSandbox PUBLIC coreemb $<$<CONFIG:Debug>:python38_d> $<$<CONFIG:Release>:python38>)
	endif()


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

	if(MV37DIST)
		target_link_directories(DearSandbox PRIVATE /Library/Frameworks/Python.framework/Versions/3.7/lib/python3.7/config-3.7-darwin)
	else()
		target_link_directories(DearSandbox PRIVATE /Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/config-3.8-darwin)
	endif()

	if(MV37DIST)
		target_link_libraries(DearSandbox PUBLIC coreemb "python3.7")
	else()
		target_link_libraries(DearSandbox PUBLIC coreemb "python3.8")
	endif()

else() # Linux

	target_link_libraries(DearSandbox

		PUBLIC
			"-lcrypt -lpthread -ldl  -lutil -lm"
			coreemb
	)
endif()