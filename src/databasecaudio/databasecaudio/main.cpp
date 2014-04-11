#include "ahm.h"

char achMCAddr[MAXADDRSTR] = TIMECAST_ADDR;
u_short nPort = TIMECAST_PORT;

int main(void)
{
	cout << "Reading library in.." << endl;
	std::unique_ptr<AudioLibrary> lib(new AudioLibrary(string("c:\\users\\Razlt\\Music\\"), string(",mp3,wav,"), 3, 500)); 
	
	cout << "read " << lib->numsongs << " songs into the library." << endl;
	for(auto s : lib->songList)
	{
		cout << "Song name: " <<  s.getData(TITLE) << endl;
	}
	std::list<std::shared_ptr<WSABUF>> plist = lib->grabPlaylist();
	BASS_Init(-1, 44100, 0,0,0);
	const char * dir = lib->songList[434].directory.c_str();
	//testMulticastServer(dir);
	tcpTestServer(dir);
	//multicastClient();
	//udpServer();
	//udpClient();
	//tcpTestClient();
	cout << "Please press any key to exit the program ..." << endl;
	fprintf(stderr, "DONE"); 
	std::cin.get();

	return 0;
<<<<<<< HEAD
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
    char streamDataBuffer[2048];
    HSTREAM streamBuffer;
	DWORD readLength = 0;
    DWORD SendBytes = 0;
    DWORD BytesTransferred = 0;
    WSABUF buffer;

    WSAOVERLAPPED ov;
    ZeroMemory(&ov, sizeof(WSAOVERLAPPED));
    
    streamBuffer = BASS_StreamCreateFile(FALSE, filename, 0, 0, BASS_STREAM_DECODE);
	int opo = BASS_ErrorGetCode();

	BASS_SAMPLE * bInfo;

	int current_len = 0;
    while(1)
    {	
        readLength = BASS_ChannelGetData(streamBuffer, streamDataBuffer, 2048);
		int err = BASS_ErrorGetCode();

        buffer.len = 2048;
        buffer.buf = streamDataBuffer;
		current_len += 2048;

        err = WSASend(socket, &buffer, 1, &SendBytes, 0, 0, NULL);
      
        ZeroMemory(&ov, sizeof(WSAOVERLAPPED));
    }
}
=======
}
>>>>>>> 1ccb1fa642a03598b41a2310fd269c3bfe528981
