
add_definitions(-DWIN32)
set_target_properties(core PROPERTIES SUFFIX ".pyd")
set_target_properties(core PROPERTIES CXX_STANDARD 17)

if(MVPY_VERSION EQUAL 36)

	target_link_libraries(core PUBLIC d3d11 python36)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"C:/Python36-x64"
			"C:/Python36-x64/include"
	)

	target_link_directories(core 
		PRIVATE 
			"C:/Python36-x64"
			"C:/Python36-x64/libs"
			"C:/Python36-x64/DLLs"
	)

elseif(MVPY_VERSION EQUAL 37)

	target_link_libraries(core PUBLIC d3d11 python37)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"C:/Python37-x64"
			"C:/Python37-x64/include"
	)

	target_link_directories(core 
		PRIVATE 
			"C:/Python37-x64"
			"C:/Python37-x64/libs"
			"C:/Python37-x64/DLLs"
	)

elseif(MVPY_VERSION EQUAL 38)
	target_link_libraries(core PUBLIC d3d11 python38)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"C:/Python38-x64"
			"C:/Python38-x64/include"
	)

	target_link_directories(core 
		PRIVATE 
			"C:/Python38-x64"
			"C:/Python38-x64/libs"
			"C:/Python38-x64/DLLs"
	)

elseif(MVPY_VERSION EQUAL 39)
	target_link_libraries(core PUBLIC d3d11 python39)

	target_include_directories(core 
		PRIVATE 
			${MARVEL_INCLUDE_DIR}
			"C:/Python39-x64"
			"C:/Python39-x64/include"
	)

	target_link_directories(core 
		PRIVATE 
			"C:/Python39-x64"
			"C:/Python39-x64/libs"
			"C:/Python39-x64/DLLs"
	)

endif()