#ifndef SONGDATA_H
#define SONGDATA_H

#include "audioManagement.h"

#define TITLE		 1
#define ARTIST		 2
#define ALBUM		 3
#define YEAR		 4
#define BRATE		 5
#define SRATE		 6
#define LENGTH		 7
#define AART		 8
#define NUM_METASTRS 9

class SongData
{
public:
	std::string directory;
	std::string fileType;
	std::string artDirectory;

	SongData(std::string directory, std::string artDirectory, std::string fileType);
	SongData(const char * metadata, const uint32_t strLens[NUM_METASTRS]);
	template<typename T>
	std::string toString(T str);
	std::string getData(int data) const;
	std::list<std::shared_ptr<WSABUF>> serialize() const;

private:
	std::shared_ptr<WSABUF>  makeSizePacket(uint32_t *dataPktSize) const;
	std::shared_ptr<WSABUF>  makeDataPacket(uint32_t dataPktSize) const;
	std::vector<std::string> metaStrings_;

	void setMetadata(TagLib::FileRef sRef);
	void setProperties(TagLib::FileRef sRef);
	void setArtist(std::string artist);
	void setAlbum(std::string album);
	void setTitle(std::string title);
	void setYear(std::string year);

};

#endif