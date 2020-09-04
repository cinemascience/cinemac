#pragma once

#include <string>
#include <iostream>
#include <sstream>

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

	for (size_t i=0; i<numPoints; i++)
	{
		myDataValues[0*numPoints+i]=farrayX->GetValue(i);
		myDataValues[1*numPoints+i]=farrayY->GetValue(i);
		myDataValues[2*numPoints+i]=farrayZ->GetValue(i);
	}

	myData.addVariable( Variable("position", 1, 1,1,numPoints,  &myDataValues[0]) );

	std::cout << "Num points loaded: " << numPoints << std::endl;


	//vtkDataArray* array = ugrid->GetPointData()->GetArray("myscalararrayname");  // to retrieve  he data array from its name
   	//vtkDataArray* array = ugrid->GetPointData()->GetScalars();   // to retrieve the data array tagged as being the active scalar
   	 // data array is an abstract class, you might want to use the real type class  for fastest access: eg: float
}

#endif //HAS_VTK_RENDERER