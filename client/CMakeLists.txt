add_executable(client_app
	src/client_app.cpp
	src/client.cpp
)

target_include_directories(client_app
   	PUBLIC 
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_BINARY_DIR}" include/ 
		${CMAKE_BINARY_DIR}/_deps/nngpp-src/include/
)

target_link_libraries(client_app
	Poco::Foundation 
	Poco::Util
	mcl
	rot
	nng
	nngpp
)

file(COPY resources/ DESTINATION resources/)


add_custom_target(runc_f
	COMMAND client_app -i resources/${input_file}_query.txt -p 12345
	COMMENT "================================================== Running Client"
)
