cmake_minimum_required (VERSION 3.13)

add_executable (DearCppSandbox)

set_property(TARGET DearCppSandbox PROPERTY CXX_STANDARD 17)

target_sources(DearCppSandbox

	PRIVATE

		"$<$<PLATFORM_ID:Windows>:main.cpp>"
)

target_include_directories(DearCppSandbox

	PRIVATE

		"../Dependencies/imgui/"
		"../DearPyGui/vendor/implot/"
		"../DearPyGui/src/"
		"../DearPyGui/src/core/"
		"../DearPyGui/src/core/AppItems"
		"../DearPyGui/src/core/DrawCommands"
		"../DearPyGui/src/core/Registries"
		"../DearPyGui/src/core/Theming"

)

target_compile_definitions(DearCppSandbox

	PUBLIC

		$<$<CONFIG:Debug>:MV_DEBUG>
		$<$<CONFIG:Release>:MV_RELEASE>
)

if (WIN32)


	set_target_properties(DearCppSandbox
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<$<CONFIG:Release>:/DearSandbox/>$<$<CONFIG:Debug>:/DearSandbox/>"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<$<CONFIG:Release>:/DearSandbox/>$<$<CONFIG:Debug>:/DearSandbox/>"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}$<$<CONFIG:Release>:/DearSandbox/>$<$<CONFIG:Debug>:/DearSandbox/>"
	)

	target_link_libraries(DearCppSandbox PUBLIC coreemb )

elseif(APPLE)

	set_property(TARGET DearCppSandbox APPEND_STRING PROPERTY COMPILE_FLAGS "-Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -arch x86_64 -g")

	target_link_libraries(DearCppSandbox PUBLIC coreemb -ldl "-framework CoreFoundation")


else() # Linux

	target_link_libraries(DearCppSandbox

		PUBLIC
			"-lcrypt -lpthread -ldl -lutil -lm"
			coreemb
	)
endif()