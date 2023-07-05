cd .\src
rd .\build
md .\build
cd .\build
..\..\CMake\bin\cmake -G "MinGW Makefiles" ..
..\..\minGW\bin\mingw32-make
chcp 65001
.\rich.exe