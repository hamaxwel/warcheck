#pragma once

#include "Entity.h"
#include <memory>
#include "Component.h"
#include <glm/glm.hpp>
#include "Math/Matrix.h"

namespace InvasionEngine {

struct InputState {
    bool moveForward;
    bool moveBackward;
    bool moveLeft;
    bool moveRight;
    bool rotateLeft;
    bool rotateRight;
    bool mouseMoved;
    float mouseDeltaX;
    float mouseDeltaY;
};

class PlayerController : public Component {
public:
    PlayerController();
    virtual ~PlayerController() = default;

    virtual bool Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Shutdown() override;

    // Entity control
    void PossessEntity(Entity* entity);
    void UnpossessEntity();
    Entity* GetPossessedEntity() const { return m_PossessedEntity; }

    // Input handling
    virtual void HandleInput();

    void SetInputState(const InputState& state);
    void SetMoveSpeed(float speed);
    void SetRotationSpeed(float speed);
    void SetMouseSensitivity(float sensitivity);

protected:
    Entity* m_PossessedEntity;
    float m_MoveSpeed;
    float m_RotationSpeed;
    float m_MouseSensitivity;
    float m_Pitch;
    float m_Yaw;
    InputState m_InputState;
};

} // namespace InvasionEngine 