#include "ahm.h"
#include "bass.h"
void sendAudioData(const char *data, bool isTCP, bool isFile, SOCKET socket);
using namespace std;
#define sRate 44100
#define SERVER_TCP_PORT 7000
#define BUFSIZE         2048

int main(void)
{
	cout << "Reading library in.." << endl;
	
	std::unique_ptr<AudioLibrary> lib(new AudioLibrary(string("C:\\Users\\RazLT\\Music\\"), string(",mp3,"), 1, 5)); 
	
	for(int i = 0; i < lib->numsongs; i++)
	{
		cout << "Title: " << lib->songList.at(i).name << " | Artist: " <<  lib->songList.at(i).artist << " | Album: " << lib->songList.at(i).album << endl;
		cout << "Bitrate: " << lib->songList.at(i).bitrate << " | Channels: " << lib->songList.at(i).channels << " | SampleRate: " << lib->songList.at(i).sampleRate << endl;
		cout << "Length: " << lib->songList.at(i).length << endl;
	}
	BASS_Init(-1, 44100, 0,0,0);
	int	n, bytes_to_read;
    int	client_len, port = SERVER_TCP_PORT, err;
	SOCKET sd, new_sd;
	struct	sockaddr_in server, client;
	char	*bp, buf[BUFSIZE];
	WSADATA WSAData;
	WORD wVersionRequested;
	
	wVersionRequested = MAKEWORD( 2, 2 ); 
	err = WSAStartup( wVersionRequested, &WSAData );
	if ( err != 0 ) //No useable DLL
	{
		printf ("DLL not found!\n");
		exit(1);
	}

	// Create a stream socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror ("Can't create a socket");
		exit(1);
	}

	// Initialize and set up the address structure
	memset((char *)&server, 0, sizeof(struct sockaddr_in)); 
	server.sin_family = AF_INET;
	server.sin_port = htons(port); 
	server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client
	
	// Bind an address to the socket
	if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("Can't bind name to socket");
		exit(1);
	}

	// Listen for connections
	// queue up to 5 connect requests
	listen(sd, 5);

    struct sockaddr_in sin;
    int len = sizeof(sin);
    if (getsockname(sd, (struct sockaddr *)&sin, &len) == -1)
        perror("getsockname");
    else
        printf("port number %d\n", ntohs(sin.sin_port));

	while (TRUE)
	{
		client_len= sizeof(client); 
		if ((new_sd = accept (sd, (struct sockaddr *)&client, &client_len)) == -1)
		{
			fprintf(stderr, "Can't accept client\n"); 
			exit(1);
		}
        else
        {

		    printf(" Remote Address:  %s\n", inet_ntoa(client.sin_addr));
		    bp = buf;
		    bytes_to_read = BUFSIZE;

            /*
		    while ((n = recv (new_sd, bp, bytes_to_read, 0)) < BUFSIZE)
		    {
			    bp += n;
			    bytes_to_read -= n;
			    if (n == 0)
				    break;
		    } 
            */
		
            while(1)
            {
				const char * dir = lib->songList.at(2).directory.c_str();
                sendAudioData(dir, true, true, new_sd);
            }

		    //ns = send (new_sd, buf, BUFSIZE, 0);
		    closesocket (new_sd);
        }
	}

	closesocket(sd);
	WSACleanup();
	cout << "Please press any key to exit the program ..." << endl;
	fprintf(stderr, "DONE"); 
	cin.get();

	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION: sendAudioData(void *data, bool isTCP, bool isFile)
*
* DATE: April 4, 2014
*
* REVISIONS: (Date and Description)
*
* DESIGNER: Abhishek Bhardwaj
*
* PROGRAMMER: Abhishek Bhardwaj
*
* INTERFACE: int ClientNetwork::sendAudioData(void *data, bool isTCP, bool isFile)
* void *data - data to be sent to the server [could be a filename or pointer to a memory location]
* bool isTCP - switch to specify what protocol to use to send the data [true for TCP, false for UDP]
* bool isFile - switch to specify if the *data is an actual file on disk or a pointer to a memory location.
* RETURNS: void
*
* NOTES: Sends audio data to the server (capable to do it with both TCP or UDP). Audio data could be an actual file
* or an actual memory location.
--------------------------------------------------------------------------------------------------------------------------*/
void sendAudioData(const char * filename, bool isTCP, bool isFile, SOCKET socket)
{
    char streamDataBuffer[2048];
    HSTREAM streamBuffer;
	DWORD readLength = 0;
    DWORD SendBytes = 0;
    DWORD BytesTransferred = 0;
    WSABUF buffer;

    WSAOVERLAPPED ov;
    ZeroMemory(&ov, sizeof(WSAOVERLAPPED));
    
    streamBuffer = BASS_StreamCreateFile(FALSE, filename, 0, 0, BASS_STREAM_DECODE);
	int opo = BASS_ErrorGetCode();

	BASS_SAMPLE * bInfo;

	int current_len = 0;
    while(1)
    {	
        readLength = BASS_ChannelGetData(streamBuffer, streamDataBuffer, 2048);
		int err = BASS_ErrorGetCode();

        buffer.len = 2048;
        buffer.buf = streamDataBuffer;
		current_len += 2048;

        err = WSASend(socket, &buffer, 1, &SendBytes, 0, 0, NULL);
      
        ZeroMemory(&ov, sizeof(WSAOVERLAPPED));
    }
}