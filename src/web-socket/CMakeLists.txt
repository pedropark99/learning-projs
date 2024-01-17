cmake_minimum_required(VERSION 3.2)

project(web_socket C)
add_executable(web_socket main.c)

IF (WIN32)
	target_link_libraries(web_socket -lws2_32)
ENDIF()
