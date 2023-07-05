cd .\src
rd .\build
md .\build
cd .\build
cmake -G "MinGW Makefiles" ..
mingw32-make
chcp 65001
.\rich.exe