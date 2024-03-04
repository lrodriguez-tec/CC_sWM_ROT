add_executable(bwt
	${CMAKE_CURRENT_SOURCE_DIR}/src/bwt.cpp
)

add_executable(chtoi
	${CMAKE_CURRENT_SOURCE_DIR}/src/chtoi.cpp
)

add_executable(makeWM
	${CMAKE_CURRENT_SOURCE_DIR}/src/wavelet.cpp
)

add_executable(my_sa_bwt
	${CMAKE_CURRENT_SOURCE_DIR}/src/my_sa_bwt.cpp
)

target_link_libraries(my_sa_bwt
	libngweight
	Poco::Util
	Poco::Foundation
)

add_custom_target(run_toi
	COMMAND my_sa_bwt resources/${input_file}.txt
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "==============================> Main \n Running: my_sa_bwt ${input_file}.txt"
)

add_custom_target(run_bwt
	COMMAND bwt < resources/${input_file}_to_ints.txt > resources/${input_file}_to_ints_bwt.txt
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "==============================> Main \n Running: bwt (results written on: resources/sequence_01_to_ints_bwt.txt"
)

add_custom_target(run_wm
	COMMAND makeWM < resources/${input_file}_to_ints_bwt.txt > resources/${input_file}_to_ints_bwt_wm.txt
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "==============================> Main \n Running: makeWM (results written on: resources/sequence_01_to_ints_bwt_wm.txt"
)
