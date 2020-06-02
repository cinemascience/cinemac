#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

#ifndef CINBASE_H
#define CINBASE_H

class CinBase {

    public:

    static std::string& GetNextIDString()
    {
        GetNextID();
        return NextIDString;
    }

    private:
    static int NextID;
    static int NextIDStringWidth;
    static std::string NextIDString;

    static int GetNextID()
    {
        NextID++;

        // set the string as well
        std::ostringstream ss;
        ss << std::setw( NextIDStringWidth ) << std::setfill( '0' ) << NextID;
        NextIDString = ss.str();

        return NextID;
    }


};

#endif
