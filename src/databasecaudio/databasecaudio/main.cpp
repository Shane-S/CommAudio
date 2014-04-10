#include "ahm.h"

char achMCAddr[MAXADDRSTR] = TIMECAST_ADDR;
u_short nPort = TIMECAST_PORT;

int main(void)
{
	cout << "Reading library in.." << endl;
	
	std::unique_ptr<AudioLibrary> lib(new AudioLibrary(string("c:\\users\\razlt\\documents\\MSIC\\"), string(",mp3,wav,"), 3, 350)); 
	
	cout << "read " << lib->numsongs << " songs into the library." << endl;
	for(auto s : lib->songList)
	{
		cout << "song name: " <<  s.getData(TITLE) << endl;
	}
	std::list<std::shared_ptr<WSABUF>> plist = lib->grabPlaylist();
	BASS_Init(-1, 44100, 0,0,0);
	const char * dir =  lib->songList[1].directory.c_str();
	//testMulticastServer(dir);
	tcpTestServer(dir);
	//multicastClient();
	//udpServer();
	cout << "Please press any key to exit the program ..." << endl;
	fprintf(stderr, "DONE"); 
	std::cin.get();

	return 0;
}