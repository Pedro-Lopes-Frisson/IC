cmake_minimum_required(VERSION 3.16)
project( Projeto3 )

SET (CMAKE_BUILD_TYPE "Release")
SET (CMAKE_BUILD_TYPE "Debug")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3")
#SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

SET (BASE_DIR ${CMAKE_SOURCE_DIR})
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/build/p3)

add_library(fcm fcm.h fcm.cpp)

add_executable( fcm_test fcm_test.cpp )
target_link_libraries( fcm_test fcm )

add_executable( findlang findlang.cpp )
target_link_libraries( findlang fcm )

add_executable( lang lang.cpp )
target_link_libraries( lang fcm )

add_executable( locatelang locatelang.cpp )
target_link_libraries( locatelang fcm )
