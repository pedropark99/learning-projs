#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// For linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3490

int linux_web_socket()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	// Functions `htonl()` and `htons()` are necessary to make sure that the bits that compose
	// the integer values are ordered accordingly to the network byte order, and not the byte order
	// native to the current machine.
	uint16_t port = htons(PORT);


	// sockaddr_in is the struct to define Ipv4 addresses which are compatible with AF_INET
	// https://www.gnu.org/software/libc/manual/html_node/Internet-Address-Formats.html
	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_addr.s_addr = htonl(INADDR_ANY); // Use the IP of the current machine
	socket_address.sin_port = port;
	memset(&(socket_address.sin_zero), '\0', 8);
	
	// bind() binds a name to the socket. Every socket we create is initially unnamed.
	// bind gives it a name which is represented by the triad of internet protocol, IP and port.
	if ( bind(sock, (struct sockaddr *) &socket_address, sizeof(socket_address)) ) {
		printf("[ERROR]: Error in bind!\n");
		close(sock);
		abort();
	}

	// Makes the socket be ready for incoming connections.
	if ( listen(sock, 10) ) {
		printf("[ERROR]: Error in listen!\n");
		close(sock);
		abort();
	}
	printf("Listening for incoming connections at %d\n", PORT);

	struct sockaddr_in their_addr; // connector's address information
	int client_connected; // file descriptor to access the connection we accepted
	socklen_t sock_addr_size = sizeof(struct sockaddr_in);

	if ((client_connected = accept(sock, (struct sockaddr *)&their_addr, &sock_addr_size))) {
		printf("[INFO]: Client connected!\n");
		printf("[INFO]: IP %s:%d connected!\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port));
	}

	char* msg = "HTTP/1.1 200 OK\nContent-Length: 48\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Hello, World!</h1></body></html>";
	int len, bytes_sent;
	len = strlen(msg);
	bytes_sent = send(client_connected, msg, len, 0);
	
	close(client_connected);
	close(sock);
	
	return 1;
}