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

void saveFile(const char * buffer)
{
	FILE * fp;
	BASS_CHANNELINFO info;
	WAVEFORMATEX wf;

	if (!(fp = fopen("BASS.WAV", "wb"))){
		cout << "Failed to send to file." << endl;
	}
	printf("writing to BASS.WAV file... press a key to stop\n");
	BASS_ChannelGetInfo(2, &info);
	wf.wFormatTag = 1;
	wf.nChannels = info.chans;
	wf.wBitsPerSample = (info.flags&BASS_SAMPLE_8BITS ? 8 : 16);
	wf.nBlockAlign = wf.nChannels*wf.wBitsPerSample / 8;
	wf.nSamplesPerSec = info.freq;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec*wf.nBlockAlign;

	fwrite("RIFF\0\0\0\0WAVEfmt \20\0\0\0", 20, 1, fp);
	fwrite(&wf, 16, 1, fp);
	fwrite("data\0\0\0\0", 8, 1, fp);

	fwrite(buffer, sizeof(buffer), 1, fp);
}