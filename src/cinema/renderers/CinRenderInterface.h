/*================================================================================
This software is open source software available under the BSD-3 license.
Copyright (c) 2017, Los Alamos National Security, LLC.
All rights reserved.

Authors:
 - Pascal Grosset
================================================================================*/
#pragma once

#include <iostream>
#include <vector>
#include <utility> 

#include "image.hpp"
#include "dataStructures.hpp"

#include "CinDataLoaderFactory.h"


class CinRenderInterface
{
	//
	// All renderers should inherit from this class and implement render
	
  protected:
	int height, width;
	std::vector< std::pair <float,float> > phi_theta; 

	std::vector<Structure> structuresList;  // data is in here
	std::vector<Image> imgs;				// results stored here
	
  public:
	CinLoaderInterface *cinDataLoader;

	
	CinRenderInterface(){};
	CinRenderInterface(int w, int h):width(w),height(h){};
	~CinRenderInterface(){};

	void addData(Structure temp){ structuresList.push_back(temp); }

	void setWindowSize(int w, int h);
	void setCameraPositions(std::vector< std::pair <float,float> > _phi_theta);

	void createPNG(std::string filename, int index);
	void createJPG(std::string filename, int index);

	virtual void init() = 0;	
	virtual void render() = 0;			// renders to *image  - returns 1 for success
};


inline void CinRenderInterface::setWindowSize(int _w, int _h)
{
	width = _w;
	height = _h;
}


inline void CinRenderInterface::setCameraPositions(std::vector< std::pair <float,float> > _phi_theta)
{
	phi_theta =_phi_theta;

	// Create space to store image
	imgs.resize( phi_theta.size() );
	for (int i=0; i<phi_theta.size(); i++)
		imgs[i].createImage(height, width);
}


inline void CinRenderInterface::createPNG(std::string filename, int index)
{
	imgs[index].outputPNG(filename);
}

inline void CinRenderInterface::createJPG(std::string filename, int index)
{
	imgs[index].outputJPG(filename);
}
