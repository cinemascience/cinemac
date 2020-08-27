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



class CinRenderInterface
{
	//
	// All renderers should inherit from this class and implement render
	
  protected:
	int height, width;
	std::vector< std::pair <float,float> > phi_theta; 
	float radius;
	float center[3];

	std::vector<float> points;		// we are only rendering points for now

	std::vector<Image> imgs;		// results stored here
	
  public:
	
	CinRenderInterface(){};
	CinRenderInterface(int w, int h):width(w),height(h){};
	~CinRenderInterface(){};

	void setOrigin(float _x, float _y, float _z){ center[0]=_x; center[1]=_y; center[2]=_z; }
    void setRegionRadius(float r){ radius = r; }

	void setWindowSize(int w, int h);
	void setCameraPositions(std::vector< std::pair <float,float> > _phi_theta);
	void setDataPoints(std::vector<float>_x, std::vector<float>_y, std::vector<float>_z);
	void createPNG(std::string filename, int index);

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


inline void CinRenderInterface::setDataPoints(std::vector<float>_x, std::vector<float>_y, std::vector<float>_z)
{
	int numPoints = _x.size();
	for (size_t i=0; i<numPoints ; i++)
	{
		points.push_back(_x[i]);
		points.push_back(_y[i]);
		points.push_back(_z[i]);
	}
}

inline void CinRenderInterface::createPNG(std::string filename, int index)
{
	imgs[index].outputPNG(filename);
}
