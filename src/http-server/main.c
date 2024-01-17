#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef WIN32
	// For windows
	#include <winsock2.h>
#else
	// For linux
	#include <sys/socket.h>
	#include <netinet/in.h>
#endif



int main(int argc, char *argv[])
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	// sockaddr_in is the struct to define Ipv4 addresses which are compatible with AF_INET
	// https://www.gnu.org/software/libc/manual/html_node/Internet-Address-Formats.html
	struct sockaddr_in socket_address = {
		AF_INET, 
		0x901f, 
		0
	};
	bind(sock, &socket_address, sizeof(socket_address));
	listen(sock, 1);
	int client_input = accept(sock, 0, 0);

	char* buffer[256] = {0};
	recv(client_input, buffer, 256, 0);

	close(sock);
	
	return 1;
}
