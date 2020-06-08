#include "CinBase.h"
#include "CinDBWriter.h"

int CinBase::NextID = -1;
int CinBase::NextIDStringWidth = 4;
std::string CinBase::NextIDString = "none";

int main() 
{
    // create a writer
    CinDBWriter writer("example.cdb");

    // set the input file 
    writer.setInputFile("/home/pascal/projects/cinemac/src/tests/data/HACC_VTK_251_0.vtu");
    //writer.setInputFile("some.pvtu");


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
    //writer.addTimestep( 2.0 );
    //writer.addTimestep( 3.0 );

    // write the database
    writer.write();

    return 1;
}

// ./dbtest 