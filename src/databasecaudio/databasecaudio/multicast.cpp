#include "audioManagement.h"

extern char achMCAddr[MAXADDRSTR];
extern u_short nPort;

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
		printf("WSAStartup failed: %d\r\n", nRet);
		exit(1);
	}

	hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (hSocket == INVALID_SOCKET) {
		printf("socket() failed, Err: %d\n", WSAGetLastError());
		exit(1);
	}

	/* Bind the socket */
	stLclAddr.sin_family = AF_INET;
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* any interface */
	stLclAddr.sin_port = 0;                 /* any port */
	nRet = bind(hSocket, (struct sockaddr*) &stLclAddr, sizeof(stLclAddr));
	if (nRet == SOCKET_ERROR) {
		printf("bind() port failed %s", WSAGetLastError());
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
		printf("setsockopt() IP_ADD_MEMBERSHIP address %s failed, Err: %d\n", achMCAddr, WSAGetLastError());
	}

	/* Assign our destination address */
	stDstAddr.sin_family = AF_INET;
	stDstAddr.sin_addr.s_addr = inet_addr(achMCAddr);
	stDstAddr.sin_port = htons(nPort);

	//recordAudio(hSocket, stDstAddr);

	while (1)
	{
		sendAudioDataUDP(dir, true, true, hSocket, &stDstAddr);
	}

	closesocket(hSocket);
	WSACleanup();

	return (0);
}
