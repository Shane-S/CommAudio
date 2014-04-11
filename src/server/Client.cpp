#include "Client.h"

using std::string;

/**
 * Retrieves the client's name.
 *
 * @return The client's name.
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 9th, 2014
 */
string Client::getName() const
{
	return name_;
}

/**
 * Retrieves the client's IP address (in network byte order).
 *
 * @return The client's host address.
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 9th, 2014
 */
SOCKADDR_IN Client::getAddr() const
{
	return addr_;
}

/**
 * 
 *
 *
 *
 */
SOCKET Client::getSock() const
{
	return tcpSock_;
}

/**
 * 
 *
 *
 *
 *
 */
SOCKET Client::getUdpSock() const
{
	return udpSock_;
}

/**
 * Sets the client's name and returns a reference to the client object.
 *
 * @param newName The name to which to set this client's name.
 * @return A reference to the client object.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 10th, 2014
 */
Client& Client::setName(const std::string &newName)
{
	name_ = newName;
	return *this;
}

/**
 * Sets the client's sockaddr_in and returns a reference to the client object.
 *
 * @param newAddr The sockaddr_in to which to set this client's sockaddr_in.
 * @return A reference to the client object.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 10th, 2014
 */
Client& Client::setAddr(const SOCKADDR_IN &newAddr)
{
	addr_ = newAddr;
	return *this;
}

/**
 * Sets the client's socket and returns a reference to the client object.
 *
 * @param newSock The socket to which to set this client's socket.
 * @return A reference to the client object.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 10th, 2014
 */
Client& Client::setSock(const SOCKET &newSock)
{
	tcpSock_ = newSock;
	return *this;
}

/**
 * Set the UDP socket for sending and receiving microphone data.
 *
 * @param udpSock The UDP socket 
 */
Client& Client::setUdpSock(const SOCKET& udpSock)
{
	udpSock_ = udpSock;
	return *this;
}

/**
 * Retrieves the handle to the audio file currently open for this client.
 *
 * @return The stream handle for the currently open audio file or -1 if no file is open.
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 9th, 2014
 */
HSTREAM Client::getStreamHandle() const
{
	return stream_;
}

/**
 * Returns whether the application is streaming audio to this client.
 *
 * @return Whether we're streaming to the client.
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date April 9th, 2014
 */
bool Client::isInProgress() const
{
	return transferCancelled_;
}

/**
 * Stops sending the audio data to the client.
 *
 * @return Nothing
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 9th, 2014
 */
void Client::cancelStream()
{
	transferCancelled_ = true;
}

/**
 * Starts sending the stream at the specified file offset.
 *
 * @param fileName The name of the file to start streaming.
 * @param offset   The offset into the file from which to start streaming.
 * @return Nothing
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 9th, 2014
 */
void Client::startStream(string fileName, QWORD startOffset)
{
	transferCancelled_ = false;
	if (!stream_)
		throw "Client::startStream: No stream open!";
	else if (fileName != fileName_) // the client is requesting a file that we don't currently have open; close the current one and open it
	{
		BASS_StreamFree(stream_);
		stream_ = BASS_StreamCreateFile(FALSE, fileName.c_str(), startOffset, 0, 0);
	}
	BASS_ChannelSetPosition(stream_, startOffset, BASS_POS_BYTE);
	// Unicast send
}
