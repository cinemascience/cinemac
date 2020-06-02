#include <string>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "CinBase.h"
#include "CinDBWriter.h"

int main() {

    // create some IDs and print them
    std::cout << CinBase::GetNextIDString() << std::endl;
    std::cout << CinBase::GetNextIDString() << std::endl;
    std::cout << CinBase::GetNextIDString() << std::endl;

    //
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::string db = cwd;
    db += "/test.cdb";
    CinDBWriter writer(db);
    writer.write();

    return 1;
}
