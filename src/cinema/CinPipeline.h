#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>

#include "CinBase.h"

#ifndef CINPIPELINE_H
#define CINPIPELINE_H

class CinPipeline : public CinBase
{
  public:

    CinPipeline(const std::string& path)
    {
        this->actor     = vtkActor::New();
        this->camera    = vtkCamera::New();
        this->mapper    = vtkDataSetMapper::New();
        this->renderer  = vtkRenderer::New();

        // assemble the pipeline
        this->mapper->ScalarVisibilityOff();

        this->actor->SetMapper(this->mapper);
        this->actor->GetProperty()->EdgeVisibilityOn();
        this->actor->GetProperty()->SetLineWidth(2.0);

        this->renderer->SetActiveCamera(this->camera);

        renderer->AddActor(this->actor);
        renderer->ResetCamera();
    }

    bool Execute()
    {
        bool result = true;
        return result;
    }

    vtkRenderer * GetRenderer()
    {
        return this->renderer;
    }

    void SetInputConnection(vtkAlgorithmOutput * output)
    {
        this->mapper->SetInputConnection(output);
    }

  protected:

    // pipeline
    vtkActor *                      actor;
    vtkCamera *                     camera;
    vtkDataSetMapper *              mapper;
    vtkRenderer *                   renderer;
};

#endif
