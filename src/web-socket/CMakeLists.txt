cmake_minimum_required(VERSION 3.2)

project(web_socket C)

IF (WIN32)
	add_executable(web_socket web_socket_windows.c)
	target_link_libraries(web_socket -lws2_32)
ELSE()
	add_executable(web_socket web_socket_linux.c)
ENDIF()
