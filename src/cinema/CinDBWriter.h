#include "CinBase.h"

#ifndef CINDBWRITER_H
#define CINDBWRITER_H

class CinDBWriter : public CinBase
{
    public:

    CinDBWriter(const std::string& path)
    {
        this->path = path;
    }

    bool write();

    protected:

    std::string path;
};
#endif
