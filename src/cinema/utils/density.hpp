#pragma once

#include <vector>
#include <algorithm>

class DensityCompute
{
    int gridDims[3];
    float cellSize[3];
    float origins[3];
    float extents[6]; //minx, miny, minz, maxx, maxy, maxz
    float minDensity, maxDensity;

    std::vector<float> densityBins;

    int getCellIndex(float x, float y, float z);
    

  public:
    DensityCompute(){};
    ~DensityCompute(){};

    void init(int dims_x, int dims_y, int dims_z, float _x, float _y, float _z, float r);

    float getMinDensity(){ return minDensity; }
    float getMaxDensity(){ return maxDensity; }
    
    float getDensity(float x, float y, float z);
    void computeDensity(float *points, size_t numPoints);
};


void DensityCompute::init(int dims_x, int dims_y, int dims_z, float _x, float _y, float _z, float r)
{
    gridDims[0] = dims_x;
    gridDims[1] = dims_y;
    gridDims[2] = dims_z;

    origins[0] = _x;
    origins[1] = _y;
    origins[2] = _z;

    extents[0] = _x - r;
    extents[1] = _y - r;
    extents[2] = _z - r;
    extents[3] = _x + r;
    extents[4] = _y + r;
    extents[5] = _z + r;

    // Compute cell size
    cellSize[0] = (extents[3] - extents[0]) / gridDims[0];
    cellSize[1] = (extents[4] - extents[1]) / gridDims[1];
    cellSize[2] = (extents[5] - extents[2]) / gridDims[2];

    densityBins.resize(dims_x * dims_y * dims_z);

    std::cout << "Cellsize: " << cellSize[0] << ", " << cellSize[1] << ", " << cellSize[2] << std::endl;
    std::cout << "extents: " << extents[0] << ", " << extents[1] << ", " << extents[2] 
                    << " - " << extents[3] << ", " << extents[4] << ", " << extents[5] << std::endl;
}


inline int DensityCompute::getCellIndex(float x, float y, float z)
{
    int xIndex = (int) ((x-extents[0])/cellSize[0]);
    int yIndex = (int) ((y-extents[1])/cellSize[1]);
    int zIndex = (int) ((z-extents[2])/cellSize[2]);

    int index = zIndex * gridDims[0]*gridDims[1] + yIndex * gridDims[0] + xIndex;
    return index;
}


inline float DensityCompute::getDensity(float x, float y, float z)
{
    int index = getCellIndex(x, y, z);
    return densityBins[index];
}


inline void DensityCompute::computeDensity(float *points, size_t numPoints)
{
    float _x, _y, _z;

    // Create bins
    std::vector<size_t> bins;
    bins.resize(gridDims[0]*gridDims[1]*gridDims[2]);

    // Accumulate particle count
    for (size_t i=0; i<numPoints; i++)
    {
        _x = points[i*3 + 0];  
        _y = points[i*3 + 1];  
        _z = points[i*3 + 2];

        int index = getCellIndex(_x, _y, _z);
        if (index > gridDims[0]*gridDims[1]*gridDims[2])
        {
            std::cout << "Error! index:" << index << " for " << _x <<", " << _y << ", " << _z << std::endl;
        }
        else
            bins[index]++;
    }



    // Compute density
    #pragma omp parallel for
    for (size_t i=0; i<bins.size(); i++)
        densityBins[i] = (float)bins[i]/numPoints;

    minDensity = *std::min_element(densityBins.begin(), densityBins.end());
    maxDensity = *std::max_element(densityBins.begin(), densityBins.end());
}