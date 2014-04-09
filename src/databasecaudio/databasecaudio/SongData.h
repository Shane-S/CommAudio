#ifndef SONGDATA_H
#define SONGDATA_H

#include "audioManagement.h"

#define NUM_METASTRS 9

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
	SongData(const char *metadata, const uint8_t strLens[NUM_METASTRS], const uint32_t artSize);
	std::string toString(TagLib::String str);

private:
	std::shared_ptr<WSABUF>  makeSizePacket(uint32_t *dataPktSize) const;
	std::shared_ptr<WSABUF>  makeDataPacket(uint32_t dataPktSize) const;
	std::vector<std::string> metaString_;

	void setMetadata(TagLib::FileRef sRef);
	void setProperties(TagLib::FileRef sRef);
	void setArtist(std::string artist);
	void setAlbum(std::string album);
	void setTitle(std::string title);
	void setYear(int year);

};

#endif