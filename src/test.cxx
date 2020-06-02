#include <string>
#include <iostream>
#include "CinBase.h"

int main() {

    // create some IDs and print them
    std::cout << CinBase::GetNextIDString() << std::endl;
    std::cout << CinBase::GetNextIDString() << std::endl;
    std::cout << CinBase::GetNextIDString() << std::endl;

    return 0;
}
