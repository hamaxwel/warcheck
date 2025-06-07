#pragma once

#include <vector>
#include <functional>

namespace InvasionEngine {

class Entity;
class RigidBodyComponent;
class ColliderComponent;

class PhysicsSystem {
public:
    PhysicsSystem();
    ~PhysicsSystem();

    void Initialize();
    void Shutdown();
    void Update(float deltaTime);
    void SetGravity(const float gravity);
    float GetGravity() const;

    // TODO: Add collision detection and physics queries

private:
    float m_Gravity = 9.81f;
};

} // namespace InvasionEngine 