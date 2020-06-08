#include "CinDatabase.h"

std::string CinDatabase::DataFileName = "data.csv";
int CinBase::NextID = -1;
int CinBase::NextIDStringWidth = 4;
std::string CinBase::NextIDString = "none";

int main() 
{
    // create a cinema database
    CinDatabase db;
    db.setPath("example.cdb");

    // check the database
    std::cout << db.getPath() << std::endl;
    std::cout << db.getDataFilePath() << std::endl;
    std::cout << db.getNextArtifactIDString() << std::endl;
    std::cout << db.getNextArtifactIDString() << std::endl;
    std::cout << db.getNextArtifactIDString() << std::endl;

    return 1;
}
