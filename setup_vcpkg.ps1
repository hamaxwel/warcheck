# Setup vcpkg and install GLM
$ErrorActionPreference = "Stop"

# Create vcpkg directory in the project
$vcpkgDir = Join-Path $PSScriptRoot "vcpkg"
if (-not (Test-Path $vcpkgDir)) {
    Write-Host "Cloning vcpkg..."
    git clone https://github.com/Microsoft/vcpkg.git $vcpkgDir
}

# Bootstrap vcpkg
$bootstrapScript = Join-Path $vcpkgDir "bootstrap-vcpkg.bat"
if (-not (Test-Path $bootstrapScript)) {
    Write-Host "Bootstrap script not found. Please check vcpkg installation."
    exit 1
}

Write-Host "Bootstrapping vcpkg..."
& $bootstrapScript

# Install GLM
Write-Host "Installing GLM..."
& (Join-Path $vcpkgDir "vcpkg.exe") install glm:x64-windows

# Create a CMake toolchain file
$toolchainFile = Join-Path $PSScriptRoot "vcpkg.cmake"
@"
set(CMAKE_TOOLCHAIN_FILE "$vcpkgDir/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")
"@ | Out-File -FilePath $toolchainFile -Encoding ASCII

Write-Host "Setup complete! You can now run CMake with:"
Write-Host "cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg.cmake" 