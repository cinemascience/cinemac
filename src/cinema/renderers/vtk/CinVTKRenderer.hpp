#pragma once

#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <vtkFloatArray.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkXMLPUnstructuredGridReader.h>
#include <vtkRendererCollection.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnsignedCharArray.h>
#include <vtkXMLPUnstructuredGridReader.h>
#include <vtkRenderWindow.h>
#include <vtkScalarBarActor.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkLookupTable.h>

#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>


#include "timer.hpp"
#include "utils.hpp"
#include "log.hpp"
#include "CinRenderInterface.h"
#include "vtkStructures/unstructuredGrid.h"

#include "image.hpp"
#include "density.hpp"


class CinVTKRenderer : public CinRenderInterface
{
	vtkUnstructuredGrid *   input;
    bool                    loaded;

    UnstructuredGrid pointData;
	DensityCompute densityBins;

    // pipeline
    vtkActor *                      actor;
    vtkCamera *                     camera;
    vtkDataSetMapper *              mapper;
    vtkXMLPUnstructuredGridReader * reader;
    vtkRenderer *                   renderer;
    vtkRenderWindow *               renderWin;

	vtkPNGWriter *                  pngWriter;

  public:
	CinVTKRenderer();

	void setData( vtkUnstructuredGrid * data );
	void setCameraPosition(float phi, float theta);

	void init();
	void render();
};


inline CinVTKRenderer::CinVTKRenderer()
{
	input     = 0;
	reader    = vtkXMLPUnstructuredGridReader::New(); 
	mapper    = vtkDataSetMapper::New();
	actor     = vtkActor::New();
	camera    = vtkCamera::New();
	renderer  = vtkRenderer::New();
	renderWin = vtkRenderWindow::New();

	pngWriter = vtkPNGWriter::New();
}


inline void CinVTKRenderer::setData( vtkUnstructuredGrid * data )
{
	input = data;
}



inline void CinVTKRenderer::setCameraPosition(float phi, float theta)
{
	camera->Azimuth(phi);
	camera->Elevation(theta);
}


inline void CinVTKRenderer::init()
{
	// Set the points to use
	pointData.setPoints(&points[0], points.size()/3, VTK_VERTEX);

	size_t numPoints = points.size()/3;
	//densityBins.init(20,20,20,  center[0],center[1],center[2], radius);
	densityBins.init(20,20,20,  extents);
	densityBins.computeDensity( &points[0], numPoints);


	// Create the color map
	vtkSmartPointer<vtkLookupTable> colorLookupTable = vtkSmartPointer<vtkLookupTable>::New();
	colorLookupTable->SetTableRange(densityBins.getMinDensity(), densityBins.getMaxDensity());
	colorLookupTable->Build();

	// Generate the colors for each point based on the color map
  	vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
  	colors->SetNumberOfComponents(3);
  	colors->SetName("Colors");

  	for(size_t i=0; i<numPoints; i++)
    {
		float _x, _y, _z;
		_x = points[i*3 + 0];  
        _y = points[i*3 + 1];  
        _z = points[i*3 + 2];

		double val = densityBins.getDensity(_x, _y, _z);
		//std::cout << "val: " << val << std::endl;
		
		double dcolor[3];
		colorLookupTable->GetColor(val, dcolor);
		//std::cout << "val:" << val << " dcolor: " << dcolor[0] << " " << dcolor[1] << " " << dcolor[2] << std::endl;

		// unsigned char color[3];
		// for(unsigned int j = 0; j < 3; j++)
		// 	color[j] = 255 * dcolor[j]/1.0;
		// //std::cout << "color: " << (int)color[0] << " " << (int)color[1] << " " << (int)color[2] << std::endl;
		
		//colors->InsertNextTupleValue(color);
		colors->InsertTuple(i,dcolor);
    }
  	pointData.uGrid->GetPointData()->SetScalars(colors);


	// Set up renderer
	mapper->SetInputData( pointData.uGrid );
	mapper->ScalarVisibilityOn();
  	mapper->SetScalarModeToUsePointData();
  	mapper->SetColorModeToMapScalars();
	mapper->SetLookupTable(colorLookupTable);

	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();
	actor->GetProperty()->SetPointSize(0.25);
	actor->GetProperty()->SetOpacity(0.5);

	vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
  	scalarBar->SetLookupTable(mapper->GetLookupTable());
  	scalarBar->SetNumberOfLabels(4);
	scalarBar->SetLookupTable( colorLookupTable );

	renderer->SetActiveCamera(camera);
	renderWin->SetOffScreenRendering(1);       // for offscreen
	renderWin->AddRenderer(renderer);

	//Add the actor to the scene
	renderer->AddActor(actor);
	renderer->AddActor2D(scalarBar);
	renderer->ResetCamera();

	renderWin->SetSize(width, height);
}


inline void CinVTKRenderer::render()
{
	Timer clock;
	int i = 0;
	for (auto _pt=phi_theta.begin(); _pt!=phi_theta.end(); _pt++)
	{
	  clock.start("render");
		// Change camera position and render
		setCameraPosition((*_pt).first, (*_pt).second);
		renderWin->Render();
	  clock.stop("render");
		
	  clock.start("read-buffer");
	  
		// Grab the rendered buffer and copy to an array
		vtkSmartPointer<vtkFloatArray> buffer = vtkSmartPointer<vtkFloatArray>::New();
		renderWin->GetRGBAPixelData(0, 0, width-1, height-1, 1, buffer);

		#pragma omp parallel for
		for (unsigned y=0; y<height; y++)
			for (unsigned x=0; x<width; x++) 
			{
				size_t index = ((height-1-y) * width *4) + x*4;

				imgs[i].pixels[index + 0] = buffer->GetValue(index + 0) * 255;
				imgs[i].pixels[index + 1] = buffer->GetValue(index + 1) * 255;
				imgs[i].pixels[index + 2] = buffer->GetValue(index + 2) * 255;
				imgs[i].pixels[index + 3] = 255;
			}
		i++;
	  clock.stop("read-buffer");

	  debugLog << " - VTK render took " << (*_pt).first << ", " << (*_pt).second << " took " << clock.getDuration("render") << " s and buffer capture took: " << clock.getDuration("read-buffer") << " s" << std::endl;
	}
}
