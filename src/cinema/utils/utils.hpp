/*================================================================================
This software is open source software available under the BSD-3 license.
Copyright (c) 2017, Los Alamos National Security, LLC.
All rights reserved.
Authors:
 - Pascal Grosset
 - Jesus Pulido
 Utils.hpp is a collection of helper functions used throughout Cbench and its
 classes. Any non-specific functions related to the various interfaces go here.
================================================================================*/
#pragma once

#include <stdio.h> 
#include <stdbool.h> 
#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
	#include <direct.h>
	#define mkdir(a, b) _mkdir(a)
#endif


struct stat finfo; // utility for folder checking

//
// Creates a folder only if it doesnt exist
inline int createFolder(std::string folderName)
{
  #ifdef _WIN32
	folderName = ".\\" + folderName;
  #endif

	int res = stat(folderName.c_str(), &finfo);

	if (finfo.st_mode & S_IFDIR)
		return 1; // Directory already exists 
	else if (res != 0)
	{
		const int dir_err = mkdir(folderName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (-1 == dir_err)
		{
			std::cout << "Could not create directory " << folderName << std::endl;
			return 0;
		}
	}
	else 
	{
		std::cout << folderName << " is not a directory!" << std::endl;
		return 0;
	}
	return 1;
}


//
// Checks if file exists
inline bool fileExisits(char *filename) 
{
	std::ifstream ifs(filename);
	return ifs.good();
}

