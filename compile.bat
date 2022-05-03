@echo off

rd /s /q "Build"
rd /s /q "bin"

cmake -B bin -S ./ -G "MinGW Makefiles"
cd bin
mingw32-make install
cd ..
if %errorlevel% == 0 (
    echo "Installing includes..."
    echo:
    python .\tools\create_includes.py
    echo "Done!"
    echo:
)