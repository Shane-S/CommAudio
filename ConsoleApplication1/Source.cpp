#include <stdio.h>
#include <winsock2.h>
#include <errno.h>
//#include <string.h>
//#include <memory.h>

#define SERVER_TCP_PORT			7000	// Default port
#define BUFSIZE					4308992	// Buffer length

int main(int argc, char **argv)
{
	int n, ns, bytes_to_read;
	int port, err;
	FILE *file;
	SOCKET sd;
	struct sockaddr_in server;
	char  *host, *bp, *rbuf;
	WSADATA WSAData;
	WORD wVersionRequested;
	
	rbuf = (char *)malloc(BUFSIZE);
	getc(stdin);
	switch (argc)
	{
	case 2:
		host = argv[1];	// Host name
		port = SERVER_TCP_PORT;
		break;
	case 3:
		host = argv[1];
		port = atoi(argv[2]);	// User specified port
		break;
	default:
		fprintf(stderr, "Usage: %s host [port]\n", argv[0]);
		exit(1);
	}

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &WSAData);
	if (err != 0) //No usable DLL
	{
		printf("DLL not found!\n");
		exit(1);
	}

	// Create the socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Cannot create socket");
		exit(1);
	}

	// Initialize and set up the address structure
	memset((char *)&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Connecting to the server
	if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		fprintf(stderr, "Can't connect to server\n");
		perror("connect");
		exit(1);
	}
	
	bp = rbuf;
	bytes_to_read = BUFSIZE;
	file = fopen("accadecca.wav", "wb");
	fseek(file, 0, SEEK_SET);
	// client makes repeated calls to recv until no more data is expected to arrive.
	while ((n = recv(sd, bp, bytes_to_read, 0)) < bytes_to_read)
	{
		if (n == -1)
		{
			int error = WSAGetLastError();
			fprintf(stderr, "Error: %d\n", error);
			break;
		}
		bp += n;
		bytes_to_read -= n;
		if (n == 0)
			break;
	}
	fwrite(rbuf, 1, BUFSIZE, file);
	fclose(file);
	closesocket(sd);
	WSACleanup();
	exit(0);
}
