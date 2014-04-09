#ifndef AUDIOLIBRARY_H
#define AUDIOLIBRARY_H

#include "audioManagement.h"
#include "SongData.h"

class AudioLibrary
{
public:
	std::vector<SongData>		songList;
	std::vector<std::string>	supportedTypes;
	int							numsongs;
	int							maxSongs;
	std::string					directory;

	AudioLibrary(std::string directory, std::string supportedTypes, int numTypes, int maxSongs);
	std::vector<std::string> grabPlaylist();
private:
	bool		checkType(std::string type);
	bool		checkName(const char * name);
	void		retrieveDirectory();
	std::string	getFileType(std::string file);
	std::string	getCoverArt(std::string songDirectory);

};

#endif