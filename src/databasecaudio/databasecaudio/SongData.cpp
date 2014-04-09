#include "SongData.h"
using std::string;
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
	if(fRef.isNull())
	{
		throw std::invalid_argument("Recieved invalid directory");
	}

	this->fileType = fileType;
	this->artDirectory = artDirectory;
	this->directory = directory;
	this->albumArt = 1;
	
	setMetadata(fRef);
	setProperties(fRef);	
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
	setArtist(toString(sRef.tag()->artist()));
	setAlbum(toString(sRef.tag()->album()));
	setTitle(toString(sRef.tag()->title()));
	this->year = sRef.tag()->year();
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
	
	length		= aProps->length();
	bitrate		= aProps->bitrate();
	channels	= aProps->channels();
	sampleRate	= aProps->sampleRate();
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
	if(artist.empty()){
		this->artist = string("N/A");
	}
	else{
		this->artist = artist;
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
	if(album.empty()){
		this->album = string("N/A");
	}
	else{
		this->album = album;
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
	if(title.empty()){
		name = string("N/A");
	}
	else{
		name = title;
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
void SongData::setYear(int year)
{
	if(year <= 0 || year >= 2015){
		this->year = 0;
	}
	else{
		this->year = year;
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
string SongData::toString(TagLib::String str)
{
	std::stringstream string_out;
	string_out << str;
	return string_out.str();
}
