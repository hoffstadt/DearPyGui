cmake_minimum_required (VERSION 3.16)

add_executable (MarvelSandbox)

target_sources(MarvelSandbox

	PRIVATE

		"main.cpp"
)

target_include_directories(MarvelSandbox

	PRIVATE

		"../Marvel/include/"

)

target_link_libraries(MarvelSandbox

	PRIVATE

		Marvel

)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Output/python/Debug/ $<TARGET_FILE_DIR:MarvelSandbox>/Dependencies)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_SOURCE_DIR}/MarvelSandbox/marvel_config.json $<TARGET_FILE_DIR:MarvelSandbox>)
