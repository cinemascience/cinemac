#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

#include "dataset.hpp"


int main(int argc, char *argv[]) 
{
    Dataset test;

    std::string name;

    int numDims = 3;
    int dimSize[3];

    dimSize[0] = 4;
    dimSize[1] = 2;
    dimSize[2] = 2;

    float extents[6];

    float *pressureData = new float[ dimSize[0]*dimSize[1]*dimSize[2] ];
    int *testData = new int[ dimSize[0]*dimSize[1]*dimSize[2] ];


    int count = 0;
    for (int z=0; z<dimSize[2]; z++)
        for (int y=0; y<dimSize[1]; y++)
            for (int x=0; x<dimSize[0]; x++)
            {
                pressureData[count] = count;
                testData[count] = -5;

                count++;
            }

    
    test.addVariable( Variable("pressure", 1, dimSize[0]*dimSize[1]*dimSize[2], "float", pressureData) );
    test.addVariable( Variable("test", 1, dimSize[0]*dimSize[1]*dimSize[2], "int", testData) );

    std::cout << "bbbb" << std::endl;

    //Variable d = test.getVariable(0);
    Variable d = test.getVariable(1);

    int i=0;
    for (int z=0; z<dimSize[2]; z++)
        for (int y=0; y<dimSize[1]; y++)
            for (int x=0; x<dimSize[0]; x++)
            {
                std::cout << d.getScalar(i) << ", ";
                i++;
            }

 
    return 1;
}

