#include "CinDBWriter.h"
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkXMLPUnstructuredGridReader.h>
#include <vtkRendererCollection.h>

bool CinDBWriter::write()
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

void CinDBWriter::setCameraPosition( float phi, float theta)
{
    this->camera->Azimuth(phi);
    this->camera->Elevation(theta);
    this->renderWin->Render();
}

bool CinDBWriter::load()
{
    // std::cout << "Loading data ..." << std::endl;
    // std::cout << "   " << this->infile << std::endl;

    //read all the data from the file
    this->reader->SetFileName(this->infile.c_str());
    this->reader->Update();
    std::cout << "num timesteps: " << this->reader->GetNumberOfTimeSteps() << std::endl;

    // assemble the pipeline
    this->mapper->SetInputConnection(this->reader->GetOutputPort());
    this->mapper->ScalarVisibilityOff();

    this->actor->SetMapper(this->mapper);
    this->actor->GetProperty()->EdgeVisibilityOn();
    this->actor->GetProperty()->SetLineWidth(2.0);

    this->renderer->SetActiveCamera(this->camera);

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

bool CinDBWriter::capture( const std::string & filename )
{
    bool result = true;

    if (!this->loaded)
    {
        this->load();
    }

    // std::cout << "Capturing " << filename << std::endl;
    this->renderWin->Render();
    vtkWindowToImageFilter * w2i = vtkWindowToImageFilter::New();
    w2i->SetInput(this->renderWin);
    w2i->SetInputBufferTypeToRGBA();
    w2i->ReadFrontBufferOff();
    w2i->Update();
    this->pngWriter->SetInputConnection(w2i->GetOutputPort());
    this->pngWriter->SetFileName(filename.c_str());
    this->pngWriter->Write();

    return result;
}

