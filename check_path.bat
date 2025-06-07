@echo off
echo Current directory:
cd
echo.
echo Full path:
echo %CD%
echo.
echo Checking for game files...
if exist setup.bat (
    echo Found setup.bat
) else (
    echo setup.bat not found
)
if exist build.bat (
    echo Found build.bat
) else (
    echo build.bat not found
)
echo.
pause 