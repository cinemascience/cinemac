#include "CinBase.h"
#include <vector>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

#ifndef CINDBWRITER_H
#define CINDBWRITER_H

class CinDBWriter : public CinBase
{
    public:

    CinDBWriter(const std::string& path)
    {
        this->path = path;
        this->input = 0;
        this->reader = vtkXMLUnstructuredGridReader::New(); 
        this->mapper = vtkDataSetMapper::New();
        this->actor  = vtkActor::New();
        this->renderer = vtkRenderer::New();
        this->renderWin = vtkRenderWindow::New();
        this->renderWinInteractor = vtkRenderWindowInteractor::New();
        this->windowToImage = vtkWindowToImageFilter::New();
        this->pngWriter = vtkPNGWriter::New();
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
    bool render( const std::string & filename);
    bool capture(const std::string & filename);

    protected:

    vtkUnstructuredGrid * input;
    vtkRenderWindow * rWindow;
    std::string path;
    std::vector<float> phi;
    std::vector<float> theta;

    // pipeline
    vtkXMLUnstructuredGridReader * reader;
    vtkDataSetMapper * mapper;
    vtkActor * actor;
    vtkRenderer * renderer;
    vtkRenderWindow * renderWin;
    vtkRenderWindowInteractor * renderWinInteractor;
    vtkWindowToImageFilter * windowToImage;
    vtkPNGWriter * pngWriter;
};

#endif
