add_executable(server_app
	src/server_app.cpp
	src/server.cpp
)


target_include_directories(server_app
   	PUBLIC 
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_BINARY_DIR}" include/ 
		 ${CMAKE_BINARY_DIR}/_deps/nngpp-src/include/
)


target_link_libraries(server_app
	Poco::Foundation 
	Poco::Util
	mcl
	rot
	nng
	nngpp
)

add_custom_target(runs_f
	COMMAND server_app -i ../../tools/resources/${input_file}_to_ints_bwt_wm.txt -p 12345 -d 10
	COMMENT "================================================== Running Server"
)
