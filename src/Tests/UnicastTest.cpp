#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include "../bass.h"

#define PERFORM_TEST(test, expected, actual) cerr << "Test " << test << ": " << (((actual) == (expected)) ? "passed!" : "failed!") << endl

#define START_STREAM 1
#define STOP_STREAM  0

#define SERVER_SUCCESS 0
#define SERVER_ERROR   1
#define SERVER_TIMEOUT 2

#define TIMEOUT 3000

using std::string;
using std::cerr;
using std::cin;
using std::endl;

SOCKET globalSock;

int RequestFile(string &fileName, QWORD startOffset, DWORD command)
{
	// Make a file request packet
	// Send the file request
	// Receive the packet type - if it's the error packet, read the size of the error string and print the error
	// If it's the song data packet, loop on the file receive, playing back music until the file is finished

	return 1;
}

int main()
{
	SOCKADDR_IN winSock;
	WSADATA data;
	string clientName("Shane Spoor");
	int namelen = clientName.length();

	string validFile("Hurt.mp3");
	string invalidFile("Doesn'tExist.mp3");
	QWORD  startAtBeginning = 0;
	QWORD  startAtMiddle = 10000;
	QWORD  invalidStartPos = (QWORD)-1;

	WSAStartup(MAKEWORD(2, 2), &data);

	memset(&winSock, 0, sizeof(SOCKADDR_IN));
	winSock.sin_addr.s_addr = inet_addr("192.168.0.48");
	winSock.sin_port = htons(7000);
	winSock.sin_family = AF_INET;
	
	// Initialise the socket and send some data
	globalSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, NULL, 0);
	connect(globalSock, (const sockaddr *)&winSock, sizeof(winSock));
	send(globalSock, (const char *)&namelen, sizeof(namelen), 0);
	send(globalSock, clientName.c_str(), clientName.length(), 0);

	cin.get();

	PERFORM_TEST("Start stream, valid file, beginning of file", SERVER_SUCCESS, RequestFile(validFile, startAtBeginning, START_STREAM));
	PERFORM_TEST("Start stream, valid file, part way through file", SERVER_SUCCESS, RequestFile(validFile, startAtMiddle, START_STREAM));
	PERFORM_TEST("Start stream, valid file, past end of file", SERVER_ERROR, RequestFile(validFile, invalidStartPos, START_STREAM));
	PERFORM_TEST("Start stream, invalid file, beginning of file", SERVER_ERROR, RequestFile(invalidFile, startAtBeginning, START_STREAM));
	PERFORM_TEST("Stop stream", SERVER_TIMEOUT, RequestFile(validFile, startAtBeginning, STOP_STREAM));

	WSACleanup();

	cin.get();
	return 0;
}
