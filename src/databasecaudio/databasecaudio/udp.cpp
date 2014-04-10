#include "audioManagement.h"

extern char achMCAddr[MAXADDRSTR];
extern u_short nPort;

char * ipudp = "192.168.43.236";

/* SENDS AUDIO DATA OVER UDP */
void sendAudioDataUDP(const char * filename, bool isTCP, bool isFile, SOCKET socket, SOCKADDR_IN * toAddr)
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
		readLength = BASS_ChannelGetData(streamBuffer, streamDataBuffer, 2048);
		int err = BASS_ErrorGetCode();

		buffer.len = BUFSIZE;
		buffer.buf = streamDataBuffer;
		current_len += BUFSIZE;

		err = WSASendTo(socket, &buffer, 1, &SendBytes, 0, (sockaddr*)toAddr, sizeof(*toAddr), NULL, NULL);
	}
}

/* RECORD SENDING */
BOOL CALLBACK MyRecordProc(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	DWORD SendBytes = 0;
	DWORD BytesTransferred = 0;
	WSABUF WSbuffer;
	int err;
	char * buf = (char*)buffer;
	connectionStruct * cStruct = (connectionStruct*)user;

	WSbuffer.buf = buf;
	WSbuffer.len = BUFSIZE;

	int pos = 0;
	while (length > pos)
	{
		buf = (char*)buf + pos;

		err = WSASendTo(cStruct->hSock, &WSbuffer, 1, &SendBytes, 0, (sockaddr*)&cStruct->toAddr, sizeof(cStruct->toAddr), NULL, NULL);

		pos += BUFSIZE;
	}
	return TRUE; // continue recording
}

/* RECORD TESTING */
void recordAudio(SOCKET socket, SOCKADDR_IN toAddr)
{
	connectionStruct cStruct;
	cStruct.hSock = socket;
	cStruct.toAddr = toAddr;

	int err = BASS_RecordInit(-1);
	err = BASS_ErrorGetCode();

	HRECORD rcHandle = BASS_RecordStart(44100, 2, 0, MyRecordProc, (void*)&cStruct);
}

/* MULTICAST TESTING CLIENT */
int multicastClient() {
	int nRet;
	BOOL  fFlag;
	SOCKADDR_IN stLclAddr, stSrcAddr;
	struct ip_mreq stMreq;         /* Multicast interface structure */
	SOCKET hSocket;
	char achInBuf[BUFSIZE];
	WSADATA stWSAData;

	nRet = WSAStartup(0x0202, &stWSAData);
	if (nRet) {
		printf("WSAStartup failed: %d\r\n", nRet);
		exit(1);
	}

	HSTREAM streamHandle = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);

	/* Get a datagram socket */
	hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (hSocket == INVALID_SOCKET) {
		printf("socket() failed, Err: %d\n", WSAGetLastError());
		WSACleanup();
		exit(1);
	}

	fFlag = TRUE;
	nRet = setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag));
	if (nRet == SOCKET_ERROR) {
		printf("setsockopt() SO_REUSEADDR failed, Err: %d\n",
			WSAGetLastError());
	}

	/* Name the socket (assign the local port number to receive on) */
	stLclAddr.sin_family = AF_INET;
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stLclAddr.sin_port = htons(nPort);
	nRet = bind(hSocket, (struct sockaddr*) &stLclAddr, sizeof(stLclAddr));
	if (nRet == SOCKET_ERROR) {
		printf("bind() port: %d failed, Err: %d\n", nPort,
			WSAGetLastError());
	}

	/* Join the multicast group so we can receive from it */
	stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
	stMreq.imr_interface.s_addr = INADDR_ANY;
	nRet = setsockopt(hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));
	if (nRet == SOCKET_ERROR) {
		printf("setsockopt() IP_ADD_MEMBERSHIP address %s failed, Err: %d\n", achMCAddr, WSAGetLastError());
	}

	for (;;) {
		int addr_size = sizeof(struct sockaddr_in);

		nRet = recvfrom(hSocket, achInBuf, BUFSIZE, 0, (struct sockaddr*)&stSrcAddr, &addr_size);
		if (nRet < 0) {
			printf("recvfrom() failed, Error: %d\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}

		int err = BASS_StreamPutData(streamHandle, achInBuf, nRet);
		BASS_ChannelPlay(streamHandle, FALSE);
	} /* end for(;;) */

	/* Leave the multicast group: With IGMP v1 this is a noop, but
	*  with IGMP v2, it may send notification to multicast router.
	*  Even if it's a noop, it's sanitary to cleanup after one's self.
	*/
	stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
	stMreq.imr_interface.s_addr = INADDR_ANY;
	nRet = setsockopt(hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));
	if (nRet == SOCKET_ERROR) {
		printf("setsockopt() IP_DROP_MEMBERSHIP address %s failed, Err: %d\n", achMCAddr, WSAGetLastError());
	}

	closesocket(hSocket);
	WSACleanup();

	return (0);
}

void udpServer()
{
	int	client_len, port, n;
	SOCKET sd;
	char	buf[BUFSIZE];
	struct	sockaddr_in server, client;
	WSADATA stWSAData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	// Initialize the DLL with version Winsock 2.2
	WSAStartup(wVersionRequested, &stWSAData);
	HSTREAM streamHandle = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);
	// Create a datagram socket
	if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("Can't create a socket");
		exit(1);
	}

	// Bind an address to the socket
	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(TIMECAST_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("Can't bind name to socket");
		exit(1);
	}
	BOOL fFlag = TRUE;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag));

	while (1)
	{
		client_len = sizeof(client);
		if ((n = recvfrom(sd, buf, BUFSIZE, 0, (struct sockaddr *)&client, &client_len)) < 0)
		{
			perror("recvfrom error");
			exit(1);
		}

		int err = BASS_StreamPutData(streamHandle, buf, BUFSIZE);
		BASS_ChannelPlay(streamHandle, TRUE);
	}

	closesocket(sd);
	WSACleanup();
	exit(0);
}

void udpClient()
{

	int i, nBufSize, err;
	SOCKET sock;
	char *buf, *buf_ptr;
	struct sockaddr_in sin;
	WSADATA stWSAData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	nBufSize = BUFSIZE;

	buf = (char*)malloc(BUFSIZE);
	memset(buf, 0, BUFSIZE);

	// Initialize the DLL with version Winsock 2.2
	WSAStartup(wVersionRequested, &stWSAData);

	// Open a connectionless, unreliable socket (Datagrams)
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
		exit(2);

	buf_ptr = buf;

	// Set the socket options such that the send buffer size is set at the
	// application layer
	if (err = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, buf_ptr, sizeof(buf)) != 0)
	{
		printf("Error in setsockopt!\n");
		exit(3);
	}
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;	 // Specify the Internet (TCP/IP) Address family
	sin.sin_port = htons(nPort); // Convert to network byte order
	sin.sin_addr.s_addr = inet_addr(ipudp);

	// Transmit data through an unconnected (UDP) socket
	if (sendto(sock, buf, BUFSIZE, 0, (struct sockaddr *)&sin, sizeof(sin)) <= 0)
	{
		perror("sendto error");
		exit(1);
	}

	closesocket(sock);
	WSACleanup();
	exit(0);
}