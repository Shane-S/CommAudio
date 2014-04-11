#include "ahm.h"

char achMCAddr[MAXADDRSTR] = TIMECAST_ADDR;
u_short nPort = TIMECAST_PORT;

int main(int argc, char ** argv)
{
	int song;
	char * mystr;
	WSADATA stWSAData;

	if (argc < 2)
	{
		printf("Requires at least 2 arguments.\n");
		exit(0);
	}

	printf("Reading library in..\n");
	std::unique_ptr<AudioLibrary> lib(new AudioLibrary(string("c:\\users\\Razlt\\Documents\\MSIC\\"), string(",mp3,wav,"), 3, 10));

	printf("Read %d songs into the library.\n", lib->numsongs);
	for (auto s : lib->songList)
	{
		printf("Title: %s | Artist: %s | Album: %s\n", s.getData(TITLE).c_str(), s.getData(ARTIST).c_str(), s.getData(ALBUM).c_str());
		printf("Bitrate: %s | Sample Rate: %s | Year: %s\n", s.getData(BRATE).c_str(), s.getData(SRATE).c_str(), s.getData(YEAR).c_str());
	}

	BASS_Init(-1, 44100, 0, 0, 0);

	int nRet = WSAStartup(0x0202, &stWSAData);
	if (nRet) {
		printf("WSAStartup failed: %d\r\n", nRet);
		exit(1);
	}

	if (strcmp(argv[1], "server") == 0)
	{
		if (strcmp(argv[2], "mic") == 0)
		{
			udpClient();
			system("pause");
			return 0;
		}

		const char * dir = lib->songList[1].directory.c_str();

		if (strcmp(argv[2], "multicast") == 0)
			multicastServer(dir);

		if (strcmp(argv[2], "unicast") == 0)
			tcpServer(dir);
	}

	if (strcmp(argv[1], "client") == 0)
	{
		if (strcmp(argv[2], "mic") == 0)
		{
			udpClient();
			system("pause");
			return 0;
		}

		const char * dir = lib->songList[1].directory.c_str();

		if (strcmp(argv[2], "multicast") == 0)
			multicastServer(dir);

		if (strcmp(argv[2], "unicast") == 0)
			tcpServer(dir);
	}

	system("pause");

	WSACleanup();
	exit(0);
}