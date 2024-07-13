if not exist ..\build mkdir ..\build
cd ..\build
rd /s /q .
cmake ..\src
cmake --build .
cd ../src

Xcopy ..\src\textures Debug\textures /I
Xcopy ..\src\fonts Debug\fonts /I
Xcopy ..\src\levels Debug\levels /I
Xcopy ..\src\engine\shaders Debug\shaders /I
Xcopy ..\src\engine\parameters.txt Debug /I

Xcopy ..\src\engine\3rd_party\lib\FreeType\win64\freetype.dll Debug /I

cd ../src