#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

#include "CinDatabase.hpp"
#include "log.hpp"
#include "readUnstructuredGrid.h"

std::stringstream debugLog;


int main(int argc, char *argv[]) 
{
    std::string filename;
    std::string renderer = "VTK";
    int width, height;
    width =  512;
    height = 512;

    if (argc > 1)
        filename = std::string(argv[1]);
    else
        filename = "cinemaTest";

    debugLog << "Renderer: " << renderer << ", dims: " << width << " x " << height << " - filename: " << filename << std::endl;

    //writer.setInputFile(argv[1]);

    std::vector<float> point_x, point_y, point_z;
    int numPoints = 30000;

    // srand(time(0)); 
	// for (int i=0; i<numPoints; i++)
	// {
	// 	point_x.push_back(1500+rand()%100 - 50);
	// 	point_y.push_back(1500+rand()%100 - 50);
	// 	point_z.push_back(1500+rand()%100 - 50);
	// }
   

    float extents[6];
    readData("/projects/groups/vizproject/HACC_Halo/cinemac/testdata/extracted.pvtu", 223441, point_x, point_y, point_z, extents);



    // create a writer
    CinDatabase writer(filename + ".cdb", renderer);


    writer.cinRenderer->setDataPoints(point_x, point_y, point_z);
    //writer.cinRenderer->setOrigin(1500,1500,1500);
    //writer.cinRenderer->setRegionRadius(50);
    writer.cinRenderer->setExtents(extents);


    // set camera positions 
    for (float phi=0; phi<350; phi+=30)
        for (float theta=0; theta<350; theta+=30) 
          writer.addCameraPosition( phi,  theta);

    // write the database
    writer.createCinemaDB(width, height);

    writeLog(filename, debugLog.str());

    return 1;
}

