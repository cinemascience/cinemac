#include "CinDBWriter.h"
#include <sys/stat.h>
#include <iostream>



bool CinDBWriter::write()
{
    bool result = true;

    mkdir(this->path.c_str(), S_IRUSR | S_IWUSR );

    return result;
}
