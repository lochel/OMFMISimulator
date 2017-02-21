# OMSimulator
OpenModelica FMI &amp; TLM based simulator


# Dependencies
Boost
FMILibrary


# Linux/MSYS Build
In source builds are disabled.

Update the **FMILibrary_ROOT** variable in the root CMakeFiles.txt in OMSimulator folder to point to the installtion folder of FMILibrary.
  - See below on how to build FMILibrary
  - FMILibrary will install everything by defult in to a directory called "install" on the root FMIL directory. e.g.
  	- ```SET (FMILibrary_ROOT <pathto>/FMIL/install)```

2. Build OMSimulator
	```
    cd OMSimulator
    mkdir build
    cd build
    cmake .. OR cmake .. "MSYS Makefiles" (if MSYS MinGW)
    make
    ```

##
# Windows
 TODO

# Hints
## Building FMILibrary
        1. Go to where you have downloaded FMILibrary
        2. mkdir build
        3. cd build
        4. cmake .. OR cmake .. "MSYS Makefiles" (if MSYS MinGW)
        5. make
        6. make install