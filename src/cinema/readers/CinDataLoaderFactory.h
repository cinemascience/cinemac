/*================================================================================
This software is open source software available under the BSD-3 license.
Copyright (c) 2017, Los Alamos National Security, LLC.
All rights reserved.

Authors:
 - Pascal Grosset
================================================================================*/

#pragma once

#include <iostream>
#include "CinLoaderInterface.h"

#ifdef HAS_VTK_RENDERER
  #include "vtk_loader/CinVTKUnstructredGridLoader.hpp"
#endif


class CinDataLoaderFactory
{
	//
	// Factory class to create renderers based on user's choice

  public:
	static CinLoaderInterface * createLoader(std::string loaderName)
	{
		std::cout << "loaderName: " << loaderName << std::endl;
	  #ifdef HAS_VTK_RENDERER
		if (loaderName == "VTK-UNSTRUCTURED")
			return new CinVtkUnstructuredGridLoader();

		//if (loaderName == "VTK-STRUCTURED")
		//	return new CinVTKStructuredLoader();
	  #endif

		//if (loaderName == "binary")
		//	return new CinBinaryLoader();
	
		// Output for error
		std::cout << "Loader "<< loaderName << " is not supported. Supported loaders are: ";


	  #ifdef HAS_VTK_RENDERER
		std::cout << "VTK-UNSTRUCTURED, VTK-STRUCTURED, ";
	  #endif
	  
		std::cout << "Binary.\n";

		return NULL;
	}
};
