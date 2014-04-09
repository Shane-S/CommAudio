#include "ahm.h"
#include "bass.h"
using namespace std;

int main(void)
{
	cout << "Reading library in.." << endl;
	
	std::unique_ptr<AudioLibrary> lib(new AudioLibrary(string("C:\\Users\\RazLT\\Music\\"), string(",mp3,"), 1, 10000)); 
	
	for(int i = 0; i < lib->numsongs; i++)
	{
		cout << "Title: " << lib->songList.at(i).name << " | Artist: " <<  lib->songList.at(i).artist << " | Album: " << lib->songList.at(i).album << endl;
		cout << "Bitrate: " << lib->songList.at(i).bitrate << " | Channels: " << lib->songList.at(i).channels << " | SampleRate: " << lib->songList.at(i).sampleRate << endl;
		cout << "Length: " << lib->songList.at(i).length << endl;
	}

	cout << "Please press any key to exit the program ..." << endl;
	fprintf(stderr, "DONE"); 
	cin.get();

	return 0;
}