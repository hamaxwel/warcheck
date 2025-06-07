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

# Function to download file
function Download-File {
    param (
        [string]$Url,
        [string]$OutFile
    )
    try {
        $webClient = New-Object System.Net.WebClient
        $webClient.Headers.Add("User-Agent", "Mozilla/5.0")
        $webClient.DownloadFile($Url, $OutFile)
        return $true
    }
    catch {
        Write-Host "Failed to download $Url"
        Write-Host $_.Exception.Message
        return $false
    }
}

# Download GLEW
Write-Host "Downloading GLEW..."
$glewUrl = "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip"
$glewZip = "dependencies\glew.zip"
if (Download-File -Url $glewUrl -OutFile $glewZip) {
    Write-Host "GLEW downloaded successfully"
} else {
    Write-Host "Failed to download GLEW"
    exit 1
}

# Download OpenAL
Write-Host "Downloading OpenAL..."
$openalUrl = "https://www.openal-soft.org/openal-binaries/openal-soft-1.22.2-bin.zip"
$openalZip = "dependencies\openal.zip"
if (Download-File -Url $openalUrl -OutFile $openalZip) {
    Write-Host "OpenAL downloaded successfully"
} else {
    Write-Host "Failed to download OpenAL"
    exit 1
}

# Download Assimp
Write-Host "Downloading Assimp..."
$assimpUrl = "https://github.com/assimp/assimp/releases/download/v5.2.3/assimp-5.2.3-windows-vs142-x64.zip"
$assimpZip = "dependencies\assimp.zip"
if (Download-File -Url $assimpUrl -OutFile $assimpZip) {
    Write-Host "Assimp downloaded successfully"
} else {
    Write-Host "Failed to download Assimp"
    exit 1
}

Write-Host "`nDownloads complete. Please extract the ZIP files manually:"
Write-Host "1. Extract glew.zip to dependencies\glew"
Write-Host "2. Extract openal.zip to dependencies\openal"
Write-Host "3. Extract assimp.zip to dependencies\assimp"
Write-Host "`nAfter extracting, run the copy_files.ps1 script to copy the files to the correct locations." 