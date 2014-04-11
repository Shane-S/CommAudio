
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
#include <thread>

#define sRate			44100
#define SERVER_TCP_PORT 7000
#define SERVER_UDP_PORT	7001
#define BUFSIZE         4096
#define TIMECAST_ADDR   "234.5.6.7"
#define TIMECAST_PORT   7001
#define MAXADDRSTR		16

typedef struct connectionStruct
{
	SOCKET hSock;
	SOCKADDR_IN toAddr;
} connectionStruct;

void udpClient(); 
int multicastServer(const char * dir);
void recordAudio(SOCKET socket, SOCKADDR_IN toAddr);
void sendAudioDataUDP(const char * filename, SOCKET socket, SOCKADDR_IN * toAddr);
void sendAudioData(const char *data, SOCKET socket);
void tcpServer(const char * dir);
void udpServer(SOCKET sock);

using std::cout; using std::endl; using std::string;

#endif