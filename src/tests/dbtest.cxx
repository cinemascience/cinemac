#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

#include "CinDatabase.hpp"

int main(int argc, char *argv[]) 
{
    std::vector<float> point_x, point_y, point_z;
    int numPoints = 50000;
    srand(time(0)); 
	for (int i=0; i<numPoints; i++)
	{
		point_x.push_back(rand()%100 - 50);
		point_y.push_back(rand()%100 - 50);
		point_z.push_back(rand()%100 - 50);
	}
    // for (int i=numPoints/2; i<numPoints; i++)
	// {
	// 	point_x.push_back(rand()%50);
	// 	point_y.push_back(rand()%50);
	// 	point_z.push_back(rand()%50 + 100);
	// }



    // create a writer
    CinDatabase writer("example.cdb", "VTK");

    writer.cinRenderer->setDataPoints(point_x, point_y, point_z);

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
    writer.createCinemaDB(640, 480);

    return 1;
}

// tests/dbtest /home/pascal/projects/cinemac/testdata/extracted.pvtu