#include <string>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "CinBase.h"
#include "CinDBWriter.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkSmartPointer.h"

int main() 
{
    // set up path
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::string db_path = cwd;
    db_path += "/zap.cdb";

    // create a writer
    CinDBWriter writer(db_path);

    // writer.setInput( reader->GetOutput() );
    writer.addCameraPosition(  0.0, 45.0);
    writer.addCameraPosition( 10.0, 45.0);
    writer.addCameraPosition( 20.0, 45.0);
    writer.addCameraPosition( 30.0, 45.0);
    writer.addCameraPosition( 40.0, 45.0);
    writer.addCameraPosition( 50.0, 45.0);
    writer.addCameraPosition( 60.0, 45.0);
    writer.addCameraPosition( 70.0, 45.0);
    writer.addCameraPosition( 80.0, 45.0);
    writer.addCameraPosition( 90.0, 45.0);

    // writer.write();
    writer.setInputFile("/Users/dhr/LANL/data/hacc/halo_hacc/results/extracted/extracted_8.vtu");
    writer.write();

    return 1;
}
