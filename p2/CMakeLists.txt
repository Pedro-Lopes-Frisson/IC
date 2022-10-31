cmake_minimum_required(VERSION 3.16)
project( Projeto2 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( exercicio1 exercicio1.cpp )
target_link_libraries( exercicio1 ${OpenCV_LIBS} )

