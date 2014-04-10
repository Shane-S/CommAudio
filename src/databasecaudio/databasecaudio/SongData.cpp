#include "SongData.h"
using namespace std;
/**
* Constructor for a SongData object. Holds all data pertaining to an audio file.
* Retrieves song data based on the passed in directories using TagLib.
*
* @param[in] directory		the directory of the song file.
* @param[in] artDirectory	the directory of the file's cover art.
* @param[in] fileType		the file format of the song file.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
SongData::SongData(string directory, string artDirectory, string fileType)
{
	TagLib::FileRef fRef(directory.c_str());
	if (fRef.isNull())
	{
		throw std::invalid_argument("Recieved invalid directory");
	}

	metaStrings_.resize(8);
	this->fileType = fileType;
	this->artDirectory = artDirectory;
	this->directory = directory;

	setMetadata(fRef);
	setProperties(fRef);
}
/**
* Parses the metadata contained in @a metadata into a struct.
*
* Note that the function prints an error message and exit the program on failure.
*
* @param metadata The serialised metadata packet containing the metadata to be parsed.
* @param lengths  The length of each string in the metadata packet.
* @param art_size The size (in bytes) of the album art for this song.
*
* @return Pointer to a metadata struct containing metadata pulled from the packet.
*         Returns NULL on failure.
*
* @designer Shane Spoor
* @author   Shane Spoor
* @date     March 29th, 2014
*/
SongData::SongData(const char * metadata, const uint32_t strLens[NUM_METASTRS])
{
	int offset = 0;

	if (!metadata)
		throw "Metadata::Metadata: No metadata provided";

	for (int i = 0; i < NUM_METASTRS; i++)
	{
		string temp(metadata + offset, strLens[i]);
		metaStrings_.push_back(temp);
		offset += strLens[i];
	}
}
/**
* Retrieves the metadata of the provided FileRef and sets
* the SongData object variables to match.
*
* @param[in] sRef	the FileRef to the location of the song file.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
void SongData::setMetadata(TagLib::FileRef sRef)
{
	setTitle(toString(sRef.tag()->title()));
	setArtist(toString(sRef.tag()->artist()));
	setAlbum(toString(sRef.tag()->album()));
	setYear(toString(sRef.tag()->year()));
}
/**
* Retrieves the audio properties of the provided FileRef and sets
* the SongData object variables to match.
*
* @param[in] sRef	the FileRef to the location of the song file.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
void SongData::setProperties(TagLib::FileRef sRef)
{
	TagLib::AudioProperties * aProps = sRef.audioProperties();
	
	metaStrings_.push_back(toString(aProps->bitrate()));
	metaStrings_.push_back(toString(aProps->sampleRate()));
	metaStrings_.push_back(toString(aProps->length()));
	metaStrings_.push_back(string("0"));
}
/**
* Sets the artist variable of the object.
* Must recieve a string, if the string is empty, the variable has N/A inserted.
*
* @param[in] artist	artist name
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
void SongData::setArtist(string artist)
{
	if (artist.empty()){
		metaStrings_.push_back(string("N/A"));
	}
	else{
		metaStrings_.push_back(artist);
	}
}
/**
* Sets the album variable of the object.
* Must recieve a string, if the string is empty, the variable has N/A inserted.
*
* @param[in] album	 the album name
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
void SongData::setAlbum(string album)
{
	if (album.empty()){
		metaStrings_.push_back(string("N/A"));
	}
	else{
		metaStrings_.push_back(album);
	}
}
/**
* Sets the title variable of the object.
* Must recieve a string, if the string is empty, the variable has N/A inserted.
*
* @param[in] title	the song title
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
void SongData::setTitle(string title)
{
	if (title.empty()){
		metaStrings_.push_back(string("N/A"));
	}
	else{
		metaStrings_.push_back(title);
	}
}
/**
* Sets the year variable of the object.
* Year should be between 0 and 2015.
*
* @param[in] year	the year the song was produced
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
void SongData::setYear(string year)
{
	if (year.empty()){
		metaStrings_.push_back(string("0"));
	}
	else{
		metaStrings_.push_back(year);
	}

}
/**
* Converts a TagLib String into a standard string.
*
* Returns the string created.
*
* @param[in] str	string to be converted.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
template<typename T>
string SongData::toString(T str)
{
	std::stringstream string_out;
	string_out << str;
	return string_out.str();
}

/**
* Serialises the metadata object in preparation for sending over the network.
*
* The first packet contains the size of each metadata string and the album art,
* the track number, and the year of release, while the second packet contains
* the actual metadata strings and album art.
*
* @designer Shane Spoor
* @author   Shane Spoor
* @date     April 3rd, 2014
*/
list<shared_ptr<WSABUF>> SongData::serialize() const
{
	uint32_t           dataPktSize; // Size of the actual data packet
	shared_ptr<WSABUF> wsaSize = makeSizePacket(&dataPktSize);
	shared_ptr<WSABUF> wsaData = makeDataPacket(dataPktSize);
	list<shared_ptr<WSABUF> > lst;

	lst.push_back(wsaSize);
	lst.push_back(wsaData);

	return lst;
}
/**
* Creates the size packet and stores the size of the data packet
* in @a dataPktSize.
*
* @param dataPktSize The size of the data packet to follow the size packet.
* @return A WSABUF structure containing the length size packet and its length.
*
* @designer Shane Spoor
* @author   Shane Spoor
* @date     April 4th, 2014
*/
shared_ptr<WSABUF> SongData::makeSizePacket(uint32_t *dataPktSize) const
{
	int    sizePktSize = NUM_METASTRS; // Size of the size packet
	size_t offset = 0;
	char   *sizeBuf = (char *)malloc(sizePktSize);
	shared_ptr<WSABUF> wsaSize(new WSABUF); // The WSABUF containing the size packet

	if (!sizeBuf)
		throw "Metadata::serialise: Could not allocate memory for sizePacket";

	// Copy all metadata string sizes into the size packet
	for (int i = 0; i < metaStrings_.size(); i++)
	{
		uint8_t strSize = metaStrings_[i].size();
		memcpy(sizeBuf + offset, &strSize, 1);
		offset++;
		*dataPktSize += strSize;
	}

	wsaSize->buf = sizeBuf;
	wsaSize->len = sizePktSize;
	return wsaSize;
}

/**
* Populates a WSABUF with the variable-sized data to be transmitted.
*
* @param dataPktSize The size of the packet to
*/
shared_ptr<WSABUF> SongData::makeDataPacket(uint32_t dataPktSize) const
{
	shared_ptr<WSABUF> wsaData(new WSABUF);
	int offset = 0;
	wsaData->buf = (char *)malloc(dataPktSize);

	if (!wsaData->buf)
		throw "Metadata::makeDataPacket: Could not allocate memory for buffer.";

	// Copy the actual strings into the data packet
	for (auto s : metaStrings_)
	{
		string::size_type strSize = s.size();
		memcpy(wsaData->buf + offset, s.c_str(), strSize);
		offset += strSize;
	}

	return wsaData;
}

/**
* Returns the metadata string requested by @a data. Throws an
* exception if the index is out of range.
*
* @param data The metadata string to get.
* @return The requested metadata string.
*
* @designer Shane Spoor
* @author   Shane Spoor
* @date     April 5th, 2014
*/
string SongData::getData(int data) const
{
	if (data >= NUM_METASTRS)
		throw "Metadata::getMetadata: Index out of range";

	return metaStrings_[data];
}
