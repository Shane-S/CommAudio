#include "ahm.h"
using std::string;
using std::list;
using std::shared_ptr;

AudioLibrary::AudioLibrary(string directory, string supportedTypes, int numTypes)
{
	if (numTypes == 0)
	{
		std::cerr << "At least one type must be provided in the format \"wav, mp3, ...\"" << std::endl;
	}

	this->numsongs = 0;
	this->maxSongs = 10000;
	this->playingSong = 0;

	for (int i = 0; i < numTypes; i++)
	{
		if (supportedTypes.at(i) == ',')
		{
			string type = supportedTypes.substr(i + 1, 3);
			this->supportedTypes.push_back(type);
		}
	}

	this->directory = directory;
}

/**
* Constructor for the audio library. Sets the supported types
* and the song library from the passed in directory.
*
* Depending on the size of the audio library, this object could be quite large. Ensure that this object is safe
* in memory with unique_ptr or new.
*
* @param[in]	directory		the directory to be turned into an audio library.
* @param[in]	supportedTypes	the audio types supported by this library.
*								Must be in the form ",wav,mp3,ogg,...". Must contain at least one format.
* @param[in]	numTypes		the number of types supported.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
AudioLibrary::AudioLibrary(string directory, string supportedTypes, int numTypes, int maxSongs)
{
	if (numTypes == 0)
	{
		std::cerr << "At least one type must be provided in the format \"wav, mp3, ...\"" << std::endl;
	}

	this->numsongs = 0;
	this->maxSongs = maxSongs;

	this->directory = directory;
	for (int i = 0; i < numTypes; i++)
	{
		if (supportedTypes.at(i) == ',')
		{
			string type = supportedTypes.substr(i + 1, 3);
			this->supportedTypes.push_back(type);
		}
	}

	retrieveDirectory();
}
/**
* Retrieves all songs contained within the directory specified at construction.
* Will only add songs to the library if they are a supported format. Assumes the library is structured
* in this way
*	main directory
*		album
*			individual songs
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
void AudioLibrary::retrieveDirectory()
{
	DIR *dir, *songDir;
	struct dirent *ent, *songEnt;
	string artDirectory;
	char albumDirectory[500] = "";

	if ((dir = opendir(directory.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (!checkName(ent->d_name))
			{
				continue;
			}

			memset(albumDirectory, 0, 500);
			strcat_s(albumDirectory, directory.c_str());
			strcat_s(albumDirectory, ent->d_name);

			artDirectory = getCoverArt(string(albumDirectory));

			if ((songDir = opendir(albumDirectory)) != NULL)
			{
				while ((songEnt = readdir(songDir)) != NULL)
				{
					int err = grabSongs(albumDirectory, artDirectory, songEnt);
					
					if(err == -2) // song is invalid
						continue;

					if(err == -1) // song library full
						break;
				}
				closedir(songDir);
			}
			if (numsongs == maxSongs){
				break;
			}
		}
		closedir(dir);
	}
}
/**
* Checks a directory for a song. Creates a new SongData object and adds it to the library
* if the song has a valid name. An exception is thrown and caught otherwise.
* 
* @param[in]	albumDirectory	the directory to be searched for songs in.
* @param[in]	artDirectory	the album art directory to be passed to song data.
* @param[in]	songEnt			structure containing the name of the 
* @return		int				-2 signals the directory had an invalid song, -1 signals that the library is full.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
int AudioLibrary::grabSongs(const char * albumDirectory, string artDirectory, struct dirent * songEnt)
{
	string fileType;
	char directory[500];
	memcpy(directory, albumDirectory, 500);

	if (numsongs == maxSongs)
		return -1;

	if (!checkName(songEnt->d_name))
		return -2;

	fileType = getFileType(string(songEnt->d_name));
	if (checkType(fileType))
	{
		strcat_s(directory, "\\");
		strcat_s(directory, songEnt->d_name);
		try
		{
			SongData sData(string(directory), artDirectory, fileType);
			this->numsongs++;
			this->songList.push_back(sData);
		}
		catch (const std::invalid_argument& e){
			std::cout << "Invalid song name!" << std::endl;
			return -2;
		}
	}

	return 0;
}
/**
* Checks for an invalid directory name.
*
* Returns FALSE if the name is invalid, true if it isn't.
*
* @param[in]	name	the name to be checked.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
bool AudioLibrary::checkName(const char * name)
{
	if ((strcmp(name, " ") == 0) || (strcmp(name, ".") == 0) || (strcmp(name, "..") == 0))
	{
		return FALSE;
	}

	return TRUE;
}
/**
* Grabs the file type from the file name.
*
* Returns the file type on success, an empty string on failure.
*
* @param[in]	file	the file to be checked.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
string AudioLibrary::getFileType(string file)
{
	string type;

	for (int i = 0; i < file.length(); i++)
	{
		if (file.at(i) == '.')
		{
			if (file.length() - i == 4)
			{
				type = file.substr(i + 1, 3);
				return type;
			}
		}
	}

	return "";
}
/**
* Grabs the directory path for the cover art.
*
* Only looks for a cover.jpg or a cover.png for album art. If none exists,
* an empty string is returned.
*
* @param[in]	songDirectory	the directory path to be searched.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
string AudioLibrary::getCoverArt(string songDirectory)
{
	DIR * dir;
	struct dirent *ent;

	if ((dir = opendir(songDirectory.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if ((strcmp(ent->d_name, "cover.jpg") == 0) || (strcmp(ent->d_name, "cover.png") == 0) || (strcmp(ent->d_name, "cover.gif") == 0))
			{
				return songDirectory + "\\" + string(ent->d_name);
			}
		}
	}

	return "N/A";
}
/**
* Checks if the file type is supported by the current audio library.
*
* Returns FALSE if the type isn't supported, TRUE otherwise.
*
* @param[in]	type	the type to be checked.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
bool AudioLibrary::checkType(string type)
{
	for (int i = 0; i < supportedTypes.size(); i++)
	{
		if (type == supportedTypes.at(i))
			return TRUE;
	}
	return FALSE;
}
/**
* Takes the songlist and puts it in a format for easy data transfer.
*
* Returns a vector of strings, where each string is the song metadata of the song at the same point.
*
* Returns a vector of song metadata strings.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
list<shared_ptr<WSABUF>> AudioLibrary::grabPlaylist()
{
	list<shared_ptr<WSABUF>> plist;
	int songs = (numsongs < 15) ? numsongs : 15;
	sentSongs += songs;

	for (int i = 0; i < songs; i++)
	{
		plist.splice(plist.end(), songList[i].serialize());
	}

	return plist;
}
/**
* Resets the song data sent counter.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
void AudioLibrary::resetSongsSent()
{
	sentSongs = 0;
}
/**
* Grabs a random song. It's not very random.
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
SongData AudioLibrary::grabRandom()
{
	int rand_v = rand() % numsongs;
	return songList[rand_v];
}
/**
* Grabs a file based on the specified title, artist and album. 
*
* Throws an invalid argument exception when no file matching the parameters is found.
*
* @param[in] title	title of the song.
* @param[in] artist artist of the song.
* @param[in] album	album of the song
* @return	 SongData A Song Data object that matches the specified parameters 
*
* @designer Ramzi Chennafi
* @author   Ramzi Chennafi
* @date     April 3rd, 2014
*/
SongData AudioLibrary::grabSpecifiedFile(string title, string artist, string album)
{
	int count = 3, match = 0;

	SongData sData = songList[0];

	for (int i = 0; i < numsongs; i++)
	{
		if (match == 3)
			return sData;
		else
			match = 0;

		sData = songList[i];
		
		if (sData.getData(TITLE) == title)
			match++;
		if (sData.getData(ARTIST) == artist)
			match++;
		if (sData.getData(ALBUM) == album)
			match++;
	}

	throw std::invalid_argument("No file found");
}