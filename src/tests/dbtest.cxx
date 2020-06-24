#include <stdio.h> 
#include <stdlib.h> 
#include<time.h> 

#include "CinBase.h"
#include "CinDBWriter.h"

int CinBase::NextID = -1;
int CinBase::NextIDStringWidth = 4;
std::string CinBase::NextIDString = "none";

int main(int argc, char *argv[]) 
{
    // create a writer
    CinDBWriter writer("example.cdb");

    // set the input file 
    //writer.setInputFile(argv[1]);
    //writer.setInputFile("some.pvtu");

    std::vector<float> point_x, point_y, point_z;
    int numPoints = 5000;
    srand(time(0)); 
	for (int i=0; i<numPoints/2; i++)
	{
		point_x.push_back(rand()%50);
		point_y.push_back(rand()%50);
		point_z.push_back(rand()%50);
	}
    for (int i=numPoints/2; i<numPoints; i++)
	{
		point_x.push_back(rand()%50);
		point_y.push_back(rand()%50);
		point_z.push_back(rand()%50 + 100);
	}
    writer.setDataPoints(point_x, point_y, point_z);


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

    // write the database
    writer.write();

    return 1;
}

// tests/dbtest /home/pascal/projects/cinemac/testdata/extracted.pvtu