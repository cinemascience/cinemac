#pragma once

#include <string>
#include <iostream>
#include <sstream>

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


inline void readData(std::string filename, size_t numPoints, std::vector<float> &vecX, std::vector<float> &vecY, std::vector<float> &vecZ, float extents[6])
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

	//size_t numPoints = 223441;

	for (size_t i=0; i<numPoints; i++)
	{
		vecX.push_back(farrayX->GetValue(i));
		vecY.push_back(farrayY->GetValue(i));
		vecZ.push_back(farrayZ->GetValue(i));
	}

	extents[0] = *std::min_element(vecX.begin(), vecX.end());
	extents[1] = *std::min_element(vecY.begin(), vecY.end());
	extents[2] = *std::min_element(vecZ.begin(), vecZ.end());

	extents[3] = *std::max_element(vecX.begin(), vecX.end());
	extents[4] = *std::max_element(vecY.begin(), vecY.end());
	extents[5] = *std::max_element(vecZ.begin(), vecZ.end());

	std::cout << "Read in extents: " << extents[0] << ", " << extents[1] << ", " << extents[2] 
                            << " - " << extents[3] << ", " << extents[4] << ", " << extents[5] << std::endl;

	//vtkDataArray* array = ugrid->GetPointData()->GetArray("myscalararrayname");  // to retrieve  he data array from its name
   	//vtkDataArray* array = ugrid->GetPointData()->GetScalars();   // to retrieve the data array tagged as being the active scalar
   	 // data array is an abstract class, you might want to use the real type class  for fastest access: eg: float
}