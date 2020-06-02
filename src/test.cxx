#include <string>
#include <iostream>
#include "CinBase.h"
#include "CinDBWriter.h"

int main() {

    // create some IDs and print them
    std::cout << CinBase::GetNextIDString() << std::endl;
    std::cout << CinBase::GetNextIDString() << std::endl;
    std::cout << CinBase::GetNextIDString() << std::endl;

    //
    CinDBWriter writer("/Users/dhr");
    writer.write();

    return 0;
}
