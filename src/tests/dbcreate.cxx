#include <string>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "CinBase.h"
#include "CinDBWriter.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkSmartPointer.h"

int main() {

    // create some IDs and print them
    std::cout << CinBase::GetNextIDString() << std::endl;
    std::cout << CinBase::GetNextIDString() << std::endl;
    std::cout << CinBase::GetNextIDString() << std::endl;

    // set up path
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::string db_path = cwd;
    db_path += "/test.cdb";

    // create a writer
    CinDBWriter writer(db_path);

    // writer.setInput( reader->GetOutput() );
    writer.addCameraPosition( 0.0, 0.5);
    writer.addCameraPosition(10.0,10.5);
    writer.addCameraPosition(20.0,20.5);
    writer.addCameraPosition(30.0,30.5);

    // writer.write();
    writer.render("/Users/dhr/LANL/data/hacc/halo_hacc/results/extracted/extracted_8.vtu");

    return 1;
}
