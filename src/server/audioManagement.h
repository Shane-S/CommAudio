
#ifndef AUDIOMANAGEMENT_H
#define AUDIOMANAGEMENT_H

#include <WinSock2.h>
#include "../bass.h"
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

typedef struct connectionStruct
{
	SOCKET hSock;
	SOCKADDR_IN toAddr;
} connectionStruct;

void recordAudio(SOCKET socket, SOCKADDR_IN toAddr);
void sendAudioDataUDP(const char * filename, bool isTCP, bool isFile, SOCKET socket, SOCKADDR_IN * toAddr);
void sendAudioData(const char *data, bool isTCP, bool isFile, SOCKET socket);
using std::cout; using std::endl; using std::string;

#endif