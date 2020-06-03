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

This should create a directory in the `build` area with the following contents (there will be no images):

````
    test.cdb/
        data.csv
````

The `data.csv` file should look like this:

````
phi,theta,FILE
0,0.5,0.png
10,10.5,1.png
20,20.5,2.png
30,30.5,3.png
````
