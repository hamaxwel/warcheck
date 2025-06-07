# Extract GLEW
Write-Host "Extracting GLEW..."
Expand-Archive -Path "temp_deps\glew.zip" -DestinationPath "temp_deps\glew" -Force

# Extract OpenAL
Write-Host "Extracting OpenAL..."
Expand-Archive -Path "temp_deps\openal.zip" -DestinationPath "temp_deps\openal" -Force

# Extract Assimp
Write-Host "Extracting Assimp..."
Expand-Archive -Path "temp_deps\assimp.zip" -DestinationPath "temp_deps\assimp" -Force

# Copy GLEW files
Write-Host "Copying GLEW files..."
Copy-Item "temp_deps\glew\glew-2.2.0\bin\Release\x64\glew32.dll" -Destination "dependencies\bin\"
Copy-Item "temp_deps\glew\glew-2.2.0\lib\Release\x64\glew32.lib" -Destination "dependencies\lib\"
Copy-Item "temp_deps\glew\glew-2.2.0\include\GL\*" -Destination "dependencies\include\GL\" -Recurse

# Copy OpenAL files
Write-Host "Copying OpenAL files..."
Copy-Item "temp_deps\openal\openal-soft-1.22.2-bin\bin\Win64\OpenAL32.dll" -Destination "dependencies\bin\"
Copy-Item "temp_deps\openal\openal-soft-1.22.2-bin\libs\Win64\OpenAL32.lib" -Destination "dependencies\lib\"
Copy-Item "temp_deps\openal\openal-soft-1.22.2-bin\include\AL\*" -Destination "dependencies\include\AL\" -Recurse

# Copy Assimp files
Write-Host "Copying Assimp files..."
Copy-Item "temp_deps\assimp\assimp-5.2.4-windows-vs142-x64\bin\assimp-vc142-mt.dll" -Destination "dependencies\bin\"
Copy-Item "temp_deps\assimp\assimp-5.2.4-windows-vs142-x64\lib\assimp-vc142-mt.lib" -Destination "dependencies\lib\"
Copy-Item "temp_deps\assimp\assimp-5.2.4-windows-vs142-x64\include\assimp\*" -Destination "dependencies\include\assimp\" -Recurse

# Clean up
Write-Host "Cleaning up..."
Remove-Item -Path "temp_deps" -Recurse -Force

Write-Host "Setup complete!" 