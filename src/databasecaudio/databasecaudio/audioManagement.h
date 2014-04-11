
#ifndef AUDIOMANAGEMENT_H
#define AUDIOMANAGEMENT_H

#include <WinSock2.h>
#include "bass.h"
#include "Ws2tcpip.h"
#include <Windows.h>
#include <iostream>
#include "taglib/fileref.h"
#include "taglib/tag.h"
#include "taglib/tpropertymap.h"
#include <dirent.h>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <list>

#define sRate 44100
#define SERVER_TCP_PORT 7000
#define BUFSIZE         4096
#define TIMECAST_ADDR   "234.5.6.8"
#define TIMECAST_PORT   43392
#define MAXADDRSTR		16
#define RECBUF			2048

typedef struct connectionStruct
{
	SOCKET hSock;
	SOCKADDR_IN toAddr;
} connectionStruct;

void tcpTestClient();
void udpClient(); // doesnt quite work
void udpServer(); // neither does this
int multicastClient();
void recordAudio(SOCKET socket, SOCKADDR_IN toAddr);
int testMulticastServer(const char * dir);
void sendAudioDataUDP(const char * filename, bool isTCP, bool isFile, SOCKET socket, SOCKADDR_IN * toAddr);
void sendAudioData(const char *data, bool isTCP, bool isFile, SOCKET socket);
void tcpTestServer(const char * dir);
using std::cout; using std::endl; using std::string;

#endif