#include "CinBase.h"
#include <vector>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLPUnstructuredGridReader.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
// #include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

#ifndef CINDBWRITER_H
#define CINDBWRITER_H

class CinDBWriter : public CinBase
{
    public:

    CinDBWriter(const std::string& path)
    {
            // path to the main cinema directory
        this->path      = path;
        this->width     = 640;
        this->height    = 480;
        this->input     = 0;
        this->reader    = vtkXMLPUnstructuredGridReader::New(); 
        this->mapper    = vtkDataSetMapper::New();
        this->actor     = vtkActor::New();
        this->renderer  = vtkRenderer::New();
        this->renderWin = vtkRenderWindow::New();
        this->camera    = vtkCamera::New();
        this->pngWriter = vtkPNGWriter::New();

        // state
        this->loaded = false;
    }

    void setPath( const std::string & path )
    {
        this->path = path;
    }

    void setInputFile( const std::string & filename )
    {
        this->infile = filename;
    }

    bool setInput( vtkUnstructuredGrid * data )
    {
        bool result = true;
        this->input = data;

        return result;
    }

    bool addTimestep( float ts )
    {
        bool result = true;
        this->timesteps.push_back(ts);

        return result;
    }

    bool addCameraPosition( float phi, float theta)
    {
        bool result = true;
        this->phi.push_back(phi);
        this->theta.push_back(theta);

        return result;
    }

    bool load();
    bool write();

    protected:

    int                     height;
    std::string             infile;
    vtkUnstructuredGrid *   input;
    bool                    loaded;
    std::string             path;
    std::vector<float>      phi;
    std::vector<float>      theta;
    std::vector<float>      timesteps;
    int                     width;

    // pipeline
    vtkActor *                      actor;
    vtkCamera *                     camera;
    vtkDataSetMapper *              mapper;
    vtkPNGWriter *                  pngWriter;
    vtkXMLPUnstructuredGridReader * reader;
    vtkRenderer *                   renderer;
    vtkRenderWindow *               renderWin;

    // member functions
    bool capture(const std::string & filename);
    void setCameraPosition( float phi, float theta);
};

#endif
