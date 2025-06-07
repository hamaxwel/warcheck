#pragma once

#include <memory>
#include <string>

namespace InvasionEngine {

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Initialize();
    void Shutdown();
    void BeginFrame();
    void EndFrame();
    void Clear(float r, float g, float b, float a);

    // TODO: Add draw methods for meshes, sprites, etc.

private:
    // Internal renderer state
};

} // namespace InvasionEngine 