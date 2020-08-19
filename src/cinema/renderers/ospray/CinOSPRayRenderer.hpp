#pragma once

#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <memory>
#include <iterator>

#include <ospray/ospray_cpp.h>
#include <ospray/ospray_util.h>
#include "ospray_testing.h"

#include "rkcommon/utility/CodeTimer.h"
#include "rkcommon/utility/SaveImage.h"
#include "rkcommon/tasking/parallel_for.h"
#include "rkcommon/math/vec.h"
#include "rkcommon/math/box.h"


#include <vtkCamera.h>

using namespace ospray;
using namespace rkcommon;
using namespace rkcommon::math;


#if 0
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
//#include "vtkStructures/unstructuredGrid.h"

#endif

#include "CinRenderInterface.h"
#include "image.hpp"

cpp::TransferFunction makeTransferFunction(const vec2f &valueRange, const std::string tfColorMap = "jet", const std::string tfOpacityMap = "linear" )
{
  cpp::TransferFunction transferFunction("piecewiseLinear");

  std::vector<vec3f> colors;
  std::vector<float> opacities;

  if (tfColorMap == "jet") {
    colors.emplace_back(0, 0, 0.562493);
    colors.emplace_back(0, 0, 1);
    colors.emplace_back(0, 1, 1);
    colors.emplace_back(0.500008, 1, 0.500008);
    colors.emplace_back(1, 1, 0);
    colors.emplace_back(1, 0, 0);
    colors.emplace_back(0.500008, 0, 0);
  } else if (tfColorMap == "rgb") {
    colors.emplace_back(0, 0, 1);
    colors.emplace_back(0, 1, 0);
    colors.emplace_back(1, 0, 0);
  } else {
    colors.emplace_back(0.f, 0.f, 0.f);
    colors.emplace_back(1.f, 1.f, 1.f);
  }

  if (tfOpacityMap == "linear") {
    opacities.emplace_back(0.f);
    opacities.emplace_back(1.f);
  }

  transferFunction.setParam("color", cpp::CopiedData(colors));
  transferFunction.setParam("opacity", cpp::CopiedData(opacities));
  transferFunction.setParam("valueRange", valueRange);
  transferFunction.commit();

  return transferFunction;
}

class CinOSPRayRenderer : public CinRenderInterface
{
	cpp::World world;

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
	CinOSPRayRenderer();

	void setData( vtkUnstructuredGrid * data );
	void setCameraPosition(float phi, float theta);

	void init();
	void render();
};


CinOSPRayRenderer::CinOSPRayRenderer()
{
	camera    = vtkCamera::New();

	/*
	input     = 0;
	reader    = vtkXMLPUnstructuredGridReader::New(); 
	mapper    = vtkDataSetMapper::New();
	actor     = vtkActor::New();
	camera    = vtkCamera::New();
	renderer  = vtkRenderer::New();
	renderWin = vtkRenderWindow::New();

	pngWriter = vtkPNGWriter::New();
	*/
}


void CinOSPRayRenderer::setData( vtkUnstructuredGrid * data )
{
	input = data;
}



void CinOSPRayRenderer::setCameraPosition(float phi, float theta)
{
	camera->Azimuth(phi);
	camera->Elevation(theta);
}


void CinOSPRayRenderer::init()
{
	cpp::Volume volume("particle");

	//AARONBAD -- there should be a better way of doing this than copying.
	std::vector<vec3f> points_vec3f;
	for(int i=0; i<points.size(); i+=3)
		points_vec3f.push_back(vec3f(points[i], points[i+1], points[i+2]));

	volume.setParam("particle.position", cpp::SharedData(points_vec3f)); //.size(), points_vec3f.data()) );

    //volume.setParam("particle.radius", cpp::SharedData(points_vec3f.size(), radius.data()));

	//volume.setParam("particle.radius", cpp::Data(radius));
	//volume.setParam("particle.weight", cpp::Data(weights));
	//volume.setParam("clampMaxCumulativeValue", clampMaxCumulativeValue);
	//volume.setParam("radiusSupportFactor", radiusSupportFactor);
	volume.commit();

	cpp::VolumetricModel model(volume);
	model.setParam("transferFunction", makeTransferFunction(vec2f(0.f, 1.f)));
	model.commit();

	cpp::Group group;
	group.setParam("volume", cpp::SharedData(model));
	group.commit();

	cpp::Instance instance(group);
	instance.commit();

	world.setParam("instance", cpp::SharedData(instance));

	cpp::Light light("ambient");
	light.setParam("visible", false);
	light.commit();

	world.setParam("light", cpp::SharedData(light));


#if 0

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

#endif

}

void CinOSPRayRenderer::render()
{
	cpp::Renderer renderer("pathtracer");
	renderer.setParam("spp", 16);
	renderer.commit();

	//AARONBAD -- why is this not working?!!
    //auto worldBounds = world.getBounds();
	vec2i imgSize(width, height);

    ospray::cpp::Camera osprayCamera("perspective");

    // create and setup framebuffer
    ospray::cpp::FrameBuffer framebuffer(
        imgSize.x, imgSize.y, OSP_FB_SRGBA, OSP_FB_COLOR | OSP_FB_ACCUM);

	int i = 0;
	for (auto _pt=phi_theta.begin(); _pt!=phi_theta.end(); _pt++)
	{
		// Change camera position and render
		setCameraPosition((*_pt).first, (*_pt).second);
		double org[3], dir[3], up[3];
		double fovy;
		camera->GetEyePosition(org);
		camera->GetDirectionOfProjection(dir);
		camera->GetViewUp(up);
		fovy = camera->GetViewAngle();

		osprayCamera.setParam("aspect", imgSize.x / (float)imgSize.y);
		osprayCamera.setParam("position", vec3f(org[0], org[1], org[2]));
		osprayCamera.setParam("direction", vec3f(dir[0], dir[1], dir[2]));
		osprayCamera.setParam("up", vec3f(up[0], up[1], up[2]));
		osprayCamera.setParam("fovy", float(fovy));
		osprayCamera.commit();

    	framebuffer.clear();
		auto future = framebuffer.renderFrame(renderer, osprayCamera, world);
		future.wait();

        float *fb = (float*)framebuffer.map(OSP_FB_COLOR);
        framebuffer.unmap(fb);

		// AARONBAD: can we just do a memcpy?
		for (size_t y=0; y<height; y++)
			for (size_t x=0; x<width; x++)
			{
				size_t index = (y * width *4) + x*4;

				imgs[i].setPixel( x,y, 0, fb[(x + y * width) * 4 + 0] );
				imgs[i].setPixel( x,y, 1, fb[(x + y * width) * 4 + 1] );
				imgs[i].setPixel( x,y, 2, fb[(x + y * width) * 4 + 2] );
				imgs[i].setPixel( x,y, 3, (float)1.0);
			}
		i++;
	}
}



