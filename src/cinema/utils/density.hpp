#pragma once

#include <vector>

class DensityCompute
{
    int gridDims[3];
    float cellSize[3];
    float extents[6]; //minx, miny, minz, maxx, maxy, maxz

    std::vector<float> densityBins;

    int getCellIndex(float x, float y, float z);

  public:

    DensityCompute(int dims_x, int dims_y, int dims_z, float _x, float _y, float _z, float r)
    {
        gridDims[0] = dims_x;
        gridDims[1] = dims_y;
        gridDims[2] = dims_z;

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
    }

    float getDensity(float x, float y, float z);
    void computeDensity(float *x, float *y, float *z, size_t numPoints);
};


inline int DensityCompute::getCellIndex(float x, float y, float z)
{
    int xIndex = (int) (x/cellSize[0]);
    int yIndex = (int) (y/cellSize[1]);
    int zIndex = (int) (z/cellSize[2]);

    int index = zIndex * gridDims[0]*gridDims[1] + yIndex * gridDims[0] + xIndex;
    return index;
}


inline float DensityCompute::getDensity(float x, float y, float z)
{
    int index = getCellIndex(_x, _y, _z);
    return densityBins[index];
}


inline void DensityCompute::computeDensity(float *x, float *y, float *z, size_t numPoints)
{
    float _x, _y, _z;

    // Create bins
    std::vector<size_t> bins;
    bins.resize(dims_x*dims_y*dims_z);

    // Accumulate particle count
    for (size_t i=0; i<numPoints; i++)
    {
        _x = x[i];  _y = y[i];  _z = z[i];

        int index = getCellIndex(_x, _y, _z);
        bins[index]++;
    }

    // Compute density
    #pragma omp parallel for
    for (size_t i=0; i<bins.size(); i++)
        densityBins = (float)bins[i]/numPoints;
}