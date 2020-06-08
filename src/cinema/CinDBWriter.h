#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkXMLPUnstructuredGridReader.h>
#include <vtkRendererCollection.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLPUnstructuredGridReader.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

#include "CinBase.h"

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

    bool load()
    {
        // std::cout << "Loading data ..." << std::endl;
        // std::cout << "   " << this->infile << std::endl;

        //read all the data from the file
        this->reader->SetFileName(this->infile.c_str());
        this->reader->Update();
        // std::cout << "num timesteps: " << this->reader->GetNumberOfTimeSteps() << std::endl;

        // assemble the pipeline
        this->mapper->SetInputConnection(this->reader->GetOutputPort());
        this->mapper->ScalarVisibilityOff();

        this->actor->SetMapper(this->mapper);
        this->actor->GetProperty()->EdgeVisibilityOn();
        this->actor->GetProperty()->SetLineWidth(2.0);

        this->renderer->SetActiveCamera(this->camera);
        this->renderWin->SetOffScreenRendering(1);       // for offscreen
        this->renderWin->AddRenderer(this->renderer);

        //Add the actor to the scene
        renderer->AddActor(this->actor);
        renderer->ResetCamera();

        //Render and interact
        this->renderWin->SetSize(this->width, this->height);
        this->renderWin->Render();

        this->loaded = true;

        return this->loaded;
    }

    bool write()
    {
        bool result = true;

        // make the destination directory
        mkdir(this->path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );

        // write data to csv file
        std::ofstream csvfile;
        std::string csvpath = this->path;
        csvpath += "/data.csv";
        csvfile.open( csvpath.c_str() );
        if (csvfile.is_open())
        {
            // header line
            csvfile << "time,phi,theta,FILE" << std::endl;
            for (auto its = begin(this->timesteps); its != end(this->timesteps); ++its)
            {
                auto ip = begin(this->phi);
                auto it = begin(this->theta);
                std::string imageName;
                std::string imagePath;
                while(ip != end(this->phi))
                {
                    imageName = CinBase::GetNextIDString() + ".png";
                    imagePath = this->path + "/" + imageName;
                    csvfile << std::to_string(*its) << "," << std::to_string(*ip) << "," << 
                            std::to_string(*it) << "," << imageName << std::endl;

                    this->setCameraPosition(*ip, *it);
                    this->capture(imagePath);
                    this->setCameraPosition(-*ip, -*it);

                    ++ip;
                    ++it;
                }
            }
        } else {
            std::cout << "unable to open file: " << csvpath << std::endl;
        }
        csvfile.close();

        return result;
    }

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
    bool capture(const std::string & filename)
    {
        bool result = true;

        if (!this->loaded)
        {
            this->load();
        }

        // std::cout << "Capturing " << filename << std::endl;
        this->renderWin->Render();
        //vtkWindowToImageFilter * w2i = vtkWindowToImageFilter::New();
        vtkSmartPointer<vtkWindowToImageFilter> w2i = vtkSmartPointer<vtkWindowToImageFilter>::New();
        w2i->SetInput(this->renderWin);
        w2i->SetInputBufferTypeToRGBA();
        w2i->ReadFrontBufferOff();
        w2i->Update();

        this->pngWriter->SetInputConnection(w2i->GetOutputPort());
        this->pngWriter->SetFileName(filename.c_str());
        this->pngWriter->Write();


       

        return result;
    }

    void setCameraPosition( float phi, float theta)
    {
        this->camera->Azimuth(phi);
        this->camera->Elevation(theta);
        this->renderWin->Render();
    }
};

#endif
