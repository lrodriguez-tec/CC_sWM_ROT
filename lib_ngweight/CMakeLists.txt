add_library(libngweight)

target_sources(libngweight
    PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src/bit_array.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/wat_array.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ngweight_util.cpp
)

target_include_directories(libngweight
    PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/
	${PROJECT_BINARY_DIR}
) 

add_executable(ngweight
	${CMAKE_CURRENT_SOURCE_DIR}/src/ngweight.cpp
)

target_include_directories(ngweight
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(ngweight
	libngweight
)

file(COPY resources/ DESTINATION resources/)

#============================================
add_custom_target(runngw 
	COMMAND ngweight -w -s 0 < resources/sample_text > resources/sample_output
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "==============================> Main \n Running: ngweight -w -s 0 < resources/sample_text > resources/sample_output"
)
