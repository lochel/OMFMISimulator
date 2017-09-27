# OMSimulator
OpenModelica FMI &amp; TLM based simulator

## Dependencies
- [Boost](http://www.boost.org/)
- [cmake](http://www.cmake.org)
- [3rdParty subproject](https://github.com/OpenModelica/OMFMISimulator-3rdParty)
  - FMILibrary
  - Lua
  - PugiXML
  - SUNDIALS CVODE
  - SUNDIALS KINSOL

## Compilation

### Linux/OSX
1. configure OMSimulator
```bash
> make config-3rdParty
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
1. configure OMSimulator
```bash
> configWinVS14.bat
```

2. build OMSimulator
```bash
> buildWinVS14.bat
```
