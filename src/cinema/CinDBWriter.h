#include "CinBase.h"
#include <vector>
#include <vtkUnstructuredGrid.h>

#ifndef CINDBWRITER_H
#define CINDBWRITER_H

class CinDBWriter : public CinBase
{
    public:

    CinDBWriter(const std::string& path)
    {
        this->path = path;
        this->input = 0;
    }

    bool setInput( vtkUnstructuredGrid * data )
    {
        bool result = true;
        this->input = data;

        return result;
    }

    bool addCameraPosition( float phi, float theta)
    {
        bool result = true;
        this->phi.push_back(phi);
        this->theta.push_back(theta);

        return result;
    }

    bool write();

    protected:

    vtkDataSet * input;
    std::string path;
    std::vector<float> phi;
    std::vector<float> theta;
};

#endif
