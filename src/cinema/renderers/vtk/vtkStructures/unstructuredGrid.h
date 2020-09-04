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
#include <vtkXMLPUnstructuredGridWriter.h>

#include <vtkPointData.h>
//#include <mpi.h>
//#include <vtkXMLPUnstructuredGridWriter.h>
//#include <vtkMPIController.h>



class UnstructuredGrid
{
	//vtkSmartPointer<vtkXMLPUnstructuredGridWriter> writer;

	vtkSmartPointer<vtkPoints> pnts;
  	vtkSmartPointer<vtkCellArray> cells;
  	vtkIdType idx;

	int myRank, numRanks;

  public:
	vtkSmartPointer<vtkUnstructuredGrid> uGrid;

	UnstructuredGrid();
	UnstructuredGrid(int myRank, int numRanks);
	~UnstructuredGrid(){};

	vtkSmartPointer<vtkUnstructuredGrid> getUGrid(){ return uGrid; }
	vtkSmartPointer<vtkDataSet> getGrid(){ return uGrid; }

	// Topology
	template <typename T> void addPoint(T *pointData);
	template <typename T> void setPoints(T *pointData, int numPoints, int cellType);
	void setPoints(vtkSmartPointer<vtkPoints> _pnts, vtkSmartPointer<vtkCellArray> _cells, int cellType);

	void pushPointsToGrid(int cellType);

	// Data
	template <typename T> void addScalarData(std::string scalarName, int numPoints, T *data);
	template <typename T> void addVectorData(std::string scalarName, int numPoints, int numComponents, T *data);

	template <typename T> void addFieldData(std::string fieldName, T *data);

	// // Writing
	//void writeParts(int numPieces, int startPiece, int SetEndPiece, std::string fileName);
	//void write(std::string fileName, int parallel=0);

	// Cleanup
	void reset();
};



inline UnstructuredGrid::UnstructuredGrid()
{
	//writer = vtkSmartPointer<vtkXMLPUnstructuredGridWriter>::New();
	uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();

	pnts = vtkSmartPointer<vtkPoints>::New();
  	cells = vtkSmartPointer<vtkCellArray>::New();

  	idx = 0;
}



inline UnstructuredGrid::UnstructuredGrid(int _myRank, int _numRanks)
{
	//writer = vtkSmartPointer<vtkXMLPUnstructuredGridWriter>::New();
	uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();

	pnts = vtkSmartPointer<vtkPoints>::New();
  	cells = vtkSmartPointer<vtkCellArray>::New();

  	idx = 0;

  	myRank = _myRank;
  	numRanks = _numRanks;
}



//
// Topology
template <typename T> 
inline void UnstructuredGrid::addPoint(T *pointData)
{
	pnts->InsertPoint(idx, pointData);
    cells->InsertNextCell(1, &idx);
    idx++;
}


template <typename T> 
inline void UnstructuredGrid::setPoints(T *pointData, int numPoints, int cellType)
{
	pnts->SetNumberOfPoints(numPoints);

	for (int i=0; i<numPoints ; ++i)
	{
		T pnt[3];
		pnt[0] = pointData[numPoints*0 + i];
		pnt[1] = pointData[numPoints*1 + i];
		pnt[2] = pointData[numPoints*2 + i];

		//std::cout << pnt[0] << ", " << pnt[1] << ", " << pnt[2] << std::endl;

		pnts->SetPoint(i, pnt[0],pnt[1],pnt[2]);
		cells->InsertNextCell(1, &idx);
		idx++;
	}

	pushPointsToGrid(cellType);
}


inline void UnstructuredGrid::setPoints(vtkSmartPointer<vtkPoints> _pnts, vtkSmartPointer<vtkCellArray> _cells, int cellType)
{
	uGrid->SetPoints(_pnts);
	uGrid->SetCells(cellType, _cells);
}


inline void UnstructuredGrid::pushPointsToGrid(int cellType)
{
	uGrid->SetPoints(pnts);
	uGrid->SetCells(cellType, cells);
}


inline void UnstructuredGrid::reset()
{
	pnts->Delete();
	cells->Delete();

	idx = 0;
}


// Attributes
template <typename T>
inline void UnstructuredGrid::addFieldData(std::string fieldName, T *data)
{
  	vtkSOADataArrayTemplate<T>* temp = vtkSOADataArrayTemplate<T>::New();

  	temp->SetNumberOfTuples(1);
  	temp->SetNumberOfComponents(1);
  	temp->SetName(fieldName.c_str());
  	temp->SetArray(0, data, 1, false, true);
  	uGrid->GetFieldData()->AddArray(temp);

  	temp->Delete();
}


//
// Data
template <typename T>
inline void UnstructuredGrid::addScalarData(std::string varName, int numPoints, T *data)
{
	vtkSOADataArrayTemplate<T>* temp = vtkSOADataArrayTemplate<T>::New();

  	temp->SetNumberOfTuples(numPoints);
  	temp->SetNumberOfComponents(1);
  	temp->SetName(varName.c_str());
  	temp->SetArray(0, data, numPoints, false, true);
  	uGrid->GetPointData()->AddArray(temp);

  	temp->Delete();
}


template <typename T>
inline void UnstructuredGrid::addVectorData(std::string varName, int numPoints, int numComponents, T *data)
{
	vtkAOSDataArrayTemplate<T>* temp = vtkAOSDataArrayTemplate<T>::New();

	temp->SetNumberOfTuples(numPoints);
  	temp->SetNumberOfComponents(numComponents);
  	temp->SetName(varName.c_str());
  	temp->SetArray(data, numPoints*numComponents, false, true);
  	uGrid->GetPointData()->AddArray(temp);

  	temp->Delete();
}


/*
//
// Writing
inline void UnstructuredGrid::writeParts(int numPieces, int startPiece, int endPiece, std::string fileName)
{
	writer->SetNumberOfPieces(numPieces);
	writer->SetStartPiece(startPiece);
	writer->SetEndPiece(endPiece);

	write(fileName, 1);
}


inline void UnstructuredGrid::write(std::string fileName, int parallel)
{
	std::string outputFilename;

	writer->SetDataModeToBinary();
    writer->SetCompressor(nullptr);

	if (parallel == 1)
	{
		auto contr = vtkSmartPointer<vtkMPIController>::New();
		contr->Initialize(NULL, NULL, 1);
		contr->SetGlobalController(contr);

		outputFilename = fileName + ".pvtu";

		writer->SetController(contr);


		writer->SetFileName(outputFilename.c_str());

		#if VTK_MAJOR_VERSION <= 5
	        writer->SetInput(uGrid);
	    #else
	        writer->SetInputData(uGrid);
	    #endif

		writer->Write();

		//contr->Finalize();
	}
	else
	{
		outputFilename = fileName + ".vtu";
	

    
		writer->SetFileName(outputFilename.c_str());

		#if VTK_MAJOR_VERSION <= 5
	        writer->SetInput(uGrid);
	    #else
	        writer->SetInputData(uGrid);
	    #endif

		writer->Write();

	}
}
*/

// example: http://visit.ilight.com/svn/visit/tags/2.4.0/vendor_branches/vtk/src/Parallel/vtkMPIController.cxx

#endif