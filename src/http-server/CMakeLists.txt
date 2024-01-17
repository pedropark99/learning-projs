cmake_minimum_required(VERSION 3.2)

project(http_server C)
add_executable(http_server main.c)

IF (WIN32)
	target_link_libraries(http_server -lws2_32)
ENDIF()
