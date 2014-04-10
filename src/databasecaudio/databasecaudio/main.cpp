#include "ahm.h"
#include "bass.h"
#include "Ws2tcpip.h"

#define sRate 44100
#define SERVER_TCP_PORT 7000
#define BUFSIZE         2048
#define TIMECAST_ADDR   "234.5.6.7"
#define TIMECAST_PORT   8910
#define MAXADDRSTR		16
#define RECBUF			2048

typedef struct connectionStruct
{
	SOCKET hSock;
	SOCKADDR_IN toAddr;
} connectionStruct;

void udpClient(); // doesnt quite work
void udpServer(); // neither does this
int multicastClient();
void recordAudio(SOCKET socket, SOCKADDR_IN toAddr);
int testMulticastServer(const char * dir);
void sendAudioDataUDP(const char * filename, bool isTCP, bool isFile, SOCKET socket, SOCKADDR_IN * toAddr);
void sendAudioData(const char *data, bool isTCP, bool isFile, SOCKET socket);
void tcpTestServer(const char * dir);
using std::cout; using std::endl; using std::string;

char achMCAddr[MAXADDRSTR] = TIMECAST_ADDR;
u_short nPort              = TIMECAST_PORT;

int main(void)
{
	cout << "Reading library in.." << endl;
	
	//std::unique_ptr<AudioLibrary> lib(new AudioLibrary(string("C:\\Users\\Raz\\Music\\"), string(",mp3,"), 1, 150)); 
	//
	//cout << "Read " << lib->numsongs << " songs into the library." << endl;
	//for(auto s : lib->songList)
	//{
	//	cout << "Song Name: " <<  s.getData(TITLE) << endl;
	//}
	//std::list<std::shared_ptr<WSABUF>> plist = lib->grabPlaylist();
	BASS_Init(-1, 44100, 0,0,0);
	/*const char * dir =  lib->songList[123].directory.c_str();
	testMulticastServer(dir);*/
	//tcpTestServer(dir);
	//multicastClient();
	udpServer();
	cout << "Please press any key to exit the program ..." << endl;
	fprintf(stderr, "DONE"); 
	std::cin.get();

	return 0;
}

/* MULTICAST TESTING SERVER */
int testMulticastServer(const char * dir) 
{
	int nRet, i;
	SOCKADDR_IN stLclAddr, stDstAddr;
	struct ip_mreq stMreq;        /* Multicast interface structure */
	SOCKET hSocket;
	WSADATA stWSAData;

	nRet = WSAStartup(0x0202, &stWSAData);
	if (nRet) {
		printf ("WSAStartup failed: %d\r\n", nRet);
		exit (1);
	}

	hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (hSocket == INVALID_SOCKET) {
		printf ("socket() failed, Err: %d\n", WSAGetLastError());
		exit(1);
	}

	/* Bind the socket */
	stLclAddr.sin_family      = AF_INET; 
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* any interface */
	stLclAddr.sin_port        = 0;                 /* any port */
	nRet = bind(hSocket, (struct sockaddr*) &stLclAddr, sizeof(stLclAddr));
	if (nRet == SOCKET_ERROR) {
		printf ("bind() port failed %s", WSAGetLastError());
	}

	/* Join the multicast group
	* 
	* NOTE: According to RFC 1112, a sender does not need to join the 
	*  group, however Microsoft requires a socket to join a group in 
	*  order to use setsockopt() IP_MULTICAST_TTL (or fails with error
	*  WSAEINVAL).
	*/
	stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
	stMreq.imr_interface.s_addr = INADDR_ANY;
	nRet = setsockopt(hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));
	if (nRet == SOCKET_ERROR) {
		printf ("setsockopt() IP_ADD_MEMBERSHIP address %s failed, Err: %d\n", achMCAddr, WSAGetLastError());
	} 

	/* Assign our destination address */
	stDstAddr.sin_family =      AF_INET;
	stDstAddr.sin_addr.s_addr = inet_addr(achMCAddr);
	stDstAddr.sin_port =        htons(nPort);

	recordAudio(hSocket, stDstAddr);
	
	while(1)
	{
		//sendAudioDataUDP(dir, true, true, hSocket, &stDstAddr);
		
	} 

	closesocket(hSocket);
	WSACleanup();

	return (0);
}

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
	
	wVersionRequested = MAKEWORD( 2, 2 ); 
	err = WSAStartup( wVersionRequested, &WSAData );
	if ( err != 0 ) //No useable DLL
	{
		printf ("DLL not found!\n");
		exit(1);
	}

	// Create a stream socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror ("Can't create a socket");
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
		client_len= sizeof(client); 
		if ((new_sd = accept (sd, (struct sockaddr *)&client, &client_len)) == -1)
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
		
            while(1)
            {
                sendAudioData(dir, true, true, new_sd);
            }

		    //ns = send (new_sd, buf, BUFSIZE, 0);
		    closesocket (new_sd);
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
    while(current_len != sizeof(streamBuffer))
    {	
        readLength = BASS_ChannelGetData(streamBuffer, streamDataBuffer, 2048);
		int err = BASS_ErrorGetCode();

        buffer.len = BUFSIZE;
        buffer.buf = streamDataBuffer;
		current_len += BUFSIZE;

        err = WSASend(socket, &buffer, 1, &SendBytes, 0, 0, NULL);
    }
}

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
    while(current_len != sizeof(streamBuffer))
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

	int pos = 0;
	while(length > pos)
	{
		WSbuffer.len = BUFSIZE;
		WSbuffer.buf = (char*)buf + pos;

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
	
	BASS_RecordSetInput(-1, BASS_INPUT_ON, 1);
	HRECORD rcHandle = BASS_RecordStart(44100, 2, 0, MyRecordProc, (void*)&cStruct);
}

/* MULTICAST TESTING CLIENT */
int multicastClient() {
	int nRet;
	BOOL  fFlag;
	SOCKADDR_IN stLclAddr, stSrcAddr;
	struct ip_mreq stMreq;         /* Multicast interface structure */
	SOCKET hSocket;
	char achInBuf [BUFSIZE];
	WSADATA stWSAData;

	nRet = WSAStartup(0x0202, &stWSAData);
	if (nRet) {
		printf ("WSAStartup failed: %d\r\n", nRet);
		exit(1);
	}

	HSTREAM streamHandle = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);
	
	/* Get a datagram socket */
	hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (hSocket == INVALID_SOCKET) {
		printf ("socket() failed, Err: %d\n", WSAGetLastError());
		WSACleanup();
		exit(1);
	}

	fFlag = TRUE;
	nRet = setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag));
	if (nRet == SOCKET_ERROR) {
		printf ("setsockopt() SO_REUSEADDR failed, Err: %d\n",
			WSAGetLastError());
	}

	/* Name the socket (assign the local port number to receive on) */
	stLclAddr.sin_family      = AF_INET;
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stLclAddr.sin_port        = htons(nPort);
	nRet = bind(hSocket, (struct sockaddr*) &stLclAddr, sizeof(stLclAddr));
	if (nRet == SOCKET_ERROR) {
		printf ("bind() port: %d failed, Err: %d\n", nPort, 
			WSAGetLastError());
	}

	/* Join the multicast group so we can receive from it */
	stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
	stMreq.imr_interface.s_addr = INADDR_ANY;
	nRet = setsockopt(hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));
	if (nRet == SOCKET_ERROR) {
		printf("setsockopt() IP_ADD_MEMBERSHIP address %s failed, Err: %d\n",achMCAddr, WSAGetLastError());
	} 

	for (;;) {
		int addr_size = sizeof(struct sockaddr_in);

		nRet = recvfrom(hSocket, achInBuf, BUFSIZE, 0, (struct sockaddr*)&stSrcAddr, &addr_size);
		if (nRet < 0) {
			printf ("recvfrom() failed, Error: %d\n", WSAGetLastError());
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
		printf ("setsockopt() IP_DROP_MEMBERSHIP address %s failed, Err: %d\n",achMCAddr, WSAGetLastError());
	} 

	closesocket(hSocket);
	WSACleanup();

	return (0);
} 

void udpServer()
{
	int	data_size = BUFSIZE, port = TIMECAST_PORT;
	int	i, j, server_len, client_len;
	SOCKET sd;
	char *pname, *host, rbuf[BUFSIZE], sbuf[BUFSIZE];
	struct	hostent	*hp;
	struct	sockaddr_in server, client;
	SYSTEMTIME stStartTime, stEndTime;
	WSADATA stWSAData;
	WORD wVersionRequested = MAKEWORD (2,2);

	// Initialize the DLL with version Winsock 2.2
	WSAStartup(wVersionRequested, &stWSAData ) ;

	// Create a datagram socket
	if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror ("Can't create a socket\n");
		exit(1);
	}

	// Store server's information
	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(TIMECAST_PORT);
	
	HSTREAM streamHandle = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);

	if ((hp = gethostbyname("127.0.0.1")) == NULL)
	{
		fprintf(stderr,"Can't get server's IP address\n");
		exit(1);
	}
	//strcpy((char *)&server.sin_addr, hp->h_addr);
	memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

	memset((char *)&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(0);  // bind to any available port
	client.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sd, (struct sockaddr *)&client, sizeof(client)) == -1)
	{
		perror ("Can't bind name to socket");
		exit(1);
	}

	BOOL fFlag = TRUE;
	
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag));

	while(1)
	{
		int ret;
		if ((ret = recvfrom (sd, rbuf, BUFSIZE, 0, (struct sockaddr *)&server, &server_len)) < 0)
		{
			perror (" recvfrom error");
			exit(1);
		}
		 
		int err = BASS_StreamPutData(streamHandle, rbuf, BUFSIZE);
		BASS_ChannelPlay(streamHandle, FALSE);
	}

	closesocket(sd);
	WSACleanup();
	exit(0);
}

void udpClient()
{
	int	data_size = BUFSIZE, port = TIMECAST_PORT;
	int	i, j, server_len, client_len;
	SOCKET sd;
	char *pname, *host, rbuf[BUFSIZE], sbuf[BUFSIZE];
	struct	hostent	*hp;
	struct	sockaddr_in server, client;
	SYSTEMTIME stStartTime, stEndTime;
	WSADATA stWSAData;
	WORD wVersionRequested = MAKEWORD (2,2);

	// Initialize the DLL with version Winsock 2.2
	WSAStartup(wVersionRequested, &stWSAData ) ;

	// Create a datagram socket
	if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror ("Can't create a socket\n");
		exit(1);
	}
	// Store server's information
	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(TIMECAST_PORT);
	
	HSTREAM streamHandle = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);

	if ((hp = gethostbyname("127.0.0.1")) == NULL)
	{
		fprintf(stderr,"Can't get server's IP address\n");
		exit(1);
	}
	//strcpy((char *)&server.sin_addr, hp->h_addr);
	memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

	memset((char *)&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(0);  // bind to any available port
	client.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sd, (struct sockaddr *)&client, sizeof(client)) == -1)
	{
		perror ("Can't bind name to socket");
		exit(1);
	}

	client_len = sizeof(client);
	if (getsockname (sd, (struct sockaddr *)&client, &client_len) < 0)
	{
		perror ("getsockname: \n");
		exit(1);
	}

	BOOL fFlag = TRUE;
	
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag));

	recordAudio(sd, server);
	while(1)
	{
	
	}

	closesocket(sd);
	WSACleanup();
	exit(0);
}