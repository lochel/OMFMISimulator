# OMSimulator
OpenModelica FMI &amp; TLM based simulator

## Dependencies
- [Boost](http://www.boost.org/)
- [cmake](http://www.cmake.org)
- [FMILibrary](http://www.jmodelica.org/FMILibrary)

## Compilation

### Linux/OSX
1. configure OMSimulator
```bash
> make config-fmil
> make config-OMSimulator
```

2. build OMSimulator
```bash
> make OMSimulator
```

3. run test suite
```bash
> make testsuite
```

### Windows
TODO

### Custom configuration
Update the `FMILibrary_ROOT` variable in the root CMakeFiles.txt in OMSimulator folder to point to the installation folder of FMILibrary.
  - FMILibrary will install everything by default in to a directory called "install" on the root FMIL directory. e.g. `SET(FMILibrary_ROOT <pathto>/FMIL/install)`
