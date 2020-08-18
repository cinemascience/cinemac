/*================================================================================
This software is open source software available under the BSD-3 license.
Copyright (c) 2017, Los Alamos National Security, LLC.
All rights reserved.

Authors:
 - Pascal Grosset
================================================================================*/

#pragma once

#include <iostream>
#include "CinRenderInterface.h"

#ifdef HAS_VTK_RENDERER
  #include "vtk/CinVTKRenderer.hpp"
#endif

#ifdef HAS_OSPRAY_RENDERER
  #include "ospray/CinOSPRayRenderer.hpp"
#endif


class CinRendererFactory
{
	//
	// Factory class to create renderers based on user's choice

  public:
	static CinRenderInterface * createRenderer(std::string rendererName)
	{
		std::cout << "rendererName: " << rendererName << std::endl;
	  #ifdef HAS_VTK_RENDERER
		if (rendererName == "VTK")
			return new CinVTKRenderer();
	  #endif

	  #ifdef HAS_OPENGL_RENDERER
		if (rendererName == "OPENGL")
			return new CinOGLRenderer();
	  #endif

	  #ifdef HAS_OSPRAY_RENDERER
		if (rendererName == "OSPRAY")
			return new CinOSPRayRenderer();
	  #endif


		// Output for error
		std::cout << "Renderer "<< rendererName << " is not supported. Supported renderers are: ";

	  #ifdef HAS_VTK_RENDERER
		std::cout << "VTK ";
	  #endif

      #ifdef HAS_OPENGL_RENDERER
		std::cout << "OPENGL ";
	  #endif

	  #ifdef HAS_OSPRAY_RENDERER
		std::cout << "OSPRAY ";
	  #endif
	  
		std::cout << ".\n";

		return NULL;
	}
};
