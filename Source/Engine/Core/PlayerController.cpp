#include "PlayerController.h"
#include "Entity.h"
#include <glm/gtx/quaternion.hpp>

namespace InvasionEngine {

PlayerController::PlayerController()
    : m_MovementSpeed(5.0f)
    , m_RotationSpeed(1.0f)
    , m_MouseSensitivity(0.1f)
    , m_MovementDirection(0.0f)
    , m_Rotation(0.0f)
{
}

PlayerController::~PlayerController()
{
}

bool PlayerController::Initialize()
{
    return true;
}

void PlayerController::Update(float deltaTime)
{
    if (auto owner = GetOwner().lock()) {
        // Get current position
        Vector3 position = owner->GetPosition();

        // Calculate movement
        Vector3 movement(0.0f);
        if (m_InputState.forward) movement.z -= 1.0f;
        if (m_InputState.backward) movement.z += 1.0f;
        if (m_InputState.left) movement.x -= 1.0f;
        if (m_InputState.right) movement.x += 1.0f;

        if (glm::length(movement) > 0.0f) {
            movement = glm::normalize(movement);
            position += movement * m_MovementSpeed * deltaTime;
        }

        // Update position
        owner->SetPosition(position);

        // Update rotation
        if (m_InputState.mouseX != 0.0f || m_InputState.mouseY != 0.0f) {
            m_Rotation.y += m_InputState.mouseX * m_MouseSensitivity;
            m_Rotation.x += m_InputState.mouseY * m_MouseSensitivity;
            owner->SetRotation(m_Rotation);
        }
    }
}

void PlayerController::Shutdown()
{
}

void PlayerController::SetInputState(const InputState& state)
{
    m_InputState = state;
}

void PlayerController::SetMoveSpeed(float speed)
{
    m_MovementSpeed = speed;
}

void PlayerController::SetRotationSpeed(float speed)
{
    m_RotationSpeed = speed;
}

void PlayerController::SetMouseSensitivity(float sensitivity)
{
    m_MouseSensitivity = sensitivity;
}

} // namespace InvasionEngine 