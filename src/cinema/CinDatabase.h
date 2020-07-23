#include "CinBase.h"
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <map>

#ifndef CINDATABASE_H
#define CINDATABASE_H

class CinDatabase : public CinBase 
{

    public:
    CinDatabase();

    void setPath( std::string pathstring )
    {
        this->path = pathstring; 
        
    }

    const std::string & getPath()
    {
        return this->path;
    }

    bool addParameter( const std::string &p )
    {
        bool result;

        if (std::find(this->parameters.begin(), this->parameters.end(), p) != this->parameters.end())
        {
            // this parameter exists 
            result = false;
        } else {
            result = true;
            this->parameters.push_back(p);
        }

        return result;
    } 

    const std::vector<std::string> & getParameters()
    {
        return this->parameters;
    }

    int addArtifact( const std::map<std::string, std::string> & values )
    {
        int id = CinDatabase::PARAMETER_DOES_NOT_EXIST; 
        // TODO: verify that there are no illegal parameters in the values list

        id = this->artifacts.size();
        this->artifacts[id] = values;

        return id;
    }

    bool getArtifact( int id, std::map<std::string, std::string> & results);

    int getNumArtifacts()
    {
        return this->artifacts.size();
    }

    void debugPrint();

    enum Errors {PARAMETER_DOES_NOT_EXIST=0}; 

    private:

    std::string path;
    std::vector<std::string> parameters;
    std::map<int, std::map< std::string, std::string >> artifacts;


};

#endif
