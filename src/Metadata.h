#ifndef METADATA_H
#define METADATA_H

#define NAME          0 /**< Song name's position in metadata array. */
#define ARTIST        1 /**< Song artist's position in metadata array. */
#define ALBUM         2 /**< Song album's position in metadata array. */
#define NUM_METASTRS  3 /**< Number of strings contained in metadata array. */

/**
 * Contains song metadata.
 *
 * @property meta_strings Array containing song metadata strings.
 * @property album_art    The album's cover image.
 * @property track        The song's track number on @a album.
 * @property year         The song's year of release.
 *
 * @struct Metadata
 */
typedef struct _Metadata
{
	char **meta_strings;
	char *album_art;
	int  track;
	int  year;
} Metadata;

#endif