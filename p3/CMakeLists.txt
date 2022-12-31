cmake_minimum_required(VERSION 3.16)
project( Projeto3 )

SET (CMAKE_BUILD_TYPE "Release")
SET (CMAKE_BUILD_TYPE "Debug")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3")
#SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

SET(BOOST_ROOT "/usr/lib/")
SET(BOOST_INCLUDEDIR "/usr/include")

find_package(Boost 1.55 REQUIRED system serialization)


SET (BASE_DIR ${CMAKE_SOURCE_DIR})
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/build/p3)

add_library(fcm fcm.h fcm.cpp)
TARGET_LINK_LIBRARIES(fcm ${Boost_LIBRARIES})


add_executable( fcm_test fcm_test.cpp )
target_link_libraries( fcm_test fcm )




add_library(fcm_novo fcm_novo.h fcm_novo.cpp)
TARGET_LINK_LIBRARIES(fcm_novo ${Boost_LIBRARIES})



add_executable( fcm_test_novo fcm_test_novo.cpp )
target_link_libraries( fcm_test_novo fcm_novo )


add_executable( findlang_novo findlang_novo.cpp )
target_link_libraries( findlang_novo fcm_novo )


add_executable( lang_novo lang_novo.cpp )
target_link_libraries( lang_novo fcm_novo )


add_executable( locatelang locatelang.cpp )
target_link_libraries( locatelang fcm_novo )

