// #include <string>
#include <iostream>
// #include <unistd.h>
// #include <stdio.h>
#include "CinBase.h"
#include "CinDBWriter.h"

int main(int argc, char **argv) 
{
    // create a writer
    CinDBWriter writer("example.cdb");

    // set the input file 
    writer.setInputFile("some.pvtu");

    if (argc == 3) {
        writer.setPath(argv[1]);
        writer.setInputFile(argv[2]);
    } else {
        std::cout << "Usage:" << std::endl;
        std::cout << "    dbtest <output database> <input data file>" << std::endl;
        exit(1);
    }

    // set camera positions 
    writer.addCameraPosition(  0.0, 45.0);
    writer.addCameraPosition( 10.0, 45.0);
    writer.addCameraPosition( 20.0, 45.0);
    writer.addCameraPosition( 30.0, 45.0);
    writer.addCameraPosition( 40.0, 45.0);
    writer.addCameraPosition( 50.0, 45.0);
    writer.addCameraPosition( 60.0, 45.0);
    writer.addCameraPosition( 70.0, 45.0);
    writer.addCameraPosition( 80.0, 45.0);
    writer.addCameraPosition( 90.0, 45.0);

    // add timesteps
    writer.addTimestep( 1.0 );
    writer.addTimestep( 2.0 );
    writer.addTimestep( 3.0 );

    // write the database
    writer.write();

    return 1;
}
