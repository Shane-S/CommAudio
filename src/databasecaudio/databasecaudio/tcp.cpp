#include "audioManagement.h"

extern char achMCAddr[MAXADDRSTR];
extern u_short nPort;
char * tcpip = "192.168.43.116";
#define TCPPORT 7000
/* TCP BASS TESTING SERVER */
void tcpTestServer(const char * dir)
{
	int	bytes_to_read;
	int	client_len, port = SERVER_TCP_PORT, err;
	SOCKET sd, new_sd;
	struct	sockaddr_in server, client;
	char	*bp, buf[BUFSIZE];
	WSADATA WSAData;
	WORD wVersionRequested;

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &WSAData);
	if (err != 0) //No useable DLL
	{
		printf("DLL not found!\n");
		exit(1);
	}

	// Create a stream socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Can't create a socket");
		exit(1);
	}

	// Initialize and set up the address structure
	memset((char *)&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

	// Bind an address to the socket
	if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("Can't bind name to socket");
		exit(1);
	}

	// Listen for connections
	// queue up to 5 connect requests
	listen(sd, 5);

	struct sockaddr_in sin;
	int len = sizeof(sin);
	if (getsockname(sd, (struct sockaddr *)&sin, &len) == -1)
		perror("getsockname");
	else
		printf("port number %d\n", ntohs(sin.sin_port));

	while (TRUE)
	{
		client_len = sizeof(client);
		if ((new_sd = accept(sd, (struct sockaddr *)&client, &client_len)) == -1)
		{
			fprintf(stderr, "Can't accept client\n");
			exit(1);
		}
		else
		{

			printf(" Remote Address:  %s\n", inet_ntoa(client.sin_addr));
			bp = buf;
			bytes_to_read = BUFSIZE;

			/*
			while ((n = recv (new_sd, bp, bytes_to_read, 0)) < BUFSIZE)
			{
			bp += n;
			bytes_to_read -= n;
			if (n == 0)
			break;
			}
			*/

			while (1)
			{
				sendAudioData(dir, true, true, new_sd);
			}

			//ns = send (new_sd, buf, BUFSIZE, 0);
			closesocket(new_sd);
		}
	}

	closesocket(sd);
	WSACleanup();
}

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION: sendAudioData(void *data, bool isTCP, bool isFile)
*
* DATE: April 4, 2014
*
* REVISIONS: (Date and Description)
*
* DESIGNER: Abhishek Bhardwaj
*
* PROGRAMMER: Abhishek Bhardwaj
*
* INTERFACE: int ClientNetwork::sendAudioData(void *data, bool isTCP, bool isFile)
* void *data - data to be sent to the server [could be a filename or pointer to a memory location]
* bool isTCP - switch to specify what protocol to use to send the data [true for TCP, false for UDP]
* bool isFile - switch to specify if the *data is an actual file on disk or a pointer to a memory location.
* RETURNS: void
*
* NOTES: Sends audio data to the server (capable to do it with both TCP or UDP). Audio data could be an actual file
* or an actual memory location.
--------------------------------------------------------------------------------------------------------------------------*/
void sendAudioData(const char * filename, bool isTCP, bool isFile, SOCKET socket)
{
	char streamDataBuffer[BUFSIZE];
	HSTREAM streamBuffer;
	DWORD readLength = 0;
	DWORD SendBytes = 0;
	DWORD BytesTransferred = 0;
	WSABUF buffer;

	streamBuffer = BASS_StreamCreateFile(FALSE, filename, 0, 0, BASS_STREAM_DECODE);
	int opo = BASS_ErrorGetCode();

	int current_len = 0;
	while (current_len != sizeof(streamBuffer))
	{
		readLength = BASS_ChannelGetData(streamBuffer, streamDataBuffer, BUFSIZE);
		int err = BASS_ErrorGetCode();

		buffer.len = BUFSIZE;
		buffer.buf = streamDataBuffer;
		current_len += BUFSIZE;

		err = WSASend(socket, &buffer, 1, &SendBytes, 0, 0, NULL);
	}
}

void tcpTestClient()
{
	int n, ns, bytes_to_read;
	int port, err;
	SOCKET sd;
	struct hostent	*hp;
	struct sockaddr_in server;
	char  *host, *bp, rbuf[BUFSIZE], sbuf[BUFSIZE], **pptr;
	WSADATA WSAData;
	WORD wVersionRequested;

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
	server.sin_port = htons(TCPPORT);
	if ((hp = gethostbyname(tcpip)) == NULL)
	{
		fprintf(stderr, "Unknown server address\n");
		exit(1);
	}

	// Copy the server address
	memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

	// Connecting to the server
	if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		fprintf(stderr, "Can't connect to server\n");
		perror("connect");
		exit(1);
	}
	printf("Connected:    Server Name: %s\n", hp->h_name);
	pptr = hp->h_addr_list;
	printf("\t\tIP Address: %s\n", inet_ntoa(server.sin_addr));
	printf("Transmiting:\n");
	memset((char *)sbuf, 0, sizeof(sbuf));

	int ibuf = 5;
	//// Transmit data throu0gh the socket
	ns = send(sd, (char*)&ibuf, sizeof(int), 0);

	char * poobuff = "Ramzi";
	ns = send(sd, poobuff, ibuf, 0);
	// client makes repeated calls to recv until no more data is expected to arrive.
	/*while ((n = recv(sd, bp, bytes_to_read, 0)) < BUFSIZE)
	{
		bp += n;
		bytes_to_read -= n;
		if (n == 0)
			break;
	}*/

	closesocket(sd);
	WSACleanup();
	exit(0);
}