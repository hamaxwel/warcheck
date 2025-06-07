# Set TLS to 1.2
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

# Create directories
$dirs = @(
    "dependencies",
    "dependencies\bin",
    "dependencies\lib",
    "dependencies\include",
    "dependencies\include\GL",
    "dependencies\include\AL",
    "dependencies\include\assimp"
)

foreach ($dir in $dirs) {
    if (-not (Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir -Force
    }
}

# Download GLEW
Write-Host "Downloading GLEW..."
$glewUrl = "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip"
$glewZip = "dependencies\glew.zip"
Invoke-WebRequest -Uri $glewUrl -OutFile $glewZip

# Download OpenAL
Write-Host "Downloading OpenAL..."
$openalUrl = "https://www.openal-soft.org/openal-binaries/openal-soft-1.22.2-bin.zip"
$openalZip = "dependencies\openal.zip"
Invoke-WebRequest -Uri $openalUrl -OutFile $openalZip

# Download Assimp
Write-Host "Downloading Assimp..."
$assimpUrl = "https://github.com/assimp/assimp/releases/download/v5.2.4/assimp-5.2.4-windows-vs142-x64.zip"
$assimpZip = "dependencies\assimp.zip"
Invoke-WebRequest -Uri $assimpUrl -OutFile $assimpZip

Write-Host "`nDownloads complete. Please extract the ZIP files manually:"
Write-Host "1. Extract glew.zip to dependencies\glew"
Write-Host "2. Extract openal.zip to dependencies\openal"
Write-Host "3. Extract assimp.zip to dependencies\assimp"
Write-Host "`nAfter extracting, run the copy_files.ps1 script to copy the files to the correct locations." 