:: RELEASE BUILD
::
:: use when in src directory
:: - clear build directory
:: - configure cmake build
:: - build project
:: - copy textures
:: - copy shaders
:: - run exe
:: - go back to src directory to finish where started

if not exist ..\build mkdir ..\build
cd ..\build
rd /s /q .
cmake ..\src
cmake --build . --config Release

Xcopy ..\src\textures Release\textures /I
Xcopy ..\src\fonts Release\fonts /I
Xcopy ..\src\levels Release\levels /I
Xcopy ..\src\engine\shaders Release\shaders /I
Xcopy ..\src\engine\parameters.txt Release /I

Xcopy ..\src\engine\3rd_party\lib\FreeType\win64\freetype.dll Release /I

cd Release
lostInCave.exe
cd ../../src