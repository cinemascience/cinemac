#pragma once

#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#ifdef HAS_VTK_RENDERER

#include <vtkFloatArray.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
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
	//vtkUnstructuredGrid *   input;
    bool                    loaded;

    // pipeline
    vtkCamera *                     camera;
    vtkXMLPUnstructuredGridReader * reader;
    vtkRenderer *                   renderer;
    vtkRenderWindow *               renderWin;

	vtkPNGWriter *                  pngWriter;

  public:
	CinVTKRenderer();

	//void setData( vtkUnstructuredGrid * data );
	void setCameraPosition(float phi, float theta);

	void renderHalo(float *pos, float *vel, float center[3], float centerColor[3], float rad, size_t numPoints);
	void renderPoints(float *pos, size_t numPoints);

	void init();
	void render();
};


inline CinVTKRenderer::CinVTKRenderer()
{
	//input     = 0;
	reader    = vtkXMLPUnstructuredGridReader::New(); 
	camera    = vtkCamera::New();
	renderer  = vtkRenderer::New();
	renderWin = vtkRenderWindow::New();

	pngWriter = vtkPNGWriter::New();
}


inline void CinVTKRenderer::setCameraPosition(float phi, float theta)
{
	camera->Azimuth(phi);
	camera->Elevation(theta);
}


inline void CinVTKRenderer::renderHalo(float *pos, float *vel, float center[3], float centerColor[3], float rad, size_t numPoints)
{
	// Create a sphere at the center
	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
  	sphereSource->SetPhiResolution(100);
  	sphereSource->SetThetaResolution(100);
	sphereSource->SetRadius(rad);

	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  	sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
	sphereActor->SetMapper(sphereMapper);
	sphereActor->GetProperty()->SetColor(centerColor[0], centerColor[1], centerColor[2]);
	sphereActor->GetProperty()->SetAmbient(0.3);
	sphereActor->GetProperty()->SetDiffuse(0.875);
	sphereActor->GetProperty()->SetSpecular(0.0);
	sphereActor->GetProperty()->SetSpecularPower(25.0);
	sphereActor->AddPosition(center[0], center[1], center[2]);

	std::cout << "center: " << center[0] << ", " << center[1] << ", " << center[2] << std::endl;
	std::cout << "centerColor: " << centerColor[0] << ", " << centerColor[1] << ", " << centerColor[2] << std::endl;


	// points
	UnstructuredGrid pointData;
	pointData.setPoints(pos, numPoints, VTK_VERTEX);

	vtkSmartPointer<vtkDataSetMapper> pointsMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	pointsMapper->SetInputData( pointData.uGrid );

	vtkSmartPointer<vtkActor> pointsActor = vtkSmartPointer<vtkActor>::New();
	pointsActor->SetMapper(pointsMapper);
	pointsActor->GetProperty()->EdgeVisibilityOn();
	pointsActor->GetProperty()->SetPointSize(0.25);
	pointsActor->GetProperty()->SetOpacity(0.5);


	renderer->AddActor(sphereActor);
	renderer->AddActor(pointsActor);
}


inline void CinVTKRenderer::renderPoints(float *pos, size_t numPoints)
{
	// points
	UnstructuredGrid pointData;
	pointData.setPoints(pos, numPoints, VTK_VERTEX);

	vtkSmartPointer<vtkDataSetMapper> pointsMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	pointsMapper->SetInputData( pointData.uGrid );

	vtkSmartPointer<vtkActor> pointsActor = vtkSmartPointer<vtkActor>::New();
	pointsActor->SetMapper(pointsMapper);
	pointsActor->GetProperty()->EdgeVisibilityOn();
	pointsActor->GetProperty()->SetPointSize(0.25);
	pointsActor->GetProperty()->SetOpacity(0.5);

	renderer->AddActor(pointsActor);
}


inline void CinVTKRenderer::init()
{
	// Render elements

	// for (int i=0; i<structuresList.size(); i++)
	// 	renderHalo(&structuresList[i].pos[0], &structuresList[i].vel[0], structuresList[i].center, 
	// 				structuresList[i].centerColor, structuresList[i].radius, structuresList[i].numPoints);

	for (int i=0; i<cinDataLoader->myData.variables.size(); i++)
	{
		size_t numPoints = cinDataLoader->myData.variables[i].dimSize[0] *
						   cinDataLoader->myData.variables[i].dimSize[1] * 
						   cinDataLoader->myData.variables[i].dimSize[2];

		float center[3] = {4.6, 5.9, 0.45};
		float centerColor[3] = {0.5,0.1,0.7};
		renderHalo(cinDataLoader->myData.variables[i].data, cinDataLoader->myData.variables[i].data, center,
				   centerColor,  0.05,  numPoints);
	}


	// Set up renderer
	renderer->SetActiveCamera(camera);
	renderWin->SetOffScreenRendering(1);       // for offscreen
	renderWin->AddRenderer(renderer);

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


	  #ifdef USING_LINUX
		#pragma omp parallel for
	  #endif
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

#endif