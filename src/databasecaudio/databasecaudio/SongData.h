#ifndef SONGDATA_H
#define SONGDATA_H

#include "audioManagement.h"

class SongData
{
public:
	std::string name;
	std::string album;
	std::string artist;
	std::string directory;
	std::string fileType;
	std::string artDirectory;
	int year;
	int bitrate;
	int length;
	int channels;
	int sampleRate;
	int albumArt;

	SongData(std::string directory, std::string artDirectory, std::string fileType);
	std::string toString(TagLib::String str);

private:
	void setMetadata(TagLib::FileRef sRef);
	void setProperties(TagLib::FileRef sRef);
	void setArtist(std::string artist);
	void setAlbum(std::string album);
	void setTitle(std::string title);
	void setYear(int year);
};

#endif