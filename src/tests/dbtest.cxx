#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

#include "CinDatabase.hpp"
#include "log.hpp"
#include "dataStructures.hpp"
#include "readUnstructuredGrid.h"

std::stringstream debugLog;


int main(int argc, char *argv[]) 
{

    std::string outFilename = "cinemaTest";
    std::string renderer = "VTK";
    std::string dataLoader = "VTK-UNSTRUCTURED";

    bool useTestData = true;

    //std::string testDataset = "/home/pascal/projects/cinemac/testdata/extracted.pvtu";
    std::string testDataset = "/Users/aaronkno/work/intel/cinemac/testdata/extracted.pvtu";
    //std::string testDataset = "/Users/aaronkno/Downloads/ts499.vtu"
    //std::string testDataset = "/projects/groups/vizproject/HACC_Halo/cinemac/testdata/extracted.pvtu"

    int width, height;
    width =  512;
    height = 512;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-r" || arg == "--renderer") {
            renderer = argv[++i];
        }
        else if (arg == "-i" || arg == "--imageSize") {
            width = std::stoi(argv[++i]);
            height = std::stoi(argv[++i]);
        }
        else if (arg == "-o" || arg == "--outFilename") {
            outFilename = argv[++i];
        }
        else if (arg == "-d" || arg == "--data") {
            testDataset = argv[++i];
            useTestData = true;
        }
        else {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
        }
    }

    debugLog << "Renderer: " << renderer << ", dims: " << width << " x " << height << " - outFilename: " << outFilename << std::endl;



     // create a writer
    CinDatabase writer(outFilename + ".cdb", renderer, dataLoader);


    std::vector<float> point_x, point_y, point_z;
    size_t numPoints;
    
    if (useTestData)
    {
        writer.cinRenderer->cinDataLoader->setFilename(testDataset);
        writer.cinRenderer->cinDataLoader->load();
    }
    else
    {
        numPoints = 30000;

        srand(time(0)); 
        for (int i=0; i<numPoints; i++)
        {
            point_x.push_back(1500+rand()%100 - 50);
            point_y.push_back(1500+rand()%100 - 50);
            point_z.push_back(1500+rand()%100 - 50);
        }

        writer.cinRenderer->addData( Structure(numPoints, point_x, point_y, point_z) );
    }

    if (getenv("CINEMAC_ONE_IMAGE") != 0)
        writer.addCameraPosition( 0,  0);
    else
    {
        // set camera positions 
        for (float phi=0; phi<350; phi+=60)
            for (float theta=0; theta<350; theta+=60) 
            writer.addCameraPosition( phi,  theta);
    }
    

   

    // write the database
    writer.createCinemaDB(width, height);


    writeLog(outFilename, debugLog.str());

    return 1;
}

// Run instructions:
// dbtest --renderer VTK --data /home/pascal/projects/cinemac/testdata/extracted.pvtu -f cinemaTestX
// dbtest --renderer OSPRAY --data /home/pascal/projects/cinemac/testdata/extracted.pvtu -f cinemaTestX
