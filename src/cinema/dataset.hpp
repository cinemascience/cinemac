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
#include <iterator>


struct Variable
{
	std::string name;	 // e.g pressure, temp, position 
	int numComponents;	 // 3 for position(x,y,z), 1 for single variables e.g temp
    size_t numElements;  // num elements
	std::string datatype;// float, double, int, ...
	void * data;		 // (points are stored as array of x, array of y, array of z)

	Variable(){};
	~Variable()
	{ 
		name = ""; 
		numComponents = 0;
		numElements = 0;
		datatype = "";

		releaseMem();
	}


	Variable(const Variable &v2)
	{     
		name = v2.name;
		numComponents = v2.numComponents;
        numElements = v2.numElements;
		datatype = v2.datatype;

		// Deep copy just in case
		allocateMem();
		std::memcpy(data, v2.data, numComponents*numElements * getDataSize(v2.datatype));
	}


	Variable(std::string _name, int _numComponents, size_t _numElements, std::string _datatype, void *_data)
	{
		name = _name;
        numComponents = _numComponents;
        numElements = _numElements;
		datatype = _datatype;

		// Deep copy just in case  
		allocateMem();
		std::memcpy(data, _data, numComponents*numElements * getDataSize(_datatype));
	}


	float getScalar(int index)
	{
		float scalar;

		if (datatype == "float")
			scalar = (float) ( (( float *)data)[index] );
		else if (datatype == "int")
			scalar = (float) ( (( int *)data)[index] );
		else
			scalar = (float) ( ((double *)data)[index] );

		return scalar;
	}


	bool allocateMem() 
	{
		size_t numElems = numComponents*numElements;

		if (datatype == "int")
			data = new int[numElems];
		else if (datatype == "float")
			data = new float[numElems];
		else if (datatype == "double")
			data = new double[numElems];
		else if (datatype == "int8_t")
			data = new int8_t[numElems];
		else if (datatype == "int16_t")
			data = new int16_t[numElems];
		else if (datatype == "int32_t")
			data = new int32_t[numElems];
		else if (datatype == "int64_t")
			data = new int64_t[numElems];
		else if (datatype == "uint8_t")
			data = new uint8_t[numElems];
		else if (datatype == "uint16_t")
			data = new uint16_t[numElems];
		else if (datatype == "uint32_t")
			data = new uint32_t[numElems];
		else if (datatype == "uint64_t")
			data = new uint64_t[numElems];
		else
			return false;

		return true;
	}


	bool releaseMem() 
	{
		if (data == nullptr) // already deallocated!
			return true;

		if (datatype == "int")
			delete[](int *) data;
		else if (datatype == "float")
			delete[](float *) data;
		else if (datatype == "double")
			delete[](double *) data;
		else if (datatype == "int8_t")
			delete[](int8_t *) data;
		else if (datatype == "int16_t")
			delete[](int16_t *) data;
		else if (datatype == "int32_t")
			delete[](int32_t *) data;
		else if (datatype == "int64_t")
			delete[](int64_t *) data;
		else if (datatype == "uint8_t")
			delete[](uint8_t *) data;
		else if (datatype == "uint16_t")
			delete[](uint16_t *) data;
		else if (datatype == "uint32_t")
			delete[](uint32_t *) data;
		else if (datatype == "uint64_t")
			delete[](uint64_t *) data;
		else
			return false;

		data = nullptr;
		return true;
	}


	int getDataSize(std::string dataType)
	{
		if (dataType == "int")
			return sizeof(int);
		else if (dataType == "float")
			return sizeof(float);
		else if (dataType == "double")
			return sizeof(double);
		else
			return 0;
	}
};


class Dataset
{
    int numDims;		// 1 for points, 2 for 2d meshes, 3 for 3d meshes
	int dimSize[3];		// we support only up to 3D meshes now
	float extents[6];   // minX, minY, minZ,  maxX, maxY, maxZ;

	std::vector<Variable> variables;
   public:

    Dataset(){};
    Dataset(int _numDims, int _dimSize[3], float _extents[6]){
        numDims = _numDims;

        for (int i=0; i<3; i++)
			dimSize[i] = _dimSize[i];
        
        for (int i=0; i<6; i++)
			extents[i] = _extents[i];
    }

    ~Dataset(){
        dimSize[0] = dimSize[1] = dimSize[2] = 0;
		numDims = 0;
    };

    size_t getNumPoints(){ return dimSize[0]*dimSize[1]*dimSize[2]; }
    void addVariable(Variable v){ variables.push_back(v); }
	

	Variable getVariable(int index){ return variables[index]; }


    void setDims(int _dimSize[3])
	{
		for (int i=0; i<3; i++)
			dimSize[i] = _dimSize[i];
	}

    void setExtents(float _extents[6])
	{
		for (int i=0; i<6; i++)
			extents[i] = _extents[i];
	}

    void setNumDims(int _numDims)
	{
        numDims = _numDims;
	}
};
