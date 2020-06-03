#include <string>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "CinBase.h"
#include "CinDBWriter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkSmartPointer.h"

int main() {

    // create some IDs and print them
    std::cout << CinBase::GetNextIDString() << std::endl;
    std::cout << CinBase::GetNextIDString() << std::endl;
    std::cout << CinBase::GetNextIDString() << std::endl;

    //
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::string db_path = cwd;
    db_path += "/test.cdb";
    CinDBWriter writer(db_path);

    vtkSmartPointer<vtkUnstructuredGrid> input = vtkSmartPointer<vtkUnstructuredGrid>::New();

    writer.setInput( input );
    writer.addCameraPosition( 0.0, 0.5);
    writer.addCameraPosition(10.0,10.5);
    writer.addCameraPosition(20.0,20.5);
    writer.addCameraPosition(30.0,30.5);

    writer.write();

    return 1;
}
