/**
 * Contains structures for transfer details and buffering data.
 *
 * @file WinStorage.h
 */

#ifndef WIN_STORAGE_H
#define WIN_STORAGE_H

#include <WinSock2.h>

#define GWLP_TRANSFERPROPS	0						// Offset value to access the transfer props pointer in wndExtra 
#define GWLP_HOSTMODE		sizeof(LPTransferProps)	// Offset value to access the host mode pointer in wndExtra
#define FILENAME_SIZE		512						// The max file name size (in bytes)
#define HOSTNAME_SIZE		128						// The max host name size (in bytes)

#endif
