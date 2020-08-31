#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

#include "CinDatabase.hpp"
#include "log.hpp"
#include "readUnstructuredGrid.h"

std::stringstream debugLog;


int main(int argc, char *argv[]) 
{

    std::string filename = "cinemaTest";
    std::string renderer = "VTK";
    int width, height;
    width =  512;
    height = 512;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-r" || arg == "--renderer") {
            renderer = argv[++i];
        }
        if (arg == "-i" || arg == "--imageSize") {
            width = std::stoi(argv[++i]);
            height = std::stoi(argv[++i]);
        }
        if (arg == "-f" || arg == "--filename") {
            filename = argv[++i];
        }
        else {
            std::cerr << "Unknown argument, assuming it is a filename): " << argv[i] << std::endl;
            filename = argv[i];
        }
    }

    debugLog << "Renderer: " << renderer << ", dims: " << width << " x " << height << " - filename: " << filename << std::endl;

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
<<<<<<< HEAD
    CinDatabase writer(filename + ".cdb", renderer);
=======
    CinDatabase writer("example.cdb", rendererName);
>>>>>>> 59777f68577e0f5e475da7a2e54fc349a9715ec1

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

