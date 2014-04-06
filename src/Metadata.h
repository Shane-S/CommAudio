#ifndef METADATA_H
#define METADATA_H

#include <memory>
#include <vector>
#include <list>
#include <string>

#define NAME          0 /**< Song name's position in metadata array. */
#define ARTIST        1 /**< Song artist's position in metadata array. */
#define ALBUM         2 /**< Song album's position in metadata array. */
#define NUM_METASTRS  3 /**< Number of strings contained in metadata array. */

/**
 * Contains song metadata.
 *
 * @class Metadata
 */
class Metadata
{
public:
	explicit Metadata(const char *metadata, const uint8_t strLens[NUM_METASTRS], const uint32_t art_size);
	explicit Metadata(const char *sizePkt, const char *dataPkt);
	~Metadata();

	std::string       getMetadata(int data) const;
	int               getYear() const;
	int               getTrack() const;
	int               getSongSize() const;
	int               getArtSize() const;
	const uint8_t     *getAlbumArt() const;
	std::list<std::shared_ptr<WSABUF> > serialise() const;

private:
	std::shared_ptr<WSABUF>  makeSizePacket(uint32_t *dataPktSize) const;
	std::shared_ptr<WSABUF>  makeDataPacket(uint32_t dataPktSize) const;

	std::vector<std::string> metaStrings_;
	uint8_t                  *albumArt_;
	uint8_t                  track_;
	uint8_t                  year_;
	uint32_t                 artSize_;
	uint32_t                 songSize_;
};

#endif