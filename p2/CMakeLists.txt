cmake_minimum_required(VERSION 3.16)
project( Projeto2 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

SET (CMAKE_BUILD_TYPE "Release")
SET (CMAKE_BUILD_TYPE "Debug")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3")
SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

SET (BASE_DIR ${CMAKE_SOURCE_DIR} )
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/build/p2)

add_executable( exercicio1 exercicio1.cpp )
target_link_libraries( exercicio1 ${OpenCV_LIBS} )

add_executable( exercicio2 exercicio2.cpp )
target_link_libraries( exercicio2 ${OpenCV_LIBS} )

add_executable( GolombCoding GolombCoding.cpp)
target_link_libraries( GolombCoding  )

add_executable (bitStream bitStream.cpp)
target_link_libraries (bitStream)

add_executable (Encoder Encoder.cpp)
target_link_libraries (Encoder sndfile)
