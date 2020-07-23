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

struct Image
{
	int height, width;
	float *data;

	Image(){ data = NULL; }
	Image(int h, int w){ createImage(h,w); }
	~Image(){ if (data!=NULL) delete []data; data=NULL; }

	void createImage(int h, int w);

	void setPixel(int i, int c, float value){ data[i*4 + c] = value; }
    void setPixel(int x, int y, int c, float value);
	float getPixel(int x, int y, int c){ return data[y*width*4 + x*4 + c]; }

	void outputOpaquePPM(std::string filename);
    void outputPNG(std::string filename);
};


inline void Image::createImage(int h, int w)
{
	height = h;
	width  = w;
	data = new float[height*width*4];

	for (size_t i=0; i<h*w*4; i++)
		data[i] = 0;
}


void Image::setPixel(int x, int y, int c, float value)
{
    data[y*width*4 + x*4 + c] = value; 
}


inline void Image::outputOpaquePPM(std::string filename)
{
    if (height == 0 || width == 0){
        std::cout << "Image " << filename << " has dimensions 0 @#$^&* ????" << std::endl;
        return;
    }

    std::ofstream outputFile( filename.c_str(), std::ios::out | std::ios::binary);
    outputFile <<  "P6\n" << width << "\n" << height << "\n" << 255 << "\n";
    
    for (int y=0; y<height; ++y){
        for (int x=0; x<width; ++x)
        {
            char color[3];

            int index = (y * width * 4) + x*4;
            color[0] = std::min(data[index+0] , 1.0f) * 255;  // red
            color[1] = std::min(data[index+1] , 1.0f) * 255;  // green 
            color[2] = std::min(data[index+2] , 1.0f) * 255;  // blue

            outputFile.write(color,3);
        }
    }
    
    outputFile.close();
}



inline void Image::outputPNG(std::string filename)
{
    uint8_t* pixels = new uint8_t[width * height * 4];
    for (unsigned y=0; y<height; y++)
        for (unsigned x=0; x<width; x++) 
        {
            int index = (y * width * 4) + x*4;

            pixels[4 * width * y + 4 * x + 0] = std::min(data[index+0] , 1.0f) * 255;
            pixels[4 * width * y + 4 * x + 1] = std::min(data[index+1] , 1.0f) * 255;
            pixels[4 * width * y + 4 * x + 2] = std::min(data[index+2] , 1.0f) * 255;
            pixels[4 * width * y + 4 * x + 3] = std::min(data[index+3] , 1.0f) * 255;
        }

    stbi_write_png(filename.c_str(), width, height, 4, pixels, width * 4);
    delete[] pixels;
}
