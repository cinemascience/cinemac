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
#include <vtkXMLPUnstructuredGridReader.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>


#include "CinRenderInterface.h"
#include "vtkStructures/unstructuredGrid.h"

#include "image.hpp"


class CinVTKRenderer : public CinRenderInterface
{
	vtkUnstructuredGrid *   input;
    bool                    loaded;

    UnstructuredGrid pointData;

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


CinVTKRenderer::CinVTKRenderer()
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


void CinVTKRenderer::setData( vtkUnstructuredGrid * data )
{
	input = data;
}



void CinVTKRenderer::setCameraPosition(float phi, float theta)
{
	camera->Azimuth(phi);
	camera->Elevation(theta);
}


void CinVTKRenderer::init()
{
	// Set the points to use
	pointData.setPoints(&points[0], points.size()/3, VTK_VERTEX);

	// Set up renderer
	mapper->SetInputData( pointData.uGrid );
	mapper->ScalarVisibilityOff();

	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();
	actor->GetProperty()->SetPointSize(2.0);
	actor->GetProperty()->SetColor(0,0,1);

	renderer->SetActiveCamera(camera);
	renderWin->SetOffScreenRendering(1);       // for offscreen
	renderWin->AddRenderer(renderer);

	//Add the actor to the scene
	renderer->AddActor(actor);
	renderer->ResetCamera();

	renderWin->SetSize(width, height);
}



void CinVTKRenderer::render()
{
	int i = 0;
	for (auto _pt=phi_theta.begin(); _pt!=phi_theta.end(); _pt++)
	{
		// Change camera position and render
		setCameraPosition((*_pt).first, (*_pt).second);
		renderWin->Render();
		
		// Grab the rendered buffer and copy to an array
		vtkSmartPointer<vtkFloatArray> buffer = vtkSmartPointer<vtkFloatArray>::New();
		renderWin->GetRGBAPixelData(0, 0, width-1, height-1, 1, buffer);

		// Copy from the vtkFloatArray to image
		// TODO: Probably Slow and needs to change!!!
		for (size_t y=0; y<height; y++)
			for (size_t x=0; x<width; x++)
			{
				size_t index = (y * width *4) + x*4;

				imgs[i].setPixel( x,y, 0, buffer->GetValue(index + 0) );
				imgs[i].setPixel( x,y, 1, buffer->GetValue(index + 1) );
				imgs[i].setPixel( x,y, 2, buffer->GetValue(index + 2) );
				imgs[i].setPixel( x,y, 3, (float)1.0);
			}
		i++;
	}
}



