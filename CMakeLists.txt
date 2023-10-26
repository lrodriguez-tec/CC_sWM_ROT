cmake_minimum_required(VERSION 3.20)

project(CC_sWM_ROT VERSION 3.2.1 DESCRIPTION "Conformance Checnking with secure Wavelet Matrix")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(CMAKE_BUILD_TYPE Debug)

find_package(Poco REQUIRED COMPONENTS Foundation Util)
find_package(nanomsg CONFIG REQUIRED)

####################################################### Fetch git cprojects libraries
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

   FetchContent_MakeAvailable(mcl cybozu nanomsgxx)
####################################################### git projects libraries

add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(tests)

configure_file(server/include/config.hpp.in config.hpp)


