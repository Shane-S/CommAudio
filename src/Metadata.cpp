#include "Packets.h"
#include "Utils.h"

using std::vector;
using std::list;
using std::string;
using std::shared_ptr;

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
Metadata::Metadata(const char *metadata, const uint8_t strLens[NUM_METASTRS], const uint32_t artSize)
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

	memcpy(&track_, metadata + offset, sizeof(uint32_t));
	memcpy(&year_, metadata + offset + sizeof(uint32_t), sizeof(uint32_t));

	albumArt_ = (uint8_t *)malloc(artSize);
	if (!albumArt_)
		throw "Metadata::Metadata: Couldn't allocate memory for albumArt_";

	memcpy(&albumArt_, metadata + offset, artSize);
}

/**
 * Frees the album art array.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     March 29th, 2014
 */
Metadata::~Metadata()
{
	free(albumArt_);
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
std::string Metadata::getMetadata(int data) const
{
	if (data >= NUM_METASTRS)
		throw "Metadata::getMetadata: Index out of range";

	return metaStrings_[data];
}

/**
 * Gets a reference to the array of bytes containing the album art.
 *
 * The array cannot be manipulated by the caller, since it is a constant pointer.
 * 
 * @return A constant pointer to an array of bytes containing the image data.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 5th, 2014
 */
const uint8_t* Metadata::getAlbumArt() const
{
	return albumArt_;
}

/**
 * Gets the song's year of release.
 *
 * @return The song's year of release.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 3rd, 2014
 */
int Metadata::getYear() const
{
	return year_;
}

/**
 * Gets the track number of this song on the CD.
 *
 * @return The song's track number.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 3rd, 2014
 */
int Metadata::getTrack() const
{
	return track_;
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
list<shared_ptr<WSABUF> > Metadata::serialise() const
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
shared_ptr<WSABUF> Metadata::makeSizePacket(uint32_t *dataPktSize) const
{
	int    sizePktSize = NUM_METASTRS + sizeof(artSize_)+sizeof(track_)+sizeof(year_); // Size of the size packet
	size_t offset = 0;
	char   *sizeBuf = (char *)malloc(sizePktSize);
	shared_ptr<WSABUF> wsaSize(new WSABUF); // The WSABUF containing the size packet

	if (!sizeBuf)
		throw "Metadata::serialise: Could not allocate memory for sizePacket";

	// Copy all metadata string sizes into the size packet
	for (auto s : metaStrings_)
	{
		uint8_t strSize = s.size();
		memcpy(sizeBuf + offset, &strSize, 1);
		offset++;
		*dataPktSize += strSize;
	}

	// Copy the size of the album art into the size packet
	memcpy(sizeBuf + offset, &artSize_, sizeof(artSize_));
	*dataPktSize += artSize_;

	wsaSize->buf = sizeBuf;
	wsaSize->len = sizePktSize;
	return wsaSize;
}

/**
 * Populates a WSABUF with the variable-sized data to be transmitted.
 *
 * @param dataPktSize The size of the packet to 
 */
shared_ptr<WSABUF> Metadata::makeDataPacket(uint32_t dataPktSize) const
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

	// Copy the album art into the packet
	memcpy(wsaData->buf + offset, albumArt_, artSize_);

	wsaData->len = offset + artSize_;
	return wsaData;
}
