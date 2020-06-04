#include "CinPipeline.h"
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>

CinPipeline::CinPipeline(const std::string& path)
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

bool CinPipeline::Execute()
{
    bool result = true;

    return result;
}

