# Set TLS to 1.2
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

# Create temp directory
$tempDir = "temp_deps"
if (-not (Test-Path $tempDir)) {
    New-Item -ItemType Directory -Path $tempDir
}

# Download GLEW
Write-Host "Downloading GLEW..."
$glewUrl = "https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0-win32.zip/download"
$glewZip = "$tempDir\glew.zip"
Invoke-WebRequest -Uri $glewUrl -OutFile $glewZip

# Download OpenAL
Write-Host "Downloading OpenAL..."
$openalUrl = "https://www.openal-soft.org/openal-binaries/openal-soft-1.22.2-bin.zip"
$openalZip = "$tempDir\openal.zip"
Invoke-WebRequest -Uri $openalUrl -OutFile $openalZip

# Download Assimp
Write-Host "Downloading Assimp..."
$assimpUrl = "https://sourceforge.net/projects/assimp/files/assimp-5.2.4/assimp-5.2.4-windows-vs142-x64.zip/download"
$assimpZip = "$tempDir\assimp.zip"
Invoke-WebRequest -Uri $assimpUrl -OutFile $assimpZip

Write-Host "Downloads complete. Files are in the $tempDir directory." 