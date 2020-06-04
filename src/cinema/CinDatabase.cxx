#include "CinDatabase.h"
#include <string>

std::string CinDatabase::DataFileName = "data.csv";

CinDatabase::CinDatabase()
{
    this->nextArtifactID    = -1;
    this->artifactIDPadding = 4;
}
