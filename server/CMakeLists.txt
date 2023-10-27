add_executable(server_app
	src/server_app.cpp
	src/server.cpp
)

target_include_directories(server_app
   	PUBLIC 
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_BINARY_DIR}" include/ 
)

target_link_libraries(server_app
	Poco::Foundation 
	Poco::Util
	mcl
	nnxx
	rot
)

file(COPY resources/ DESTINATION resources/)

add_custom_target(runs
	COMMAND server_app -i resources/sample.wm -p 12345
	COMMENT "================================================== Running Server"
)
