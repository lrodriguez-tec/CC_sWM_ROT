
find_package(Protobuf REQUIRED)


protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../proto_files/enc_index.proto)


add_library(rot
	src/WM.cpp
	${PROTO_SRCS}
	${PROTO_HDRS} 
)


target_include_directories(rot
	PUBLIC
		"${PROJECT_BINARY_DIR}"
		include/ 
)


foreach(PROTO_HDR ${PROTO_HDRS})
	get_filename_component(PROTO_HDR_PATH ${PROTO_HDR} DIRECTORY)
	target_include_directories(rot
		PUBLIC
			${PROTO_HDR_PATH}
	)
endforeach()


target_link_libraries(rot
    PUBLIC
        protobuf::libprotobuf
)
