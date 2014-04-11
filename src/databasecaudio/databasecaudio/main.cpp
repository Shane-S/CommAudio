#include "ahm.h"

char achMCAddr[MAXADDRSTR] = TIMECAST_ADDR;
u_short nPort = TIMECAST_PORT;

int main(void)
{
	cout << "Reading library in.." << endl;
	std::unique_ptr<AudioLibrary> lib(new AudioLibrary(string("c:\\users\\Razlt\\Music\\"), string(",mp3,wav,"), 3, 10));

	cout << "read " << lib->numsongs << " songs into the library." << endl;
	for (auto s : lib->songList)
	{
		cout << "Song name: " << s.getData(TITLE) << endl;
	}
	std::list<std::shared_ptr<WSABUF>> plist = lib->grabPlaylist();
	BASS_Init(-1, 44100, 0, 0, 0);

	SongData sData;

	try{
		sData = lib->grabSpecifiedFile("When A Fire Starts To Burn", "Disclosure", "Settle");
		cout << sData.getData(TITLE) << endl;
	}
	catch (const std::invalid_argument& e){
		printf("Could not find file.\n");
	}

	const char * dir = lib->songList[5].directory.c_str();
	//testMulticastServer(dir);
	//tcpTestServer(dir);
	//multicastClient();
	//udpServer();
	udpClient();
	//tcpTestClient();
	cout << "Please press any key to exit the program ..." << endl;
	fprintf(stderr, "DONE");
	std::cin.get();

	return 0;
}