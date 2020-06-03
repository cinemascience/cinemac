#include "CinDBWriter.h"
#include <sys/stat.h>
#include <iostream>
#include <fstream>
// #include "vtkWindowToImageFilter.h"
// #include "vtkPNGWriter.h"

#include <vtkSmartPointer.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkNamedColors.h>


bool CinDBWriter::write()
{
    bool result = true;

    // make the destination directory
    mkdir(this->path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );

    // write data to csv file
    std::ofstream csvfile;
    std::string csvpath = this->path;
    csvpath += "/data.csv";
    std::cout << csvpath << std::endl;
    csvfile.open( csvpath.c_str() );
    if (csvfile.is_open())
    {
        csvfile << "phi,theta,FILE" << std::endl;

        std::vector<float>::iterator ip = this->phi.begin(); 
        std::vector<float>::iterator it = this->theta.begin(); 
        int curfile = 0;
        while (ip != this->phi.end())
        {
            csvfile << *ip << "," << *it << "," << curfile << ".png" << std::endl;
            std::cout << "HERE 0.0" << std::endl;
            ++ip; 
            ++it; 
            ++curfile;
        }
    } else {
        std::cout << "unable to open file: " << csvpath << std::endl;
    }
    csvfile.close();

    return result;
}

bool CinDBWriter::render( const std::string & filename )
{
    //read all the data from the file
    this->reader->SetFileName(filename.c_str());
    this->reader->Update();

    //Create a mapper and actor
    this->mapper->SetInputConnection(this->reader->GetOutputPort());
    this->mapper->ScalarVisibilityOff();

    this->actor->SetMapper(this->mapper);
    this->actor->GetProperty()->EdgeVisibilityOn();
    this->actor->GetProperty()->SetLineWidth(2.0);

    //Create a renderer, render window, and interactor
    this->renderWin->AddRenderer(this->renderer);
    this->renderWinInteractor->SetRenderWindow(this->renderWin);

    //Add the actor to the scene
    renderer->AddActor(this->actor);

    //Render and interact
    this->renderWin->SetSize(640, 480);

    this->renderWin->Render();
    // this->renderWinInteractor->Start();
    //
    this->capture("0000.png");

    return EXIT_SUCCESS;
}

bool CinDBWriter::capture( const std::string & filename )
{
    bool result = true;

    this->windowToImage->SetInput(this->renderWin);
    // this->windowToImage->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
    this->windowToImage->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
    this->windowToImage->ReadFrontBufferOff(); // read from the back buffer
    this->windowToImage->Update();

    this->pngWriter->SetFileName(filename.c_str());
    this->pngWriter->SetInputConnection(this->windowToImage->GetOutputPort());
    this->pngWriter->Write();

    return result;
}

