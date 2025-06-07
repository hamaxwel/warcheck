# InvasionEngine

A modern C++ OpenGL game engine with advanced features for FPS/tactics games.

## Features
- Entity/Component system
- 3D model loading (via Assimp)
- Advanced audio system with 3D positional audio (via OpenAL)
- Menu system with customizable UI
- Map loading and management
- Modern OpenGL rendering
- Physics and collision detection
- AI behavior system

## Dependencies
- CMake 3.10+
- C++17 compiler
- OpenGL
- GLEW
- OpenAL
- Assimp
- jsoncpp
- stb_image (included)
- stb_vorbis (included)

## Build Instructions

### Prerequisites
Install required dependencies:

#### Windows
```powershell
vcpkg install glew:x64-windows
vcpkg install openal-soft:x64-windows
vcpkg install assimp:x64-windows
vcpkg install jsoncpp:x64-windows
```

#### Linux
```bash
sudo apt-get install libglew-dev
sudo apt-get install libopenal-dev
sudo apt-get install libassimp-dev
sudo apt-get install libjsoncpp-dev
```

### Build
```bash
mkdir build
cd build
cmake ..
make
```

### Run
```bash
./InvasionEngine
```

## Controls
- WASD: Move
- Mouse: Look
- Left Mouse: Shoot
- R: Reload
- Enter: Menu/Select
- Escape: Pause
- F1: Toggle Debug Info

## Asset Structure
```
Assets/
├── Models/
│   ├── Characters/
│   ├── Weapons/
│   └── Environment/
├── Textures/
├── Sounds/
└── Maps/
```

## License
MIT 