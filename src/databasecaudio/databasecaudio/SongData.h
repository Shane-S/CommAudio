#ifndef SONGDATA_H
#define SONGDATA_H

#include "audioManagement.h"

#define TITLE		 0
#define ARTIST		 1
#define ALBUM		 2
#define YEAR		 3
#define BRATE		 4
#define SRATE		 5
#define LENGTH		 6
#define AART		 7
#define NUM_METASTRS 8

class SongData
{
public:
	std::string directory;
	std::string fileType;
	std::string artDirectory;

	SongData() {}
	SongData(std::string directory, std::string artDirectory, std::string fileType);
	SongData(const char * metadata, const uint32_t strLens[NUM_METASTRS]);
	template<typename T>
	std::string toString(T str);
	std::string getData(int data) const;
	std::list<std::shared_ptr<WSABUF>> serialize() const;

private:
	std::vector<std::string> metaStrings_;

	std::shared_ptr<WSABUF>  makeSizePacket(uint32_t *dataPktSize) const;
	std::shared_ptr<WSABUF>  makeDataPacket(uint32_t dataPktSize) const;
	void setMetadata(TagLib::FileRef sRef);
	void setProperties(TagLib::FileRef sRef);
	void setAlbumArt();
	void setArtist(std::string artist);
	void setAlbum(std::string album);
	void setTitle(std::string title);
	void setYear(std::string year);

};

#endif