@echo off
REM run this on wsl using "cmd.exe /C configWinVS14.bat"

echo # config fmil
if exist "3rdParty\FMIL\build\win\" RMDIR /S /Q 3rdParty\FMIL\build\win
if exist "3rdParty\FMIL\install\win\" RMDIR /S /Q 3rdParty\FMIL\install\win
MKDIR 3rdParty\FMIL\build\win
CD 3rdParty\FMIL\build\win
cmake -G "Visual Studio 14 2015 Win64" -DFMILIB_INSTALL_PREFIX=..\..\install\win -DFMILIB_BUILD_TESTS:BOOL="0" -DFMILIB_GENERATE_DOXYGEN_DOC:BOOL="0" -DFMILIB_BUILD_STATIC_LIB:BOOL="1" -DBUILD_TESTING:BOOL="0" -DFMILIB_BUILD_BEFORE_TESTS:BOOL="0" ..\..
CD ..\..\..\..
echo # build fmil
@call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
msbuild.exe "3rdParty\FMIL\build\win\INSTALL.vcxproj" /t:Build /p:configuration=Debug

echo # config OMFMISimulator
if exist "build\win\" RMDIR /S /Q build\win
MKDIR build\win
CD build\win
cmake -G "Visual Studio 14 2015 Win64" ..\..
CD ..\..

pause
