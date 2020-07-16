#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <utility> 

#include "utils.hpp"
#include "CinRendererFactory.h"

#pragma once


class CinDatabase
{
    std::string path;
	std::vector< std::pair <float,float> > phi_theta; 

  public:
	CinRenderInterface *cinRenderer;

    CinDatabase(std::string dbName, std::string renderer);
	void addCameraPosition(float phi, float theta);
	void createCinemaDB(int width, int height);
};


CinDatabase::CinDatabase(std::string dbName, std::string renderer)
{
	// Create the folders for the cinema database
	path = dbName;
	createFolder(path);

	std::string imageFolder = path + "/image";
	createFolder(imageFolder);

	cinRenderer = CinRendererFactory::createRenderer(renderer);
}


inline void CinDatabase::addCameraPosition(float phi, float theta)
{
	phi_theta.push_back( std::make_pair(phi,theta) );
}



inline void CinDatabase::createCinemaDB(int width, int height)
{
	// Set window size and angles for phi, theta
	cinRenderer->setWindowSize(width, height);
	cinRenderer->setCameraPositions(phi_theta);

	// Render the images
	cinRenderer->init();
	cinRenderer->render();

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
			std::string imageName = path + "/image/" + "_" + std::to_string((*pt).first) + "_" + std::to_string((*pt).second) + "_img.png";
			cinRenderer->createPNG(imageName, count);

			// Fill in row in csv file
			std::string imagePath = imageName;
			csvfile << std::to_string((*pt).first) << "," 
					<< std::to_string((*pt).second) << "," 
					<< imageName << std::endl;

			++count;
		}
	} 
	else 
	{
		std::cout << "unable to open file: " << csvpath << std::endl;
	}
	csvfile.close();
	
}


