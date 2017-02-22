# OMSimulator
OpenModelica FMI &amp; TLM based simulator

## Dependencies
- Boost
- FMILibrary

## Linux/MSYS Build
* Config dependencies: `make config`
* Build OMSimulator: `make OMSimulator`

### Custom configuration
Update the **FMILibrary_ROOT** variable in the root CMakeFiles.txt in OMSimulator folder to point to the installation folder of FMILibrary.
  - FMILibrary will install everything by default in to a directory called "install" on the root FMIL directory. e.g.
    - ```SET (FMILibrary_ROOT <pathto>/FMIL/install)```

## Windows Build
 TODO
