#include "CinBase.h"
#include <string>
#include <iomanip>
#include <sstream>

#ifndef CINDATABASE_H
#define CINDATABASE_H

class CinDatabase : public CinBase 
{

    public:
    CinDatabase();

    void setPath( std::string pathstring )
    {
        this->path = pathstring; 
        this->datafilepath = this->path + "/" + CinDatabase::DataFileName;
        
    }

    const std::string & getPath()
    {
        return this->path;
    }

    const std::string & getDataFilePath()
    {
        return this->datafilepath;
    }

    std::string getNextArtifactIDString()
    {
        ++(this->nextArtifactID);

        std::ostringstream ss;
        ss << std::setw( this->artifactIDPadding ) << std::setfill( '0' ) << this->nextArtifactID; 

        return ss.str();
    }

    private:

    std::string path;
    std::string datafilepath;
    int         nextArtifactID;
    int         artifactIDPadding;

    // class statics
    static std::string DataFileName;


};

#endif
