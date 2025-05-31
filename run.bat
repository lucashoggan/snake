rm -rf build
cmake -B build -G "MinGW Makefiles"
cd build
mingw32-make.exe
cd ..