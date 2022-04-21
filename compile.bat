cmake -B bin -S ./ -G "MinGW Makefiles"
cd bin
mingw32-make install
cd ..
