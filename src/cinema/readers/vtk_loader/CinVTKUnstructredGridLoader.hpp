#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits>

#ifdef HAS_VTK_RENDERER

#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSOADataArrayTemplate.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkFloatArray.h>


#include <vtkXMLPUnstructuredGridReader.h>

#include "CinLoaderInterface.h"

class CinVtkUnstructuredGridLoader: public CinLoaderInterface
{
	void init(){};
	void load();
	void loadVariable(std::string variableName);
};


inline void CinVtkUnstructuredGridLoader::load()
{
	vtkSmartPointer<vtkXMLPUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLPUnstructuredGridReader>::New();
  	reader->SetFileName(filename.c_str());
  	reader->Update();

	vtkDataArray* arrayX = reader->GetOutput()->GetPointData()->GetArray("x");
	vtkFloatArray* farrayX = vtkFloatArray::SafeDownCast(arrayX); 

	vtkDataArray* arrayY = reader->GetOutput()->GetPointData()->GetArray("y");
	vtkFloatArray* farrayY = vtkFloatArray::SafeDownCast(arrayY); 

	vtkDataArray* arrayZ = reader->GetOutput()->GetPointData()->GetArray("z");
	vtkFloatArray* farrayZ = vtkFloatArray::SafeDownCast(arrayZ); 

	size_t numPoints = reader->GetOutput()->GetNumberOfPoints();


	std::vector<float> myDataValues;
	myDataValues.resize(numPoints*3);

	float extents[6] = {std::numeric_limits<float>::max(),
						std::numeric_limits<float>::max(),
						std::numeric_limits<float>::max(),
						std::numeric_limits<float>::min(),
						std::numeric_limits<float>::min(),
						std::numeric_limits<float>::min()};

	for (size_t i=0; i<numPoints; i++)
	{
		myDataValues[0*numPoints+i]=farrayX->GetValue(i);
		myDataValues[1*numPoints+i]=farrayY->GetValue(i);
		myDataValues[2*numPoints+i]=farrayZ->GetValue(i);

		extents[0] = std::min(extents[0],farrayX->GetValue(i));
		extents[1] = std::min(extents[1],farrayY->GetValue(i));
		extents[2] = std::min(extents[2],farrayZ->GetValue(i));

		extents[3] = std::max(extents[3],farrayX->GetValue(i));
		extents[4] = std::max(extents[4],farrayY->GetValue(i));
		extents[5] = std::max(extents[5],farrayZ->GetValue(i));
	}

	std::cout << "Read in ~ Num points : " << numPoints << std::endl;
	std::cout << "Read in ~ extents: " << extents[0] << ", " << extents[1] << ", " << extents[2] 
                            << " - " << extents[3] << ", " << extents[4] << ", " << extents[5] << std::endl;

	addVariable( Variable("position", numPoints, 3, extents, &myDataValues[0]) );
}


inline void CinVtkUnstructuredGridLoader::loadVariable(std::string variableName)
{
	vtkSmartPointer<vtkXMLPUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLPUnstructuredGridReader>::New();
  	reader->SetFileName(filename.c_str());
  	reader->Update();

	vtkDataArray* arrayX = reader->GetOutput()->GetPointData()->GetArray(variableName.c_str());
	vtkFloatArray* farrayX = vtkFloatArray::SafeDownCast(arrayX); 

	size_t numPoints = reader->GetOutput()->GetNumberOfPoints();


	std::vector<float> myDataValues;
	myDataValues.resize(numPoints);

	float extents[6] = {std::numeric_limits<float>::max(),
						std::numeric_limits<float>::min(),
						0,
						0,
						0,
						0};

	for (size_t i=0; i<numPoints; i++)
	{
		myDataValues[0*numPoints+i]=farrayX->GetValue(i);

		extents[0] = std::min(extents[0],farrayX->GetValue(i));
		extents[1] = std::min(extents[1],farrayX->GetValue(i));
	}

	std::cout << "Read in ~ Num points : " << numPoints << std::endl;
	std::cout << "Read in ~ extents: " << extents[0] << " - " << extents[1] << std::endl;

	addVariable( Variable(variableName, numPoints, 1, extents , &myDataValues[0]) );
}

#endif //HAS_VTK_RENDERER