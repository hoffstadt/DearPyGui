cmake_minimum_required (VERSION 3.13)

add_executable(marvel_d3d11 WIN32)
set_property(TARGET marvel_d3d11 PROPERTY CXX_STANDARD 17)

target_sources(marvel_d3d11

	PRIVATE

		"main.cpp"

)

target_include_directories(marvel_d3d11

	PRIVATE

		"./shaders"

)

target_link_libraries(marvel_d3d11
	PRIVATE
		d3d11
		d3dcompiler
)