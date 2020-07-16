# cinemac: C++ cinema library

**This is under development** and has not been released yet. Use at your own risk.

This is the repository for the `cinemac` library, a library of C++ based capabilities for Cinema.


## setup environment

````
    source env/darwin.sh
````

## build instructions

The cmake build requires VTK, and will look for it. You may have to set that directory.

````
    git clone git@github.com:cinemascience/cinemac.git
    cd cinemac
    git checkout hacc_cinema
    mkdir build
    cd build
    ccmake ../src/
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
        0000.png
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
        ...
        0029.png
````

The `data.csv` file should look like this:

````
phi,theta,FILE
1.000000,0.000000,45.000000,0000.png
1.000000,10.000000,45.000000,0001.png
1.000000,20.000000,45.000000,0002.png
1.000000,30.000000,45.000000,0003.png
1.000000,40.000000,45.000000,0004.png
1.000000,50.000000,45.000000,0005.png
1.000000,60.000000,45.000000,0006.png
1.000000,70.000000,45.000000,0007.png
1.000000,80.000000,45.000000,0008.png
1.000000,90.000000,45.000000,0009.png
2.000000,0.000000,45.000000,0010.png
2.000000,10.000000,45.000000,0011.png
2.000000,20.000000,45.000000,0012.png
2.000000,30.000000,45.000000,0013.png
2.000000,40.000000,45.000000,0014.png
2.000000,50.000000,45.000000,0015.png
2.000000,60.000000,45.000000,0016.png
2.000000,70.000000,45.000000,0017.png
2.000000,80.000000,45.000000,0018.png
2.000000,90.000000,45.000000,0019.png
3.000000,0.000000,45.000000,0020.png
3.000000,10.000000,45.000000,0021.png
3.000000,20.000000,45.000000,0022.png
3.000000,30.000000,45.000000,0023.png
3.000000,40.000000,45.000000,0024.png
3.000000,50.000000,45.000000,0025.png
3.000000,60.000000,45.000000,0026.png
3.000000,70.000000,45.000000,0027.png
3.000000,80.000000,45.000000,0028.png
3.000000,90.000000,45.000000,0029.png
````
