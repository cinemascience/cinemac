// #include "CinBase.h"
#include "CinDatabase.h"
// #include "CinDBWriter.h"

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
