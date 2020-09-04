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


struct Variable
{
	float *data;
	std::string name;	// e.g pressure, temp, position (points are stored as array of x, array of y, array of z)
	int numDims;		// 1 for points, 2 for 2d grids, 3 for 3d grids
	int dimSize[3];		// we support only up to 3D meshes now

	Variable(){};
	Variable(std::string _name, int _numDims, int dimX, int dimY, int dimZ, float *_data)
	{
		name = _name;
		numDims = _numDims;
		dimSize[0] = dimX;
		dimSize[1] = dimY;
		dimSize[2] = dimZ;

		data = new float[dimX*dimY*dimZ];
		for (int i=0; i<(dimX*dimY*dimZ); i++)
			data[i] = _data[i];
	}
};


class Dataset
{
	std::string type; // mesh or point 
	
  public:
	std::vector<Variable> variables;

	Dataset(){};
	void addVariable( Variable x ){ variables.push_back(x); }
};


class CinLoaderInterface
{
	//
	// All renderers should inherit from this class and implement render
	
  protected:
	std::string filename;
	
	
  public:
	Dataset myData;
	
	CinLoaderInterface(){};
	CinLoaderInterface(std::string _filename){ setFilename(_filename); };
	~CinLoaderInterface(){};

	void setFilename(std::string _filename){ filename = _filename; };

	virtual void init() = 0;	
	virtual void load() = 0;						// renders to *image  - returns 1 for success
	void loadVariable(std::string variableName);
};

