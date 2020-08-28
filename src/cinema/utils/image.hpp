#pragma once

#include <cstddef>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
// #include <malloc.h>
#include <string.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "log.hpp"
#include "timer.hpp"

struct Image
{
	int height, width;
    uint8_t* pixels;

	Image(){ pixels = NULL; }
	Image(int h, int w){ createImage(h,w); }
	~Image(){ if (pixels!=NULL) delete []pixels; pixels=NULL; }

	void createImage(int h, int w);

	void setPixel(int i, int c, float value){ pixels[i*4 + c] = value; }
    void setPixel(int x, int y, int c, float value);
	float getPixel(int x, int y, int c){ return pixels[y*width*4 + x*4 + c]; }

    void outputPNG(std::string filename);
    void outputJPG(std::string filename);
};


inline void Image::createImage(int h, int w)
{
	height = h;
	width  = w;
    pixels = new uint8_t[width * height * 4];
}


inline void Image::setPixel(int x, int y, int c, float value)
{
    pixels[y*width*4 + x*4 + c] = value; 
}


inline void Image::outputPNG(std::string filename)
{
    stbi_write_png(filename.c_str(), width, height, 4, pixels, width * 4);
    delete[] pixels;
    pixels = NULL;
}

inline void Image::outputJPG(std::string filename)
{
    stbi_write_jpg(filename.c_str(), width, height, 4, pixels, width * 4);
    delete[] pixels;
    pixels = NULL;
}
