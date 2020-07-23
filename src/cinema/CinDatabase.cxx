#include "CinDatabase.h"
#include <string>

CinDatabase::CinDatabase()
{
}

bool CinDatabase::getArtifact( int id, std::map<std::string, std::string> & results)
{
    bool result = false;
    results.clear();

    auto const& ait = this->artifacts.find(id);
    if (ait != this->artifacts.end())
    {
        result = true;
        for (auto const& pit : this->parameters) 
        {
            auto const& found = *(ait)->second.find(pit);
            if (found != *(ait)->second.end())
            {
                results[pit] = found.second; 
            } else {
                results[pit] = ""; 
            }
        } 
    }

    return result;
}

void CinDatabase::debugPrint()
{
    std::cout << "printing database ..." << std::endl;
    std::cout << "  path: " << this->path << std::endl;
    std::cout << "  parameters: " <<  std::endl;
    for (auto it = begin(this->parameters); it != end(this->parameters); it++)
    {
        std::cout << "    " << *it << std::endl;
    }
    std::cout << "  artifacts: " <<  std::endl;
    std::map<std::string, std::string> results;
    for (int i=0; i< this->getNumArtifacts(); i++ )
    {
        this->getArtifact(i, results);
        std::cout << "    ";
        for (auto const& x : results) 
        {
            std::cout << x.first << ":" << x.second << " ";
        }
        std::cout << std::endl; 
    }
}
