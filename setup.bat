@echo off
echo Setting up InvasionEngine dependencies...

REM Create directories
if not exist dependencies mkdir dependencies
if not exist dependencies\bin mkdir dependencies\bin
if not exist dependencies\lib mkdir dependencies\lib
if not exist dependencies\include mkdir dependencies\include

REM Download dependencies
echo Downloading dependencies...
echo Downloading GLEW...
powershell -Command "& {[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri 'https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0-win32.zip/download' -OutFile 'dependencies\glew.zip'}"

echo Downloading OpenAL...
powershell -Command "& {[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri 'https://www.openal-soft.org/openal-binaries/openal-soft-1.22.2-bin.zip' -OutFile 'dependencies\openal.zip'}"

echo Downloading Assimp...
powershell -Command "& {[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri 'https://sourceforge.net/projects/assimp/files/assimp-5.2.4/assimp-5.2.4-windows-vs142-x64.zip/download' -OutFile 'dependencies\assimp.zip'}"

REM Check if downloads were successful
if not exist dependencies\glew.zip (
    echo Failed to download GLEW
    pause
    exit /b 1
)
if not exist dependencies\openal.zip (
    echo Failed to download OpenAL
    pause
    exit /b 1
)
if not exist dependencies\assimp.zip (
    echo Failed to download Assimp
    pause
    exit /b 1
)

REM Extract dependencies
echo Extracting dependencies...
powershell -Command "& {Expand-Archive -Path 'dependencies\glew.zip' -DestinationPath 'dependencies' -Force}"
powershell -Command "& {Expand-Archive -Path 'dependencies\openal.zip' -DestinationPath 'dependencies' -Force}"
powershell -Command "& {Expand-Archive -Path 'dependencies\assimp.zip' -DestinationPath 'dependencies' -Force}"

REM Copy DLLs to bin directory
echo Copying DLLs...
copy dependencies\glew-2.2.0\bin\Release\x64\glew32.dll dependencies\bin\
copy dependencies\openal-soft-1.22.2-bin\bin\Win64\OpenAL32.dll dependencies\bin\
copy dependencies\assimp-5.2.4-windows-vs142-x64\bin\assimp-vc142-mt.dll dependencies\bin\

REM Copy lib files
echo Copying library files...
copy dependencies\glew-2.2.0\lib\Release\x64\glew32.lib dependencies\lib\
copy dependencies\openal-soft-1.22.2-bin\libs\Win64\OpenAL32.lib dependencies\lib\
copy dependencies\assimp-5.2.4-windows-vs142-x64\lib\assimp-vc142-mt.lib dependencies\lib\

REM Copy include files
echo Copying include files...
xcopy /E /I /Y dependencies\glew-2.2.0\include\GL dependencies\include\GL
xcopy /E /I /Y dependencies\openal-soft-1.22.2-bin\include\AL dependencies\include\AL
xcopy /E /I /Y dependencies\assimp-5.2.4-windows-vs142-x64\include\assimp dependencies\include\assimp

REM Clean up
echo Cleaning up...
del dependencies\*.zip

echo Setup complete!
pause 