#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 3490

// For windows
#include <winsock.h>

int main()
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct hostent* localhost = gethostbyname("");
	char* localhost_ip = inet_ntoa(*(struct in_addr *)*localhost->h_addr_list);

	// sockaddr_in is the struct to define Ipv4 addresses which are compatible with AF_INET
	// https://www.gnu.org/software/libc/manual/html_node/Internet-Address-Formats.html
	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_addr.s_addr = inet_addr(localhost_ip); // Use the IP of the current machine
	socket_address.sin_port = htons(PORT);

	if (bind(sock, (struct sockaddr*) &socket_address, sizeof(socket_address))) {
		printf("[ERROR]: Error in bind!\n");
		closesocket(sock);
		abort();
	}

	if (listen(sock, 10)) {
		printf("[ERROR]: Error in listen!\n");
		closesocket(sock);
		abort();
	}
	printf("Listening for incoming connections at %d\n", PORT);

	struct sockaddr_in their_addr;
	int client_connected; // file descriptor to access the connection we accepted
	auto addr_size = sizeof(struct sockaddr_in);
	if ((client_connected = accept(sock, (struct sockaddr *) &their_addr, &addr_size))) {
		printf("[INFO]: Client connected!\n");
		printf("[INFO]: IP %s:%d connected!\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port));
	}

	char* msg = "HTTP/1.1 200 OK\nContent-Length: 48\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Hello, World!</h1></body></html>";
	int len, bytes_sent;
	len = strlen(msg);
	bytes_sent = send(client_connected, msg, len, 0);

	int iResult = closesocket(sock);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"[INFO]: closesocket failed with error = %d\n", WSAGetLastError() );
        WSACleanup();
        return 1;
    }

	return 1;
}
