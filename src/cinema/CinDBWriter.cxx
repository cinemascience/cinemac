#include "CinDBWriter.h"
#include <sys/stat.h>
#include <iostream>



bool CinDBWriter::write()
{
    bool result = true;

    // make the destination directory
    mkdir(this->path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );

    // write data to csv file
    ofstream csvfile;
    std::string csvpath = this->path;
    csvpath += "/data.csv";
    std::cout << csvpath << std::endl;
    csvfile.open( csvpath.c_str() );
    if (csvfile.is_open())
    {
        csvfile << "phi,theta,FILE" << std::endl;

        std::vector<float>::iterator ip = this->phi.begin(); 
        std::vector<float>::iterator it = this->theta.begin(); 
        int curfile = 0;
        while (ip != this->phi.end())
        {
            csvfile << *ip << "," << *it << "," << curfile << ".png" << std::endl;
            ++ip; 
            ++it; 
            ++curfile;
        }
    } else {
        std::cout << "unable to open file: " << csvpath << std::endl;
    }
    csvfile.close();

    return result;
}
