#pragma once

#include <cstddef>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <malloc/malloc.h>
#include <string.h>

#include "lodepng.h"

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



inline void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) 
{
    //Encode the image
    unsigned error = lodepng::encode(filename, image, width, height);

     //if there's an error, display it
    if (error) 
        std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}


inline void Image::outputPNG(std::string filename)
{
    std::vector<unsigned char> image;
    image.resize(width * height * 4);

    for (unsigned y=0; y<height; y++)
        for (unsigned x=0; x<width; x++) 
        {
            int index = (y * width * 4) + x*4;

            image[4 * width * y + 4 * x + 0] = std::min(data[index+0] , 1.0f) * 255;
            image[4 * width * y + 4 * x + 1] = std::min(data[index+1] , 1.0f) * 255;
            image[4 * width * y + 4 * x + 2] = std::min(data[index+2] , 1.0f) * 255;
            image[4 * width * y + 4 * x + 3] = 255;
        }


    unsigned error = lodepng::encode(filename, image, width, height);

    if (error) 
        std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
    //encodeOneStep(filename.c_str(), image, width, height);
}