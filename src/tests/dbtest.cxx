// #include <string>
// #include <iostream>
// #include <unistd.h>
// #include <stdio.h>
#include "CinBase.h"
#include "CinDBWriter.h"

int main() 
{
    // create a writer
    CinDBWriter writer("example.cdb");

    // set the input file 
    writer.setInputFile("some.pvtu");

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

    // write the database
    writer.write();

    return 1;
}
