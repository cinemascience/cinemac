# cinemac: C++ cinema library

**This is under development** and has not been released yet. Use at your own risk.

This is the repository for the `cinemac` library, a library of C++ based capabilities for Cinema.

## build instructions

The cmake build requires VTK, and will look for it. You may have to set that directory.

````
    git clone git@github.com:cinemascience/cinemac.git
    cd cinemac
    mkdir build
    cd build
    cmake ../src -DCMAKE_INSTALL_PREFIX=/your/install/prefix
````

Once this is done, you should be able to include and link to the installed library. For an example of how to use the libray, look at the contents of `tests/dbtest.cxx`.

## testing

Run the following to test:

````
    cd build
    ./tests/dbtest
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
0.000000,45.000000,0001.png
10.000000,45.000000,0002.png
20.000000,45.000000,0003.png
30.000000,45.000000,0004.png
40.000000,45.000000,0005.png
50.000000,45.000000,0006.png
60.000000,45.000000,0007.png
70.000000,45.000000,0008.png
80.000000,45.000000,0009.png
90.000000,45.000000,0010.png
````
