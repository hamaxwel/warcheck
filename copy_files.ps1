# Copy GLEW files
Write-Host "Copying GLEW files..."
Copy-Item "dependencies\glew\glew-2.2.0\bin\Release\x64\glew32.dll" -Destination "dependencies\bin\"
Copy-Item "dependencies\glew\glew-2.2.0\lib\Release\x64\glew32.lib" -Destination "dependencies\lib\"
Copy-Item "dependencies\glew\glew-2.2.0\include\GL\*" -Destination "dependencies\include\GL\" -Recurse

# Copy OpenAL files
Write-Host "Copying OpenAL files..."
Copy-Item "dependencies\openal\openal-soft-1.22.2-bin\bin\Win64\OpenAL32.dll" -Destination "dependencies\bin\"
Copy-Item "dependencies\openal\openal-soft-1.22.2-bin\libs\Win64\OpenAL32.lib" -Destination "dependencies\lib\"
Copy-Item "dependencies\openal\openal-soft-1.22.2-bin\include\AL\*" -Destination "dependencies\include\AL\" -Recurse

# Copy Assimp files
Write-Host "Copying Assimp files..."
Copy-Item "dependencies\assimp\assimp-5.2.3-windows-vs142-x64\bin\assimp-vc142-mt.dll" -Destination "dependencies\bin\"
Copy-Item "dependencies\assimp\assimp-5.2.3-windows-vs142-x64\lib\assimp-vc142-mt.lib" -Destination "dependencies\lib\"
Copy-Item "dependencies\assimp\assimp-5.2.3-windows-vs142-x64\include\assimp\*" -Destination "dependencies\include\assimp\" -Recurse

Write-Host "`nSetup complete! You can now delete the ZIP files and extracted folders." 