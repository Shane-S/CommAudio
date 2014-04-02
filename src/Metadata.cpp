#include "Packets.h"
#include "Utils.h"

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
Metadata *parse_metadata(char *metadata, uint8_t *str_lens, uint32_t art_size)
{
	Metadata *meta_info = (Metadata *)malloc(sizeof(Metadata));
	int offset = 0;

	if (!metadata)
	{
		LogError(TEXT("parse_metadata"), TEXT("metadata was NULL"));
		exit(1);
	}
	else if (!meta_info)
	{
		LogError(TEXT("parse_metadata"), TEXT("couldn't allocate memory for meta_info"));
		exit(1);
	}

	meta_info->meta_strings = (char **)malloc(sizeof(char *)* NUM_METASTRS);
	if (!meta_info->meta_strings)
	{
		LogError(TEXT("parse_metadata"), TEXT("couldn't allocate memory for meta_strings"));
		exit(1);
	}

	meta_info->meta_strings[NAME] = (char *)malloc(str_lens[NAME] + 1);
	meta_info->meta_strings[ARTIST] = (char *)malloc(str_lens[ARTIST] + 1);
	meta_info->meta_strings[ALBUM] = (char *)malloc(str_lens[ALBUM] + 1);

	if (!meta_info->meta_strings[NAME] || !meta_info->meta_strings[ARTIST] || !meta_info->meta_strings[ALBUM])
	{
		LogError(TEXT("parse_metadata"), TEXT("couldn't allocate memory for meta_strings element"));
		exit(1);
	}

	for (int i = 0; i < NUM_METASTRS; i++)
	{
		memcpy(meta_info->meta_strings[i], metadata + offset, str_lens[i]);
		offset += str_lens[i];
	}

	meta_info->album_art = (char *)malloc(art_size);
	if (!meta_info)
	{
		LogError(TEXT("parse_metadata"), TEXT("couldn't allocate memory for album art"));
		exit(1);
	}
	memcpy(meta_info->album_art, metadata + offset, art_size);
	offset += art_size;

	memcpy(&meta_info->track, metadata + offset, sizeof(uint32_t));
	memcpy(&meta_info->year, metadata + offset + sizeof(uint32_t), sizeof(uint32_t));

	return meta_info;
}

/**
 * Frees the memory occuppied by a metadata struct.
 *
 * @param metadata The metadata struct to free.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     March 29th, 2014
 */
void destroy_metadata(Metadata *metadata)
{
	for (int i = 0; i < NUM_METASTRS; i++)
		free(metadata->meta_strings[i]);

	free(metadata->meta_strings);
	free(metadata);
}
