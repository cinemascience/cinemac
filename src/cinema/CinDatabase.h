#include <string>
#include <iomanip>
#include <sstream>

#include "CinBase.h"

#ifndef CINDATABASE_H
#define CINDATABASE_H



class CinDatabase : public CinBase 
{

  public:
    CinDatabase();

    void setPath( std::string pathstring );

    const std::string & getPath();
    const std::string & getDataFilePath();
    std::string getNextArtifactIDString();
  
  private:

    std::string path;
    std::string datafilepath;
    int         nextArtifactID;
    int         artifactIDPadding;

    // class statics
    static std::string DataFileName;
};



inline CinDatabase::CinDatabase()
{
    this->nextArtifactID    = -1;
    this->artifactIDPadding = 4;
}

inline void CinDatabase::setPath( std::string pathstring )
{
    this->path = pathstring; 
    this->datafilepath = this->path + "/" + CinDatabase::DataFileName;
    
}

inline const std::string & CinDatabase::getPath()
{
    return this->path;
}

inline const std::string & CinDatabase::getDataFilePath()
{
    return this->datafilepath;
}

inline std::string CinDatabase::getNextArtifactIDString()
{
    ++(this->nextArtifactID);

    std::ostringstream ss;
    ss << std::setw( this->artifactIDPadding ) << std::setfill( '0' ) << this->nextArtifactID; 

    return ss.str();
}

#endif
