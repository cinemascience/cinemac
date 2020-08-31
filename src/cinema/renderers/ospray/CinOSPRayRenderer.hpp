#pragma once

#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <limits>

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

#include "ArcballCamera.h"

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


cpp::TransferFunction makeTransferFunction(const vec2f &valueRange, const std::string tfColorMap = "jet2", const std::string tfOpacityMap = "jet" )
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
  }  if (tfColorMap == "jet2") {
    colors.emplace_back(0, 0, 0.562493);
    colors.emplace_back(0, 0, 1);
    colors.emplace_back(0, .1, 1);
    colors.emplace_back(0.100008, .5, 1);
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
  else if (tfOpacityMap == "jet") {
    opacities.emplace_back(0.f);
    opacities.emplace_back(0.f);
    opacities.emplace_back(.005f);
    opacities.emplace_back(.15f);
    opacities.emplace_back(5.f);
	opacities.emplace_back(1.f);
	opacities.emplace_back(0.f);
  }

  transferFunction.setParam("color", cpp::CopiedData(colors));
  transferFunction.setParam("opacity", cpp::CopiedData(opacities));
  transferFunction.setParam("valueRange", valueRange);
  transferFunction.commit();

  return transferFunction;
}

class CinOSPRayRenderer : public CinRenderInterface
{
	box3f bounds;

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
	OSPError init_error = ospInit();
	if (init_error != OSP_NO_ERROR)
	{
		std::cerr << "Could not initialize OSPRay" << std::endl;
		exit(0);
	}

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
	cpp::Volume volume("particle");

	//AARONBAD -- there should be a better way of doing this than copying.
	std::vector<vec3f> points_vec3f;
	std::vector<float> radius_float;
	std::vector<float> weight_float;

	bounds.lower = vec3f(std::numeric_limits<float>::max());
	bounds.upper = vec3f(std::numeric_limits<float>::min());

	uint32_t p=0;
	for(int i=0; i<points.size(); i+=3, p++)
	{
		vec3f point(points[i], points[i+1], points[i+2]);
		points_vec3f.push_back(point);
		radius_float.push_back(.7f);
		weight_float.push_back(.5f);
		bounds.lower = min(bounds.lower, point);
		bounds.upper = max(bounds.upper, point);
		//std::cerr << "point " << points_vec3f[p] << std::endl;
	}

	volume.setParam("particle.position", cpp::SharedData(points_vec3f));
	volume.setParam("particle.radius", cpp::SharedData(radius_float));
	volume.setParam("particle.weight", cpp::SharedData(weight_float));
    volume.setParam("estimateValueRanges", false);

	volume.setParam("clampMaxCumulativeValue", 1.f);
	volume.setParam("radiusSupportFactor", 5.f);
	volume.commit();

	cpp::VolumetricModel model(volume);
	model.setParam("transferFunction", makeTransferFunction(vec2f(0.f, 1.f)));
	model.commit();

	cpp::Group group;
	group.setParam("volume", cpp::CopiedData(model));
	group.commit();

	cpp::Instance instance(group);
	instance.commit();

	cpp::World world;
	world.setParam("instance", cpp::CopiedData(instance));

	cpp::Light light("ambient");
	light.setParam("visible", false);
	light.commit();

	world.setParam("light", cpp::CopiedData(light));
	world.commit();

	cpp::Renderer renderer("scivis");
	renderer.setParam("pixelSamples", 1);
	//renderer.setParam("backgroundColor", 1.0f); // white, transparent

	renderer.setParam("volumeSamplingRate", 2.f);
	renderer.setParam("densityScale", 100.f); 

	renderer.commit();

	vec2i imgSize(width, height);
	ArcballCamera arcball((const box3f &)bounds, imgSize);

	int i = 0;
	for (auto _pt=phi_theta.begin(); _pt!=phi_theta.end(); _pt++)
	{
		vec3f cam_pos{0.f, 0.f, 0.f};
 		vec3f cam_up{0.f, 1.f, 0.f};
  		vec3f cam_view{0.1f, 0.f, 0.f};
		float fovy = 45.f;
		cam_pos = bounds.center();
		cam_pos.x -= bounds.size().x * .1f;

		//AARONBAD -- need to figure out how to rotate a quaternion camera using phi and theta
		arcball.rotate( vec2f(0.f, 0.f), vec2f(0.1f, 0.f) );
		cam_pos = arcball.eyePos();
		cam_up = arcball.upDir();
		cam_view = arcball.lookDir();

		ospray::cpp::Camera osprayCamera("perspective");
		osprayCamera.setParam("aspect", imgSize.x / (float)imgSize.y);
		osprayCamera.setParam("position", cam_pos);
		osprayCamera.setParam("direction", cam_view);
		osprayCamera.setParam("up", cam_up);
		osprayCamera.setParam("fovy", float(fovy));
		osprayCamera.commit();

	    ospray::cpp::FrameBuffer framebuffer(
    	    imgSize.x, imgSize.y, OSP_FB_SRGBA, OSP_FB_COLOR | OSP_FB_ACCUM);

    	framebuffer.clear();
		auto future = framebuffer.renderFrame(renderer, osprayCamera, world);
		future.wait();

		uint8_t *fb = (uint8_t *)framebuffer.map(OSP_FB_COLOR);
		/*
		char ppmfile[64];
		std::sprintf(ppmfile, "ospray_frame_%d.ppm", i);

    	rkcommon::utility::writePPM(ppmfile, imgSize.x, imgSize.y, fb);

		std::cerr << "wrote ppm" << std::endl;
		*/

		// AARONBAD: can we just do a memcpy?
		for (size_t y=0; y<height; y++)
			for (size_t x=0; x<width; x++)
			{
				const uint32_t index = (x + y * width) * 4;

				imgs[i].setPixel( x,y, 0, float(fb[index + 0]) / 255.f );
				imgs[i].setPixel( x,y, 1, float(fb[index + 1]) / 255.f );
				imgs[i].setPixel( x,y, 2, float(fb[index + 2]) / 255.f );
				imgs[i].setPixel( x,y, 3, (float)1.0);
			}
		i++;

		framebuffer.unmap(fb);
	}
}


