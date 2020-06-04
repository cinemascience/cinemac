# cinemac: C++ cinema library

This is the repository for the `cinemac` library, a library of C++ based capabilities for Cinema.

## build instructions

The cmake build requires VTK, and will look for it. You may have to set that directory.

````
    git clone git@github.com:cinemascience/cinemac.git
    cd cinemac
    mkdir build
    cd build
    ccmake ../src -DCMAKE_INSTALL_PREFIX=/your/install/prefix
````

Once this is done, you should be able to include and link to the installed library. For an example of how to use the libray, look at the contents of `tests/dbcreate.cxx`.

## testing

Run the following to test:

````
    cd build
    ./tests/dbcreate
````

This will create a directory in the `build` area with the following contents:

````
    example.cdb/
        data.csv
        0001.png
        0002.png
        0003.png
        0004.png
        0005.png
        0006.png
        0007.png
        0008.png
        0009.png
        0010.png
````

The `data.csv` file should look like this:

````
phi,theta,FILE
0,45,0001.png
10,45,0002.png
20,45,0003.png
30,45,0004.png
40,45,0005.png
50,45,0006.png
60,45,0007.png
70,45,0008.png
80,45,0009.png
90,45,0010.png
````
