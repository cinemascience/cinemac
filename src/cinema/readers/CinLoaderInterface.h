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
#include <algorithm>
#include <iterator>

struct Variable
{
	std::string name;	// e.g pressure, temp, position 
	int numComponents;	// 3 for position(x,y,z), 1 for single variables e.g temp
	int numDims;		// 1 for points, 2 for 2d meshes, 3 for 3d meshes
	size_t dimSize[3];	// we support only up to 3D meshes now
	float extents[6];   // minX, minY, minZ,  maxX, maxY, maxZ;
	float *data;		// (points are stored as array of x, array of y, array of z)

	Variable(){};
	~Variable()
	{ 
		delete []data; data=NULL; 
		dimSize[0] = dimSize[1] = dimSize[2] = 0;
		numDims = 0;
		numComponents = 0;
		name = ""; 
	}

	Variable(const Variable &v2)
	{
		name = v2.name;
		numDims = v2.numDims;
		dimSize[0] = v2.dimSize[0];
		dimSize[1] = v2.dimSize[1];
		dimSize[2] = v2.dimSize[2];
		numComponents = v2.numComponents;

		for (int i=0; i<6; i++)
			extents[i] = v2.extents[i];

		// Deep copy just in case
		size_t numPoints = dimSize[0]*dimSize[1]*dimSize[2];
		data = new float[numPoints*numComponents];
		for (int i=0; i<numPoints*numComponents; i++)
			data[i] = v2.data[i];
	}

	Variable(std::string _name, int _numDims, int dimX, int dimY, int dimZ, int _numComponents, float *_data)
	{
		name = _name;
		numDims = _numDims;
		dimSize[0] = dimX;
		dimSize[1] = dimY;
		dimSize[2] = dimZ;
		numComponents = _numComponents;

		// Deep copy just in case
		size_t numPoints = dimSize[0]*dimSize[1]*dimSize[2];
		data = new float[numPoints];
		for (int i=0; i<numPoints; i++)
			data[i] = _data[i];
	}


	// specially for points
	Variable(std::string _name, size_t _numPoints, int _numComponents, float _extents[6], float *_data)
	{
		name = _name;
		numDims = 1;
		numComponents = _numComponents;
		dimSize[0] = 1;
		dimSize[1] = 1;
		dimSize[2] = _numPoints;

		for (int i=0; i<6; i++)
			extents[i] = _extents[i];

		size_t numPoints = dimSize[0]*dimSize[1]*dimSize[2];
		data = new float[numPoints*numComponents];
		for (int i=0; i<numPoints*numComponents; i++)
		{
			data[i] = _data[i];
			//std::cout << data[i] << std::endl;
		}
	}


	size_t getNumPoints(){ return dimSize[0]*dimSize[1]*dimSize[2]; }

	void setExtents(float _extents[6])
	{
		for (int i=0; i<6; i++)
			extents[i] = _extents[i];
	}
};



class CinLoaderInterface
{
	//
	// All renderers should inherit from this class and implement render
  protected:
	std::string filename;
	
  public:
	std::vector<Variable> variables;	// for simplicity
	
	CinLoaderInterface(){};
	CinLoaderInterface(std::string _filename){ setFilename(_filename); };
	~CinLoaderInterface(){};

	void setFilename(std::string _filename){ filename = _filename; };
	void addVariable(Variable x){ 
		Variable temp(x);
		variables.emplace_back(temp); 
	}
//std::string _name, size_t _numPoints, int _numComponents, float _extents[6], float *_data
	virtual void init() = 0;	
	virtual void load() = 0;						// renders to *image  - returns 1 for success
	void loadVariable(std::string variableName);
};

