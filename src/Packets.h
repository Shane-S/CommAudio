#ifndef PACKETS_H
#define PACKETS_H

#include <cstdint>
#include "Metadata.h"

// Packet types
#define CLIENT_SERVICE   0
#define CLIENT_MUSIC_CTL 1
#define SERVER_VOICE     2
#define SERVER_SONG_DATA 3
#define SERVER_SRCH_RES  4

// Stream control messages (client to server)
#define STREAM_START  0 /**< Tells the server to start the stream. */
#define STREAM_STOP   1 /**< Tells the server to stop the stream. */

// Service requests (client to server)
#define SERVICE_MUSIC 0 /**< The client wants to stream music. */
#define SERVICE_VOICE 1 /**< The client wants to join voice chat. */

/**
 * Allows the client to request the type of service they want.
 *
 * @property service_type   The type of service (MUSIC or VOICE) desired by the
 *                          client.
 * @property clientname_len The length of the client's name, if specified. If the
 *                          client doesn't specify a name, their IP address will
 *                          be used as a voice chat identifier instead.
 *
 * @struct C_SERVICE_PKT
 */
typedef struct pkt00
{
	uint8_t service_type;
	uint8_t clientname_len;
} C_SERVICE_PKT;

/**
 * Sent when the user wants to stop or start streaming a file.
 *
 * @property control_request What to do on the server side (START or STOP the stream).
 * @property file_offset     The offset from which to start (ignored if command is STOP).
 * @property filename_len    The length of the name of the audio file on which to invoke the
 *                           command. If the server receives a STOP on one file and a START 
 *                           on anoher, it assumes that the first file is to be closed.
 *
 * @struct C_MUSIC_CTRL_PKT
 */
typedef struct pkt01
{
	uint8_t  control_request;
	uint32_t file_offset;
	uint8_t  filename_len;
} C_MUSIC_CTRL_PKT;

/**
 * Sent from the server to other clients in the voice chat.
 *
 * The proceeding packet will contain the name (or the IP address, if the
 * client didn't specify a name) of the client talking as well as the actul
 * voice data.
 *
 * @property clientname_len The length of the sending client's name.
 * @property audio_len      The size, in bytes, of the audio chunk.
 *
 * @struct S_VOICE_PKT
 */
typedef struct pkt02
{
	uint8_t  clientname_len;
	uint32_t audio_len;
} S_VOICE_PKT;

/**
 * Sends details about song metadata to follow.
 *
 * @property str_lens   The length of each part of the metadata (name, artist, album).
 * @property img_size   The size (in bytes) of the album art included in the metadata.
 * @property song_size  The size (in bytes) of the song data to follow the metadata.
 * @property chunk_size The size of each chunk of audio data. Mainly applicable to WAV files.
 *
 * @struct S_SONG_DATA
 */
typedef struct pkt03
{
	uint8_t  str_lens[NUM_METASTRS];
	uint32_t img_size;
	uint32_t song_size;
	uint16_t chunk_size;
} S_SONG_DATA;

#endif
