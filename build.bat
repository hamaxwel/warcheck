@echo off
echo Building InvasionEngine...

if not exist build mkdir build
cd build

cmake ..
if errorlevel 1 (
    echo CMake failed!
    pause
    exit /b 1
)

cmake --build . --config Release
if errorlevel 1 (
    echo Build failed!
    pause
    exit /b 1
)

echo Build successful!
echo Running game...
cd ..
build\Release\InvasionEngine.exe 