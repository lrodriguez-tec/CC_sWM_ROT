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

add_custom_target(runc
	COMMAND client_app -i resources/query.txt -p 12345
	COMMENT "================================================== Running Client"
)

add_custom_target(runc_01
	COMMAND client_app -i resources/query_01.txt -p 12345
	COMMENT "================================================== Running Client"
)

add_custom_target(runc_f
	COMMAND client_app -i resources/${input_file}_query.txt -p 12345
	COMMENT "================================================== Running Client"
)

#######################################################
add_executable(client_times_app
	src/client_times_app.cpp
	src/client.cpp
)

target_include_directories(client_times_app
   	PUBLIC 
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_BINARY_DIR}" include/ 
		${CMAKE_BINARY_DIR}/_deps/nngpp-src/include/
)

target_link_libraries(client_times_app
	Poco::Foundation 
	Poco::Util
	mcl
	rot
	nng
	nngpp
)

add_custom_target(runc_tf
	COMMAND client_times_app -i resources/${input_file}_query.txt -p 12345
	COMMENT "================================================== Running times Client"
)

add_custom_command(OUTPUT FOO2.out
	COMMAND client_times_app -i resources/${input_file}_query_02.txt -p 12345
)

add_custom_command(OUTPUT FOO4.out
	COMMAND client_times_app -i resources/${input_file}_query_04.txt -p 12345
)

add_custom_command(OUTPUT FOO6.out
	COMMAND client_times_app -i resources/${input_file}_query_06.txt -p 12345
)

add_custom_command(OUTPUT FOO8.out
	COMMAND client_times_app -i resources/${input_file}_query_08.txt -p 12345
)

add_custom_target(run_simple_01_times
	${CMAKE_COMMAND} -E echo "Executing simple_01 times" 
	DEPENDS FOO2.out FOO4.out FOO6.out FOO8.out
)

##==================================================

add_custom_command(OUTPUT A02.out
	COMMAND client_times_app -i resources/${input_file}_query_02.txt -p 12345
)
add_custom_command(OUTPUT A04.out
	COMMAND client_times_app -i resources/${input_file}_query_04.txt -p 12345
)
add_custom_command(OUTPUT A06.out
	COMMAND client_times_app -i resources/${input_file}_query_06.txt -p 12345
)
add_custom_command(OUTPUT A08.out
	COMMAND client_times_app -i resources/${input_file}_query_08.txt -p 12345
)
add_custom_command(OUTPUT A10.out
	COMMAND client_times_app -i resources/${input_file}_query_10.txt -p 12345
)
add_custom_command(OUTPUT A12.out
	COMMAND client_times_app -i resources/${input_file}_query_12.txt -p 12345
)
add_custom_command(OUTPUT A14.out
	COMMAND client_times_app -i resources/${input_file}_query_14.txt -p 12345
)
add_custom_command(OUTPUT A16.out
	COMMAND client_times_app -i resources/${input_file}_query_16.txt -p 12345
)

add_custom_target(run_AO_Sequence_times
	${CMAKE_COMMAND} -E echo "Executing AO_Sequence times" 
	#DEPENDS A02.out A04.out A06.out A08.out A10.out A12.out A14.out A16.out
	#DEPENDS A10.out A12.out A14.out A16.out
	DEPENDS A16.out
)
