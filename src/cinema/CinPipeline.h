#include "CinBase.h"
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderer.h>

#ifndef CINPIPELINE_H
#define CINPIPELINE_H

class CinPipeline : public CinBase
{
    public:

    CinPipeline(const std::string& path);

    bool Execute();

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
