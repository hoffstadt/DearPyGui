cmake_minimum_required (VERSION 3.16)

add_executable (MarvelSandbox)

set_property(TARGET MarvelSandbox PROPERTY CXX_STANDARD 17)

target_sources(MarvelSandbox

	PRIVATE

		"main.cpp"
        "../Marvel/MarvelApplication.cpp"
)

target_include_directories(MarvelSandbox

	PRIVATE

		"../Marvel/include/"

)

target_link_directories(MarvelSandbox

	PRIVATE

		"../cmake-build-release/Marvel/"

)

target_link_libraries(MarvelSandbox

	PRIVATE

		marvel

)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Output/python/Debug/ $<TARGET_FILE_DIR:MarvelSandbox>/Dependencies)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_SOURCE_DIR}/MarvelSandbox/marvel_config.json $<TARGET_FILE_DIR:MarvelSandbox>)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${CMAKE_SOURCE_DIR}/Output/python/Debug/python3$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:MarvelSandbox>)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${CMAKE_SOURCE_DIR}/Output/python/Debug/python38$<$<CONFIG:Debug>:_d>.dll $<TARGET_FILE_DIR:MarvelSandbox>)

add_custom_command(TARGET MarvelSandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${CMAKE_SOURCE_DIR}/cmake-build-release/Marvel/marvel.pyd $<TARGET_FILE_DIR:MarvelSandbox>/Dependencies)
