cmake_minimum_required (VERSION 3.16)

add_executable (EmbeddedPython)

target_sources(EmbeddedPython

	PRIVATE

		"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
		


)

target_include_directories(EmbeddedPython

	PRIVATE

		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/cpython/Include/"

)

target_link_directories(EmbeddedPython

	PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/cpython/PCbuild/amd64/"
)

target_link_libraries(EmbeddedPython

	PRIVATE

		python3
)


add_custom_command(TARGET EmbeddedPython PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Resources/python/Debug/ $<TARGET_FILE_DIR:EmbeddedPython>)