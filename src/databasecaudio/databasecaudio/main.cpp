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

int _kbhit()
{
	int r;
	fd_set rfds;
	struct timeval tv;
	struct termios term, oterm;
	tcgetattr(0, &oterm);
	memcpy(&term, &oterm, sizeof(term));
	cfmakeraw(&term);
	tcsetattr(0, TCSANOW, &term);
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	tv.tv_sec = tv.tv_usec = 0;
	r = select(1, &rfds, NULL, NULL, &tv);
	tcsetattr(0, TCSANOW, &oterm);
	return r;
}

void saveFile(const char * buffer, int bufsize, HSTREAM stream)
{
	FILE * fp;
	BASS_CHANNELINFO info;
	WAVEFORMATEX wf;

	if ((fp = fopen("stream.wav", "wb")) == 0)
	{
		cout << "Failed to open file" << endl;
		return;
	}

	//get and save the channel information
	BASS_ChannelGetInfo(stream, &info);

	wf.wFormatTag = 1;
	wf.nChannels = info.chans;
	wf.wBitsPerSample = (info.flags&BASS_SAMPLE_8BITS ? 8 : 16);
	wf.nBlockAlign = wf.nChannels*wf.wBitsPerSample / 8;
	wf.nSamplesPerSec = info.freq;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec*wf.nBlockAlign;
	//write the header to the file first.
	fwrite("RIFF\0\0\0\0WAVEfmt \20\0\0\0", 20, 1, fp);
	fwrite(&wf, 16, 1, fp);

	fwrite(buffer, sizeof(char), bufsize, fp);

}