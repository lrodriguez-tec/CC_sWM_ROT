cmake_minimum_required(VERSION 3.20)

project(CC_sWM_ROT VERSION 4.0.0 DESCRIPTION "Conformance Checnking with secure Wavelet Matrix")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(CMAKE_BUILD_TYPE Debug)

configure_file(server/include/config.hpp.in config.hpp)

################################################## Libraries

find_package(Poco REQUIRED COMPONENTS Foundation Util)
find_package(nanomsg CONFIG REQUIRED)
find_package(Protobuf REQUIRED)
find_package(nng CONFIG REQUIRED)

######################################## Fetch git cprojects
   include(FetchContent)
   
   FetchContent_Declare(mcl
     GIT_REPOSITORY git@github.com:herumi/mcl.git
     GIT_TAG v1.86.0
   )
   
   FetchContent_Declare(cybozu
     GIT_REPOSITORY git@github.com:herumi/cybozulib.git
     GIT_TAG release20210218
   )

   FetchContent_Declare(nanomsgxx
	   GIT_REPOSITORY git@github.com:achille-roussel/nanomsgxx.git
	   #GIT_REPOSITORY git@github.com:noahness/nanomsgxx.git
	   #GIT_TAG feature/export_target_cmake
   )

   FetchContent_Declare(nngpp
		GIT_REPOSITORY git@github.com:cwzx/nngpp.git 
		GIT_TAG nng-v1.3.0
   )

   FetchContent_MakeAvailable(mcl cybozu nanomsgxx nngpp)
##################################################

include_directories(${CMAKE_BINARY_DIR}/_deps/nngpp-src/include/)

include_directories(${Protobuf_INCLUDE_DIRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto_files/enc_index.proto)


add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(tests)
add_subdirectory(rot_lib)
add_subdirectory(tools)			#tools to generate BWT, WM and RankCF.
add_subdirectory(lib_ngweight)	#library SA used only for testings and tools
