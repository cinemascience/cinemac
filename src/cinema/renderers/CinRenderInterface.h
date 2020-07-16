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

	std::vector<float> points;		// we are only rendering points for now
	
  public:
	std::vector<Image> imgs;		// results stored here

	CinRenderInterface(){};
	CinRenderInterface(int w, int h):width(w),height(h){};
	~CinRenderInterface(){};

	void setWindowSize(int w, int h);
	void setCameraPositions(std::vector< std::pair <float,float> > _phi_theta);
	void setDataPoints(std::vector<float>_x, std::vector<float>_y, std::vector<float>_z);

	virtual void init() = 0;	
	virtual void render() = 0;			// renders to *image  - returns 1 for success
};


void CinRenderInterface::setWindowSize(int _w, int _h)
{
	width = _w;
	height = _h;
}


void CinRenderInterface::setCameraPositions(std::vector< std::pair <float,float> > _phi_theta)
{
	phi_theta =_phi_theta;
}


void CinRenderInterface::setDataPoints(std::vector<float>_x, std::vector<float>_y, std::vector<float>_z)
{
	int numPoints = _x.size();
	for (size_t i=0; i<numPoints ; i++)
	{
		points.push_back(_x[i]);
		points.push_back(_y[i]);
		points.push_back(_z[i]);
	}
}