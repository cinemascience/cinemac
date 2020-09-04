#pragma once

#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <utility> 

#include "timer.hpp" 
#include "utils.hpp"
#include "log.hpp"
#include "CinRendererFactory.h"



class CinDatabase
{
    std::string path;
	std::vector< std::pair <float,float> > phi_theta; 

  public:
	CinRenderInterface *cinRenderer;

    CinDatabase(std::string dbName, std::string renderer, std::string dataLoader);
	void addCameraPosition(float phi, float theta);
	void createCinemaDB(int width, int height);
};


CinDatabase::CinDatabase(std::string dbName, std::string renderer, std::string dataLoader)
{
	// Create the folders for the cinema database
	path = dbName;
	createFolder(path);

	std::string imageFolder = path + "/image";
	createFolder(imageFolder);

	cinRenderer = CinRendererFactory::createRenderer(renderer);
	cinRenderer->cinDataLoader = CinDataLoaderFactory::createLoader(dataLoader);
}


inline void CinDatabase::addCameraPosition(float phi, float theta)
{
	phi_theta.push_back( std::make_pair(phi,theta) );
}


inline void CinDatabase::createCinemaDB(int width, int height)
{
	Timer clock;

  clock.start("render");
	// Set window size and angles for phi, theta
	cinRenderer->setWindowSize(width, height);
	cinRenderer->setCameraPositions(phi_theta);

	// Render the images
	cinRenderer->init();
	cinRenderer->render();
  clock.stop("render");
  debugLog << "Render: " << clock.getDuration("render") << " s" << std::endl;


  clock.start("create_struc");

	// Create the Cinema Database
	std::ofstream csvfile;
	std::string csvpath = path + "/data.csv";

	csvfile.open( csvpath.c_str() );
	if (csvfile.is_open())
	{
		csvfile << "phi,theta,FILE" << std::endl;
		int count = 0;
		for (auto pt=phi_theta.begin(); pt<phi_theta.end(); pt++)
		{
			// Save the image to disk
			std::string imageName = path + "/image/_" + std::to_string((*pt).first) + "_" + std::to_string((*pt).second) + "_img.jpg";
			std::string imageDBName =       "image/_" + std::to_string((*pt).first) + "_" + std::to_string((*pt).second) + "_img.jpg";
		  clock.start("save-to-disk");
			cinRenderer->createJPG(imageName, count);
		  clock.stop("save-to-disk");
		  	debugLog << " - save-to-disk: " << clock.getDuration("save-to-disk") << " s" << std::endl;


			// Fill in row in csv file
			std::string imagePath = imageName;
			csvfile << std::to_string((*pt).first) << "," 
					<< std::to_string((*pt).second) << "," 
					<< imageDBName << std::endl;

			++count;
		}
	} 
	else 
	{
		std::cout << "unable to open file: " << csvpath << std::endl;
	}
	csvfile.close();
  clock.stop("create_struc");
  debugLog << "create_struc: " << clock.getDuration("create_struc") << " s" << std::endl;
}


